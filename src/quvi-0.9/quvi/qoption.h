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

#ifndef qoption_h
#define qoption_h

/** @file qoption.h */

/** @enum QuviOption
@brief Options to be used with @ref quvi_set */
typedef enum
{
  /** Default: None */
  QUVI_OPTION_CALLBACK_STATUS,
  /** Default: None */
  QUVI_OPTION_CALLBACK_STATUS_USERDATA,
  /** Enable cookies for libcurl handle.
   * Default: QUVI_FALSE. When enabled (QUVI_TRUE), libcurl will
   * parse the received cookies and use them in the subsequent HTTP
   * requests.
   * @note
   *  When disabled, quvi.http.cookie (of the quvi-object(7)) is also
   *  disabled
   * @sa
   * http://curl.haxx.se/libcurl/c/curl_easy_setopt.html#CURLOPTCOOKIEFILE
   * @sa
   * http://curl.haxx.se/libcurl/c/curl_easy_setopt.html#CURLOPTCOOKIELIST
   * @sa
   * http://curl.haxx.se/libcurl/c/curl_easy_setopt.html#CURLOPTCOOKIEJAR
   */
  QUVI_OPTION_ALLOW_COOKIES,
  /** Identify as the user-agent to the HTTP servers.
   * Default: "Mozilla/5.0".
   * @note
   *    libquvi scripts may override this setting at any time. The
   *    original value set using this option will be restored each time
   *    the network accessing functions are called, e.g.
   *    quvi.http.fetch
   * @sa http://curl.haxx.se/libcurl/c/curl_easy_setopt.html#CURLOPTUSERAGENT
   */
  QUVI_OPTION_USER_AGENT,
  /** Autodetect and set the proxy for new connections.
   * Default: QUVI_FALSE. When enabled (QUVI_TRUE), libquvi uses libproxy
   * to detect the system proxy settings and sets the CURLOPT_PROXY to the
   * libproxy returned value. This is done for each new connection.
   * @sa http://curl.haxx.se/libcurl/c/curl_easy_setopt.html#CURLOPTPROXY
   * @sa http://libproxy.googlecode.com/
   */
  QUVI_OPTION_AUTOPROXY
} QuviOption;

#endif /* qoption_h */

/* vim: set ts=2 sw=2 tw=72 expandtab: */
