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
#include <glib.h>

#include "quvi.h"
/* -- */
#include "lua/getfield.h"

gpointer l_get_reg_userdata(lua_State *l, const gchar *k)
{
  lua_pushstring(l, k);
  lua_gettable(l, LUA_REGISTRYINDEX);

  if (!lua_isuserdata(l, -1))
    luaL_error(l, "expected to find the key `%s' in LUA_REGISTRYINDEX", k);

  return ((gpointer) lua_touserdata(l, -1));
}

#ifdef _1 /* Unused */

static const gchar *_E =
  "%s: %s: expected to find the key `%s' in the returned table";

#define _pop(ltype, ctype, dval) \
  do { \
    ctype r = dval; \
    lua_pushstring(l ,k); \
    lua_gettable(l, -2); \
    if (!lua_is##ltype(l, -1)) \
      luaL_error(l, _E, script_path, script_func, k); \
    r = lua_to##ltype(l, -1); \
    lua_pop(l, 1); \
    return (r); \
  } while (0)

const gchar *l_getfield_s(lua_State *l, const gchar *k,
                          const gchar *script_path,
                          const gchar *script_func)
{
  _pop(string, const gchar*, NULL);
}

const gdouble l_getfield_n(lua_State *l, const gchar *k,
                           const gchar *script_path,
                           const gchar *script_func)
{
  _pop(number, gdouble, 0);
}

const gboolean l_getfield_b(lua_State *l, const gchar *k,
                            const gchar *script_path,
                            const gchar *script_func)
{
  _pop(boolean, gboolean, FALSE);
}

QuviError l_getfield_table_iter_s(lua_State *l,
                                  const gpointer dst,
                                  const gchar *k,
                                  const gchar *script_path,
                                  const gchar *script_func,
                                  l_callback_getfield_table_iter_s cb)
{
  QuviError rc = QUVI_OK;

  lua_pushstring(l, k);
  lua_gettable(l, -2);

  if (!lua_istable(l, -1))
    {
      luaL_error(l, "%s: %s: expected to find the table `%s'",
                 script_path, script_func, k);
    }

  lua_pushnil(l);

  while (lua_next(l, -2) && rc == QUVI_OK)
    {
      rc = (cb)(dst, lua_tostring(l, -1));
      lua_pop(l, 1);
    }

  lua_pop(l, 1);

  return (rc);
}
#endif /* _1, unused */

/* vim: set ts=2 sw=2 tw=72 expandtab: */
