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

/** @file scan_new.c */

#include "config.h"

#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_net_s.h"
#include "_quvi_scan_s.h"
/* -- */
#include "net/handle.h"
#include "net/fetch.h"
#include "lua/exec.h"
#include "misc/scan_new.h"

/** @cond NODOC */
struct _exec_scan_script_s
{
  struct
  {
    _quvi_scan_t scan;
    _quvi_net_t net;
  } handle;
};

typedef struct _exec_scan_script_s *_exec_scan_script_t;

static void _exec_scan_script(gpointer p, gpointer userdata)
{
  _exec_scan_script_t ess = (_exec_scan_script_t) userdata;
  _quvi_t q = (ess != NULL) ? ess->handle.scan->handle.quvi : NULL;

  if (p == NULL || userdata == NULL)
    return;

  if (q->status.rc == QUVI_OK)
    {
      q->status.rc = l_exec_scan_script_parse(
                       ess->handle.scan, p,
                       ess->handle.net->fetch.content->str);
    }
}
/** @endcond */

extern gint c_reset(_quvi_t);

/** @brief Scan URL contents for supported embedded media URLs
@return New handle, @ref quvi_scan_free it when done using it
@note
  - Calling this function restores the current @ref sess_handle to its
    initial state (cookies cleared, custom headers cleared, default
    user-agent string restored, ...)
  - Use @ref quvi_ok for checking if an error occurred
@sa @ref scan_media
@ingroup scan
*/
quvi_scan_t quvi_scan_new(quvi_t handle, const char *url)
{
  _quvi_t q = (_quvi_t) handle;
  _quvi_scan_t qs = NULL;

  /* If G_DISABLE_CHECKS is defined then the check is not performed. */
  g_return_val_if_fail(handle != NULL, NULL);
  g_return_val_if_fail(url != NULL, NULL);

  c_reset(q);

  qs = m_scan_new(q, url);
  {
    _quvi_net_t n = NULL;
    n_fetch(q, &n, qs->url.input->str, NULL);

    if (quvi_ok(q) == QUVI_TRUE)
      {
        struct _exec_scan_script_s e;

        e.handle.scan = qs;
        e.handle.net = n;

        g_slist_foreach(q->scripts.scan, _exec_scan_script, &e);
      }
    n_free(n);
    n = NULL;
  }
  return (qs);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
