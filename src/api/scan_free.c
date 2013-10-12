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

/** @file scan_free.c */

#include "config.h"

#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_scan_s.h"
/* -- */
#include "misc/slst.h"

static void _url_free(gpointer p, gpointer userdata)
{
  if (p == NULL)
    return;

  g_free(p);
  p = NULL;
}

/** @brief Free all of memory used by a scan handle
@note If handle is NULL the function simply returns
@sa @ref scan_media
@ingroup scan
*/
void quvi_scan_free(quvi_scan_t handle)
{
  _quvi_scan_t qs = (_quvi_scan_t) handle;

  if (handle == NULL)
    return;

  /* URLs */

  m_slist_free_full(qs->url.media, _url_free);
  qs->url.media = NULL;

  g_string_free(qs->url.input, TRUE);
  qs->url.input = NULL;

  g_free(qs);
  qs = NULL;
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
