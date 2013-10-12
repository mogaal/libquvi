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

#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <quvi.h>

#include "examples.h"

struct _opts_s
{
  gboolean autoproxy;
  gboolean cookies;
  gboolean verbose;
  gboolean best;
  gchar *stream;
  gchar **url;
};

static struct _opts_s opts;

static const GOptionEntry entries[] =
{
  {
    "stream", 's', 0, G_OPTION_ARG_STRING, &opts.stream,
    "Select stream ID, or a comma-separated list of IDs", "ID"
  },
  {
    "best", 'b', 0, G_OPTION_ARG_NONE, &opts.best,
    "Choose the best available stream, negates --stream", NULL
  },
  {
    "autoproxy", 'a', 0, G_OPTION_ARG_NONE, &opts.autoproxy,
    "Enable the autoproxy feature", NULL
  },
  {
    "verbose", 'v', 0, G_OPTION_ARG_NONE, &opts.verbose,
    "Verbose libcurl output", NULL
  },
  {
    "cookies", 'c', 0, G_OPTION_ARG_NONE, &opts.cookies,
    "Parse and use HTTP cookies", NULL
  },
  {
    G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_STRING_ARRAY, &opts.url, "URL"
  },
  {NULL, 0, 0, 0, NULL, NULL, NULL}
};

static void dump_stream()
{
  gchar *url, *id;

  quvi_media_get(qm, QUVI_MEDIA_STREAM_PROPERTY_URL, &url);
  quvi_media_get(qm, QUVI_MEDIA_STREAM_PROPERTY_ID, &id);

  g_print("  id='%s', url='%s'\n", id, url);
}

static void dump_streams()
{
  while (quvi_media_stream_next(qm) == QUVI_TRUE)
    dump_stream();
}

static gint opts_new(gint argc, gchar **argv)
{
  GOptionContext *c;
  GError *e;
  gint r;

  c = g_option_context_new("URL");
  r = EXIT_SUCCESS;
  e = NULL;

  g_option_context_set_help_enabled(c, TRUE);
  g_option_context_add_main_entries(c, entries, NULL);

  if (g_option_context_parse(c, &argc, &argv, &e) == FALSE)
    {
      g_printerr("error: %s\n", e->message);
      g_error_free(e);
      r = EXIT_FAILURE;
      e = NULL;
    }

  g_option_context_free(c);
  c = NULL;

  /* Check input. */

  if (opts.url == NULL)
    {
      g_printerr("error: no input URL\n");
      return (EXIT_FAILURE);
    }

  return (r);
}

static void opts_free()
{
  g_free(opts.stream);
  opts.stream = NULL;

  g_strfreev(opts.url);
  opts.url = NULL;
}

typedef quvi_callback_status qcs;

gint main(gint argc, gchar **argv)
{
  gint i,r;
  gchar *s;

  g_assert(qm == NULL);
  g_assert(q == NULL);

  memset(&opts, 0, sizeof(struct _opts_s));
  setlocale(LC_ALL, "");

  r = opts_new(argc, argv);
  if (r != EXIT_SUCCESS)
    return (r);

  q = quvi_new();
  examples_exit_if_error();

  if (opts.autoproxy == TRUE)
    examples_enable_autoproxy();

  if (opts.cookies == TRUE)
    examples_enable_cookies();

  if (opts.verbose == TRUE)
    examples_enable_verbose();

  quvi_set(q, QUVI_OPTION_CALLBACK_STATUS, (qcs) examples_status);

  for (i=0; opts.url[i] != NULL; ++i)
    {
      qm = quvi_media_new(q, opts.url[i]);
      examples_exit_if_error();

      quvi_media_get(qm, QUVI_MEDIA_PROPERTY_TITLE, &s);
      g_print("[%s]\n  title='%s'\n", __func__, s);

      if (opts.best == TRUE)
        {
          quvi_media_stream_choose_best(qm);
          dump_stream();
        }
      else if (opts.stream != NULL)
        {
          quvi_media_stream_select(qm, opts.stream);
          examples_exit_if_error();
          dump_stream();
        }
      else
        dump_streams();

      quvi_media_free(qm);
      qm = NULL;
    }

  opts_free();
  examples_cleanup();

  g_assert(qm == NULL);
  g_assert(q == NULL);

  return (r);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
