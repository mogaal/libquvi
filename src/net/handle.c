/* libquvi
 * Copyright (C) 2012  Toni Gundogdu <legatvs@gmail.com>
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

#include <lua.h>
#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_net_s.h"
#include "_quvi_net_resolve_s.h"
/* -- */
#include "net/handle.h"

gpointer n_new(_quvi_t q, const gchar *url)
{
  _quvi_net_t n = g_new0(struct _quvi_net_s, 1);
  n->http_metainfo.content_type = g_string_new(NULL);
  n->status.errmsg = g_string_new(NULL);
  n->fetch.content = g_string_new(NULL);
  n->url.addr = g_string_new(url);
  n->handle.quvi = q;
  return (n);
}

void n_free(gpointer p)
{
  _quvi_net_t n = (_quvi_net_t) p;

  if (n == NULL)
    return;

  g_string_free(n->url.addr, TRUE);
  n->url.addr = NULL;

  g_string_free(n->status.errmsg, TRUE);
  n->status.errmsg = NULL;

  g_string_free(n->fetch.content, TRUE);
  n->fetch.content = NULL;

  g_string_free(n->http_metainfo.content_type, TRUE);
  n->http_metainfo.content_type = NULL;

  g_free(n);
  n = NULL;
}

gpointer n_resolve_new(_quvi_t q, const gchar *url)
{
  _quvi_net_resolve_t r = g_new0(struct _quvi_net_resolve_s, 1);
  r->status.errmsg = g_string_new(NULL);
  r->url.addr = g_string_new(url);
  r->url.dst  = g_string_new(NULL);
  r->handle.quvi = q;
  return (r);
}

void n_resolve_free(gpointer p)
{
  _quvi_net_resolve_t r = (_quvi_net_resolve_t) p;

  g_string_free(r->status.errmsg, TRUE);
  r->status.errmsg = NULL;

  g_string_free(r->url.addr, TRUE);
  r->url.addr = NULL;

  g_string_free(r->url.dst, TRUE);
  r->url.dst = NULL;

  g_free(r);
  r = NULL;
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
