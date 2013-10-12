/* libquvi
 * Copyright (C) 2013  Toni Gundogdu <legatvs@gmail.com>
 *
 * This file is part of libquvi <http://quvi.sourceforge.net/>.
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General
 * Public License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

/*
 * NOTE: The error messages produced in these functions are intended for
 *       developers. They would typically be seen when a new script is
 *       being developed or an old one is being maintained.
 *
 *       These messages should be clear, indicating the actual error,
 *       minimizing the time spent on locating the problem in the script.
 */

#include "config.h"

#include <lauxlib.h>
#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_subtitle_s.h"
#include "_quvi_script_s.h"
/* -- */
#include "misc/subtitle.h"
#include "lua/setfield.h"
#include "lua/chk.h"
#include "lua/def.h"

static const gchar script_func[] = "parse";

/* Verify that the language has an ID (if there are >1 languages). */
static void _chk_lang_id(lua_State *l, _quvi_subtitle_type_t qst,
                         _quvi_subtitle_lang_t qsl, const gchar *script_path,
                         const gint i)
{
  if (g_slist_length(qst->languages) <2)
    return;

  if (qsl->id->len ==0)
    {
      g_warning("%s: %s: `qargs.%s[%d].%s' should not be empty; "
                "each language should have an ID when there are >1 languages",
                script_path, script_func, SUS_SUBTITLES, i, SUSSL_ID);
    }
}

typedef void (*foreach_cb)(lua_State*, _quvi_subtitle_type_t,
                           const gchar*, const gint);

/* Process a "sublevel" of a dictionary, e.g. qargs.subtitles.lang */
static void _chk_subtitle_sublevel(lua_State *l, const gchar *level,
                                   _quvi_subtitle_type_t qst,
                                   const gchar *script_path,
                                   const foreach_cb cb, const gint i)
{
  if (lua_isstring(l, LI_KEY) && lua_istable(l, LI_VALUE))
    {
      if (g_strcmp0(lua_tostring(l, LI_KEY), level) ==0)
        cb(l, qst, script_path, i);
    }
}

/* Return a new language handle. */
static gpointer _subtitle_lang_new(_quvi_subtitle_type_t qst)
{
  _quvi_subtitle_lang_t qsl = g_new0(struct _quvi_subtitle_lang_s, 1);
  /* Handle. */
  qsl->handle.quvi = qst->handle.quvi;
  /* Strings. */
  qsl->translated = g_string_new(NULL);
  qsl->original = g_string_new(NULL);
  qsl->code = g_string_new(NULL);
  qsl->url = g_string_new(NULL);
  qsl->id = g_string_new(NULL);
  /* double */
  qsl->format = qst->format;
  return (qsl);
}

/* Return a new data handle. */
static gpointer _subtitle_type_new(const _quvi_subtitle_t qsub)
{
  _quvi_subtitle_type_t qst = g_new0(struct _quvi_subtitle_type_s, 1);
  /* Handle. */
  qst->handle.quvi = qsub->handle.quvi;
  /* double. */
  qst->format = -1;
  qst->type = -1;
  return (qst);
}

/* Must have at least one language URL. */
static _quvi_subtitle_lang_t _chk_url(lua_State *l, const gchar *script_path,
                                      _quvi_subtitle_lang_t qsl, gint i)
{
  if (qsl->url->len ==0)
    {
      m_subtitle_lang_free(qsl);

      luaL_error(l, "%s: %s: must return `qargs.%s[%d].%s[1].%s'",
                 script_path, script_func, SUS_SUBTITLES, i,
                 SUSS_LANG, SUSSL_URL);
    }
  return (qsl);
}

/* Return a new lang handle. */
static _quvi_subtitle_lang_t _new_lang(lua_State *l, const gchar *script_path,
                                       const _quvi_subtitle_type_t qst,
                                       const gint i)
{
  _quvi_subtitle_lang_t qsl = _subtitle_lang_new(qst);
  lua_pushnil(l);
  while (lua_next(l, LI_KEY))
    {
      l_chk_assign_s(l, SUSSL_TRANSLATED, qsl->translated, TRUE);
      l_chk_assign_s(l, SUSSL_ORIGINAL, qsl->original, TRUE);
      l_chk_assign_s(l, SUSSL_CODE, qsl->code, TRUE);
      l_chk_assign_s(l, SUSSL_URL, qsl->url, TRUE);
      l_chk_assign_s(l, SUSSL_ID, qsl->id, TRUE);
      lua_pop(l, 1);
    }
  return (_chk_url(l, script_path, qsl, i));
}

/* For each qargs.subtitles.lang property. */
static void _foreach_lang_property(lua_State *l, _quvi_subtitle_type_t qst,
                                   const gchar *script_path, const gint i)
{
  _quvi_subtitle_lang_t qsl;
  gint j;

  lua_pushnil(l);
  j = 0;

  while (lua_next(l, LI_KEY))
    {
      if (lua_istable(l, LI_VALUE))
        {
          qsl = _new_lang(l, script_path, qst, ++j);
          _chk_lang_id(l, qst, qsl, script_path, j);
          qst->languages = g_slist_prepend(qst->languages, qsl);
        }
      lua_pop(l, 1);
    }
}

/* Verify required subtitle data properties. */
static gpointer _chk_req_type(lua_State *l, _quvi_subtitle_type_t qst,
                              const gint i, const gchar *script_path)
{
  static const gchar *_E = "%s: %s: must return `qargs.%s[%d].%s'";

  if (qst->format <0)
    {
      luaL_error(l, _E, script_path, script_func,
                 SUS_SUBTITLES, i, SUSS_FORMAT);
    }

  if (qst->type <0)
    {
      luaL_error(l, _E, script_path, script_func,
                 SUS_SUBTITLES, i, SUSS_TYPE);
    }

  /* Not all videos have subtitles of any kind. */
  if (g_slist_length(qst->languages) ==0)
    {
      m_subtitle_type_free(qst);
      return (NULL);
    }

#ifdef _1
  if (g_slist_length(qst->languages) ==0)
    {
      luaL_error(l, "%s: %s: qargs.%s[%d].%s must not be empty",
                 script_path, script_func, SUS_SUBTITLES, i, SUSS_LANG);
    }
#endif

  qst->languages = g_slist_reverse(qst->languages);
  return (qst);
}

/* Return a new data handle. */
static _quvi_subtitle_type_t _new_type(lua_State *l, _quvi_subtitle_t qsub,
                                       const gchar *script_path, const gint i)
{
  _quvi_subtitle_type_t qst = _subtitle_type_new(qsub);
  lua_pushnil(l);
  while (lua_next(l, LI_KEY))
    {
      _chk_subtitle_sublevel(l, SUSS_LANG, qst, script_path,
                             _foreach_lang_property, i);
      l_chk_assign_n(l, SUSS_FORMAT, &qst->format);
      l_chk_assign_n(l, SUSS_TYPE, &qst->type);
      lua_pop(l, 1);
    }
  return (_chk_req_type(l, qst, i, script_path));
}

/* For each qargs.subtitle */
static void _foreach_subtitle(lua_State *l, _quvi_subtitle_t qsub,
                              const gchar *script_path)
{
  _quvi_subtitle_type_t qst;
  gint i;

  lua_pushnil(l);
  i = 0;

  while (lua_next(l, LI_KEY))
    {
      if (lua_istable(l, LI_VALUE))
        {
          qst = _new_type(l, qsub, script_path, ++i);
          if (qst != NULL)
            qsub->types = g_slist_prepend(qsub->types, qst);
        }
      lua_pop(l, 1);
    }
  qsub->types = g_slist_reverse(qsub->types);
}

static void _chk_subtitles(lua_State *l, _quvi_subtitle_t qsub,
                           const gchar *script_path)
{
  lua_pushstring(l, SUS_SUBTITLES);
  lua_gettable(l, LI_KEY);

  if (lua_istable(l, LI_VALUE))
    _foreach_subtitle(l, qsub, script_path);
  else
    {
      static const gchar *_E =
        "%s: %s: must return a dictionary containing the `qargs.%s'";

      luaL_error(l, _E, script_path, script_func, SUS_SUBTITLES);
    }
  lua_pop(l, 1);
}

extern gint c_reset(_quvi_t);

QuviError l_exec_subtitle_script_parse(gpointer p, GSList *sl)
{
  _quvi_subtitle_t qsub;
  _quvi_script_t qs;
  lua_State *l;

  qsub = (_quvi_subtitle_t) p;
  l = qsub->handle.quvi->handle.lua;

  c_reset(qsub->handle.quvi);

  qs = (_quvi_script_t) sl->data;
  lua_getglobal(l, script_func);

  if (!lua_isfunction(l, -1))
    {
      luaL_error(l, "%s: the function `%s' was not found",
                 qs->fpath->str, script_func);
    }

  lua_newtable(l);
  l_set_reg_userdata(l, USERDATA_QUVI_T, (gpointer) qsub->handle.quvi);
  l_setfield_s(l, SUS_INPUT_URL, qsub->url.input->str, -1);

  if (lua_pcall(l, 1, 1, 0))
    {
      g_string_assign(qsub->handle.quvi->status.errmsg, lua_tostring(l, -1));
      return (QUVI_ERROR_SCRIPT);
    }

  if (!lua_istable(l, -1))
    {
      static const gchar *_E =
        "%s: %s: must return a dictionary, this is typically the `qargs'";

      luaL_error(l, _E, qs->fpath->str, script_func);
    }

  _chk_subtitles(l, qsub, qs->fpath->str);
  lua_pop(l, 1);

  return (QUVI_OK);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
