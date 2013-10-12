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

/* media/ted.lua uses the "goto_url" instruction. */

static const gchar URL[] =
  "http://www.ted.com/talks/paul_lewis_crowdsourcing_the_news.html";

static const gchar TITLE[] =
  "TEDxThessaloniki - Paul Lewis - Citizen Journalism";

static void test_goto_url()
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

  qm = quvi_media_new(q, URL);
  g_assert_cmpint(qerr_m(q, URL), ==, QUVI_OK);
  g_assert(qm != NULL);

  quvi_media_get(qm, QUVI_MEDIA_PROPERTY_TITLE, &s);
  g_assert_cmpint(quvi_errcode(q), ==, QUVI_OK);
  g_assert_cmpstr(s, ==, TITLE);

  quvi_media_free(qm);
  quvi_free(q);
}

gint main(gint argc, gchar **argv)
{
  g_test_init(&argc, &argv, NULL);
  g_test_add_func("/quvi/goto URL", test_goto_url);
  return (g_test_run());
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
