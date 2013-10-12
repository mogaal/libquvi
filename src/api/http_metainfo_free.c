/* libquvi
 * Copyright (C) 2012  Toni Gundogdu <legatvs@gmail.com>
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

/** @file http_metainfo_free.c */

#include "config.h"

#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_http_metainfo_s.h"

/** @brief Free all of memory used by a HTTP meta-info handle
@note If handle is NULL the function simply returns
@ingroup http_metainfo
*/
void quvi_http_metainfo_free(quvi_http_metainfo_t handle)
{
  _quvi_http_metainfo_t v = (_quvi_http_metainfo_t) handle;

  if (handle == NULL)
    return;

  g_string_free(v->url.input, TRUE);
  v->url.input = NULL;

  g_string_free(v->content_type, TRUE);
  v->content_type = NULL;

  g_string_free(v->file_ext, TRUE);
  v->file_ext = NULL;

  g_free(v);
  v = NULL;
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
