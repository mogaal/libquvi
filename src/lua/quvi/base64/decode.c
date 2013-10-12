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

#include "config.h"

#include <lauxlib.h>
#include <gcrypt.h>
#include <glib.h>
#include <quvi.h>

#include "_quvi_s.h"
/* -- */
#include "gcrypt/crypto.h"
#include "lua/quvi/opts.h"
#include "lua/getfield.h"
#include "lua/setfield.h"
#include "lua/def.h"

gint l_quvi_base64_decode(lua_State *l)
{
  const gchar *data;
  _quvi_t q;

  /* quvi handle */

  q = (_quvi_t) l_get_reg_userdata(l, USERDATA_QUVI_T);
  g_assert(q != NULL);

  /* function args */

  data = luaL_checkstring(l, 1);
  lua_pop(l, 1);

  /* Return a table of results. */

  lua_newtable(l);
  l_setfield_s(l, QO_ERROR_MESSAGE, q->status.errmsg->str, -1);
  l_setfield_n(l, QO_QUVI_CODE, q->status.rc);
  {
    guchar *s;
    gsize n;

    s = g_base64_decode(data, &n);
#ifdef _1
    crypto_dump("s", s, n);
#endif
    l_setfield_s(l, QO_PLAINTEXT, (gchar*) s, n);
    g_free(s);
  }
  return (1); /* no. of returned values (a table) */
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
