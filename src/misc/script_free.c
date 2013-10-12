/* libquvi
 * Copyright (C) 2012-2013  Toni Gundogdu <legatvs@gmail.com>
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

#include "config.h"

#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_script_s.h"
/* -- */
#include "misc/script_free.h"

void m_script_free(gpointer p, gpointer userdata)
{
  _quvi_script_t qs = (_quvi_script_t) p;

  if (p == NULL)
    return;

  /* Export. */

  g_string_free(qs->export.format, TRUE);
  qs->export.format = NULL;

  /* Media, playlist. */

  g_string_free(qs->domains, TRUE);
  qs->domains = NULL;

  /* Any */

  g_string_free(qs->fpath, TRUE);
  qs->fpath = NULL;

  g_string_free(qs->fname, TRUE);
  qs->fname = NULL;

  g_string_free(qs->sha1, TRUE);
  qs->sha1 = NULL;

  g_free(p);
  p = NULL;
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
