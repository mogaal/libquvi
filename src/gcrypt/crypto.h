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

#ifndef crypto_h

typedef enum
{
  CRYPTO_MODE_ENCRYPT,
  CRYPTO_MODE_DECRYPT,
  CRYPTO_MODE_HASH
} CryptoMode;

struct crypto_s
{
  struct
  {
    gboolean should_pad;
    gcry_cipher_hd_t h;
    gsize blklen;
    gsize keylen;
    guint flags;
    gchar *key;
    gint mode;
  } cipher;
  struct
  {
    guchar *data;
    gsize dlen;
  } out;
  CryptoMode mode;
  gchar *errmsg;
  gint algo;
  gint rc;
};

typedef struct crypto_s *crypto_t;

crypto_t crypto_new(const gchar*, const CryptoMode, gchar*,
                    const gint, const guint);
void crypto_free(crypto_t);

gint crypto_exec(crypto_t, const guchar*, const gsize);

void crypto_dump(const gchar*, const guchar*, const gsize);
gboolean crypto_ok(crypto_t);

gchar *crypto_bytes2hex(const guchar*, const gsize);
guchar *crypto_hex2bytes(const gchar*, gsize*);

#endif /* crypto_h */

/* vim: set ts=2 sw=2 tw=72 expandtab: */

