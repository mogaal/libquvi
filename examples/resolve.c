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

/* Check whether URL redirects to a new location. */

#include "config.h"

#include <locale.h>
#include <stdlib.h>
#include <glib.h>
#include <quvi.h>

#include "examples.h"

typedef quvi_callback_status qcs;

static void help()
{
  g_print("Usage: resolve [-v] [URL]\n");
  examples_cleanup();
  exit(0);
}

static const gchar *example_url = "http://is.gd/34YWaF";

gint main(gint argc, gchar **argv)
{
  gchar *url = NULL;
  gint i = 1;

  g_assert(qr == NULL);
  g_assert(q == NULL);

  setlocale(LC_ALL, "");

  q = quvi_new();
  examples_exit_if_error();

  quvi_set(q, QUVI_OPTION_CALLBACK_STATUS, (qcs) examples_status);

  for (; i<argc; ++i)
    {
      if (g_strcmp0("-v", argv[i]) == 0)
        examples_enable_verbose();
      else if (g_strcmp0("-a", argv[i]) == 0)
        examples_enable_autoproxy();
      else if (g_strcmp0("-h", argv[i]) == 0)
        help();
      else
        url = argv[i];
    }

  if (url == NULL)
    {
      g_printerr("[%s] no input given: using example URL instead\n",
                 __func__);
      url = (gchar*) example_url;
    }

  g_printerr("[%s] url=%s\n", __func__, url);

  qr = quvi_resolve_new(q, url);
  examples_exit_if_error();

  if (quvi_resolve_forwarded(qr) == QUVI_TRUE)
    {
      g_print("[%s] resolved=%s\n",
              __func__, quvi_resolve_destination_url(qr));
    }
  else
    g_print("[%s] no redirection\n", __func__);

  examples_cleanup();

  g_assert(qr == NULL);
  g_assert(q == NULL);

  return (0);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
