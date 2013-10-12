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

#include "config.h"

#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_macro.h"
#include "_quvi_net_resolve_s.h"
/* -- */
#include "net/resolve.h"
#include "net/handle.h"

extern QuviError c_resolve(_quvi_t, _quvi_net_resolve_t);

QuviError n_resolve(_quvi_t q, _quvi_net_resolve_t r)
{
  QuviError rc = QUVI_OK;

  if (q->cb.status != NULL)
    {
      const glong p = q_makelong(QUVI_CALLBACK_STATUS_RESOLVE, 0);

      if (q->cb.status(p, 0, q->cb.userdata.status) != QUVI_OK)
        return (QUVI_ERROR_CALLBACK_ABORTED);
    }

  if (q->cb.resolve != NULL)
    rc = q->cb.resolve(r);
  else
    rc = c_resolve(q, r);

  if (rc == QUVI_OK)
    {
      if (q->cb.status != NULL)
        {
          const glong p = q_makelong(QUVI_CALLBACK_STATUS_RESOLVE,
                                     QUVI_CALLBACK_STATUS_DONE);

          if (q->cb.status(p, 0, q->cb.userdata.status) != QUVI_OK)
            rc = QUVI_ERROR_CALLBACK_ABORTED;
        }
    }
  else
    {
      if (r->status.errmsg->len >0)
        g_string_assign(q->status.errmsg, r->status.errmsg->str);
      else
        {
          g_string_assign(q->status.errmsg,
                          "unknown error: callback returned empty errmsg");
        }
    }

  q->status.resp_code = r->status.resp_code;
  q->status.rc = rc;

  return (rc);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
