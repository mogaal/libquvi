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

/** @file subtitle_lang_next.c */

#include "config.h"

#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_subtitle_s.h"

/** @brief Traverse to next available language @ref sub_type
@return NULL if none was found
@sa @ref parse_subtitle
@ingroup @ref subprop
*/
const quvi_subtitle_lang_t quvi_subtitle_lang_next(quvi_subtitle_type_t handle)
{
  _quvi_subtitle_type_t qst = (_quvi_subtitle_type_t) handle;

  /* If G_DISABLE_CHECKS is defined then the check is not performed. */
  g_return_val_if_fail(handle != NULL, QUVI_FALSE);

  qst->curr.language = (qst->curr.language != NULL)
                       ? g_slist_next(qst->curr.language)
                       : qst->languages;

  return ((qst->curr.language != NULL)
          ? qst->curr.language->data
          : NULL);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */

