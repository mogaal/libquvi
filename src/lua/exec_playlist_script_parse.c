/* libquvi
 * Copyright (C) 2012,2013  Toni Gundogdu <legatvs@gmail.com>
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
#include "_quvi_playlist_s.h"
#include "_quvi_script_s.h"
/* -- */
#include "lua/setfield.h"
#include "lua/chk.h"
#include "lua/def.h"
#include "misc/playlist.h"

static const gchar script_func[] = "parse";

static gpointer _playlist_media_new()
{
  _quvi_playlist_media_t qpm = g_new0(struct _quvi_playlist_media_s, 1);
  qpm->title = g_string_new(NULL);
  qpm->url = g_string_new(NULL);
  return (qpm);
}

static gboolean _new_media(lua_State *l, _quvi_playlist_t qp,
                           const gchar *script_path, const gint i,
                           _quvi_playlist_media_t *qpm)
{
  *qpm = _playlist_media_new();

  lua_pushnil(l);
  while (lua_next(l, LI_KEY)) /* For each qargs.media */
    {
      l_chk_assign_n(l, PSM_DURATION_MS, &(*qpm)->duration_ms);
      l_chk_assign_s(l, PSM_TITLE, (*qpm)->title, TRUE);
      l_chk_assign_s(l, PSM_URL, (*qpm)->url, TRUE);
      lua_pop(l, 1);
    }

  if ((*qpm)->url->len ==0)
    {
      m_playlist_media_free(*qpm);
      *qpm = NULL;
    }

  return ( ((*qpm)->url->len >0) ? TRUE:FALSE);
}

/* For each qargs.media */
static void _foreach_media(lua_State *l, _quvi_playlist_t qp,
                           const gchar *script_path)
{
  _quvi_playlist_media_t qpm;
  gint i;

  i = 0;

  lua_pushnil(l);
  while (lua_next(l, LI_KEY))
    {
      if (lua_istable(l, LI_VALUE))
        {
          if (_new_media(l, qp, script_path, ++i, &qpm) == TRUE)
            qp->media = g_slist_prepend(qp->media, qpm);
        }
      lua_pop(l, 1);
    }
  qp->media = g_slist_reverse(qp->media);
}

/* Check for qargs.media */
static void _chk_media(lua_State *l, _quvi_playlist_t qp,
                       const gchar *script_path)
{
  lua_pushstring(l, PS_MEDIA);
  lua_gettable(l, LI_KEY);

  if (lua_istable(l, LI_VALUE))
    _foreach_media(l, qp, script_path);
  else
    {
      g_warning("%s: %s: should return a dictionary containing "
                "the `qargs.%s' dictionary",
                script_path, script_func, PS_MEDIA);
    }
  lua_pop(l, 1);
}

static void _chk_optional(lua_State *l, _quvi_playlist_t qp)
{
  lua_pushnil(l);
  while (lua_next(l, LI_KEY))
    {
      l_chk_assign_s(l, PS_THUMB_URL, qp->url.thumbnail, TRUE);
      l_chk_assign_s(l, PS_ID, qp->id.playlist, TRUE);
      l_chk_assign_s(l, PS_TITLE, qp->title, TRUE);
      lua_pop(l, 1);
    }
}

extern gint c_reset(_quvi_t);

QuviError l_exec_playlist_script_parse(gpointer p, GSList *sl)
{
  _quvi_playlist_t qp;
  _quvi_script_t qs;
  lua_State *l;

  qp = (_quvi_playlist_t) p;
  l = qp->handle.quvi->handle.lua;

  c_reset(qp->handle.quvi);

  qs = (_quvi_script_t) sl->data;
  lua_getglobal(l, script_func);

  if (!lua_isfunction(l, -1))
    {
      luaL_error(l, "%s: the function `%s' was not found",
                 qs->fpath->str, script_func);
    }

  lua_newtable(l);
  l_set_reg_userdata(l, USERDATA_QUVI_T, (gpointer) qp->handle.quvi);
  l_setfield_s(l, PS_INPUT_URL, qp->url.input->str, -1);

  if (lua_pcall(l, 1, 1, 0))
    {
      g_string_assign(qp->handle.quvi->status.errmsg, lua_tostring(l, -1));
      return (QUVI_ERROR_SCRIPT);
    }

  if (!lua_istable(l, -1))
    {
      static const gchar *_E =
        "%s: %s: must return a dictionary, this is typically the `qargs'";

      luaL_error(l, _E, qs->fpath->str, script_func);
    }

  _chk_optional(l, qp);
  _chk_media(l, qp, qs->fpath->str);

  lua_pop(l, 1);
  return (QUVI_OK);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
