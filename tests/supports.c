/* libquvi
 * Copyright (C) 2012  Toni Gundogdu <legatvs@gmail.com>
 *
 * This file is part of libquvi <http://quvi.sourceforge.net/>.
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General
 * Public License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <glib.h>
#include <quvi.h>
#include <curl/curl.h>

#include "tests.h"

static const gchar URL[] =
  "http://www.gaskrank.tv/tv/motorrad-oldtimer/1928-henderson-deluxe-alt-und--19115.htm";

static const gchar URL_SHORT[] = "http://is.gd/jg9P5c";
static const gchar URL_NOSUP[] = "http://example.com/";
static const gchar URL_INV[] = "foobar";

static void test_supports_online()
{
  QuviBoolean r;
  quvi_t q;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

  chk_verbose(q);

  r = quvi_supports(q, URL,
                    QUVI_SUPPORTS_MODE_OFFLINE-1, /* Defaults to online. */
                    QUVI_SUPPORTS_TYPE_MEDIA);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  g_assert_cmpint(r, ==, QUVI_TRUE);

  r = quvi_supports(q, URL,
                    QUVI_SUPPORTS_MODE_ONLINE+1, /* Defaults to online */
                    QUVI_SUPPORTS_TYPE_MEDIA);
  g_assert_cmpint(qerr_m(q, URL), ==, QUVI_OK);
  g_assert_cmpint(r, ==, QUVI_TRUE);

  r = quvi_supports(q, URL,
                    QUVI_SUPPORTS_MODE_ONLINE,
                    0xffff /* any */);
  g_assert_cmpint(qerr_m(q, URL), ==, QUVI_OK);
  g_assert_cmpint(r, ==, QUVI_TRUE);

  r = quvi_supports(q, URL,
                    QUVI_SUPPORTS_MODE_ONLINE,
                    QUVI_SUPPORTS_TYPE_MEDIA);
  g_assert_cmpint(qerr_m(q, URL), ==, QUVI_OK);
  g_assert_cmpint(r, ==, QUVI_TRUE);

  r = quvi_supports(q, URL_SHORT,
                    QUVI_SUPPORTS_MODE_ONLINE,
                    QUVI_SUPPORTS_TYPE_MEDIA);
  g_assert_cmpint(qerr_m(q, URL_SHORT), ==, QUVI_OK);
  g_assert_cmpint(r, ==, QUVI_TRUE);

  r = quvi_supports(q, URL /* Media URL */,
                    QUVI_SUPPORTS_MODE_ONLINE,
                    QUVI_SUPPORTS_TYPE_PLAYLIST /*!*/);
  g_assert_cmpint(r, ==, QUVI_FALSE);
  g_assert_cmpint(qerr_m(q, URL), ==, QUVI_ERROR_NO_SUPPORT);

  r = quvi_supports(q, URL_NOSUP,
                    QUVI_SUPPORTS_MODE_ONLINE,
                    QUVI_SUPPORTS_TYPE_ANY);
  g_assert_cmpint(r, ==, QUVI_FALSE);
  g_assert_cmpint(qerr_m(q, URL_NOSUP), ==, QUVI_ERROR_NO_SUPPORT);

  r = quvi_supports(q, URL_INV,
                    QUVI_SUPPORTS_MODE_ONLINE,
                    QUVI_SUPPORTS_TYPE_ANY);
  g_assert_cmpint(r, ==, QUVI_FALSE);
  g_assert_cmpint(qerr_m(q, URL_INV), ==, QUVI_ERROR_CALLBACK);

  quvi_free(q);
}

static void test_supports_offline()
{
  QuviBoolean r;
  quvi_t q;

  if (chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

  r = quvi_supports(q, URL,
                    QUVI_SUPPORTS_MODE_OFFLINE,
                    QUVI_SUPPORTS_TYPE_ANY);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  g_assert_cmpint(r, ==, QUVI_TRUE);

  r = quvi_supports(q, URL /* Media URL */,
                    QUVI_SUPPORTS_MODE_OFFLINE,
                    QUVI_SUPPORTS_TYPE_PLAYLIST /*!*/);
  g_assert_cmpint(r, ==, QUVI_FALSE);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_ERROR_NO_SUPPORT);

  r = quvi_supports(q, URL_SHORT,
                    QUVI_SUPPORTS_MODE_OFFLINE,
                    QUVI_SUPPORTS_TYPE_ANY);
  g_assert_cmpint(r, ==, QUVI_FALSE);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_ERROR_NO_SUPPORT);

  r = quvi_supports(q, URL_NOSUP,
                    QUVI_SUPPORTS_MODE_OFFLINE,
                    QUVI_SUPPORTS_TYPE_ANY);
  g_assert_cmpint(r, ==, QUVI_FALSE);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_ERROR_NO_SUPPORT);

  r = quvi_supports(q, URL_INV,
                    QUVI_SUPPORTS_MODE_OFFLINE,
                    QUVI_SUPPORTS_TYPE_ANY);
  g_assert_cmpint(r, ==, QUVI_FALSE);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_ERROR_NO_SUPPORT);

  quvi_free(q);
}

static void test_supports_net_err()
{
  QuviBoolean r;
  quvi_t q;
  CURL *c;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

  chk_verbose(q);

  /* Simulate a network error. See also quvi_supports C API
   * documentation. */

  quvi_get(q, QUVI_INFO_CURL_HANDLE, &c);
  g_assert(c != NULL);
  curl_easy_setopt(c, CURLOPT_PROXY, "http://localhost:12345");

  r = quvi_supports(q, URL, QUVI_SUPPORTS_MODE_ONLINE,
                    QUVI_SUPPORTS_TYPE_ANY);
  g_assert_cmpint(r, ==, QUVI_FALSE);
  g_assert_cmpint(qerr_m(q, URL), !=, QUVI_ERROR_NO_SUPPORT);
  g_assert_cmpint(qerr_m(q, URL), ==, QUVI_ERROR_CALLBACK);

  quvi_free(q);
}

gint main(gint argc, gchar **argv)
{
  g_test_init(&argc, &argv, NULL);
  g_test_add_func("/quvi/supports (online)", test_supports_online);
  g_test_add_func("/quvi/supports (offline)", test_supports_offline);
  g_test_add_func("/quvi/supports (network error)", test_supports_net_err);
  return (g_test_run());
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
