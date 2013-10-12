/* libquvi
 * Copyright (C) 2013  Toni Gundogdu <legatvs@gmail.com>
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
  gboolean format_subrip;
  gboolean autoproxy;
  gboolean verbose;
  gchar *language;
  gchar **url;
};

static struct _opts_s opts;

static const GOptionEntry entries[] =
{
  {
    "format-subrip", 's', 0, G_OPTION_ARG_NONE, &opts.format_subrip,
    "Retrieve selected language, convert it to SubRip and dump to stdout",
    NULL
  },
  {
    "language", 'l', 0, G_OPTION_ARG_STRING, &opts.language,
    "Select subtitle language by ID, or a comma-separated list of IDs", "ID"
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
    G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_STRING_ARRAY, &opts.url, "URL"
  },
  {NULL, 0, 0, 0, NULL, NULL, NULL}
};

static gint opts_new(gint argc, gchar **argv)
{
  GOptionContext *c;
  GError *e;
  gint r;

  c = g_option_context_new("URL");
  r = EXIT_SUCCESS;
  e = NULL;

  g_option_context_add_main_entries(c, entries, NULL);
  g_option_context_set_help_enabled(c, TRUE);

  if (g_option_context_parse(c, &argc, &argv, &e) == FALSE)
    {
      g_printerr("error: %s\n", e->message);
      r = EXIT_FAILURE;
      g_error_free(e);
    }
  g_option_context_free(c);

  if (r == EXIT_SUCCESS && opts.url == NULL)
    {
      g_printerr("error: no input URL\n");
      return (EXIT_FAILURE);
    }
  return (r);
}

static void dump_language(const quvi_subtitle_lang_t l)
{
  gchar *translated, *original, *code, *id, *url;

  quvi_subtitle_lang_get(l, QUVI_SUBTITLE_LANG_PROPERTY_TRANSLATED,
                         &translated);

  quvi_subtitle_lang_get(l, QUVI_SUBTITLE_LANG_PROPERTY_ORIGINAL,
                         &original);

  quvi_subtitle_lang_get(l, QUVI_SUBTITLE_LANG_PROPERTY_CODE, &code);
  quvi_subtitle_lang_get(l, QUVI_SUBTITLE_LANG_PROPERTY_URL, &url);
  quvi_subtitle_lang_get(l, QUVI_SUBTITLE_LANG_PROPERTY_ID, &id);

  g_printerr("  lang: translated=%s, original=%s, code=%s, id=%s\n"
          "    url=%s\n", translated, original, code, id, url);
}

static void dump_languages(const quvi_subtitle_type_t t)
{
  quvi_subtitle_lang_t l;
  while ( (l = quvi_subtitle_type_next(t)) != NULL)
    dump_language(l);
}

static void dump_subtitle(quvi_subtitle_type_t t)
{
  gdouble type, format;
  quvi_subtitle_type_get(t, QUVI_SUBTITLE_TYPE_PROPERTY_FORMAT, &format);
  quvi_subtitle_type_get(t, QUVI_SUBTITLE_TYPE_PROPERTY_TYPE, &type);
  g_printerr("[%s] subtitle: format=%g, type=%g\n", __func__, format, type);
}

static void dump_subtitles()
{
  quvi_subtitle_type_t t;
  gint i;

  i = 0;
  while ( (t = quvi_subtitle_type_next(qsub)) != NULL)
    {
      dump_subtitle(t);
      dump_languages(t);
      ++i;
    }

  if (i == 0)
    g_printerr("subtitles: none found\n");
}

static void opts_free()
{
  g_free(opts.language);
  g_strfreev(opts.url);

  memset(&opts, 0, sizeof(struct _opts_s));
}

typedef quvi_callback_status qcs;

gint main(gint argc, gchar **argv)
{
  quvi_subtitle_export_t e;
  quvi_subtitle_lang_t l;
  gint r, i;

  memset(&opts, 0, sizeof(struct _opts_s));
  setlocale(LC_ALL, "");

  r = opts_new(argc, argv);
  if (r != EXIT_SUCCESS)
    return (r);

  q = quvi_new();
  examples_exit_if_error();

  if (opts.autoproxy == TRUE)
    examples_enable_autoproxy();

  if (opts.verbose == TRUE)
    examples_enable_verbose();

  if (opts.format_subrip == TRUE && opts.language == NULL)
    {
      g_printerr("error: --format-subrip requires --language\n");
      examples_cleanup();
      opts_free();
      return (EXIT_FAILURE);
    }

  quvi_set(q, QUVI_OPTION_CALLBACK_STATUS, (qcs) examples_status);

  for (r=EXIT_SUCCESS, i=0; (opts.url[i] != NULL && r == EXIT_SUCCESS); ++i)
    {
      qsub = quvi_subtitle_new(q, opts.url[i]);
      examples_exit_if_error();

      if (opts.language != NULL)
        {
          l = quvi_subtitle_select(qsub, opts.language);
          examples_exit_if_error();
          dump_language(l);
          if (opts.format_subrip == TRUE)
            {
              e = quvi_subtitle_export_new(l, "srt");
              examples_exit_if_error();
              g_print("%s", quvi_subtitle_export_data(e));
              quvi_subtitle_export_free(e);
            }
        }
      else
        dump_subtitles();
    }
  examples_cleanup();
  opts_free();

  return (r);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
