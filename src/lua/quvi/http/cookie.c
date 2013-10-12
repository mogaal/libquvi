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

#include <string.h>
#include <lauxlib.h>
#include <glib.h>
#include <curl/curl.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
/* -- */
#include "lua/quvi/opts.h"
#include "lua/getfield.h"
#include "lua/setfield.h"
#include "lua/def.h"

static gint _ret(lua_State *l, const _quvi_t q)
{
  lua_newtable(l); /* Return a table of results. */
  l_setfield_s(l, QO_ERROR_MESSAGE, q->status.errmsg->str, -1);
  l_setfield_n(l, QO_QUVI_CODE, q->status.rc);
  return (1); /* no. of returned values (a table) */
}

struct _cookie_opts_s
{
  const gchar *s;
  gint mode;
};

typedef struct _cookie_opts_s *_cookie_opts_t;

typedef enum
{
  COOKIE_MODE_SESSION = 0x01,
  COOKIE_MODE_FILE,
  COOKIE_MODE_LIST,
  COOKIE_MODE_JAR
} CookieMode;

static gint _setopt(lua_State *l, const _quvi_t q, const CURLoption copt,
                    const _cookie_opts_t co, const gboolean croak_if_error)
{
  CURLcode r;

  if (co->mode == COOKIE_MODE_SESSION)
    r = curl_easy_setopt(q->handle.curl, copt, (glong) g_strtod(co->s, NULL));
  else
    r = curl_easy_setopt(q->handle.curl, copt, co->s);

  if (r != CURLE_OK)
    {
      g_string_printf(q->status.errmsg, "%s", curl_easy_strerror(r));
      q->status.rc = QUVI_ERROR_CALLBACK;

      if (croak_if_error == TRUE)
        luaL_error(l, "%s", q->status.errmsg->str);
    }
  return (_ret(l, q));
}

static void _chk_cookie_opts(lua_State *l, GSList *opts, _cookie_opts_t co)
{
  GSList *p;

  l_quvi_object_opts_chk_given(l, opts, "cookie");

  /* required */

  l_quvi_object_opts_chk_req_s(QUVI_OBJECT_OPTION_HTTP_COOKIE_MODE,
                               HRE_COOKIE_MODE, co->mode, n);
}

gint l_quvi_http_cookie(lua_State *l)
{
  struct _cookie_opts_s co;
  gboolean croak_if_error;
  CURLoption copt;
  GSList *opts;
  _quvi_t q;

  /* quvi handle */

  q = (_quvi_t) l_get_reg_userdata(l, USERDATA_QUVI_T);
  g_assert(q != NULL);

  if (q->opt.allow_cookies == QUVI_FALSE)
    return (_ret(l, q)); /* Do nothing if cookies have been disabled. */

  /* arg1 */

  memset(&co, 0, sizeof(struct _cookie_opts_s));
  co.s = luaL_checkstring(l, 1);
  lua_pop(l, 1);

  /* options */

  opts = l_quvi_object_opts_new(l, 2);
  croak_if_error = l_quvi_object_opts_croak_if_error(l, opts);

  _chk_cookie_opts(l, opts, &co);
  l_quvi_object_opts_free(opts);

  /* mode */

  switch (co.mode)
    {
    case COOKIE_MODE_SESSION:
      copt = CURLOPT_COOKIESESSION;
      break;

    case COOKIE_MODE_FILE:
      copt = CURLOPT_COOKIEFILE;
      break;

    case COOKIE_MODE_LIST:
      copt = CURLOPT_COOKIELIST;
      break;

    case COOKIE_MODE_JAR:
      copt = CURLOPT_COOKIEJAR;
      break;

    default:
      g_string_printf(q->status.errmsg,
                      "[%s] invalid cookie function `0x%02x'",
                      __func__, co.mode);

      q->status.rc = QUVI_ERROR_CALLBACK;
      copt = CURLOPT_COOKIESESSION;

      g_warning("%s", q->status.errmsg->str);
    }
  return (_setopt(l, q, copt, &co, croak_if_error));
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
