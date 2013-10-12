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
#include <glib.h>
#include <quvi.h>

#include "examples.h"

static gint usage(const gchar *argv0)
{
  g_printerr("usage: %s <content-type>\n", argv0);
  return (EXIT_SUCCESS);
}

gint main(gint argc, gchar **argv)
{
  quvi_file_ext_t qfe;
  gint i;

  g_assert(q == NULL);
  setlocale(LC_ALL, "");

  if (argc < 2)
    return (usage(argv[0]));

  q = quvi_new();
  examples_exit_if_error();

  for (i=1; i<argc; ++i)
    {
      qfe = quvi_file_ext_new(q, argv[i]);
      examples_exit_if_error();

      g_print("%s -> %s\n", argv[i], quvi_file_ext_get(qfe));
      quvi_file_ext_free(qfe);
    }
  examples_cleanup();

  return (EXIT_SUCCESS);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
