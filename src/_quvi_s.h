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

#ifndef _quvi_s_h
#define _quvi_s_h

typedef gint (*quvi_callback_http_metainfo)(gpointer);
typedef gint (*quvi_callback_resolve)(gpointer);
typedef gint (*quvi_callback_fetch)(gpointer);

struct _quvi_s
{
  struct
  {
    quvi_callback_http_metainfo http_metainfo;
    quvi_callback_resolve resolve;
    quvi_callback_status status;
    quvi_callback_fetch fetch;
    struct
    {
      gpointer status;
    } userdata;
  } cb;
  struct
  {
    gboolean allow_cookies;
    GString *user_agent;
    gboolean autoproxy;
  } opt;
  struct
  {
    GString *errmsg;
    glong resp_code;
    QuviError rc;
  } status;
  struct
  {
    gpointer proxy;
    gpointer curl;
    gpointer lua;
  } handle;
  struct
  {
    struct curl_slist *headers;
  } http;
  struct
  {
    struct
    {
      GSList *subtitle_export;
      GSList *subtitle;
      GSList *playlist;
      GSList *media;
      GSList *scan;
    } curr;
    GSList *subtitle_export;
    GSList *subtitle;
    GSList *playlist;
    GSList *media;
    GSList *scan;
    GSList *util;
  } scripts;
};

typedef struct _quvi_s *_quvi_t;

#endif /* _quvi_s_h */

/* vim: set ts=2 sw=2 tw=72 expandtab: */
