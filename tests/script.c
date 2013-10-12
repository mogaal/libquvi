/* libquvi
 * Copyright (C) 2012-2013  Toni Gundogdu <legatvs@gmail.com>
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

static gint count(const quvi_t q, const QuviScriptType t)
{
  gint c = 0;
  while (quvi_script_next(q, t) == QUVI_TRUE)
    ++c;
  return (c);
}

static gsize len(const quvi_t q,
                 const QuviScriptType t,
                 const QuviScriptProperty p)
{
  gchar *s = NULL;

  quvi_script_get(q, t, p, &s);
  g_assert(s != NULL);

  return (strlen(s));
}

static void test_script()
{
  QuviScriptType t;
  quvi_t q;

  if (chk_skip(__func__) == TRUE)
    return;

  q = quvi_new();
  g_assert(q != NULL);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);

  /* Properties. */
  for (t=QUVI_SCRIPT_TYPE_PLAYLIST; t<QUVI_SCRIPT_TYPE_SCAN+1; ++t)
    {
      g_assert_cmpint(count(q, t), >=, 1);

      while (quvi_script_next(q, t) == QUVI_TRUE)
        {
          /* Shared with all scripts. */
          g_assert_cmpint(len(q, t, QUVI_SCRIPT_PROPERTY_FILEPATH), >, 0);
          g_assert_cmpint(len(q, t, QUVI_SCRIPT_PROPERTY_FILENAME), >, 0);
          g_assert_cmpint(len(q, t, QUVI_SCRIPT_PROPERTY_SHA1), >, 0);

          /* Media script specific. */
          if (t == QUVI_SCRIPT_TYPE_MEDIA || t == QUVI_SCRIPT_TYPE_PLAYLIST)
            g_assert_cmpint(len(q, t, QUVI_SCRIPT_PROPERTY_DOMAINS),>,0);
        }
    }

  /* Boundaries */

  /* Script type. */
  {
    const gint c = count(q, QUVI_SCRIPT_TYPE_MEDIA);

    g_assert_cmpint(c, ==, count(q, QUVI_SCRIPT_TYPE_SUBTITLE_EXPORT-1));
    g_assert_cmpint(c, ==, count(q, QUVI_SCRIPT_TYPE_SCAN+1));
  }

  /* Property. Do this last (quvi_script_next is called once). */
  {
    gchar *s;

    t = QUVI_SCRIPT_TYPE_MEDIA;
    g_assert(quvi_script_next(q, t) == QUVI_TRUE);

    quvi_script_get(q, t, QUVI_SCRIPT_PROPERTY_EXPORT_FORMAT-1, &s);
    g_assert_cmpint(quvi_errcode(q), ==, QUVI_ERROR_INVALID_ARG);

    quvi_script_get(q, t, QUVI_SCRIPT_PROPERTY_SHA1+1, &s);
    g_assert_cmpint(quvi_errcode(q), ==, QUVI_ERROR_INVALID_ARG);
  }

  quvi_free(q);
}

gint main(gint argc, gchar **argv)
{
  g_test_init(&argc, &argv, NULL);
  g_test_add_func("/quvi/script", test_script);
  return (g_test_run());
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
