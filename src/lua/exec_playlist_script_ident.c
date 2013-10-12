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
#include "_quvi_playlist_s.h"
#include "_quvi_script_s.h"
/* -- */
#include "lua/setfield.h"
#include "lua/chk.h"
#include "lua/def.h"

static const gchar script_func[] = "ident";

static QuviError _chk_results(lua_State *l, _quvi_script_t qs)
{
  QuviError r;

  r = (l_chk_can_parse_url(l, qs, PS_CAN_PARSE_URL,
                           PS_DOMAINS, script_func) == TRUE)
      ? QUVI_OK
      : QUVI_ERROR_NO_SUPPORT;

  lua_pop(l, 1);
  return (r);
}

QuviError l_exec_playlist_script_ident(gpointer p, GSList *sl)
{
  _quvi_playlist_t qp;
  _quvi_script_t qs;
  lua_State *l;

  qp = (_quvi_playlist_t) p;
  l = qp->handle.quvi->handle.lua;

  qs = (_quvi_script_t) sl->data;
  lua_pushnil(l);

  if (luaL_dofile(l, qs->fpath->str))
    luaL_error(l, "%s", lua_tostring(l, -1));

  lua_getglobal(l, script_func);

  if (!lua_isfunction(l, -1))
    {
      static const gchar *_E = "%s: the function `%s' was not found";
      luaL_error(l, _E, qs->fpath->str, script_func);
    }

  lua_newtable(l);
  l_setfield_s(l, PS_INPUT_URL, qp->url.input->str, -1);

  if (lua_pcall(l, 1, 1, 0))
    {
      g_string_assign(qp->handle.quvi->status.errmsg, lua_tostring(l, -1));
      return (QUVI_ERROR_SCRIPT);
    }

  if (!lua_istable(l, -1))
    {
      luaL_error(l, "%s: %s: must return a dictionary",
                 qs->fpath->str, script_func);
    }
  return (_chk_results(l, qs));
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
