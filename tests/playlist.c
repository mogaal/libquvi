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

static void test_playlist_core()
{
  static const gchar URL[] =
    "http://soundcloud.com/volt-icarus2-otherupload/sets/bgm/";

  quvi_playlist_t qp;
  gdouble n;
  quvi_t q;
  gchar *s;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

  chk_verbose(q);

  qp = quvi_playlist_new(q, URL);
  g_assert_cmpint(qerr_m(q, URL), ==, QUVI_OK);
  g_assert(qp != NULL);

  /* Boundary check: the first -1 */
  quvi_playlist_get(qp, QUVI_PLAYLIST_PROPERTY_THUMBNAIL_URL-1, &s);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_ERROR_INVALID_ARG);

  /* Boundary check: the last +1 */
  quvi_playlist_get(qp, QUVI_PLAYLIST_MEDIA_PROPERTY_DURATION_MS+1, &s);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_ERROR_INVALID_ARG);

  quvi_playlist_get(qp, QUVI_PLAYLIST_PROPERTY_THUMBNAIL_URL, &s);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  g_assert_cmpint(strlen(s), >, 0);

  quvi_playlist_get(qp, QUVI_PLAYLIST_PROPERTY_TITLE, &s);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  g_assert_cmpstr(s, ==, "허스키익스프레스bgm");

  quvi_playlist_get(qp, QUVI_PLAYLIST_PROPERTY_ID, &s);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  g_assert_cmpstr(s, ==, "volt-icarus2-otherupload_bgm");

  /* This should advance the current media pointer to the first media
   * item in the returned list. */
  quvi_playlist_get(qp, QUVI_PLAYLIST_MEDIA_PROPERTY_TITLE, &s);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  /* First media title. */
  g_assert_cmpstr(s, ==,
                  "은빛설원의 노래(Song of silver snowy)_Husky "
                  "Express Main bgm");

  /* This should continue from the 2nd item, not the 1st in the list. */
  quvi_playlist_media_next(qp);

  quvi_playlist_get(qp, QUVI_PLAYLIST_MEDIA_PROPERTY_TITLE, &s);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  /* Second media title. */
  g_assert_cmpstr(s, ==,
                  "햇살을 가르며Hike(Hike bisecting the "
                  "sunshine)_Husky Express bgm");

  quvi_playlist_media_reset(qp);

  {
    gint i = 0;
    while (quvi_playlist_media_next(qp) == QUVI_TRUE)
      {
        quvi_playlist_get(qp, QUVI_PLAYLIST_MEDIA_PROPERTY_DURATION_MS, &n);
        g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
        g_assert_cmpfloat(n, >, 0);

        quvi_playlist_get(qp, QUVI_PLAYLIST_MEDIA_PROPERTY_TITLE, &s);
        g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
        g_assert_cmpint(strlen(s), >, 0);

        if (i == 0)
          {
            /* Confirm that this is the first item, the call to
             * quvi_playlist_reset earlier should have reset the
             * current location. */
            g_assert_cmpstr(s, ==,
                            "은빛설원의 노래(Song of silver snowy)_Husky "
                            "Express Main bgm");
          }

        quvi_playlist_get(qp, QUVI_PLAYLIST_MEDIA_PROPERTY_URL, &s);
        g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
        g_assert_cmpint(strlen(s), >, 0);

        ++i;
      }
    g_assert_cmpint(i, >, 1);
  }

  quvi_playlist_free(qp);
  quvi_free(q);
}

static void test_playlist_short()
{
  static const gchar URL[] = "http://is.gd/3oNTko";

  quvi_playlist_t qp;
  quvi_t q;
  gchar *s;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

  chk_verbose(q);

  qp = quvi_playlist_new(q, URL);
  g_assert_cmpint(qerr_m(q, URL), ==, QUVI_OK);
  g_assert(qp != NULL);

  quvi_playlist_get(qp, QUVI_PLAYLIST_PROPERTY_THUMBNAIL_URL, &s);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  g_assert_cmpint(strlen(s), >, 0);

  quvi_playlist_get(qp, QUVI_PLAYLIST_PROPERTY_TITLE, &s);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  g_assert_cmpstr(s, ==, "허스키익스프레스bgm");

  quvi_playlist_get(qp, QUVI_PLAYLIST_PROPERTY_ID, &s);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  g_assert_cmpstr(s, ==, "volt-icarus2-otherupload_bgm");

  {
    gint i = 0;
    while (quvi_playlist_media_next(qp) == QUVI_TRUE)
      {
        quvi_playlist_get(qp, QUVI_PLAYLIST_MEDIA_PROPERTY_URL, &s);
        g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
        g_assert_cmpint(strlen(s), >, 0);
        ++i;
      }
    g_assert_cmpint(i, >, 1);
  }

  quvi_playlist_free(qp);
  quvi_free(q);
}

static void test_playlist_escaped_url()
{
  static const gchar URL[] =
    "http://www.youtube.com/playlist%3Flist%3DPLlbnzwCkgkTBBXWz595XaKs_kkXek0gQP%26";

  quvi_playlist_t qp;
  quvi_t q;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

  chk_verbose(q);

  qp = quvi_playlist_new(q, URL);
  g_assert_cmpint(qerr_m(q, URL), ==, QUVI_OK);
  g_assert(qp != NULL);

  quvi_playlist_free(qp);
  quvi_free(q);
}

static void test_playlist_nosupport()
{
  static const gchar URL[] = "http://example.com/";

  quvi_playlist_t qp;
  quvi_t q;
  gchar *s;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

  chk_verbose(q);

  qp = quvi_playlist_new(q, URL);
  g_assert_cmpint(qerr_m(q, URL), ==, QUVI_ERROR_NO_SUPPORT);
  g_assert(qp != NULL);

  quvi_playlist_get(qp, QUVI_PLAYLIST_PROPERTY_THUMBNAIL_URL, &s);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  g_assert_cmpstr(s, ==, "");

  quvi_playlist_get(qp, QUVI_PLAYLIST_PROPERTY_TITLE, &s);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  g_assert_cmpstr(s, ==, "");

  quvi_playlist_get(qp, QUVI_PLAYLIST_PROPERTY_ID, &s);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  g_assert_cmpstr(s, ==, "");

  quvi_playlist_free(qp);
  quvi_free(q);
}

gint main(gint argc, gchar **argv)
{
  g_test_init(&argc, &argv, NULL);
  g_test_add_func("/quvi/playlist (core)", test_playlist_core);
  g_test_add_func("/quvi/playlist (short)", test_playlist_short);
  g_test_add_func("/quvi/playlist (escaped URL)", test_playlist_escaped_url);
  g_test_add_func("/quvi/playlist (nosupport)", test_playlist_nosupport);
  return (g_test_run());
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
