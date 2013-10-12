/* libquvi
 * Copyright (C) 2012,2013  Toni Gundogdu <legatvs@gmail.com>
 *
 * This file is part of libquvi <http://quvi.sourceforge.net/>.
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General
 * Public License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

/** @file version.c */

#include "config.h"

#include <string.h>
#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"

static void kval(GKeyFile *f, const gchar *k, gchar *dst,
                 const gsize dst_size)
{
  gchar *r = g_key_file_get_string(f, "libquvi-scripts", k, NULL);
  if (r != NULL)
    {
      gchar *s = g_strescape(g_strstrip(r), NULL);
      g_snprintf(dst, dst_size, "%s", s);
      g_free(s);
      g_free(r);
    }
}

struct scripts_version_s
{
  gchar configuration[128];
  gchar version[32];
};

typedef struct scripts_version_s *scripts_version_t;

static void scripts_version_read(scripts_version_t v)
{
  GKeyFile *f = g_key_file_new();
  v->configuration[0]= '\0';
  v->version[0]= '\0';
  if (g_key_file_load_from_file(f, VERSIONFILE, G_KEY_FILE_NONE, NULL)==TRUE)
    {
      kval(f, "configuration", v->configuration, sizeof(v->configuration));
      kval(f, "version", v->version, sizeof(v->version));
    }
  g_key_file_free(f);
}

static struct scripts_version_s sv;

static const gchar *read_scripts_version(const QuviVersion qv)
{
  scripts_version_read(&sv);
  if (qv == QUVI_VERSION_SCRIPTS_CONFIGURATION)
    return (sv.configuration);
  else
    return (sv.version);
}

static const gchar *_version[] =
{
#ifdef VN
  VN
#else
  PACKAGE_VERSION
#endif
  ,
  BUILD_OPTS,
  CC ", " CFLAGS,
  CANONICAL_TARGET,
  BUILD_TIME
};

/** @return NULL-terminated version string
@note Do not attempt to free the returned string
@ingroup convenience
*/
const char *quvi_version(QuviVersion version)
{
  switch (version)
    {
    case QUVI_VERSION_SCRIPTS_CONFIGURATION:
    case QUVI_VERSION_SCRIPTS:
      return (read_scripts_version(version));

    case QUVI_VERSION_CONFIGURATION:
    case QUVI_VERSION_BUILD_CC_CFLAGS:
    case QUVI_VERSION_BUILD_TARGET:
    case QUVI_VERSION_BUILD_TIME:
      return (_version[version]);

    default:
      break;
    }
  return (_version[QUVI_VERSION]);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
