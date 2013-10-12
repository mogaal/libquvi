/* quvi
 * Copyright (C) 2013  Toni Gundogdu <legatvs@gmail.com>
 *
 * This file is part of quvi <http://quvi.sourceforge.net/>.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

/** @file errcode.c */

#include "config.h"

#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"

/** @return Last library error code
@ingroup convenience
*/
QuviError quvi_errcode(quvi_t handle)
{
  _quvi_t q = (_quvi_t) handle;

  if (handle == NULL)
    return (QUVI_ERROR_INVALID_ARG);

  return (q->status.rc);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
