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

#include "config.h"

#include <string.h>
#include <lauxlib.h>
#include <glib.h>
#include <curl/curl.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
/* -- */
#include "lua/quvi/opts.h"
#include "lua/getfield.h"
#include "lua/setfield.h"
#include "lua/def.h"

extern glong c_reset_headers(_quvi_t);

gint l_quvi_http_header(lua_State *l)
{
  gboolean croak_if_error;
  const gchar *s;
  GSList *opts;
  CURLcode cc;
  _quvi_t q;

  /* quvi handle */

  q = (_quvi_t) l_get_reg_userdata(l, USERDATA_QUVI_T);
  g_assert(q != NULL);

  /* arg1 */

  s = luaL_checkstring(l, 1);
  lua_pop(l, 1);

  /* options */

  opts = l_quvi_object_opts_new(l, 2);
  croak_if_error = l_quvi_object_opts_croak_if_error(l, opts);
  l_quvi_object_opts_free(opts);

  /* apply */

  if (strlen(s) >0)
    {
      CURL *c = q->handle.curl;
      q->http.headers = curl_slist_append(q->http.headers, s);
      cc = curl_easy_setopt(c, CURLOPT_HTTPHEADER, q->http.headers);
    }
  else
    cc = c_reset_headers(q);

  if (cc != CURLE_OK)
    {
      g_string_printf(q->status.errmsg, "%s", curl_easy_strerror(cc));
      q->status.rc = QUVI_ERROR_CALLBACK;

      if (croak_if_error == TRUE)
        luaL_error(l, "%s", q->status.errmsg->str);
    }

  /* Return a table of results. */

  lua_newtable(l);
  l_setfield_s(l, QO_ERROR_MESSAGE, q->status.errmsg->str, -1);
  l_setfield_n(l, QO_QUVI_CODE, q->status.rc);

  return (1); /* no. of returned values (a table) */
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
