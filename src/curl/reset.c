/* libquvi
 * Copyright (C) 2013  Toni Gundogdu <legatvs@gmail.com>
 *
 * This file is part of libquvi <http://quvi.sourceforge.net/>
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

#include <curl/curl.h>
#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"

glong c_reset_headers(_quvi_t q)
{
  CURLcode cc;
  CURL *c;

  c = q->handle.curl;
  cc = curl_easy_setopt(c, CURLOPT_HTTPHEADER, NULL); /* clear custom headers */

  curl_slist_free_all(q->http.headers);
  q->http.headers = NULL;

  return (cc);
}

gint c_reset(_quvi_t q)
{
  CURL *c = q->handle.curl;
#ifdef _1
  /*
   * If a program re-using the libquvi's curl session handle sets the
   * CURLOPT_USERAGENT (or other options), the value would be lost.
   */
  curl_easy_reset(c);
#endif
  c_reset_headers(q);

  curl_easy_setopt(c, CURLOPT_USERAGENT, (q->opt.user_agent->len >0)
                   ? q->opt.user_agent->str
                   : "Mozilla/5.0");

  curl_easy_setopt(c, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(c, CURLOPT_NOBODY, 0L);

  if (q->opt.allow_cookies == QUVI_TRUE)  /* clear cookies in memory */
    curl_easy_setopt(c, CURLOPT_COOKIELIST, "ALL");

#ifdef _1 /* Use whatever libcurl defaults to. */
  curl_easy_setopt(c, CURLOPT_MAXREDIRS, 5L); /* http://is.gd/kFsvE4 */
#endif

  return (QUVI_OK);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
