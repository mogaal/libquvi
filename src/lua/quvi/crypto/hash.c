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
#include "lua/quvi/crypto/opts.h"
#include "lua/quvi/crypto/err.h"
#include "lua/quvi/opts.h"
#include "lua/getfield.h"
#include "lua/setfield.h"
#include "lua/def.h"

gint l_quvi_crypto_hash(lua_State *l)
{
  struct l_quvi_object_crypto_opts_s co;
  gboolean croak_if_error;
  GSList *opts;
  crypto_t c;
  _quvi_t q;

  memset(&co, 0, sizeof(struct l_quvi_object_crypto_opts_s));

  /* quvi handle */

  q = (_quvi_t) l_get_reg_userdata(l, USERDATA_QUVI_T);
  g_assert(q != NULL);

  /* function args */

  co.text = luaL_checkstring(l, 1);
  lua_pop(l, 1);

  /* options */

  opts = l_quvi_object_opts_new(l, 2);
  croak_if_error = l_quvi_object_opts_croak_if_error(l, opts);
  l_quvi_object_crypto_hash_chk_opts(l, opts, &co);

  /* generate message digest (return as a hexstring). */

  c = crypto_new(co.algoname, CRYPTO_MODE_HASH, NULL, -1, -1);
  q->status.rc = l_quvi_object_crypto_chk_if_failed(l, c, croak_if_error, q);
  {
    guchar *s;
    gsize n;

    s = crypto_hex2bytes(co.text, &n);
    if (s != NULL)
      {
#ifdef _1
        crypto_dump("s", s, n);
#endif
        crypto_exec(c, s, n);
        g_free(s);
        q->status.rc =
          l_quvi_object_crypto_chk_if_failed(l, c, croak_if_error, q);
      }
    else
      l_quvi_object_crypto_invalid_hexstr(l, q, croak_if_error);
  }

  /* Return a table of results. */

  lua_newtable(l);
  l_setfield_s(l, QO_ERROR_MESSAGE, q->status.errmsg->str, -1);
  l_setfield_n(l, QO_QUVI_CODE, q->status.rc);

  if (q->status.rc == QUVI_OK)
    {
      gchar *s = crypto_bytes2hex(c->out.data, c->out.dlen);
      l_setfield_s(l, QO_DIGEST, s, -1);
      g_free(s);
    }

  l_quvi_object_opts_free(opts);
  crypto_free(c);

  return (1); /* no. of returned values (a table) */
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
