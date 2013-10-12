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

#include "config.h"

#include <lauxlib.h>
#include <lualib.h>
#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_media_s.h"
/* -- */
#include "lua/load_util_script.h"

static const gchar script_fname[]= "convert_entities.lua";
static const gchar script_func[] = "convert_entities";

/* Convert HTML entities in the media title. */
QuviError l_exec_util_convert_entities(_quvi_media_t qm)
{
  lua_State *l;
  QuviError rc;
  _quvi_t q;

  q = qm->handle.quvi;
  rc = l_load_util_script(q, script_fname, script_func);

  if (rc != QUVI_OK)
    return (rc);

  l = q->handle.lua;
  lua_pushstring(l, qm->title->str);

  /*
   * 2=qargs,string [qargs: set in l_load_util_script]
   * 1=returns a string
   */
  if (lua_pcall(l, 2, 1, 0))
    {
      g_string_assign(q->status.errmsg, lua_tostring(l, -1));
      return (QUVI_ERROR_SCRIPT);
    }

  if (!lua_isstring(l, -1))
    luaL_error(l, "%s: did not return a string", script_func);

  g_string_assign(qm->title, lua_tostring(l, -1));
  lua_pop(l, 1);

  return (QUVI_OK);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
