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
#include "lua/def.h"

void
l_quvi_object_crypto_invalid_hexstr(lua_State *l, _quvi_t q,
                                    const gboolean croak_if_error)
{
  g_string_assign(q->status.errmsg, "invalid hex string value");
  q->status.rc = QUVI_ERROR_CALLBACK;

  if (croak_if_error ==TRUE)
    luaL_error(l, "%s", q->status.errmsg->str);
}

QuviError
l_quvi_object_crypto_chk_if_failed(lua_State *l, const crypto_t c,
                                   const gboolean croak_if_error, _quvi_t q)
{
  if (crypto_ok(c) == TRUE)
    return (QUVI_OK);

  g_string_assign(q->status.errmsg, c->errmsg);

  if (croak_if_error ==TRUE)
    luaL_error(l, "%s", c->errmsg);

  return (QUVI_ERROR_CALLBACK);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
