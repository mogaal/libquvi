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

/*
 * TODO: When a suitable test URL is found
 * - Add check for QUVI_MEDIA_STREAM_PROPERTY_VIDEO_BITRATE_KBIT_S
 * - Add check for QUVI_MEDIA_STREAM_PROPERTY_AUDIO_BITRATE_KBIT_S
 */

#include "config.h"

#include <string.h>
#include <glib.h>
#include <quvi.h>

#include "tests.h"

static gchar *URLs[] =
{
  "http://www.gaskrank.tv/tv/motorrad-oldtimer/1928-henderson-deluxe-alt-und--19115.htm",
  "http://www.gaskrank.tv/tv/motorrad-oldtimer/1912-harley-davidson-superscho-9481.htm",
  NULL
};

static gchar *TITLEs[] =
{
  "1928 Henderson Deluxe - alt und mit der Patina des Alters aber läuft",
  "1912 Harley Davidson - superschön restauriert",
  NULL
};

#define chk_len(p) \
  do {\
    gchar *s = NULL; \
    quvi_media_get(qm, p, &s); \
    g_assert(s != NULL); \
    g_test_message("%s=%s", #p, s); \
    g_assert_cmpint(strlen(s), >, 1); \
  } while (0)

#define chk_val(p) \
  do {\
    gdouble n = -1; \
    quvi_media_get(qm, p, &n); \
    g_test_message("%s=%f", #p, n); \
    g_assert_cmpfloat(n, >, -1);\
  } while (0)

static void test_media_core()
{
  quvi_media_t qm;
  quvi_t q;
  gchar *s;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

  chk_verbose(q);

  qm = quvi_media_new(q, URLs[0]);
  g_assert_cmpint(qerr_m(q, URLs[0]), ==, QUVI_OK);
  g_assert(qm != NULL);

  /* Boundary check: the first -1 */
  quvi_media_get(qm, QUVI_MEDIA_PROPERTY_THUMBNAIL_URL-1, &s);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_ERROR_INVALID_ARG);

  /* Boundary check: the last +1 */
  quvi_media_get(qm, QUVI_MEDIA_STREAM_PROPERTY_VIDEO_WIDTH+1, &s);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_ERROR_INVALID_ARG);

  /* string */
  chk_len(QUVI_MEDIA_PROPERTY_THUMBNAIL_URL);
  chk_len(QUVI_MEDIA_PROPERTY_TITLE);
  chk_len(QUVI_MEDIA_PROPERTY_ID);

  chk_len(QUVI_MEDIA_STREAM_PROPERTY_URL);

  /* double */
  chk_val(QUVI_MEDIA_PROPERTY_START_TIME_MS);
  chk_val(QUVI_MEDIA_PROPERTY_DURATION_MS);

  quvi_media_free(qm);
  quvi_free(q);
}

static void test_media_multi()
{
  static const gchar URL[] =
    "http://www.spiegel.de/video/64-jaehrige-schwimmt-von-kuba-richtung-florida-video-1293301.html";

  quvi_media_t qm;
  GString *b;
  gchar *s;
  quvi_t q;
  gint i;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

  chk_verbose(q);

  qm = quvi_media_new(q, URL);
  g_assert_cmpint(qerr_m(q, URL), ==, QUVI_OK);
  g_assert(qm != NULL);

  /* string */
  chk_len(QUVI_MEDIA_PROPERTY_THUMBNAIL_URL);
  chk_len(QUVI_MEDIA_PROPERTY_TITLE);
  chk_len(QUVI_MEDIA_PROPERTY_ID);

  /* double */
  chk_val(QUVI_MEDIA_PROPERTY_START_TIME_MS);
  chk_val(QUVI_MEDIA_PROPERTY_DURATION_MS);

  b = g_string_new(NULL);
  i = 0;

  while (quvi_media_stream_next(qm) == QUVI_TRUE)
    {
      chk_len(QUVI_MEDIA_STREAM_PROPERTY_URL);
      chk_len(QUVI_MEDIA_STREAM_PROPERTY_ID);

      quvi_media_get(qm, QUVI_MEDIA_STREAM_PROPERTY_URL, &s);
      g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

      if (b->len >0) /* Make sure each media stream URL is unique */
        g_assert_cmpstr(b->str, !=, s);

      g_string_assign(b, s);

      chk_len(QUVI_MEDIA_STREAM_PROPERTY_VIDEO_ENCODING);
      chk_len(QUVI_MEDIA_STREAM_PROPERTY_CONTAINER);

      chk_val(QUVI_MEDIA_STREAM_PROPERTY_VIDEO_HEIGHT);
      chk_val(QUVI_MEDIA_STREAM_PROPERTY_VIDEO_WIDTH);

      ++i;
    }
  g_assert_cmpint(i, >, 1);

  g_string_free(b, TRUE);
  b = NULL;

  quvi_media_free(qm);
  quvi_free(q);
}

static void test_media_select()
{
  static const gchar URL[] =
    "http://www.spiegel.de/video/64-jaehrige-schwimmt-von-kuba-richtung-florida-video-1293301.html";

  quvi_media_t qm;
  gchar *s, *best;
  GSList *curr;
  GSList *ids;
  quvi_t q;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

  chk_verbose(q);

  qm = quvi_media_new(q, URL);
  g_assert_cmpint(qerr_m(q, URL), ==, QUVI_OK);
  g_assert(qm != NULL);

  ids = NULL;
  while (quvi_media_stream_next(qm) == QUVI_TRUE)
    {
      quvi_media_get(qm, QUVI_MEDIA_STREAM_PROPERTY_ID, &s);
      ids = g_slist_prepend(ids, g_strdup(s));
    }
  ids = g_slist_reverse(ids);

  /*
   * Default stream. This should also advance the current list pointer,
   * so that when quvi_media_stream_next is called the next time, it
   * should return the 2nd stream in the list, not the first one.
   */

  quvi_media_get(qm, QUVI_MEDIA_STREAM_PROPERTY_ID, &s);
  curr = g_slist_nth(ids, 0);
  g_assert_cmpstr(curr->data, ==, s);

  quvi_media_stream_next(qm);
  quvi_media_get(qm, QUVI_MEDIA_STREAM_PROPERTY_ID, &s);
  curr = g_slist_nth(ids, 1);
  g_assert_cmpstr(curr->data, ==, s);

  /* Reset. */

  quvi_media_stream_reset(qm);
  quvi_media_get(qm, QUVI_MEDIA_STREAM_PROPERTY_ID, &s);
  curr = g_slist_nth(ids, 0);
  g_assert_cmpstr(curr->data, ==, s);

  /* The best stream -- anything but the default (first) stream. */

  quvi_media_stream_choose_best(qm);
  quvi_media_get(qm, QUVI_MEDIA_STREAM_PROPERTY_ID, &s);
  curr = g_slist_nth(ids, 0);
  g_assert_cmpstr(curr->data, !=, s);
  best = g_strdup(s);

  /* Select. */

  quvi_media_stream_select(qm, "foo,bar,baz,best,croak");
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  quvi_media_get(qm, QUVI_MEDIA_STREAM_PROPERTY_ID, &s);
  g_assert_cmpstr(s, ==, best);
  g_free(best);

  quvi_media_stream_select(qm, "foo,bar,baz,croak");
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_ERROR_KEYWORD_CROAK);
  quvi_media_get(qm, QUVI_MEDIA_STREAM_PROPERTY_ID, &s);
  curr = g_slist_nth(ids, 0);
  g_assert_cmpstr(curr->data, ==, s);

  quvi_media_stream_select(qm, "foo,bar,baz");
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  quvi_media_get(qm, QUVI_MEDIA_STREAM_PROPERTY_ID, &s);
  curr = g_slist_nth(ids, 0); /* Should be the default stream (first) */
  g_assert_cmpstr(curr->data, ==, s);

  quvi_media_stream_select(qm, "^\\w\\w\\d_\\w+_40\\dk_\\d20p$,bar,baz,croak");
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  quvi_media_get(qm, QUVI_MEDIA_STREAM_PROPERTY_ID, &s);
  g_assert_cmpstr(s, ==, "mp4_mpeg4_404k_320p");

  quvi_media_stream_select(qm, "foo,^\\w\\w\\d_\\w+_14\\d+k_\\d+6p$,baz,croak");
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  quvi_media_get(qm, QUVI_MEDIA_STREAM_PROPERTY_ID, &s);
  g_assert_cmpstr(s, ==, "mp4_h264_1400k_576p");

  quvi_media_free(qm);
  quvi_free(q);
}

static void test_media_short()
{
  static const gchar URL[] = "http://tinyurl.com/d8s3y54";

  quvi_media_t qm;
  quvi_t q;
  gchar *s;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

  chk_verbose(q);

  qm = quvi_media_new(q, URL);
  g_assert_cmpint(qerr_m(q, URL), ==, QUVI_OK);
  g_assert(qm != NULL);

  quvi_media_get(qm, QUVI_MEDIA_PROPERTY_TITLE, &s);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  g_assert_cmpstr(s, ==, TITLEs[1]);

  quvi_media_free(qm);
  quvi_free(q);
}

static void test_media_starttime()
{
  static const gchar *URL =
    "http://www.youtube.com/watch?v=LWxTGJ3TK1U#t=2m22s";

  quvi_media_t qm;
  gdouble st = 0;
  quvi_t q;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

  chk_verbose(q);

  qm = quvi_media_new(q, URL);
  g_assert_cmpint(qerr_m(q, URL), ==, QUVI_OK);
  g_assert(qm != NULL);

  quvi_media_get(qm, QUVI_MEDIA_PROPERTY_START_TIME_MS, &st);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  g_assert_cmpint(st, ==, 142000);

  /* YouTube videos should give us this data. */
  chk_len(QUVI_MEDIA_STREAM_PROPERTY_AUDIO_ENCODING);

  quvi_media_free(qm);
  quvi_free(q);
}

static void test_media_escaped_url()
{
  static gchar URL[] =
    "http%3A//www.liveleak.com/view%3Fi%3D77f_1378178228";

  quvi_media_t qm;
  quvi_t q;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

  chk_verbose(q);

  qm = quvi_media_new(q, URL);
  g_assert_cmpint(qerr_m(q, URL), ==, QUVI_OK);
  g_assert(qm != NULL);

  quvi_media_free(qm);
  quvi_free(q);
}

static void test_media_nosupport()
{
  static const gchar URL[] = "http://example.com";

  quvi_media_t qm;
  quvi_t q;
  gchar *s;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

  chk_verbose(q);

  qm = quvi_media_new(q, URL);
  g_assert_cmpint(qerr_m(q, URL), ==, QUVI_ERROR_NO_SUPPORT);
  g_assert(qm != NULL);

  quvi_media_get(qm, QUVI_MEDIA_PROPERTY_TITLE, &s);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  g_assert_cmpstr(s, ==, "");

  quvi_media_free(qm);
  quvi_free(q);
}

static void test_media_same_q()
{
  quvi_media_t qm;
  gint i = 0;
  quvi_t q;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

  chk_verbose(q);

  for (; URLs[i] != NULL; ++i)
    {
      gchar *s = NULL;

      qm = quvi_media_new(q, URLs[i]);
      g_assert_cmpint(qerr_m(q, URLs[i]), ==, QUVI_OK);
      g_assert(qm != NULL);

      quvi_media_get(qm, QUVI_MEDIA_PROPERTY_TITLE, &s);
      g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
      g_assert_cmpstr(s, ==, TITLEs[i]);

      quvi_media_free(qm);
    }
  quvi_free(q);
}

gint main(gint argc, gchar **argv)
{
  g_test_init(&argc, &argv, NULL);
  g_test_add_func("/quvi/media (core)", test_media_core);
  g_test_add_func("/quvi/media (>1 streams)", test_media_multi);
  g_test_add_func("/quvi/media (select)", test_media_select);
  g_test_add_func("/quvi/media (short)", test_media_short);
  g_test_add_func("/quvi/media (escaped URL)", test_media_escaped_url);
  g_test_add_func("/quvi/media (nosupport)", test_media_nosupport);
  g_test_add_func("/quvi/media (start_time)", test_media_starttime);
  g_test_add_func("/quvi/media (same handle)", test_media_same_q);
  return (g_test_run());
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
