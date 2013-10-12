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

gint l_quvi_base64_encode(lua_State *l)
{
  gboolean croak_if_error;
  const gchar *raw;
  GSList *opts;
  _quvi_t q;
  gchar *r;

  r = NULL;

  /* quvi handle */

  q = (_quvi_t) l_get_reg_userdata(l, USERDATA_QUVI_T);
  g_assert(q != NULL);

  /* function args */

  raw = luaL_checkstring(l, 1);
  lua_pop(l, 1);

  /* options */

  opts = l_quvi_object_opts_new(l, 2);
  croak_if_error = l_quvi_object_opts_croak_if_error(l, opts);

  /* encode */
  {
    guchar *s;
    gsize n;

    s = crypto_hex2bytes(raw, &n);
    if (s != NULL)
      {
#ifdef _1
        crypto_dump("s", s, n);
#endif
        r = g_base64_encode(s, n);
        g_free(s);
      }
    else
      {
        const gchar *m = "invalid hex string value";

        q->status.rc = QUVI_ERROR_CALLBACK;
        if (croak_if_error ==FALSE)
          g_string_assign(q->status.errmsg, m);
        else
          luaL_error(l, "%s", m);
      }
  }

  /* Return a table of results. */

  lua_newtable(l);
  l_setfield_s(l, QO_ERROR_MESSAGE, q->status.errmsg->str, -1);
  l_setfield_n(l, QO_QUVI_CODE, q->status.rc);

  if (r != NULL)
    {
      l_setfield_s(l, QO_BASE64, r, -1);
      g_free(r);
    }

  l_quvi_object_opts_free(opts);
  return (1); /* no. of returned values (a table) */
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
