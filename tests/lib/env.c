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

#include <string.h>
#include <glib.h>
#include <quvi.h>
#include <curl/curl.h>

#include "tests.h"

static gboolean chk(const gchar *w, const gchar *m)
{
  const gchar *s = g_getenv(w);
  if (s == NULL || strlen(s) == 0)
    {
      if (m != NULL)
        g_test_message("%s", m);
      return (FALSE);
    }
  return (TRUE);
}

gboolean chk_internet()
{
  return (chk("TEST_INTERNET", "SKIP: Set TEST_INTERNET to enable"));
}

void chk_verbose(quvi_t q)
{
  g_assert(q != NULL);
  if (chk("TEST_VERBOSE", NULL) == TRUE)
    {
      CURL *c = NULL;
      g_setenv("LIBQUVI_VERBOSE_SCRIPTS", "1", TRUE);
      quvi_get(q, QUVI_INFO_CURL_HANDLE, &c);
      curl_easy_setopt(c, CURLOPT_VERBOSE, 1L);
    }
}

/* Check if test was set to be skipped.
 * Example:
 *  env TEST_SKIP=test_supports_online,test_quvi make test
 */
gboolean chk_skip(const gchar *test)
{
  const gchar *e = g_getenv("TEST_SKIP");
  gboolean r = FALSE;

  if (e == NULL || strlen(e) == 0)
    return (FALSE);
  {
    gchar **s = g_strsplit(e, ",", 0);
    gint i = -1;

    while (s != NULL && s[++i] != NULL)
      {
        if (match(test, s[i]) == TRUE)
          {
            g_test_message("SKIP: Remove '%s' from TEST_SKIP to enable", test);
            r = TRUE;
            break;
          }
      }
    g_strfreev(s);
    s = NULL;
  }
  return (r);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
