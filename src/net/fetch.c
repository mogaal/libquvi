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

#include <lauxlib.h>
#include <string.h>
#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_net_s.h"
#include "_quvi_macro.h"
/* -- */
#include "lua/quvi/opts.h"
#include "net/handle.h"

extern gchar* to_utf8(const gchar*, const gchar*);
extern QuviError c_fetch(_quvi_net_t);

void n_fetch(_quvi_t q, _quvi_net_t *n, const gchar *url, GSList *opts)
{
  *n = n_new(q, url);

  if (q->cb.status != NULL)
    {
      const glong p = q_makelong(QUVI_CALLBACK_STATUS_FETCH, 0);

      if (q->cb.status(p, (gpointer) url, q->cb.userdata.status) != QUVI_OK)
        {
          q->status.rc = QUVI_ERROR_CALLBACK_ABORTED;
          return;
        }
    }

  if (q->cb.fetch != NULL)
    q->status.rc = q->cb.fetch(*n);
  else /* Fetch using cURL (default). */
    q->status.rc = c_fetch(*n);

  if (quvi_ok(q) == QUVI_TRUE && (*n)->status.resp_code == 200)
    {
      /* To UTF8. */

      if (opts != NULL)
        {
          GSList *from;

          if (l_quvi_object_opts_is_set(q->handle.lua, opts,
                                        QUVI_OBJECT_OPTION_FETCH_FROM_CHARSET,
                                        &from, NULL, FALSE)
              == TRUE)
            {
              l_quvi_object_opt_t o;
              gchar *c;

              o = (l_quvi_object_opt_t) from->data;
              c = to_utf8((*n)->fetch.content->str, o->value.str);

              if (c != NULL)
                {
                  g_string_assign((*n)->fetch.content, c);
                  g_free(c);
                }
            }
        }

      /* Update status. */
      if (q->cb.status != NULL)
        {
          const glong p = q_makelong(QUVI_CALLBACK_STATUS_FETCH,
                                     QUVI_CALLBACK_STATUS_DONE);

          if (q->cb.status(p, 0, q->cb.userdata.status) != QUVI_OK)
            q->status.rc = QUVI_ERROR_CALLBACK_ABORTED;
        }
    }
  else
    {
      /* Save returned error message. */
      if ((*n)->status.errmsg->len >0)
        g_string_assign(q->status.errmsg, (*n)->status.errmsg->str);
      else
        {
          g_string_assign(q->status.errmsg,
                          "unknown error: fetch: callback returned "
                          "empty errmsg");
        }
    }

  q->status.resp_code = (*n)->status.resp_code;
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
