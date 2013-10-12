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

/** @file subtitle_type_reset.c */

#include "config.h"

#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_subtitle_s.h"

/** @brief Resets to the first available @ref sub_type
@sa @ref parse_subtitle
@ingroup subprop
*/
void quvi_subtitle_type_reset(quvi_subtitle_t handle)
{
  _quvi_subtitle_t qsub = (_quvi_subtitle_t) handle;

  /* If G_DISABLE_CHECKS is defined then the check is not performed. */
  g_return_if_fail(handle != NULL);

  qsub->curr.type = NULL;
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
