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

#include <stdlib.h>
#include <gcrypt.h>
#include <glib/gstdio.h>

#include "gcrypt/crypto.h"

static void _fail_set(crypto_t c, gchar *errmsg)
{
  c->errmsg = errmsg;
  c->rc = EXIT_FAILURE;
}

static crypto_t _fail(crypto_t c, gchar *errmsg)
{
  _fail_set(c, errmsg);
  return (c);
}

static gint _failn(crypto_t c, gchar *errmsg)
{
  _fail_set(c, errmsg);
  return (c->rc);
}

static crypto_t _setkey(crypto_t c, gchar *key)
{
  gcry_error_t e;
  gsize keylen;
#ifdef _1
  g_message("[%s] key=%s", __func__, key);
#endif
  c->cipher.key = (gchar*) crypto_hex2bytes(key, &keylen);
  if (c->cipher.key == NULL || keylen ==0)
    {
      return (_fail(c,
                    g_strdup("crypto_hex2bytes failed: invalid "
                             "hexadecimal string length")));
    }
#ifdef _1
  crypto_dump("c->cipher.key", (guchar*) c->cipher.key, keylen);
#endif
  c->cipher.keylen = gcry_cipher_get_algo_keylen(c->algo);
  if (c->cipher.keylen ==0)
    {
      return (_fail(c, g_strdup_printf("gcry_cipher_get_algo_keylen failed "
                                       "c->cipher.keylen=%" G_GSIZE_FORMAT", "
                                       "keylen=%" G_GSIZE_FORMAT,
                                       c->cipher.keylen, keylen)));
    }

#ifdef _1
  if (keylen > c->cipher.keylen)
    {
      g_warning("key length exceeds %lu, ignoring the exceeding bytes",keylen);
      keylen = c->cipher.keylen;
    }
#endif

  e = gcry_cipher_setkey(c->cipher.h, c->cipher.key, keylen);
  if (e != 0)
    {
      return (_fail(c,
                    g_strdup_printf("gcry_cipher_setkey failed: %s",
                                    gpg_strerror(e))));
    }
  c->rc = EXIT_SUCCESS;
  return (c);
}

static crypto_t _cipher_new(crypto_t c, gchar *key,
                            const gchar *algoname,
                            const gint cipher_mode,
                            const guint cipher_flags)
{
  gcry_error_t e;

  c->algo = gcry_cipher_map_name(algoname);
  if (c->algo ==0)
    {
      return (_fail(c, g_strdup_printf("algorithm `%s' was not available",
                                       algoname)));
    }

  c->cipher.flags = cipher_flags;
  c->cipher.mode = cipher_mode;

  c->cipher.should_pad = (cipher_mode != GCRY_CIPHER_MODE_STREAM
                          && cipher_mode != GCRY_CIPHER_MODE_CFB
                          && cipher_mode != GCRY_CIPHER_MODE_OFB)
                         ? TRUE
                         : FALSE;

  c->cipher.blklen = gcry_cipher_get_algo_blklen(c->algo);
  if (c->cipher.blklen ==0)
    return (_fail(c, g_strdup("gcry_cipher_get_algo_blklen failed")));

  e = gcry_cipher_open(&c->cipher.h, c->algo, cipher_mode, cipher_flags);
  if (e != 0)
    {
      return (_fail(c, g_strdup_printf("gcry_cipher_open failed: %s",
                                       gpg_strerror(e))));
    }
  return (_setkey(c, key));
}

static crypto_t _hash_new(crypto_t c, const gchar *algoname)
{
  c->algo = gcry_md_map_name(algoname);
  if (c->algo ==0)
    {
      return (_fail(c, g_strdup_printf("algorithm `%s' was not available",
                                       algoname)));
    }
  return (c);
}

crypto_t crypto_new(const gchar *algoname, const CryptoMode crypto_mode,
                    gchar *key, const gint cipher_mode,
                    const guint cipher_flags)
{
  crypto_t c = g_new0(struct crypto_s, 1);
  c->mode = crypto_mode;
  if (crypto_mode != CRYPTO_MODE_HASH)
    _cipher_new(c, key, algoname, cipher_mode, cipher_flags);
  else
    _hash_new(c, algoname);
  return (c);
}

void crypto_free(crypto_t c)
{
  if (c == NULL)
    return;

  if (c->cipher.h != NULL)
    gcry_cipher_close(c->cipher.h);
  c->cipher.h = NULL;

  g_free(c->cipher.key);
  c->cipher.key = NULL;

  g_free(c->out.data);
  c->out.data = NULL;

  g_free(c->errmsg);
  c->errmsg = NULL;

  g_free(c);
  c = NULL;
}

gboolean crypto_ok(crypto_t c)
{
  return ((c->rc == EXIT_SUCCESS) ? TRUE:FALSE);
}

static gint _encrypt_blk(crypto_t c, const guchar *data, const gsize dlen,
                         guchar *out /* size: c->blklen */)
{
  gcry_error_t e;
  gpointer p;

  memcpy(out, data, dlen);

  /* Pad to block length. */
  if (c->cipher.should_pad == TRUE && dlen < c->cipher.blklen)
    {
      gsize i = dlen;
      out[i++] = 0x80;
      while (i < c->cipher.blklen)
        out[i++] = 0x00;
    }

  /* in-place */
  e = gcry_cipher_encrypt(c->cipher.h, out, c->cipher.blklen, NULL, 0);
  if (e != 0)
    {
      return (_failn(c,
                     g_strdup_printf("gcry_cipher_encrypt failed: %s",
                                     gpg_strerror(e))));
    }

  /* Append. */
  p = g_realloc(c->out.data, c->out.dlen + c->cipher.blklen);
  if (p != NULL)
    {
      c->out.data = p;
      memcpy(&(c->out.data[c->out.dlen]), out, c->cipher.blklen);
      c->out.dlen += c->cipher.blklen;
    }
  return (EXIT_SUCCESS);
}

static gint _decrypt_blk(crypto_t c, const guchar *data, const gsize dlen,
                         guchar *out /* size: c->blk_len */)
{
  gcry_error_t e;
  gpointer p;
  gsize i, n;

  memcpy(out, data, dlen);

  /* in-place */
  e = gcry_cipher_decrypt(c->cipher.h, out, c->cipher.blklen, NULL, 0);
  if (e != 0)
    {
      return (_failn(c,
                     g_strdup_printf("gcry_cipher_decrypt failed: %s",
                                     gpg_strerror(e))));
    }

  n = c->cipher.blklen;

  if (c->cipher.should_pad == TRUE)
    {
      for (i=0; i<n; ++i) /* Strip padding. */
        {
          if (out[i] == 0x80
              && (i >0 || i ==n || (i<n && out[i+1] == 0x00)) )
            {
              n = i;
              break;
            }
        }
    }

  p = g_realloc(c->out.data, c->out.dlen+n); /* Append. */
  if (p != NULL)
    {
      c->out.data = p;
      memcpy(&(c->out.data[c->out.dlen]), out, n);
      c->out.dlen += n;
    }
  return (EXIT_SUCCESS);
}

static gint _setiv(crypto_t c)
{
  const gcry_error_t e = gcry_cipher_setiv(c->cipher.h, NULL, 0);
  if (e != 0)
    {
      return (_failn(c, g_strdup_printf("gcry_cipher_setiv failed: %s",
                                        gpg_strerror(e))));
    }
  return (EXIT_SUCCESS);
}

typedef gint (*blk_cb)(crypto_t, const guchar*, const gsize, guchar*);

static gint _cipher_exec(crypto_t c, const guchar *data, const gsize size)
{
  guchar *out;
  gsize n, m;
  blk_cb cb;
  gint r;

  if (_setiv(c) != EXIT_SUCCESS)
    return (EXIT_FAILURE);

  cb = (c->mode == CRYPTO_MODE_ENCRYPT)
       ? _encrypt_blk
       : _decrypt_blk;

  g_assert(c->out.data == NULL);
  g_assert(c->out.dlen == 0);

  m = size - (size % c->cipher.blklen);
  out = g_malloc0(c->cipher.blklen);
  r = EXIT_SUCCESS;

  n = 0;
  while ( (n < m) && (r == EXIT_SUCCESS) )
    {
      r = cb(c, (guchar*) data+n, c->cipher.blklen, out);
      n += c->cipher.blklen;
    }

  if ( (size % c->cipher.blklen) && (r == EXIT_SUCCESS) )
    r = cb(c, (guchar*) data+n, (size % c->cipher.blklen), out);

  g_free(out);
  c->rc = r;

  return (r);
}

gint _hash_exec(crypto_t c, const guchar *data, const gsize size)
{
  guint dlen;
  gsize n;

  dlen = gcry_md_get_algo_dlen(c->algo);
  n = dlen * sizeof(guchar);

  c->out.data = g_malloc0(n);
  c->out.dlen = n;

  gcry_md_hash_buffer(c->algo, c->out.data, data, size);
#ifdef _1
  g_message("[%s] algo=%s", __func__, gcry_md_algo_name(c->algo));
  g_message("[%s] dlen=%u, c->out.dlen=%lu", __func__, dlen, c->out.dlen);
#endif
  return (EXIT_SUCCESS);
}

gint crypto_exec(crypto_t c, const guchar *data, const gsize size)
{
  g_assert(data != NULL);
  g_assert(size >0);

  g_assert(c->out.data == NULL);
  g_assert(c->out.dlen == 0);

  if (c->mode != CRYPTO_MODE_HASH)
    return (_cipher_exec(c, data, size));

  return (_hash_exec(c, data, size));
}

void crypto_dump(const gchar *w, const guchar *p, const gsize n)
{
  gsize i = 0;
  g_print("%s=", w);
  while (i<n)
    g_print("%02x", p[i++] & 0xff);
  g_print(" =(%" G_GSIZE_FORMAT ")\n", i);
}

guchar *crypto_hex2bytes(const gchar *hexstr, gsize *size)
{
  const gchar *p;
  guint n, b;
  guchar *r;

  *size = 0;

  n = strlen(hexstr);
  if ( (n % 2) ==1)
    return (NULL); /* Invalid hexadecimal string length. */

  r = g_malloc0_n(n/2, sizeof(guchar));
  p = hexstr;

  while ( *p && (n = sscanf((gchar*) p, "%02x", &b)) == 1)
    {
      r[(*size)++] = b;
      p += 2;
    }
  return (r);
}

gchar *crypto_bytes2hex(const guchar *data, const gsize n)
{
  GString *s;
  gchar *r;
  gsize i;

  g_assert(data != NULL);
  g_assert(n>0);
#ifdef _1
  crypto_dump("data", data, n);
#endif
  s = g_string_new(NULL);
  i = 0;

  while (i<n)
    g_string_append_printf(s, "%02x", data[i++] & 0xff);

  r = s->str;
  g_string_free(s, FALSE);

  return (r);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
