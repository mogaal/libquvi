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

#include <locale.h>
#include <stdlib.h>
#include <glib.h>
#include <quvi.h>

#include "examples.h"

static void usage()
{
  g_printerr("Usage: http_metainfo <URL>\n");
  exit(0);
}

typedef quvi_callback_status qcs;

gint main(gint argc, gchar **argv)
{
  gchar *url = NULL;
  gint i = 1;

  g_assert(qmi == NULL);
  g_assert(q == NULL);

  setlocale(LC_ALL, "");

  if (argc < 2)
    usage();

  q = quvi_new();
  examples_exit_if_error();

  for (; i<argc; ++i)
    {
      if (g_strcmp0("-v", argv[i]) == 0)
        examples_enable_verbose();
      else if (g_strcmp0("-a", argv[i]) == 0)
        examples_enable_autoproxy();
      else
        url = argv[i];
    }

  if (url == NULL)
    usage();

  quvi_set(q, QUVI_OPTION_CALLBACK_STATUS, (qcs) examples_status);

  qmi = quvi_http_metainfo_new(q, url);
  examples_exit_if_error();
  {
    gchar *ct, *fe;
    gdouble cl;

    quvi_http_metainfo_get(qmi,
      QUVI_HTTP_METAINFO_PROPERTY_FILE_EXTENSION, &fe);

    quvi_http_metainfo_get(qmi,
      QUVI_HTTP_METAINFO_PROPERTY_CONTENT_TYPE, &ct);

    quvi_http_metainfo_get(qmi,
      QUVI_HTTP_METAINFO_PROPERTY_LENGTH_BYTES, &cl);

    g_print("content_type=%s\nlength_bytes=%.0f\nfile_ext=%s\n",
            ct, cl, fe);
  }
  examples_cleanup();

  g_assert(qmi == NULL);
  g_assert(q == NULL);

  return (0);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
