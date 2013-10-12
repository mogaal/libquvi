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

extern gchar *l_exec_util_resolve_redirections(_quvi_t, const gchar*);

/*
 * The caller should always check the q->status.rc value after calling
 * these functions.
 */

void m_resolve_url(_quvi_t q, const gchar *url, GString *dst)
{
  gchar *r;
  g_assert(dst != NULL);
  r = l_exec_util_resolve_redirections(q, url);
  if (r != NULL)
    {
      g_string_assign(dst, r);
      g_free(r);
    }
}

/* Identical to the above but reuses the GString. */
void m_resolve(_quvi_t q, GString *dst)
{
  gchar *u;
  g_assert(dst != NULL);
  u = g_strdup(dst->str);
  m_resolve_url(q, u, dst);
  g_free(u);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
