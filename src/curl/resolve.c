/* libquvi
 * Copyright (C) 2012  Toni Gundogdu <legatvs@gmail.com>
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

#include <glib/gi18n-lib.h>
#include <glib.h>
#include <curl/curl.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_net_resolve_s.h"
/* -- */
#include "curl/autoproxy.h"

/* Set cURL options. */
static void _set_opts(_quvi_net_resolve_t r, CURL *c)
{
  curl_easy_setopt(c, CURLOPT_URL, r->url.addr->str);
  /* Use HEAD request: we're only interested in the header metadata. */
  curl_easy_setopt(c, CURLOPT_NOBODY, 1L); /* GET -> HEAD. */

  c_autoproxy(r->handle.quvi, r->url.addr->str);
}

static void _reset_opts(CURL *c)
{
  curl_easy_setopt(c, CURLOPT_HTTPGET, 1L); /* HEAD -> GET. */
}

static const gchar *_EOK = N_("The server responded with the code %03ld");

/* Check whether the server returned a redirection URL. */
static QuviError _chk_redir(_quvi_net_resolve_t r, CURL *c)
{
  CURLcode curlcode;
  QuviError rc;

  curlcode = curl_easy_perform(c);
  curl_easy_getinfo(c, CURLINFO_RESPONSE_CODE, &r->status.resp_code);

  rc = QUVI_OK;

  if (curlcode == CURLE_OK && r->status.resp_code == 200)
    {
      gchar *u = NULL;
      curl_easy_getinfo(c, CURLINFO_EFFECTIVE_URL, &u);
      /* Leave comparison for resolve_redirections.lua */
      g_string_assign(r->url.dst, u);
    }
  else
    {
      if (curlcode == CURLE_OK)
        {
          g_string_printf(r->status.errmsg,
                          g_dgettext(GETTEXT_PACKAGE, _EOK),
                          r->status.resp_code);
        }
      else
        {
          const gchar *s = curl_easy_strerror(curlcode);
          const glong c = r->status.resp_code;
          const gint cc = curlcode;
#define _ENO "%s (HTTP/%03ld, cURL=0x%03x)"
          g_string_printf(r->status.errmsg, _ENO, s, c, cc);
#undef _ENO
        }
      rc = QUVI_ERROR_CALLBACK;
    }
  return (rc);
}

/* Resolve an URL redirection if needed. */
QuviError c_resolve(_quvi_t q, _quvi_net_resolve_t r)
{
  QuviError rc;
  CURL *c;

  c = q->handle.curl;

  _set_opts(r, c);
  rc = _chk_redir(r, c);
  _reset_opts(c);

  return (rc);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
