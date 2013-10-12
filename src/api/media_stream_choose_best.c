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

/** @file media_stream_choose_best.c */

#include "config.h"

#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_media_s.h"

/** @brief Selects the @ref m_stream that was determined to be of the "best quality"
@note
  - Only useful when there are >1 media streams available, otherwise the
    function falls back to the first available @ref m_stream (the
    default)
  - The @ref m_script determines the best quality @ref m_stream
@sa @ref parse_media
@ingroup mediaprop
*/
void quvi_media_stream_choose_best(quvi_media_t handle)
{
  _quvi_media_stream_t qms;
  _quvi_media_t qm;

  qm = (_quvi_media_t) handle;

  /* If G_DISABLE_CHECKS is defined then the check is not performed. */
  g_return_if_fail(handle != NULL);

  quvi_media_stream_reset(qm);

  while (quvi_media_stream_next(qm) == QUVI_TRUE)
    {
      qms = (_quvi_media_stream_t) qm->curr.stream->data;
      g_assert(qms != NULL);

      if (qms->flags.best == TRUE)
        break;
    }

  /*
   * If nothing was flagged as such, let it fall through.
   * Use whatever is the first.
   */
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
