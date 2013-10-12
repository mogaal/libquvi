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

#include <glib.h>

#include "misc/slst.h"

void m_slist_free_full(GSList *p, GFunc f)
{
  if (p == NULL)
    return;

#ifdef HAVE_GLIB_2_28
  g_slist_free_full(p, f);
#else
  g_slist_foreach(p, f, NULL);
  g_slist_free(p);
#endif
  p = NULL;
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
