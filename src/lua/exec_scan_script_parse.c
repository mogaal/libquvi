/* libquvi
 * Copyright (C) 2012  Toni Gundogdu <legatvs@gmail.com>
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
#include "_quvi_scan_s.h"
#include "_quvi_script_s.h"
/* -- */
#include "lua/setfield.h"
#include "lua/def.h"

static const gchar script_func[] = "parse";

static void _foreach_media_url(lua_State *l, _quvi_scan_t qs,
                               const gchar *script_path)
{
  lua_pushnil(l);
  while (lua_next(l, LI_KEY))
    {
      if (lua_isstring(l, LI_KEY) && lua_isstring(l, LI_VALUE))
        {
          const gchar *url = lua_tostring(l, LI_VALUE);
          qs->url.media = g_slist_prepend(qs->url.media, g_strdup(url));
        }
      lua_pop(l, 1);
    }
  qs->url.media = g_slist_reverse(qs->url.media);
}

/* Check for 'qargs.media_url'. */
static void _chk_media_url(lua_State *l, _quvi_scan_t qs,
                           const gchar *script_path)
{
  lua_pushstring(l, SS_MEDIA_URL);
  lua_gettable(l, LI_KEY);

  if (lua_istable(l, LI_VALUE))
    _foreach_media_url(l, qs, script_path);
  else
    {
      g_warning("%s: %s: should return a dictionary containing "
                "the `qargs.%s'",
                script_path, script_func, SS_MEDIA_URL);
    }
  lua_pop(l, 1);
}

QuviError l_exec_scan_script_parse(gpointer p, gpointer _qss,
                                   const gchar *data)
{
  _quvi_script_t qss;
  _quvi_scan_t qs;
  lua_State *l;

  qss = (_quvi_script_t) _qss;
  qs = (_quvi_scan_t) p;

  l = qs->handle.quvi->handle.lua;
  lua_pushnil(l);

  if (luaL_dofile(l, qss->fpath->str))
    luaL_error(l, "%s", lua_tostring(l, -1));

  lua_getglobal(l, script_func);

  if (!lua_isfunction(l, -1))
    {
      luaL_error(l, "%s: the function `%s' was not found",
                 qss->fpath->str, script_func);
    }

  lua_newtable(l);
  l_set_reg_userdata(l, USERDATA_QUVI_T, (gpointer) qs->handle.quvi);
  l_setfield_s(l, SS_INPUT_URL, qs->url.input->str, -1);
  l_setfield_s(l, SS_CONTENT, data, -1);

  if (lua_pcall(l, 1, 1, 0))
    {
      g_string_assign(qs->handle.quvi->status.errmsg, lua_tostring(l, -1));
      return (QUVI_ERROR_SCRIPT);
    }

  if (!lua_istable(l, -1))
    {
      static const gchar *_E =
        "%s: %s: must return a dictionary, this is typically the `qargs'";

      luaL_error(l, _E, qss->fpath->str, script_func);
    }

  _chk_media_url(l, qs, qss->fpath->str);
  lua_pop(l, 1);

  return (QUVI_OK);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
