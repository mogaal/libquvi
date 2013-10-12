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

/** @file free.c */

#include "config.h"

#include <lualib.h>
#include <proxy.h>
#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_script_s.h"
/* -- */
#include "misc/script_free.h"
#include "misc/slst.h"

/** @cond NODOC */
extern void c_close(_quvi_t);
/** @endcond */

/** @brief Free all of memory used by a library handle
@note If handle is NULL the function simply returns
@sa @ref getting_started
@ingroup lib
*/
void quvi_free(quvi_t handle)
{
  _quvi_t q = (_quvi_t) handle;

  if (handle == NULL)
    return;

  c_close(q);

  /* Strings. */

  g_string_free(q->opt.user_agent, TRUE);
  q->opt.user_agent = NULL;

  g_string_free(q->status.errmsg, TRUE);
  q->status.errmsg = NULL;

  /* Scripts. */

  m_slist_free_full(q->scripts.subtitle_export, m_script_free);
  q->scripts.subtitle_export = NULL;

  m_slist_free_full(q->scripts.subtitle, m_script_free);
  q->scripts.subtitle = NULL;

  m_slist_free_full(q->scripts.playlist, m_script_free);
  q->scripts.playlist = NULL;

  m_slist_free_full(q->scripts.media, m_script_free);
  q->scripts.media = NULL;

  m_slist_free_full(q->scripts.scan, m_script_free);
  q->scripts.scan = NULL;

  m_slist_free_full(q->scripts.util, m_script_free);
  q->scripts.util = NULL;

  /* Handles. */

  if (q->handle.lua != NULL)
    {
      lua_close(q->handle.lua);
      q->handle.lua = NULL;
    }

  if (q->handle.proxy != NULL)
    {
      px_proxy_factory_free(q->handle.proxy);
      q->handle.proxy = NULL;
    }

  g_free(q);
  q = NULL;
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
