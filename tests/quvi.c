/* libquvi
 * Copyright (C) 2012,2013  Toni Gundogdu <legatvs@gmail.com>
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

#include <stdlib.h>
#include <glib.h>
#include <quvi.h>

#include "tests.h"

static void test_quvi_core()
{
  gpointer p;
  quvi_t q;
  glong n;

  if (chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  g_assert_cmpstr(quvi_errmsg(q), ==, "Not an error");
  g_assert_cmpint(quvi_ok(q), ==, QUVI_TRUE);

  /* quvi_get */

  n = 0;
  quvi_get(q, QUVI_INFO_RESPONSE_CODE-1, &n);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_ERROR_INVALID_ARG);
  g_assert_cmpint(quvi_ok(q), ==, QUVI_FALSE);
  g_assert_cmpint(n, ==, 0);

  p = NULL;
  quvi_get(q, QUVI_INFO_RESPONSE_CODE-1, &p);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_ERROR_INVALID_ARG);
  g_assert_cmpint(quvi_ok(q), ==, QUVI_FALSE);
  g_assert(p == NULL);

  n = 0;
  quvi_get(q, QUVI_INFO_RESPONSE_CODE+1, &n);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_ERROR_INVALID_ARG);
  g_assert_cmpint(quvi_ok(q), ==, QUVI_FALSE);
  g_assert_cmpint(n, ==, 0);

  p = NULL;
  quvi_get(q, QUVI_INFO_CURL_HANDLE, &p);
  g_assert_cmpint(quvi_ok(q), ==, QUVI_TRUE);
  g_assert(p != NULL);

  p = NULL;
  quvi_get(q, QUVI_INFO_CURL_HANDLE+1, &p);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_ERROR_INVALID_ARG);
  g_assert_cmpint(quvi_ok(q), ==, QUVI_FALSE);
  g_assert(p == NULL);

  /* quvi_set */

  quvi_set(q, QUVI_OPTION_CALLBACK_STATUS-1, 0);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_ERROR_INVALID_ARG);
  g_assert_cmpint(quvi_ok(q), ==, QUVI_FALSE);

  quvi_set(q, QUVI_OPTION_AUTOPROXY+1, 0);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_ERROR_INVALID_ARG);
  g_assert_cmpint(quvi_ok(q), ==, QUVI_FALSE);

  quvi_free(q);
}

static void test_version()
{
  if (chk_skip(__func__) == TRUE)
    return;

  if (g_test_trap_fork(0,
                       G_TEST_TRAP_SILENCE_STDOUT|G_TEST_TRAP_SILENCE_STDERR))
    {
      g_print("%s", quvi_version(QUVI_VERSION));
      g_printerr("%s", quvi_version(QUVI_VERSION_SCRIPTS));
      exit(0);
    }
  g_test_trap_assert_passed();
  g_test_trap_assert_stdout("v?.?.?*");
  g_test_trap_assert_stderr("v?.?*");

  g_assert_cmpstr(quvi_version(QUVI_VERSION_SCRIPTS+1),
                  ==, quvi_version(QUVI_VERSION));

  g_assert_cmpstr(quvi_version(QUVI_VERSION-1),
                  ==, quvi_version(QUVI_VERSION));
}

static void test_file_ext()
{
  struct lookup_s
  {
    gchar *input;
    gchar *output;
  };

  static const struct lookup_s l[] =
  {
    {"video/x-flv", "flv"},
    {"audio/mpeg", "mp3"},
    {"video/mp4", "mp4"},
    {"foo", "flv"},
    {NULL, NULL}
  };

  quvi_file_ext_t qfe;
  quvi_t q;
  gint i;

  if (chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  g_assert_cmpint(quvi_ok(q), ==, QUVI_TRUE);

  i = 0;
  while (l[i].input != NULL)
    {
      qfe = quvi_file_ext_new(q, l[i].input);
      g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
      g_assert_cmpint(quvi_ok(q), ==, QUVI_TRUE);
      g_assert_cmpstr(l[i].output, ==, quvi_file_ext_get(qfe));
      quvi_file_ext_free(qfe);
      ++i;
    }
  quvi_free(q);
}

gint main(gint argc, gchar **argv)
{
  g_test_init(&argc, &argv, NULL);
  g_test_add_func("/quvi (core)", test_quvi_core);
  g_test_add_func("/quvi/version", test_version);
  g_test_add_func("/quvi/file_ext", test_file_ext);
  return (g_test_run());
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
