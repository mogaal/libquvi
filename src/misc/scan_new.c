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

#include "config.h"

#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_scan_s.h"
/* -- */
#include "misc/scan_new.h"

gpointer m_scan_new(_quvi_t q, const gchar *url)
{
  _quvi_scan_t qs = g_new0(struct _quvi_scan_s, 1);
  /* URL */
  qs->url.input = g_string_new(url);
  /* Handle */
  qs->handle.quvi = q;
  return (qs);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
