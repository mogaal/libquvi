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

/** @file http_metainfo_new.c */

#include "config.h"

#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_http_metainfo_s.h"

static gpointer _http_metainfo_new(_quvi_t q, const gchar *url)
{
  _quvi_http_metainfo_t qv = g_new0(struct _quvi_http_metainfo_s, 1);
  qv->content_type = g_string_new(NULL);
  qv->file_ext = g_string_new(NULL);
  qv->url.input = g_string_new(url);
  qv->handle.quvi = q;
  return (qv);
}

/** @cond NODOC */
QuviError n_http_metainfo(_quvi_http_metainfo_t);
/** @endcond */

/** @brief Query meta-info properties for an URL
@return New handle, @ref quvi_http_metainfo_free when done using it
@note
 - Support for determining the file extension is very limited
 - Use @ref quvi_ok for checking if an error occurred
@ingroup http_metainfo
*/
quvi_http_metainfo_t quvi_http_metainfo_new(quvi_t handle, const char *url)
{
  _quvi_http_metainfo_t v;
  _quvi_t q;

  /* If G_DISABLE_CHECKS is defined then the check is not performed. */
  g_return_val_if_fail(handle != NULL, NULL);
  g_return_val_if_fail(url != NULL, NULL);

  q = (_quvi_t) handle;
  v = _http_metainfo_new(q, url);

  q->status.rc = n_http_metainfo(v);

  return (v);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
