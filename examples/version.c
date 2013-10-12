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

#include <glib.h>
#include <quvi.h>

gint main(gint argc, gchar **argv)
{
  g_print("libquvi %s\n", quvi_version(QUVI_VERSION));
  g_print("  built on %s for %s\n    with %s\n",
          quvi_version(QUVI_VERSION_BUILD_TIME),
          quvi_version(QUVI_VERSION_BUILD_TARGET),
          quvi_version(QUVI_VERSION_BUILD_CC_CFLAGS));
  g_print("libquvi-scripts %s\n"
          "  configuration: %s\n",
          quvi_version(QUVI_VERSION_SCRIPTS),
          quvi_version(QUVI_VERSION_SCRIPTS_CONFIGURATION));
  g_print("configuration:\n  %s\n",
          quvi_version(QUVI_VERSION_CONFIGURATION));
  return (0);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
