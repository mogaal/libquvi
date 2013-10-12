/* libquvi
 * Copyright (C) 2013  Toni Gundogdu <legatvs@gmail.com>
 *
 * This file is part of libquvu <http://quvi.sourceforge.net/>.
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

static const gchar *URLs[] =
{
  "http://youtube.com/watch?v=0QRO3gKj3qw",
  "http://youtube.com/watch?v=ntLPcVAyNPE",
  NULL
};

static void _t_chk_val(quvi_subtitle_type_t qst,
                       const QuviSubtitleTypeProperty p,
                       const gchar *n)
{
  gdouble d = -1;
  quvi_subtitle_type_get(qst, p, &d);
  g_assert_cmpint(d, !=, -1);
  g_test_message("%s=%g", n, d);
}

#define t_chk_val(p) \
  do { _t_chk_val(qst, p, #p); } while (0)

static void _l_chk_len(quvi_subtitle_lang_t qsl,
                       const QuviSubtitleLangProperty p,
                       const gchar *n)
{
  gchar *s = NULL;
  quvi_subtitle_lang_get(qsl, p, &s);
  g_assert(s != NULL);
  g_test_message("%s=%s", n, s);
  g_assert_cmpint(strlen(s), >, 1);
}

#define l_chk_len(p) \
  do { _l_chk_len(qsl, p, #p); } while (0)

static void _chk_shared_properties(quvi_t q, quvi_subtitle_t qsub,
                                   quvi_subtitle_type_t qst,
                                   quvi_subtitle_lang_t qsl)
{
  gdouble v;

  /* type */

  t_chk_val(QUVI_SUBTITLE_TYPE_PROPERTY_FORMAT);
  t_chk_val(QUVI_SUBTITLE_TYPE_PROPERTY_TYPE);

  /* lang */

  l_chk_len(QUVI_SUBTITLE_LANG_PROPERTY_URL);
  l_chk_len(QUVI_SUBTITLE_LANG_PROPERTY_ID);

  /* These are currently provided for (YouTube) CCs only. */

  qst = quvi_subtitle_type_next(qsub);
  g_assert(qst != NULL); /* Advance to the CC which should be next. */

  quvi_subtitle_type_get(qst, QUVI_SUBTITLE_TYPE_PROPERTY_FORMAT, &v);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  g_assert_cmpint(v, ==, QUVI_SUBTITLE_FORMAT_TT);

  quvi_subtitle_type_get(qst, QUVI_SUBTITLE_TYPE_PROPERTY_TYPE, &v);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  g_assert_cmpint(v, ==, QUVI_SUBTITLE_TYPE_CC);

  qsl = quvi_subtitle_lang_next(qst);
  g_assert(qsl != NULL);

  l_chk_len(QUVI_SUBTITLE_LANG_PROPERTY_TRANSLATED);
  l_chk_len(QUVI_SUBTITLE_LANG_PROPERTY_ORIGINAL);
  l_chk_len(QUVI_SUBTITLE_LANG_PROPERTY_CODE);
}

static void test_subtitle_core()
{
  quvi_subtitle_export_t qse;
  quvi_subtitle_type_t qst;
  quvi_subtitle_lang_t qsl;
  quvi_subtitle_t qsub;
  gdouble v;
  quvi_t q;
  gchar *s;
  gint i,j;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

  chk_verbose(q);

  qsub = quvi_subtitle_new(q, URLs[0]);
  g_assert_cmpint(qerr_m(q, URLs[0]), ==, QUVI_OK);
  g_assert(qsub != NULL);

  /* The first subtitle type. */
  qst = quvi_subtitle_type_next(qsub);
  g_assert(qst != NULL);

  /* type: Boundary check: the first -1 */
  quvi_subtitle_type_get(qst, QUVI_SUBTITLE_TYPE_PROPERTY_FORMAT-1, &v);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_ERROR_INVALID_ARG);

  /* type: Boundary check: the last +1 */
  quvi_subtitle_type_get(qst, QUVI_SUBTITLE_TYPE_PROPERTY_TYPE+1, &v);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_ERROR_INVALID_ARG);

  /* The first language of the first language. */
  qsl = quvi_subtitle_lang_next(qst);
  g_assert(qsl != NULL);

  /* lang: Boundary check: the first -1 */
  quvi_subtitle_lang_get(qst, QUVI_SUBTITLE_LANG_PROPERTY_TRANSLATED-1, &s);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_ERROR_INVALID_ARG);

  /* lang: Boundary check: the last +1 */
  quvi_subtitle_lang_get(qst, QUVI_SUBTITLE_LANG_PROPERTY_ID+1, &v);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_ERROR_INVALID_ARG);

  _chk_shared_properties(q, qsub, qst, qsl);

  quvi_subtitle_lang_reset(qst);
  quvi_subtitle_type_reset(qsub);

  for (i=0, j=0; (qst = quvi_subtitle_type_next(qsub)) != NULL; ++i)
    {
      for (; (qsl = quvi_subtitle_lang_next(qst)) != NULL; ++j);
    }

  g_assert_cmpint(i, ==, 2);
  g_assert_cmpint(j, >, 12);

  /* export */

  qsl = quvi_subtitle_select(qsub, "foo"); /* use the first available */
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

  qse = quvi_subtitle_export_new(qsl, "srt");
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  g_assert_cmpint(strlen(quvi_subtitle_export_data(qse)), >, 0);
  quvi_subtitle_export_free(qse);

  qse = quvi_subtitle_export_new(qsl, "foo");
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_ERROR_NO_SUPPORT);
  quvi_subtitle_export_free(qse);

  quvi_subtitle_free(qsub);
  quvi_free(q);
}

static void test_subtitle_short()
{
  static const gchar *URL = "http://is.gd/DIVaRF";

  quvi_subtitle_type_t qst;
  quvi_subtitle_lang_t qsl;
  quvi_subtitle_t qsub;
  quvi_t q;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

  chk_verbose(q);

  qsub = quvi_subtitle_new(q, URL);
  g_assert_cmpint(qerr_m(q, URL), ==, QUVI_OK);
  g_assert(qsub != NULL);

  qst = quvi_subtitle_type_next(qsub);
  g_assert(qst != NULL);

  qsl = quvi_subtitle_lang_next(qst);
  g_assert(qsl != NULL);

  _chk_shared_properties(q, qsub, qst, qsl);

  quvi_subtitle_free(qsub);
  quvi_free(q);
}

static void test_subtitle_select()
{
  quvi_subtitle_type_t qst;
  quvi_subtitle_lang_t qsl;
  quvi_subtitle_t qsub;
  GSList *curr, *ids; /* language IDs */
  quvi_t q;
  gchar *s;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

  chk_verbose(q);

  qsub = quvi_subtitle_new(q, URLs[0]);
  g_assert_cmpint(qerr_m(q, URLs[0]), ==, QUVI_OK);
  g_assert(qsub != NULL);

  /* Construct the lookup list. */
  ids = NULL;
  while ( (qst = quvi_subtitle_type_next(qsub)) != NULL)
    {
      while ( (qsl = quvi_subtitle_lang_next(qst)) != NULL)
        {
          quvi_subtitle_lang_get(qsl, QUVI_SUBTITLE_LANG_PROPERTY_ID, &s);
          ids = g_slist_prepend(ids, g_strdup(s));
        }
    }
  ids = g_slist_reverse(ids);

  /* Should croak. */

  qsl = quvi_subtitle_select(qsub, "foo,bar,baz,croak,cc_en");
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_ERROR_KEYWORD_CROAK);
  g_assert(qsl == NULL);

  /* Should return cc_en. */

  qsl = quvi_subtitle_select(qsub, "foo,bar,baz,cc_en");
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  g_assert(qsl != NULL);

  quvi_subtitle_lang_get(qsl, QUVI_SUBTITLE_LANG_PROPERTY_TRANSLATED, &s);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  g_assert_cmpstr(s, ==, "English");

  quvi_subtitle_lang_get(qsl, QUVI_SUBTITLE_LANG_PROPERTY_ORIGINAL, &s);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  g_assert_cmpstr(s, ==, "English");

  quvi_subtitle_lang_get(qsl, QUVI_SUBTITLE_LANG_PROPERTY_CODE, &s);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  g_assert_cmpstr(s, ==, "en");

  quvi_subtitle_lang_get(qsl, QUVI_SUBTITLE_LANG_PROPERTY_URL, &s);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  g_assert_cmpint(strlen(s), >, 1);

  quvi_subtitle_lang_get(qsl, QUVI_SUBTITLE_LANG_PROPERTY_ID, &s);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  g_assert_cmpstr(s, ==, "cc_en");

  /* Should return the default language (the first). */

  qsl = quvi_subtitle_select(qsub, "foo,bar,baz");
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  g_assert(qsl != NULL);

  quvi_subtitle_lang_get(qsl, QUVI_SUBTITLE_LANG_PROPERTY_ID, &s);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  curr = g_slist_nth(ids, 0);
  g_assert_cmpstr(curr->data, ==, s);

  g_slist_foreach(ids, (GFunc) g_free, NULL);
  g_slist_free(ids);

  quvi_subtitle_free(qsub);
  quvi_free(q);
}

static void test_subtitle_nosupport()
{
  static const gchar URL[] = "http://example.com";

  quvi_subtitle_t qsub;
  quvi_t q;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

  chk_verbose(q);

  qsub = quvi_subtitle_new(q, URL);
  g_assert_cmpint(qerr_m(q, URL), ==, QUVI_ERROR_NO_SUPPORT);
  g_assert(qsub != NULL);
  g_assert(quvi_subtitle_type_next(qsub) == NULL);

  quvi_subtitle_free(qsub);
  quvi_free(q);
}

static void test_subtitle_sameq()
{
  quvi_subtitle_type_t qst;
  quvi_subtitle_lang_t qsl;
  quvi_subtitle_t qsub;
  quvi_t q;
  gint i;

  if (chk_internet() == FALSE || chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

  chk_verbose(q);

  i = 0;
  while (URLs[i] != NULL)
    {
      qsub = quvi_subtitle_new(q, URLs[i]);
      g_assert_cmpint(qerr_m(q, URLs[i]), ==, QUVI_OK);
      g_assert(qsub != NULL);

      qst = quvi_subtitle_type_next(qsub);
      g_assert(qst != NULL);
      t_chk_val(QUVI_SUBTITLE_TYPE_PROPERTY_TYPE);

      qsl = quvi_subtitle_lang_next(qst);
      g_assert(qsl != NULL);
      l_chk_len(QUVI_SUBTITLE_LANG_PROPERTY_ID);

      quvi_subtitle_free(qsub);
      ++i;
    }
  quvi_free(q);
}

gint main(gint argc, gchar **argv)
{
  g_test_init(&argc, &argv, NULL);
  g_test_add_func("/quvi/subtitle (core)", test_subtitle_core);
  g_test_add_func("/quvi/subtitle (short)", test_subtitle_short);
  g_test_add_func("/quvi/subtitle (select)", test_subtitle_select);
  g_test_add_func("/quvi/subtitle (nosupport)", test_subtitle_nosupport);
  g_test_add_func("/quvi/subtitle (sameq)", test_subtitle_sameq);
  return (g_test_run());
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
