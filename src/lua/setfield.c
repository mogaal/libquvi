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

#include <glib.h>
#include <lua.h>

#define _push(t,a) \
  do { \
    lua_pushstring(l,k); \
    lua_push##t(l,a); \
    lua_settable(l,-3); \
  } while (0)

void l_setfield_s(lua_State *l, const gchar *k, const gchar *s, const gint n)
{
  if (n >0)
    {
      lua_pushstring(l,k);
      lua_pushlstring(l,s,n);
      lua_settable(l,-3);
      return;
    }
  _push(string, s);
}

void l_setfield_n(lua_State *l, const gchar *k, const glong n)
{
  _push(number, n);
}

void l_setfield_b(lua_State *l, const gchar *k, const gint b)
{
  _push(boolean, b);
}

void l_set_reg_userdata(lua_State *l, const gchar *k, gpointer p)
{
  lua_pushstring(l, k);
  lua_pushlightuserdata(l, p);
  lua_settable(l, LUA_REGISTRYINDEX);
}

#ifdef _1 /* Unused */
void l_set_luserdata(lua_State *l, const gchar *k, gpointer p)
{
  _push(lightuserdata, p);
}
#endif

/* vim: set ts=2 sw=2 tw=72 expandtab: */
