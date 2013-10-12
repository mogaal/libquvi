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

#include "quvi.h"
/* -- */
#include "_quvi_s.h"

/* Append a new path to the "package.path" string. */
void l_modify_pkgpath(_quvi_t q, const gchar *path)
{
  lua_State *l = q->handle.lua;

  lua_getglobal(l, "package");
  lua_getfield(l, -1, "path");
  {
    GString *s = g_string_new(lua_tostring(l, -1));
    g_string_append_printf(s, ";%s/?.lua", path);

    lua_pop(l, 1);
    lua_pushstring(l, s->str);
    lua_setfield(l, -2, "path");
    lua_pop(l, 1);

    g_string_free(s, TRUE);
    s = NULL;
  }
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
