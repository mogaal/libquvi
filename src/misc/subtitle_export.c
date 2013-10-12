/* libquvi
 * Copyright (C) 2013  Toni Gundogdu <legatvs@gmail.com>
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
#include "_quvi_subtitle_export_s.h"
/* -- */
#include "misc/subtitle_export.h"

gpointer m_subtitle_export_new(_quvi_t q, const gchar *url)
{
  _quvi_subtitle_export_t qse = g_new0(struct _quvi_subtitle_export_s, 1);
  /* URL */
  qse->url.input = g_string_new(url);
  /* Handle */
  qse->handle.quvi = q;
  /* Format */
  qse->format.to = g_string_new(NULL);
  /* Other */
  qse->data = g_string_new(NULL);
  return (qse);
}

void m_subtitle_export_free(_quvi_subtitle_export_t qse)
{
  if (qse == NULL)
    return;

  g_string_free(qse->url.input, TRUE);
  qse->url.input = NULL;

  g_string_free(qse->format.to, TRUE);
  qse->format.to = NULL;

  g_string_free(qse->data, TRUE);
  qse->data = NULL;

  g_free(qse);
  qse = NULL;
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
