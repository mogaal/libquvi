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

#include <glib/gi18n-lib.h>
#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_subtitle_export_s.h"
#include "_quvi_subtitle_s.h"
#include "_quvi_script_s.h"
/* -- */
#include "misc/match_subtitle_export_script.h"
#include "misc/subtitle_export.h"
#include "lua/exec.h"

extern const gchar *show_script; /* m_scan_scripts */

QuviError
_match_format_to_subtitle_export_script(_quvi_subtitle_export_t qse,
                                        GSList **curr)
{
  *curr = qse->handle.quvi->scripts.subtitle_export;
  while (*curr != NULL)
    {
      const QuviError rc = l_exec_subtitle_export_script_ident(qse, *curr);

      if (rc == QUVI_OK)
        return (rc);

      *curr = g_slist_next(*curr);
    }
  return (QUVI_ERROR_NO_SUPPORT);
}

QuviError
m_match_subtitle_export_script(_quvi_t q, _quvi_subtitle_export_t *dst,
                               _quvi_subtitle_lang_t qsl,
                               const gchar *to_format)
{
  QuviError rc;
  GSList *s;

  *dst = m_subtitle_export_new(q, qsl->url->str);
  g_string_assign((*dst)->format.to, to_format);
  (*dst)->format.from = qsl->format;

  rc = _match_format_to_subtitle_export_script(*dst, &s);
  if (rc == QUVI_ERROR_NO_SUPPORT)
    {
      g_string_printf(q->status.errmsg,
                      _("No support: Could not find a subtitle export "
                        "script for format `%s'"), to_format);
      return (rc);
    }
  else if (rc != QUVI_OK)
    return (rc);

  if (show_script != NULL && strlen(show_script) >0)
    {
      const _quvi_script_t qs = (const _quvi_script_t) s->data;

      g_message("[%s] libquvi: %s: input URL accepted",
                __func__, qs->fpath->str);
    }
  return (l_exec_subtitle_export_script_export(*dst, s));
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
