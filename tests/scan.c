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

#include <string.h>
#include <glib.h>
#include <quvi.h>

#include "tests.h"

static quvi_t new_q()
{
  quvi_t q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  chk_verbose(q);
  return (q);
}

static void chk_results(quvi_scan_t qs)
{
  const gchar *s;
  gint i = 0;
  while ((s = quvi_scan_next_media_url(qs)) != NULL)
    {
      g_assert_cmpint(strlen(s), >, 0);
      ++i;
    }
  g_assert_cmpint(i, >=, 1);
}

static void test_scan_core()
{
  static const gchar URL[] =
    "http://www.fangoria.com/index.php/home/all-news/1-latest-news/"
    "6981-new-qprometheusq-trailer-has-landed";

  quvi_scan_t qs;
  quvi_t q;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = new_q();

  qs = quvi_scan_new(q, URL);
  g_assert_cmpint(qerr_m(q, URL), ==, QUVI_OK);
  g_assert(qs != NULL);
  chk_results(qs);

  quvi_scan_free(qs);
  quvi_free(q);
}

static void test_scan_short()
{
  static const gchar URL[] = "http://is.gd/yUeWit";

  quvi_scan_t qs;
  quvi_t q;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = new_q();

  qs = quvi_scan_new(q, URL);
  g_assert_cmpint(qerr_m(q, URL), ==, QUVI_OK);
  g_assert(qs != NULL);
  chk_results(qs);

  quvi_scan_free(qs);
  quvi_free(q);
}

static void test_scan_noresults()
{
  static const gchar URL[] = "http://example.com/";

  quvi_scan_t qs;
  quvi_t q;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = new_q();

  qs = quvi_scan_new(q, URL);
  g_assert_cmpint(qerr_m(q, URL), ==, QUVI_OK);
  g_assert(qs != NULL);
  g_assert(quvi_scan_next_media_url(qs) == NULL);

  quvi_scan_free(qs);
  quvi_free(q);
}

struct user_data_s
{
  gint n;
};

typedef struct user_data_s *user_data_t;

static QuviError status_cb(glong status_type, gpointer data,
                           gpointer user_data)
{
  user_data_t u = (user_data_t) user_data;
  g_assert(user_data != NULL);
  ++u->n;
  return (QUVI_OK);
}

static void test_scan_userdata()
{
  static const gchar URL[] = "http://is.gd/yUeWit";

  struct user_data_s u;
  quvi_scan_t qs;
  quvi_t q;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = new_q();
  u.n = 0;

  quvi_set(q, QUVI_OPTION_CALLBACK_STATUS, (quvi_callback_status) status_cb);
  quvi_set(q, QUVI_OPTION_CALLBACK_STATUS_USERDATA, &u);

  g_assert_cmpint(u.n, ==, 0);
  qs = quvi_scan_new(q, URL);
  g_assert_cmpint(qerr_m(q, URL), ==, QUVI_OK);
  g_assert(qs != NULL);

  quvi_scan_free(qs);
  quvi_free(q);

  g_assert_cmpint(u.n, >, 0);
}

gint main(gint argc, gchar **argv)
{
  g_test_init(&argc, &argv, NULL);
  g_test_add_func("/quvi/scan (core)", test_scan_core);
  g_test_add_func("/quvi/scan (short)", test_scan_short);
  g_test_add_func("/quvi/scan (noresults)", test_scan_noresults);
  g_test_add_func("/quvi/scan (userdata)", test_scan_userdata);
  return (g_test_run());
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
