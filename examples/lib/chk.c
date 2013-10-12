/* libquvi
 * Copyright (C) 2012  Toni Gundogdu <legatvs@gmail.com>
 *
 * This file is part of libquvi <http://quvi.sourceforge.net/>.
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General
 * Public License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <glib.h>
#include <quvi.h>

#include "examples.h"

gboolean examples_chk_val_s(gchar *opt_val, gchar **ok_val, gchar **v)
{
  gint i,r;

  if (opt_val == NULL)
    return (TRUE);

  for (i=0, r=FALSE; ok_val[i] != NULL; ++i)
    {
      if (g_strcmp0(ok_val[i], opt_val) == 0)
        {
          r = TRUE;
          break;
        }
    }

  if (r == FALSE)
    *v = opt_val;

  return (r);
}

gboolean examples_chk_val_sv(gchar **opt_val, gchar **ok_val, gchar **v)
{
  gint i,r;

  if (opt_val == NULL)
    return (TRUE);

  for (i=0, r=TRUE; opt_val[i] != NULL && r == TRUE; ++i)
    r = examples_chk_val_s(opt_val[i], ok_val, v);

  return (r);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
