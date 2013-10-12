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

#ifndef l_quvi_crypto_opts_h
#define l_quvi_crypto_opts_h

struct l_quvi_object_crypto_opts_s
{
  const gchar *algoname;
  const gchar *text; /* multi-purpose: ciphertext, plaintext */
  struct
  {
    guint flags;
    gchar *key;
    gint mode;
  } cipher;
};

typedef struct l_quvi_object_crypto_opts_s *l_quvi_object_crypto_opts_t;

void l_quvi_object_crypto_chk_opts(lua_State*, GSList*,
                                   l_quvi_object_crypto_opts_t);

void l_quvi_object_crypto_hash_chk_opts(lua_State*, GSList*,
                                        l_quvi_object_crypto_opts_t);

#endif /* l_quvi_crypto_opts_h */

/* vim: set ts=2 sw=2 tw=72 expandtab: */
