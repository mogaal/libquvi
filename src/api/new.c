/* libquvi
 * Copyright (C) 2012-2013  Toni Gundogdu <legatvs@gmail.com>
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

/** @file new.c */

#include "config.h"

#include <glib/gi18n-lib.h>
#include <glib.h>
#include <string.h>
#include <proxy.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"

static gpointer _quvi_new()
{
  _quvi_t q = g_new0(struct _quvi_s, 1);
  q->opt.user_agent = g_string_new(NULL);
  q->status.errmsg = g_string_new(NULL);
  return (q);
}

/** @cond NODOC */
extern QuviError m_scan_scripts(_quvi_t q);
extern QuviError l_init(_quvi_t q);
extern QuviError c_init(_quvi_t q);
extern QuviError g_init();
/** @endcond */

/** @brief Create a new library handle
@return New handle, @ref quvi_free it when done using it
@note Use @ref quvi_ok for checking if an error occurred
@sa @ref getting_started
@ingroup lib
*/
quvi_t quvi_new()
{
  _quvi_t q;

  bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);

  q = _quvi_new();
  q->status.rc = l_init(q);

  if (q->status.rc == QUVI_OK)
    q->status.rc = m_scan_scripts(q);

  if (q->status.rc == QUVI_OK)
    q->status.rc = c_init(q);

  if (q->status.rc == QUVI_OK)
    q->status.rc = g_init();

  if (q->status.rc == QUVI_OK)
    {
      q->handle.proxy = px_proxy_factory_new();
      if (q->handle.proxy == NULL)
        q->status.rc = QUVI_ERROR_PROXY_INIT;
    }
  return (q);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
