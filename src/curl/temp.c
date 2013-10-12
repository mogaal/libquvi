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

#include <string.h>
#include <glib.h>
/* -- */
#include "temp.h"

_c_temp_t c_temp_new()
{
  return g_new0(struct _c_temp_s, 1);
}

void c_temp_free(_c_temp_t p)
{
  if (p == NULL)
    return;

  g_free(p->p);
  p->p =  NULL;

  g_free(p);
  p = NULL;
}

/* cURL write callback. */
gsize c_temp_wrcb(gpointer p, gsize sz, gsize nmemb, gpointer d)
{
  const gsize rsize = sz*nmemb;
  gpointer *np;
  _c_temp_t ct;

  ct = (_c_temp_t) d;
  np = g_realloc(ct->p, ct->size+rsize+1);

  if (np != NULL)
    {
      ct->p = (gchar*) np;
      memcpy(&(ct->p[ct->size]), p, rsize);
      ct->size += rsize;
      ct->p[ct->size] = '\0';
    }
  return (rsize);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
