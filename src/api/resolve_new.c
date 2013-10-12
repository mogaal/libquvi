/* libquvi
 * Copyright (C) 2012,2013  Toni Gundogdu <legatvs@gmail.com>
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

/** @file resolve_new.c */

#include "config.h"

#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_net_resolve_s.h"
/* -- */
#include "misc/resolve.h"
#include "net/resolve.h"
#include "net/handle.h"

/** @brief Resolve URL redirection
@return New handle, @ref quvi_resolve_free it when done using it
@note Use @ref quvi_ok for checking if an error occurred
@ingroup resolve
*/
quvi_resolve_t quvi_resolve_new(quvi_t handle, const char *url)
{
  _quvi_net_resolve_t r;
  _quvi_t q;

  /* If G_DISABLE_CHECKS is defined then the check is not performed. */
  g_return_val_if_fail(handle != NULL, NULL);
  g_return_val_if_fail(url != NULL, NULL);

  q = (_quvi_t) handle;
  r = n_resolve_new(q, url);

  m_resolve_url(q, url, r->url.dst);
  return (r);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
