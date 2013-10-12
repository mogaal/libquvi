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

#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <quvi.h>

#include "examples.h"

struct _opts_s
{
  gchar **property;
  gchar *type;
};

static struct _opts_s opts;

static const GOptionEntry entries[] =
{
  {
    "property", 'p', 0, G_OPTION_ARG_STRING_ARRAY, &opts.property,
    "Script property to output", "PROPERTY"
  },
  {
    "type", 't', 0, G_OPTION_ARG_STRING, &opts.type,
    "Script type", "TYPE"
  },
  {NULL, 0, 0, 0, NULL, NULL, NULL}
};

struct _property_lookup_s
{
  QuviScriptProperty to;
  const gchar *from;
};

static const struct _property_lookup_s property_conv[] =
{
  {QUVI_SCRIPT_PROPERTY_EXPORT_FORMAT, "export.format"},
  {QUVI_SCRIPT_PROPERTY_FILEPATH, "filepath"},
  {QUVI_SCRIPT_PROPERTY_FILENAME, "filename"},
  {QUVI_SCRIPT_PROPERTY_DOMAINS,  "domains"},
  {QUVI_SCRIPT_PROPERTY_SHA1,     "sha1"},
  {0, NULL}
};

static void dump_script(QuviScriptType type)
{
  QuviScriptProperty qsp;
  gchar *s;
  gint i,j;

  for (i=0; opts.property[i] != NULL; ++i)
    {
      const gchar *p = opts.property[i];
      for (j=0; property_conv[j].from != NULL; ++j)
        {
          if (g_strcmp0(p, property_conv[j].from) == 0)
            {
              qsp = property_conv[j].to;
              break;
            }
        }
      quvi_script_get(q, type, qsp, &s);
      if (i == 0)
        g_print("[%s]\n", __func__);
      g_print("  %s=%s\n", p, s);
    }
}

static gchar **property_sv()
{
  gchar **r;
  gint i,j;

  i=0;
  while (property_conv[i].from != NULL) ++i;
  r = g_new(gchar*, i+1);

  i=j=0;
  while (property_conv[j].from != NULL)
    r[i++] = g_strdup(property_conv[j++].from);
  r[i] = NULL;

  return (r);
}

static gboolean chk_property_values()
{
  gchar **v, *s;
  gboolean r;

  v = property_sv();
  r = examples_chk_val_sv(opts.property, v, &s);
  if (r == FALSE)
    {
      g_printerr(
        "error: invalid value (`%s') for the option `--property'\n", s);
    }
  g_strfreev(v);
  return (r);
}

struct _type_lookup_s
{
  QuviScriptType to;
  const gchar *from;
};

static const struct _type_lookup_s type_conv[] =
{
  {QUVI_SCRIPT_TYPE_SUBTITLE_EXPORT, "subtitle.export"},
  {QUVI_SCRIPT_TYPE_SUBTITLE, "subtitle"},
  {QUVI_SCRIPT_TYPE_PLAYLIST, "playlist"},
  {QUVI_SCRIPT_TYPE_MEDIA,    "media"},
  {QUVI_SCRIPT_TYPE_SCAN,     "scan"},
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

static QuviScriptType type_n()
{
  gint i;
  for (i=0; type_conv[i].from != NULL; ++i)
    {
      if (g_strcmp0(opts.type, type_conv[i].from) == 0)
        return (type_conv[i].to);
    }
  return (QUVI_SCRIPT_TYPE_MEDIA);
}

static gint opts_new(gint argc, gchar **argv)
{
  GOptionContext *c;
  GError *e;
  gint r;

  c = g_option_context_new(NULL);
  r = EXIT_SUCCESS;
  e = NULL;

  g_option_context_set_help_enabled(c, TRUE);
  g_option_context_add_main_entries(c, entries, NULL);

  if (g_option_context_parse(c, &argc, &argv, &e) == FALSE)
    {
      g_printerr("error: %s\n", e->message);
      g_error_free(e);
      r = EXIT_FAILURE;
    }
  g_option_context_free(c);

  /* Set the defaults. */

  if (opts.property == NULL)
    {
      gchar *v[] = {"filename", "domains", NULL};
      opts.property = g_strdupv(v);
    }

  if (opts.type == NULL)
    opts.type = g_strdup("media");

  /* Check input. */

  if (chk_property_values() == FALSE)
    return (EXIT_FAILURE);

  if (chk_type_values() == FALSE)
    return (EXIT_FAILURE);

  return (r);
}

static void opts_free()
{
  g_strfreev(opts.property);
  opts.property = NULL;

  g_free(opts.type);
  opts.type = NULL;
}

gint main(gint argc, gchar **argv)
{
  QuviScriptType type;
  gint r;

  setlocale(LC_ALL, "");

  memset(&opts, 0, sizeof(struct _opts_s));
  g_assert(q == NULL);

  r = opts_new(argc, argv);
  if (r != EXIT_SUCCESS)
    return (r);

  q = quvi_new();
  examples_exit_if_error();

  type = type_n();
  {
    gchar *p = g_strjoinv(",", opts.property);
    g_printerr("[%s] type=%s (0x%x), property=%s\n",
               __func__, opts.type, type, p);
    g_free(p);
  }

  while (quvi_script_next(q, type) == QUVI_TRUE)
    dump_script(type);

  examples_cleanup();
  opts_free();

  g_assert(q == NULL);
  return (r);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
