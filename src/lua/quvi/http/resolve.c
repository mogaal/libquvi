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

#include "config.h"

#include <lauxlib.h>
#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_net_s.h"
#include "_quvi_net_resolve_s.h"
/* -- */
#include "lua/def.h"
#include "lua/getfield.h"
#include "lua/setfield.h"
#include "lua/quvi/opts.h"
#include "net/resolve.h"
#include "net/handle.h"

gint l_quvi_http_resolve(lua_State *l)
{
  gboolean croak_if_error;
  _quvi_net_resolve_t r;
  const gchar *url, *r_url;
  GSList *opts;
  _quvi_t q;

  q = (_quvi_t) l_get_reg_userdata(l, USERDATA_QUVI_T);

  url = luaL_checkstring(l, 1);
  lua_pop(l, 1);

  opts = l_quvi_object_opts_new(l, 2);
  croak_if_error = l_quvi_object_opts_croak_if_error(l, opts);
#ifdef _UNUSED
  l_quvi_object_opts_curl(opts, q);
#endif
  r = n_resolve_new(q, url);
  q->status.rc = n_resolve(q, r);

  lua_newtable(l);
  l_setfield_n(l, QO_RESPONSE_CODE, q->status.resp_code);
  l_setfield_n(l, QO_QUVI_CODE, q->status.rc);
  l_setfield_s(l, QO_ERROR_MESSAGE, (q->status.rc != QUVI_OK)
               ? q->status.errmsg->str
               : GS_EMPTY_S, -1);

  r_url = GS_EMPTY_S;

  if (quvi_ok(q) == QUVI_TRUE)
    {
      r_url = (r->url.dst->len >0)
              ? r->url.dst->str
              : GS_EMPTY_S;
    }
  else
    {
      if (croak_if_error == TRUE)
        luaL_error(l, "%s", q->status.errmsg->str);
    }
  l_setfield_s(l, QO_RESOLVED_URL, r_url, -1);

  l_quvi_object_opts_free(opts);
  n_resolve_free(r);

  return (1); /* no. of returned values (one table) */
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
