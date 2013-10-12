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
  gboolean verbose;
  gboolean online;
  gchar *type;
  gchar **url;
};

static struct _opts_s opts;

static const GOptionEntry entries[] =
{
  {
    "type", 't', 0, G_OPTION_ARG_STRING, &opts.type,
    "Script type", "TYPE"
  },
  {
    "online", 's', 0, G_OPTION_ARG_NONE, &opts.online,
    "Check online", NULL
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

struct _type_lookup_s
{
  QuviSupportsType to;
  const gchar *from;
};

static const struct _type_lookup_s type_conv[] =
{
  {QUVI_SUPPORTS_TYPE_PLAYLIST, "playlist"},
  {QUVI_SUPPORTS_TYPE_SUBTITLE, "subtitle"},
  {QUVI_SUPPORTS_TYPE_MEDIA,    "media"},
  {QUVI_SUPPORTS_TYPE_ANY,      "any"},
  {0, NULL}
};

static gchar **type_sv()
{
  gchar **r;
  gint i,j;

  i=0;
  while (type_conv[i].from != NULL) ++i;
  r = g_new(gchar*, i+1);

  i=j=0;
  while (type_conv[j].from != NULL)
    r[i++] = g_strdup(type_conv[j++].from);
  r[i] = NULL;

  return (r);
}

static gboolean chk_type_values()
{
  gchar **v, *s;
  gboolean r;

  v = type_sv();
  r = examples_chk_val_s(opts.type, v, &s);
  if (r == FALSE)
    {
      g_printerr(
        "error: invalid value (`%s') for the option `--type'\n", s);
    }

  g_strfreev(v);
  v = NULL;

  return (r);
}

static QuviSupportsType type_n()
{
  gint i;
  for (i=0; type_conv[i].from != NULL; ++i)
    {
      if (g_strcmp0(opts.type, type_conv[i].from) == 0)
        return (type_conv[i].to);
    }
  return (QUVI_SUPPORTS_TYPE_MEDIA);
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

  /* Set default values. */

  if (opts.type == NULL)
    opts.type = g_strdup("any");

  /* Check input. */

  if (chk_type_values() == FALSE)
    return (EXIT_FAILURE);

  if (opts.url == NULL)
    {
      g_printerr("error: no input URL\n");
      return (EXIT_FAILURE);
    }

  return (r);
}

static void opts_free()
{
  g_strfreev(opts.url);
  opts.url = NULL;

  g_free(opts.type);
  opts.type = NULL;
}

static QuviSupportsMode mode = QUVI_SUPPORTS_MODE_OFFLINE;
static QuviSupportsType type = QUVI_SUPPORTS_TYPE_ANY;

static void chk_support(const gchar *url)
{
  const QuviBoolean r = quvi_supports(q, url, mode, type);

  /* Always check for any network errors with QUVI_SUPPORTS_MODE_ONLINE. */
  if (r == FALSE && mode == QUVI_SUPPORTS_MODE_ONLINE)
    {
      const glong ec = quvi_errcode(q);
      if (ec != QUVI_ERROR_NO_SUPPORT)
        {
          g_printerr("\nerror: %s\n", quvi_errmsg(q));
          return;
        }
    }
  g_print("%s: %s\n", url, (r == QUVI_TRUE) ? "yes":"no");
}

typedef quvi_callback_status qcs;

gint main(gint argc, gchar **argv)
{
  gint i,r;

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

  if (opts.verbose == TRUE)
    examples_enable_verbose();

  if (opts.online == TRUE)
    mode = QUVI_SUPPORTS_MODE_ONLINE;

  quvi_set(q, QUVI_OPTION_CALLBACK_STATUS, (qcs) examples_status);
  type = type_n();

  g_printerr("[%s] type=%s (0x%x), mode=0x%x\n",
             __func__, opts.type, type, mode);

  for (i=0; opts.url[i] != NULL; ++i)
    chk_support(opts.url[i]);

  opts_free();
  examples_cleanup();

  g_assert(q == NULL);
  return (r);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
