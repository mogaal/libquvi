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

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_subtitle_s.h"
/* -- */
#include "lua/exec.h"

QuviError l_match_url_to_subtitle_script(_quvi_subtitle_t qsub, GSList **curr)
{
  *curr = qsub->handle.quvi->scripts.subtitle;
  while (*curr != NULL)
    {
      const QuviError rc = l_exec_subtitle_script_ident(qsub, *curr);

      if (rc == QUVI_OK)
        return (rc);

      *curr = g_slist_next(*curr);
    }
  return (QUVI_ERROR_NO_SUPPORT);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
