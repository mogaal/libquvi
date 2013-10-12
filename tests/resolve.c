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

#include "tests.h"

static void test_resolve_core()
{
  static const gchar URL[] = "http://is.gd/9lrXf8";

  static const gchar DST[] =
    "http://www.collegehumor.com/video/6781885/batman-blows-his-cover";

  quvi_resolve_t qr;
  quvi_t q;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

  chk_verbose(q);

  qr = quvi_resolve_new(q, URL);
  g_assert_cmpint(qerr_m(q, URL), ==, QUVI_OK);
  g_assert(qr != NULL);

  g_assert(quvi_resolve_forwarded(qr) == QUVI_TRUE);
  g_assert_cmpstr(quvi_resolve_destination_url(qr), ==, DST);

  quvi_resolve_free(qr);
  quvi_free(q);
}

#ifdef _1 /* Comment out until a new one with >1 redirections is found. */
static void test_resolve_3()
{
  static const gchar URL[] = "http://is.gd/gQ4pYW";

  static const gchar DST[] =
    "http://www.thefirearmblog.com/blog/2012/06/11/shooting-drones/"
    "?utm_source=feedburner&utm_medium=feed&utm_campaign="
    "Feed%3A+TheFirearmBlog+%28The+Firearm+Blog%29";

  quvi_resolve_t qr;
  quvi_t q;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

  chk_verbose(q);

  qr = quvi_resolve_new(q, URL);
  g_assert_cmpint(qerr_m(q, URL), ==, QUVI_OK);
  g_assert(qr != NULL);

  g_assert(quvi_resolve_forwarded(qr) == QUVI_TRUE);
  g_assert_cmpstr(quvi_resolve_destination_url(qr), ==, DST);

  quvi_resolve_free(qr);
  quvi_free(q);
}
#endif /* _1 */

static void test_resolve_nodst()
{
  static const gchar URL[] = "http://www.youtube.com/";

  quvi_resolve_t qr;
  quvi_t q;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

  chk_verbose(q);

  qr = quvi_resolve_new(q, URL);
  g_assert_cmpint(qerr_m(q, URL), ==, QUVI_OK);
  g_assert(qr != NULL);

  g_assert_cmpint(quvi_resolve_forwarded(qr), ==, QUVI_FALSE);

  quvi_resolve_free(qr);
  quvi_free(q);
}

static void test_resolve_nodst_t_param()
{
  static const gchar URL[] =
    "http://www.youtube.com/watch?v=G4evlxq34og#t=3m20";

  quvi_resolve_t qr;
  quvi_t q;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

  chk_verbose(q);

  qr = quvi_resolve_new(q, URL);
  g_assert_cmpint(qerr_m(q, URL), ==, QUVI_OK);
  g_assert(qr != NULL);

  /*
   * [UPDATE] 2012-11-19: g00gle servers no longer strip the #t param
   * The description below no longer applies, although we still test
   * this, in case this changes back.
   *
   * [ISSUE] g00gle servers redirect to the media URL stripping #t
   * g00gle servers redirect to the media URL stripping the #t
   * parameter. The library should not be fooled by this trickery.
   * The workaround is implemented in libquvi-scripts
   * (resolve_redirections.lua).
   */
  g_assert_cmpint(quvi_resolve_forwarded(qr), ==, QUVI_FALSE);

  quvi_resolve_free(qr);
  quvi_free(q);
}

/* [UPDATE] 2012-11-19: g00gle servers now seem to redirect to
 * the media URL without stripping the #t parameter. The test has been
 * modified to test that shortened URLs redirect to the expected media
 * URL.
 *
 * [ISSUE] #t parameter is lost with shortened URLs
 * Shortened Y0uTube media URLs with the #t parameter do not work
 * with the library. The test below should redirect to
 *   http://www.youtube.com/watch?v=G4evlxq34og
 * missing the #t parameter, see the above test for the description.
 */
static void test_resolve_shortened_with_t_param()
{
  static const gchar URL[] = "http://is.gd/TRWtNf";
  /* http://www.youtube.com/watch?v=G4evlxq34og#t=3m20 */

  static const gchar DST[] =
    "http://www.youtube.com/watch?v=G4evlxq34og#t=3m20";
#ifdef _1 /* [UPDATE] 2012-11-19: See above */
    "http://www.youtube.com/watch?v=G4evlxq34og";
#endif

  quvi_resolve_t qr;
  quvi_t q;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

  chk_verbose(q);

  qr = quvi_resolve_new(q, URL);
  g_assert_cmpint(qerr_m(q, URL), ==, QUVI_OK);
  g_assert(qr != NULL);

  g_assert_cmpint(quvi_resolve_forwarded(qr), ==, QUVI_TRUE);
  g_assert_cmpstr(quvi_resolve_destination_url(qr), ==, DST);

  quvi_resolve_free(qr);
  quvi_free(q);
}

gint main(gint argc, gchar **argv)
{
  g_test_init(&argc, &argv, NULL);
  g_test_add_func("/quvi/resolve (core)", test_resolve_core);
#ifdef _1
  g_test_add_func("/quvi/resolve (3)", test_resolve_3);
#endif
  g_test_add_func("/quvi/resolve (nodst)", test_resolve_nodst);
  g_test_add_func("/quvi/resolve (nodst #t param)",
                  test_resolve_nodst_t_param);
  g_test_add_func("/quvi/resolve (short URL with #t param)",
                  test_resolve_shortened_with_t_param);
  return (g_test_run());
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
