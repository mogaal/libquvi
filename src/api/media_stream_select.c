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

/** @file media_stream_select.c */

#include "config.h"

#include <glib/gi18n-lib.h>
#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_media_s.h"
/* -- */
#include "misc/re.h"

static QuviError _select(_quvi_media_t qm, const gchar *id)
{
  _quvi_media_stream_t qms;
  gboolean found_flag;
  QuviError rc;
  gchar **r;
  gint i;

  quvi_media_stream_reset(qm);

  r = g_strsplit(id, ",", 0);
  found_flag = FALSE;
  rc = QUVI_OK;

  for (i=0; (r[i] != NULL && found_flag == FALSE); ++i)
    {
      if (g_strcmp0(r[i], "croak") ==0)
        {
          rc = QUVI_ERROR_KEYWORD_CROAK;
          break;
        }
      else if (g_strcmp0(r[i], "best") == 0)
        {
          quvi_media_stream_choose_best(qm);
          break;
        }
      else
        {
          while (quvi_media_stream_next(qm) == QUVI_TRUE)
            {
              /* TODO: Use quvi_media_get? */
              qms = (_quvi_media_stream_t) qm->curr.stream->data;

              found_flag = m_match(qms->id->str, r[i]);
              if (found_flag == TRUE)
                break;
            }

          if (found_flag == FALSE) /* Use the first stream as a fallback. */
            quvi_media_stream_reset(qm);
        }
    }
  g_strfreev(r);
  return (rc);
}

/** @brief Select a @ref m_stream matching a @ref m_stream_id

Matches the @ref m_stream_id (pattern) to the available media stream
IDs and selects the stream. This function returns immediately
if a matching ID was found.  The ID value may be a comma-separated value
(e.g. "foo,bar,baz"). The ID may also contain the keywords 'croak' and
'best' (see the notes below).
@note
  - ID value is used as regular expression pattern
  - ID may contain the reserved keyword 'best'
    - Defining this in the ID is identical to calling
      @ref quvi_media_stream_choose_best, refer to it for details
  - ID may contain the reserved keyword 'croak'
    - This will cause the function to exit immediately when it is reached
    - The result may be checked with @ref quvi_ok
      - The code may be retrieved using @ref quvi_get
      - The error message may be retrieved using @ref quvi_errmsg
  - If nothing matched (and the 'croak' keyword was specified) the
    function will return the first (default) available language
  - Always confirm the result with @ref quvi_ok
@sa @ref parse_media
@ingroup mediaprop
*/
void quvi_media_stream_select(quvi_media_t handle, const char *id)
{
  _quvi_media_t qm;
  _quvi_t q;

  /* If G_DISABLE_CHECKS is defined then the check is not performed. */
  g_return_if_fail(handle != NULL);

  qm = (_quvi_media_t) handle;
  q = qm->handle.quvi;

  q->status.rc = _select(qm, id);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
