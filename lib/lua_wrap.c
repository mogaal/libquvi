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
static char *script_path = NULL;

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

        lua_pop(l, 2);
    }
    lua_pop(l, 1);

    rc = fetch_to_mem(qv, url, st, &data);

    if (rc == QUVI_OK) {
        luaL_buffinit(l, &b);
        luaL_addstring(&b, data);
        luaL_pushresult(&b);
        _free(data);
    }
    else
        luaL_error(l, qv->quvi->errmsg);

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

static QUVIcode
new_lua_script (_quvi_lua_script_t *dst) {
    struct _quvi_lua_script_s *qls;

    qls = calloc(1, sizeof(*qls));
    if (!qls)
        return (QUVI_MEM);

    *dst = qls;

    return (QUVI_OK);
}

/* find lua scripts. */

static QUVIcode
scan_lua_scripts (_quvi_t quvi, const char *path) {
    struct dirent *de;
    DIR *dir;

    dir = opendir(path);
    if (!dir)
        return (QUVI_OK);

    while ( (de = readdir(dir)) ) {
        char *suffix;
        QUVIcode rc;

        rc = regexp_capture(
            quvi,
            de->d_name,
            "\\.(.*)$",
            0,
            0,
            &suffix,
            (void *) 0
        );

        if (rc != QUVI_OK)
            continue;

        if (strcmp(suffix, "lua") == 0) {
            _quvi_lua_script_t qls;

            rc = new_lua_script(&qls);
            if (rc != QUVI_OK)
                return (rc);

            asprintf((char**)&qls->basename, "%s", de->d_name);
            asprintf((char**)&qls->path, "%s/%s", path, de->d_name);

            llst_add(&quvi->scripts, qls);
        }

        _free(suffix);
    }

    closedir(dir);

    return (QUVI_OK);
}

static QUVIcode
find_lua_scripts (_quvi_t quvi) {
    static const char luawebsite_dir[] = "lua/website";
    char *homedir, *path, *basedir;
    char buf[PATH_MAX];
    QUVIcode rc;

    path = NULL;

    /* QUVI_BASEDIR. */
    basedir = getenv("QUVI_BASEDIR");
    if (basedir) {
        asprintf(&path, "%s/%s", basedir, luawebsite_dir);
        rc = scan_lua_scripts(quvi, path);
        _free(path);

        if (rc != QUVI_OK)
            return (rc);
    }

    /* Current working directory. */
    asprintf(&path, "%s/%s", getcwd(buf,sizeof(buf)), luawebsite_dir);
    rc = scan_lua_scripts(quvi, path);
    _free(path);

    if (rc != QUVI_OK)
        return (rc);

    /* Home directory. */
    homedir = getenv("HOME");
    if (homedir) {
        asprintf(&path, "%s/.quvi/%s", homedir, luawebsite_dir);
        rc = scan_lua_scripts(quvi, path);
        _free(path);

        if (rc != QUVI_OK)
            return (rc);

        asprintf(&path, "%s/.config/quvi/%s", homedir, luawebsite_dir);
        rc = scan_lua_scripts(quvi, path);
        _free(path);

        if (rc != QUVI_OK)
            return (rc);

        asprintf(&path, "%s/.local/share/quvi/%s", homedir, luawebsite_dir);
        rc = scan_lua_scripts(quvi, path);
        _free(path);

        if (rc != QUVI_OK)
            return (rc);
    }

    /* --datadir. */
    asprintf(&path, "%s/%s", DATADIR, luawebsite_dir);
    rc = scan_lua_scripts(quvi, path);
    _free(path);

    if (rc != QUVI_OK)
        return (rc);

    /* pkgdatadir. */
    asprintf(&path, "%s/%s", PKGDATADIR, luawebsite_dir);
    rc = scan_lua_scripts(quvi, path);
    _free(path);

    return (rc);
}

static const luaL_Reg reg_meth[] = {
    {"fetch",       l_quvi_fetch},
    {"unescape",    l_quvi_unescape},
    {NULL,NULL}
};

/* init. */

int
init_lua (_quvi_t quvi) {
    QUVIcode rc;

    quvi->lua = (lua_State*) lua_open();
    if (!quvi->lua)
        return (QUVI_LUAINIT);

    luaL_openlibs(quvi->lua);
    luaL_openlib(quvi->lua, "quvi", reg_meth, 1);

    rc = find_lua_scripts(quvi);
    if (rc != QUVI_OK)
        return (rc);

    return (llst_size(quvi->scripts) ? QUVI_OK:QUVI_NOLUAWEBSITE);
}

void
free_lua (_quvi_t *quvi) {
    llst_node_t curr = (*quvi)->scripts;
    while (curr) {
        _quvi_lua_script_t s = (_quvi_lua_script_t) curr->data;
        _free(s->basename);
        _free(s->path);
        curr = curr->next;
    }
    llst_free(&(*quvi)->scripts);

    lua_close((*quvi)->lua);
    (*quvi)->lua = NULL;
}

static void
set_key (lua_State *l, const char *key) {
    lua_pushstring(l, key);
    lua_gettable(l, -2);
}

#define _istype(t) \
    if (!lua_is##t(l, -1)) \
        luaL_error(l, "##t type `%s' key not found (or is nil) in table", key);

static char*
get_field_s (lua_State *l, const char *key) {
    const char *s;

    set_key(l, key);

    _istype(string)

    s = lua_tostring(l, -1);
    lua_pop(l, 1);

    return ((char*)s);
}

static int
get_field_b (lua_State *l, const char *key) {
    int b;

    set_key(l, key);

    _istype(boolean)

    if (!lua_isboolean(l, -1))
        luaL_error(l, "boolean type `%s' key not found in table", key);

    b = lua_toboolean(l, -1);
    lua_pop(l, 1);

    return (b);
}

static QUVIcode
iter_video_url (lua_State *l, const char *key, _quvi_video_t qv) {
    QUVIcode rc = QUVI_OK;

    set_key(l, key);

    _istype(table)

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
    script_path = qls->path;

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
        ident->domain  = strdup(get_field_s(l,"domain"));
        ident->formats = strdup(get_field_s(l,"formats"));
        rc = get_field_b(l,"will_handle")
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
        setvid(video->host_id, "%s", get_field_s(l, "host_id"));
        setvid(video->title,   "%s", get_field_s(l, "title"));
        setvid(video->id,      "%s", get_field_s(l, "id"));
        rc = iter_video_url(l, "url", video);
    }
    else
        luaL_error(l, "expected `ident' function to return a table");

    lua_pop(l, 1);

    return (rc);
}

/* find handler host script for the url. */

QUVIcode
find_host_script (_quvi_video_t video) {
    llst_node_t curr;
    _quvi_t quvi;
    lua_State *l;
    QUVIcode rc;

    qv   = video;
    quvi = video->quvi; /* seterr macro uses this. */
    curr = quvi->scripts;
    l    = quvi->lua;

    while (curr) {
        struct lua_ident_s ident;

        ident.quvi    = video->quvi;
        ident.url     = video->page_link;
        ident.domain  = NULL;
        ident.formats = NULL;

        /* check if script ident this url. */
        rc = run_ident_func(&ident, curr);
        if (rc == QUVI_OK) {
            /* found script. */
            _free(ident.domain);
            _free(ident.formats);
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


