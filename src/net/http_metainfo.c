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

#include <glib/gi18n-lib.h>
#include <glib.h>
#include <lauxlib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_http_metainfo_s.h"
#include "_quvi_net_s.h"
#include "_quvi_macro.h"
/* -- */
#include "net/handle.h"

extern QuviError l_exec_util_to_file_ext(_quvi_t, const gchar*, GString*);
extern QuviError c_http_metainfo(_quvi_t, _quvi_net_t);

static const gdouble MIN_LIKELY_MEDIA_LENGTH = 50*1024;

static QuviError _http_metainfo(_quvi_http_metainfo_t qmi)
{
  _quvi_net_t n;
  QuviError rc;
  _quvi_t q;

  q = qmi->handle.quvi;
  n = n_new(q, qmi->url.input->str);

  if (q->cb.http_metainfo != NULL)
    rc = q->cb.http_metainfo(n);
  else
    rc = c_http_metainfo(q, n); /* Query using cURL (default). */

  if (rc == QUVI_OK)
    {
      rc = l_exec_util_to_file_ext(q, n->http_metainfo.content_type->str,
                                   qmi->file_ext);
      if (rc == QUVI_OK)
        {
          g_string_assign(qmi->content_type, n->http_metainfo.content_type->str);
          qmi->length_bytes = n->http_metainfo.content_length;
        }

      if (q->cb.status != NULL)
        {
          const glong p = q_makelong(QUVI_CALLBACK_STATUS_HTTP_QUERY_METAINFO,
                                     QUVI_CALLBACK_STATUS_DONE);

          if (q->cb.status(p, 0, q->cb.userdata.status) != QUVI_OK)
            rc = QUVI_ERROR_CALLBACK_ABORTED;
        }
    }
  else
    {
      /* Save returned error message. */
      if (n->status.errmsg->len >0)
        g_string_assign(q->status.errmsg, n->status.errmsg->str);
      else
        {
          g_string_assign(q->status.errmsg,
                          "unknown error: http_metainfo: callback returned "
                          "an empty errmsg");
        }
    }

  q->status.resp_code = n->status.resp_code;
  n_free(n);

  return (rc);
}

QuviError n_http_metainfo(_quvi_http_metainfo_t qmi)
{
  _quvi_t q;
  gchar *s;

  q = qmi->handle.quvi;
  s = g_uri_parse_scheme(qmi->url.input->str);

  if (s != NULL) /* HTTP(S) only. */
    {
      const gboolean r = (gboolean) g_strcmp0(s, "http") == 0
                         || g_strcmp0(s, "https") == 0;

      g_free(s);
      s = NULL;

      if (r == FALSE)
        return (QUVI_OK); /* Skip process. */
    }
  else
    {
      g_string_printf(q->status.errmsg,
                      _("Failed to parse URL: %s"), qmi->url.input->str);
      return (QUVI_ERROR_INVALID_ARG); /* Skip process. */
    }

  if (q->cb.status != NULL)
    {
      const glong p = q_makelong(QUVI_CALLBACK_STATUS_HTTP_QUERY_METAINFO, 0);

      if (q->cb.status(p, 0, q->cb.userdata.status) != QUVI_OK)
        return (QUVI_ERROR_CALLBACK_ABORTED);
    }

  return (_http_metainfo(qmi));
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
