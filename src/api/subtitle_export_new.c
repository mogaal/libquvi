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

/** @file subtitle_export_new.c */

#include "config.h"

#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_subtitle_export_s.h"
#include "_quvi_subtitle_s.h"
/* -- */
#include "misc/match_subtitle_export_script.h"

/** @brief Create a new subtitle export handle
@return New handle, @ref quvi_subtitle_export_free it when done using it
@note Use @ref quvi_ok to check if an occurred
@sa @ref parse_subtitle
@ingroup subprop
*/
quvi_subtitle_export_t
quvi_subtitle_export_new(quvi_subtitle_lang_t handle, char *to_format)
{
  _quvi_subtitle_export_t qse;
  _quvi_subtitle_lang_t qsl;
  _quvi_t q;

  /* If G_DISABLE_CHECKS is defined then the check is not performed. */
  g_return_val_if_fail(handle != NULL, NULL);

  qsl = (_quvi_subtitle_lang_t) handle;
  q = qsl->handle.quvi;
  qse = NULL;

  q->status.rc = m_match_subtitle_export_script(q, &qse, qsl, to_format);
  return (qse);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
