/* libquvi
 * Copyright (C) 2012,2013  Toni Gundogdu <legatvs@gmail.com>
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

#include <curl/curl.h>
#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"

extern gint c_reset(_quvi_t);

QuviError c_init(_quvi_t q)
{
  curl_global_init(CURL_GLOBAL_ALL);

  q->handle.curl = curl_easy_init();
  if (q->handle.curl == NULL)
    return (QUVI_ERROR_CURL_INIT);

  if (q->opt.allow_cookies == QUVI_TRUE) /* Enable cookies */
    curl_easy_setopt(q->handle.curl, CURLOPT_COOKIEFILE, "");

  return (c_reset(q));
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
