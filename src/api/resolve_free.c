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

/** @file resolve_free.c */

#include "config.h"

#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_net_resolve_s.h"
/* -- */
#include "net/handle.h"

/** @brief Free all of memory used by a resolve handle
@note If handle is NULL the function simply returns
@ingroup resolve
*/
void quvi_resolve_free(quvi_resolve_t handle)
{
  _quvi_net_resolve_t r = (_quvi_net_resolve_t) handle;

  if (handle == NULL)
    return;

  n_resolve_free(r);
  r = NULL;
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
