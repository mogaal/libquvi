/* 
* Copyright (C) 2010 Toni Gundogdu.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "config.h"

#include <string.h>
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#include <dirent.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <assert.h>

#include <lualib.h>
#include <lauxlib.h>

#include "quvi/quvi.h"
#include "internal.h"
#include "util.h"
#include "curl_wrap.h"
#include "lua_wrap.h"

static _quvi_video_t qv = NULL;

/* c functions for lua. */

static int
l_quvi_fetch (lua_State *l) {
    QUVIstatusType st;
    char *data, *url;
    luaL_Buffer b;
    _quvi_t quvi;
    QUVIcode rc;

    quvi = qv->quvi;
    st   = QUVISTATUSTYPE_PAGE;

    if (!lua_isstring(l, 1))
        luaL_error(l, "`quvi.fetch' expects `url' argument");

    url = (char*) lua_tostring(l,1);

    if (lua_isstring(l, 2)) {
        const char *type = lua_tostring(l, 2);

        if (strcmp(type, "config") == 0)
            st = QUVISTATUSTYPE_CONFIG;
        else if (strcmp(type, "playlist") == 0)
            st = QUVISTATUSTYPE_PLAYLIST;

        lua_remove(l, 2);
    }
    lua_remove(l, 1);

    rc = fetch_to_mem(qv, url, lua_tostring(l, 1), st, &data);

    if (rc == QUVI_OK) {
        luaL_buffinit(l, &b);
        luaL_addstring(&b, data);
        luaL_pushresult(&b);
        _free(data);
    }
    else {
        _free(data);
        luaL_error(l, qv->quvi->errmsg);
    }

    return (1);
}

static int
l_quvi_unescape (lua_State *l) {
    luaL_Buffer b;
    char *tmp;

    if (!lua_isstring(l, -1))
        luaL_error(l, "expected a string");

    tmp = unescape(qv->quvi, strdup((char*)lua_tostring(l,1)) );
    lua_pop(l, 1);

    luaL_buffinit(l, &b);
    luaL_addstring(&b, tmp);
    _free(tmp);
    luaL_pushresult(&b);

    return (1);
}

/* wrapper. */

static QUVIcode
new_lua_script (_quvi_lua_script_t *dst) {
    struct _quvi_lua_script_s *qls;

    qls = calloc(1, sizeof(*qls));
    if (!qls)
        return (QUVI_MEM);

    *dst = qls;

    return (QUVI_OK);
}

typedef int (*filter_func)(const struct dirent*);

static QUVIcode
scan_dir (llst_node_t *dst, const char *path, filter_func filter) {
    char *show_scandir;
    struct dirent *de;
    DIR *dir;

    dir = opendir(path);
    if (!dir)
        return (QUVI_OK);

    show_scandir = getenv ("QUVI_SHOW_SCANDIR");

    if (show_scandir)
        fprintf (stderr, "%s: %s\n", __PRETTY_FUNCTION__, path);

    while ( (de = readdir(dir)) ) {
        if (filter(de)) {
            _quvi_lua_script_t qls;

            QUVIcode rc = new_lua_script(&qls);

            if (rc != QUVI_OK)
                return (rc);

            asprintf((char**)&qls->basename, "%s", de->d_name);
            asprintf((char**)&qls->path, "%s/%s", path, de->d_name);

            llst_add(dst, qls);
        }
    }

    closedir(dir);

    return (QUVI_OK);
}

static QUVIcode
scan_known_dirs (llst_node_t *dst, const char *spath, filter_func filter) {
    char *homedir, *path, *basedir;
    char buf[PATH_MAX];

#define _scan \
    do { \
        QUVIcode rc = scan_dir (dst, path, filter); \
        _free(path); \
        if (rc != QUVI_OK) \
            return (rc); \
    } while (0)

    /* QUVI_BASEDIR. */
    basedir = getenv("QUVI_BASEDIR");
    if (basedir) {
        asprintf(&path, "%s/%s", basedir, spath);
        _scan;
        return (QUVI_OK);
    }

    /* Current working directory. */
    asprintf(&path, "%s/%s", getcwd(buf,sizeof(buf)), spath);
    _scan;

    /* Home directory. */
    homedir = getenv("HOME");
    if (homedir) {
        asprintf(&path, "%s/.quvi/%s", homedir, spath);
        _scan;
        asprintf(&path, "%s/.config/quvi/%s", homedir, spath);
        _scan;
        asprintf(&path, "%s/.local/share/quvi/%s", homedir, spath);
        _scan;
    }

    /* --datadir. */
    asprintf(&path, "%s/%s", DATADIR, spath);
    _scan;

    /* pkgdatadir. */
    asprintf(&path, "%s/%s", PKGDATADIR, spath);
    _scan;

#undef _scan

    return (QUVI_OK);
}

static const luaL_Reg reg_meth[] = {
    {"fetch",       l_quvi_fetch},
    {"unescape",    l_quvi_unescape},
    {NULL,NULL}
};

static int
lua_files_only (const struct dirent *de)
    { return (de->d_name[0] != '.' && strstr(de->d_name, ".lua") != 0); }

/* init. */

int
init_lua (_quvi_t quvi) {
    QUVIcode rc;

    quvi->lua = (lua_State*) lua_open();
    if (!quvi->lua)
        return (QUVI_LUAINIT);

    luaL_openlibs(quvi->lua);
    luaL_openlib(quvi->lua, "quvi", reg_meth, 1);

    rc = scan_known_dirs (
        &quvi->util_scripts,
        "lua/util",
        lua_files_only
    );

    if (rc != QUVI_OK)
        return (rc);

    if (llst_size(quvi->util_scripts) == 0)
        return (QUVI_NOLUAUTIL);

    rc = scan_known_dirs (
        &quvi->website_scripts,
        "lua/website",
        lua_files_only
    );

    if (rc != QUVI_OK)
        return (rc);

    return (llst_size(quvi->website_scripts) ? QUVI_OK:QUVI_NOLUAWEBSITE);
}

void
free_lua (_quvi_t *quvi) {

#define _rel(w) \
    do { \
        llst_node_t curr = w; \
        while (curr) { \
            _quvi_lua_script_t s = (_quvi_lua_script_t) curr->data; \
            _free(s->basename); \
            _free(s->path); \
            curr = curr->next; \
        } \
    } while(0)

    _rel((*quvi)->util_scripts);
    _rel((*quvi)->website_scripts);
#undef _rel

    llst_free(&(*quvi)->util_scripts);
    assert ((*quvi)->util_scripts == NULL);

    llst_free(&(*quvi)->website_scripts);
    assert ((*quvi)->website_scripts == NULL);

    lua_close((*quvi)->lua);
    (*quvi)->lua = NULL;
}

static void
set_key (lua_State *l, const char *key) {
    lua_pushstring(l, key);
    lua_gettable(l, -2);
}

#define _istype(t) \
    do { \
        if (!lua_is##t(l, -1)) { \
            luaL_error(l, \
                "%s: undefined value for key `%s' in the returned table, " \
                "expected `%s' type value", \
                    qls->path, key, #t); \
        } \
    } while (0)

static char*
get_field_s (lua_State *l, _quvi_lua_script_t qls, const char *key) {
    const char *s;

    set_key(l, key);

    _istype(string);

    s = lua_tostring(l, -1);
    lua_pop(l, 1);

    return ((char*)s);
}

static int
get_field_b (lua_State *l, _quvi_lua_script_t qls, const char *key) {
    int b;

    set_key(l, key);

    _istype(boolean);

    b = lua_toboolean(l, -1);
    lua_pop(l, 1);

    return (b);
}

static QUVIcode
iter_video_url (
    lua_State *l,
    _quvi_lua_script_t qls,
    const char *key,
    _quvi_video_t qv)
{
    QUVIcode rc = QUVI_OK;

    set_key(l, key);

    _istype(table);

    lua_pushnil(l);

    while (lua_next(l, -2) && rc == QUVI_OK) {
        if (!lua_isstring(l, -1))
            luaL_error (l, "expected array to contain only strings");
        rc = add_video_link(&qv->link, "%s", lua_tostring(l, -1));
        lua_pop(l, 1);
    }
    lua_pop(l, 1);

    return (rc);
}

#undef _istype

static void
set_field (lua_State *l, const char *key, const char *value) {
    lua_pushstring(l, key);
    lua_pushstring(l, value);
    lua_settable(l, -3);
}



/* Util scripts. */



/* Finds the specified util script from the list. */

static _quvi_lua_script_t
find_util_script (_quvi_t quvi, const char *basename) {
    llst_node_t curr = quvi->util_scripts;
    while (curr) {
        _quvi_lua_script_t s = (_quvi_lua_script_t) curr->data;
        if (strcmp(s->basename, basename) == 0)
            return (s);
        curr = curr->next;
    }
    return (NULL);
}

/* Executes the `suffix_from_contenttype' lua function. */

QUVIcode
run_lua_suffix_func (_quvi_t quvi, _quvi_video_link_t qvl) {
    const static char func_name[] = "suffix_from_contenttype";
    _quvi_lua_script_t qls;
    lua_State *l;

    assert (quvi != NULL);
    assert (qvl != NULL);

    qls = find_util_script(quvi, "content_type.lua");
    if (!qls)
        return (QUVI_NOLUAUTIL);

    l = quvi->lua;
    assert (l != NULL);

    lua_pushnil(l);
    lua_getglobal(l, func_name);

    if (luaL_dofile(l, qls->path))
        luaL_error(l, "%s", lua_tostring(l,-1));

    lua_getglobal(l, func_name);

    if (!lua_isfunction(l, -1))
        luaL_error(l, "%s: `%s' function not found", qls->path, func_name);

    lua_pushstring(l, qvl->content_type);

    if (lua_pcall(l, 1, 1, 0))
        luaL_error(l, "%s", lua_tostring(l,-1));

    if (lua_isstring(l, -1)) {
        setvid(qvl->suffix, "%s", lua_tostring(l, -1));
    }
    else {
        luaL_error(l,
            "expected `%s' function to return a string", func_name);
    }

    lua_pop(l, 1);

    return (QUVI_OK);
}


/* Executes the `charset_from_data' lua function. */

QUVIcode
run_lua_charset_func (_quvi_video_t video, const char *data) {
    const static char func_name[] = "charset_from_data";
    _quvi_lua_script_t qls;
    _quvi_t quvi;
    lua_State *l;

    assert (video != NULL);
    quvi = video->quvi;
    assert (quvi != NULL);

    qls = find_util_script(quvi, "charset.lua");
    if (!qls)
        return (QUVI_NOLUAUTIL);

    l = quvi->lua;
    assert (l != NULL);

    lua_pushnil(l);
    lua_getglobal(l, func_name);

    if (luaL_dofile(l, qls->path))
        luaL_error(l, "%s", lua_tostring(l,-1));

    lua_getglobal(l, func_name);

    if (!lua_isfunction(l, -1))
        luaL_error(l, "%s: `%s' function not found", qls->path, func_name);

    lua_pushstring(l, data);

    if (lua_pcall(l, 1, 1, 0))
        luaL_error(l, "%s", lua_tostring(l,-1));

    if (lua_isstring(l, -1)) {
        setvid(video->charset, "%s", lua_tostring(l,-1));
    }
    else if (lua_isnil(l, -1)) {
        /* Charset was not found. We can live with that. */
    }
    else {
        luaL_error(l,
            "expected `%s' function to return a string", func_name);
    }

    lua_pop(l, 1);

    return (QUVI_OK);
}



/* Website scripts. */



/* Executes the host script's "ident" function. */

QUVIcode
run_ident_func (lua_ident_t ident, llst_node_t node) {
    _quvi_lua_script_t qls;
    _quvi_t quvi;
    lua_State *l;
    QUVIcode rc;

    assert (ident != NULL);
    assert (node != NULL);

    quvi = ident->quvi;
    assert (quvi != NULL); /* seterr macro uses this. */

    l = quvi->lua;
    assert (l != NULL);

    rc   = QUVI_NOSUPPORT;
    qls  = (_quvi_lua_script_t) node->data;

    lua_pushnil(l);
    lua_pushnil(l);

    lua_setglobal(l, "ident");
    lua_setglobal(l, "parse");

    if (luaL_dofile(l, qls->path)) {
        seterr("%s", lua_tostring(l,-1));
        return (QUVI_LUA);
    }

    lua_getglobal(l, "ident");

    if (!lua_isfunction(l, -1)) {
        seterr("%s: `ident' function not found", qls->path);
        return (QUVI_LUA);
    }

    if (ident->url)
        lua_pushstring(l, ident->url);

    if (lua_pcall(l, ident->url?1:0, 1, 0)) {
        seterr("%s", lua_tostring(l,-1));
        return (QUVI_LUA);
    }

    if (lua_istable(l, -1)) {
        ident->domain  = strdup(get_field_s(l,qls,"domain"));
        ident->formats = strdup(get_field_s(l,qls,"formats"));
        rc = get_field_b(l,qls,"handles")
            ? QUVI_OK
            : QUVI_NOSUPPORT;
    }
    else
        luaL_error(l, "expected `ident' function to return a table");

    lua_pop(l, 1);

    return (rc);
}

/* Executes the host script's "parse" function. */

static QUVIcode
run_parse_func (lua_State *l, llst_node_t node, _quvi_video_t video) {
    _quvi_lua_script_t qls;
    _quvi_t quvi;
    QUVIcode rc;

    assert (node != NULL);
    assert (video != NULL);

    quvi = video->quvi; /* seterr macro needs this. */
    qls  = (_quvi_lua_script_t) node->data;

    lua_getglobal(l, "parse");

    if (!lua_isfunction(l, -1)) {
        seterr("%s: `parse' function not found", qls->path);
        return (QUVI_LUA);
    }

    lua_newtable(l);
    set_field(l, "page_url",         video->page_link);
    set_field(l, "requested_format", video->quvi->format);
    set_field(l, "redirect",         "");
#ifdef _1_
    set_field(l, "host_id",  "");
    set_field(l, "title",    "");
    set_field(l, "id",       "");
    set_field(l, "url",      "");
#endif

    if (lua_pcall(l, 1, 1, 0)) {
        seterr("%s", lua_tostring(l,-1));
        return (QUVI_LUA);
    }

    if (lua_istable(l, -1)) {

        setvid(video->redirect, "%s", get_field_s(l, qls, "redirect"));

        if (strlen(video->redirect) == 0) {
            setvid(video->host_id, "%s", get_field_s(l, qls, "host_id"));
            setvid(video->title,   "%s", get_field_s(l, qls, "title"));
            setvid(video->id,      "%s", get_field_s(l, qls, "id"));
            rc = iter_video_url(l, qls, "url", video);
        }

    }
    else
        luaL_error(l, "expected `ident' function to return a table");

    lua_pop(l, 1);

    return (rc);
}

/* Match host script to the url. */

QUVIcode
find_host_script (_quvi_video_t video) {
    llst_node_t curr;
    _quvi_t quvi;
    lua_State *l;
    QUVIcode rc;

    qv   = video;
    quvi = video->quvi; /* seterr macro uses this. */
    curr = quvi->website_scripts;
    l    = quvi->lua;

    while (curr) {
        struct lua_ident_s ident;

        ident.quvi    = video->quvi;
        ident.url     = video->page_link;
        ident.domain  = NULL;
        ident.formats = NULL;

        /* check if script ident this url. */
        rc = run_ident_func(&ident, curr);

        _free(ident.domain);
        _free(ident.formats);

        if (rc == QUVI_OK) {
            /* found script. */
            return (run_parse_func(l, curr, video));
        }
        else if (rc != QUVI_NOSUPPORT) {
            /* script error. terminate. */
            return (rc);
        }

        curr = curr->next;
    }

    seterr("no support: %s", video->page_link);

    return (QUVI_NOSUPPORT);
}


