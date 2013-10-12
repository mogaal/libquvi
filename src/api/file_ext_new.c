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

/** @file file_ext_new.c */

#include "config.h"

#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_file_ext_s.h"

static gpointer _file_ext_new(_quvi_t q)
{
  _quvi_file_ext_t qfe = g_new0(struct _quvi_file_ext_s, 1);
  qfe->result.file_ext = g_string_new(NULL);
  qfe->handle.q = q;
  return (qfe);
}

/** @cond NODOC */
extern QuviError l_exec_util_to_file_ext(_quvi_t, const gchar*, GString*);
/** @endcond */

/** @brief Convert content-type to file extension
@return New handle, @ref quvi_file_ext_free it when done using it
@note
  - The result is a guess at best and returned by the
    util/to_file_ext.lua of libquvi-scripts
  - Use @ref quvi_ok for checking if an error occurred
@sa @ref quvi_file_ext_free
@ingroup convenience
*/
quvi_file_ext_t quvi_file_ext_new(quvi_t handle, const char *s)
{
  _quvi_file_ext_t qfe;
  _quvi_t q;

  /* If G_DISABLE_CHECKS is defined then the check is not performed. */
  g_return_val_if_fail(handle != NULL, NULL);
  g_return_val_if_fail(s != NULL, NULL);

  qfe = _file_ext_new(handle);
  q = (_quvi_t) handle;

  q->status.rc = l_exec_util_to_file_ext(q, s, qfe->result.file_ext);

  return (qfe);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
