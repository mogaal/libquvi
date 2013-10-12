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

static void _chk_opts_common_req(lua_State *l, GSList *opts,
                                 l_quvi_object_crypto_opts_t co)
{
  GSList *p;

  l_quvi_object_opts_chk_req_s(QUVI_OBJECT_OPTION_CRYPTO_ALGORITHM,
                               HRE_ALGORITHM, co->algoname, str);
}

/* Encrypt and decrypt option checks. */
void l_quvi_object_crypto_chk_opts(lua_State *l, GSList *opts,
                                   l_quvi_object_crypto_opts_t co)
{
  GSList *p;

  l_quvi_object_opts_chk_given(l, opts, "crypto");
  _chk_opts_common_req(l, opts, co);

  /* required */

  l_quvi_object_opts_chk_req_s(QUVI_OBJECT_OPTION_CRYPTO_CIPHER_MODE,
                               HRE_CIPHER_MODE, co->cipher.mode, n);

  l_quvi_object_opts_chk_req_s(QUVI_OBJECT_OPTION_CRYPTO_CIPHER_KEY,
                               HRE_CIPHER_KEY, co->cipher.key, str);

  /* optional */

  l_quvi_object_opts_chk_opt_s(QUVI_OBJECT_OPTION_CRYPTO_CIPHER_FLAGS,
                               co->cipher.flags, n);
}

/* Hash option checks. */
void l_quvi_object_crypto_hash_chk_opts(lua_State *l, GSList *opts,
                                        l_quvi_object_crypto_opts_t co)
{
  l_quvi_object_opts_chk_given(l, opts, "crypto");
  _chk_opts_common_req(l, opts, co);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
