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

#ifndef l_quvi_opts_h
#define l_quvi_opts_h

typedef enum
{
  QUVI_OBJECT_OPTION_CROAK_IF_ERROR     = 0x01,
  /* fetch */
  QUVI_OBJECT_OPTION_FETCH_FROM_CHARSET = 0x20,
  /* http */
  QUVI_OBJECT_OPTION_HTTP_COOKIE_MODE   = 0x40,
#ifdef _UNUSED
  QUVI_OBJECT_OPTION_HTTP_USER_AGENT,
#endif
  /* crypto */
  QUVI_OBJECT_OPTION_CRYPTO_CIPHER_FLAGS = 0x60,
  QUVI_OBJECT_OPTION_CRYPTO_CIPHER_MODE,
  QUVI_OBJECT_OPTION_CRYPTO_CIPHER_KEY,
  QUVI_OBJECT_OPTION_CRYPTO_ALGORITHM
} QuviObjectOption;

struct l_quvi_object_opt_s
{
  struct
  {
    gchar *str;
    gdouble n;
  } value;
  gdouble id;
};

typedef struct l_quvi_object_opt_s *l_quvi_object_opt_t;

#define l_quvi_object_opts_chk_req_s(qoo,what,dst,type)\
  do {\
    l_quvi_object_opts_is_set(l, opts, qoo, &p, what, TRUE);\
    dst = ((l_quvi_object_opt_t) p->data)->value.type;\
  } while (0)

#define l_quvi_object_opts_chk_req_cb(n,what,cb)\
  do {\
    l_quvi_object_opts_is_set(l, opts, n, &p, what, TRUE);\
    cb(l, co, p);\
  } while (0)

#define l_quvi_object_opts_chk_opt_s(qoo,dst,type)\
  do {\
    if (l_quvi_object_opts_is_set(l, opts, qoo, &p, NULL, FALSE) == TRUE)\
      dst = ((l_quvi_object_opt_t) p->data)->value.type;\
  } while (0)

GSList *l_quvi_object_opts_new(lua_State*, gint);
void l_quvi_object_opts_free(GSList*);

gboolean l_quvi_object_opts_is_set(lua_State*, GSList*,
                                   const QuviObjectOption, GSList**,
                                   const gchar*, const gboolean);
#ifdef _UNUSED
void l_quvi_object_opts_curl(GSList*, _quvi_t);
#endif

void l_quvi_object_opts_chk_given(lua_State*, GSList*, const gchar*);
gboolean l_quvi_object_opts_croak_if_error(lua_State *l, GSList*);

#endif /* l_quvi_opts_h */

/* vim: set ts=2 sw=2 tw=72 expandtab: */
