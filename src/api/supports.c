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

/** @file supports.c */

#include "config.h"

#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_media_s.h"
#include "_quvi_playlist_s.h"
#include "_quvi_subtitle_s.h"
/* -- */
#include "misc/match_playlist_script.h"
#include "misc/match_subtitle_script.h"
#include "misc/match_media_script.h"

static QuviBoolean _supports_playlist(_quvi_t q, const gchar *url,
                                      const QuviSupportsMode mode)
{
  _quvi_playlist_t qp = NULL;

  q->status.rc = m_match_playlist_script(q, &qp, url,
                                         (mode == QUVI_SUPPORTS_MODE_OFFLINE)
                                         ? QM_MATCH_PS_SUPPORTED_OFFLINE
                                         : QM_MATCH_PS_SUPPORTED_ONLINE);
  if (qp != NULL)
    {
      quvi_playlist_free((quvi_playlist_t) qp);
      qp = NULL;
    }
  return (quvi_ok(q));
}

static QuviBoolean _supports_subtitle(_quvi_t q, const gchar *url,
                                      const QuviSupportsMode mode)
{
  _quvi_subtitle_t qsub = NULL;

  q->status.rc = m_match_subtitle_script(q, &qsub, url,
                                         (mode == QUVI_SUPPORTS_MODE_OFFLINE)
                                         ? QM_MATCH_SUBS_SUPPORTED_OFFLINE
                                         : QM_MATCH_SUBS_SUPPORTED_ONLINE);

  if (qsub != NULL)
    {
      quvi_subtitle_free((quvi_subtitle_t) qsub);
      qsub = NULL;
    }
  return (quvi_ok(q));
}

static QuviBoolean _supports_media(_quvi_t q, const gchar *url,
                                   const QuviSupportsMode mode)
{
  _quvi_media_t qm = NULL;

  q->status.rc = m_match_media_script(q, &qm, url,
                                      (mode == QUVI_SUPPORTS_MODE_OFFLINE)
                                      ? QM_MATCH_MS_SUPPORTED_OFFLINE
                                      : QM_MATCH_MS_SUPPORTED_ONLINE);
  if (qm != NULL)
    {
      quvi_media_free((quvi_media_t) qm);
      qm = NULL;
    }
  return (quvi_ok(q));
}

/** @brief Check whether the URL is supported

Check if any of the scripts accepts the URL. Both offline and online
checking is supported.

@note
 - Offline check will fail with most @ref sh_url
 - Online check will resolve @ref sh_url

Type may be a combination.

@code
quvi_supports(q, URL, QUVI_SUPPORTS_OFFLINE,
              QUVI_SUPPORTS_TYPE_PLAYLIST | QUVI_SUPPORTS_TYPE_MEDIA);
@endcode

@ingroup convenience
*/
QuviBoolean quvi_supports(quvi_t handle, const char *url,
                          QuviSupportsMode mode, QuviSupportsType type)
{
  QuviBoolean found;
  _quvi_t q;

  /* If G_DISABLE_CHECKS is defined then the check is not performed. */
  g_return_val_if_fail(handle != NULL, QUVI_FALSE);
  g_return_val_if_fail(url != NULL, QUVI_FALSE);

  q = (_quvi_t) handle;
  q->status.rc = QUVI_OK;
  found = QUVI_FALSE;

  if (type & QUVI_SUPPORTS_TYPE_PLAYLIST)
    found = _supports_playlist(q, url, mode);

  if (q->status.rc == QUVI_OK || q->status.rc == QUVI_ERROR_NO_SUPPORT)
    {
      if (type & QUVI_SUPPORTS_TYPE_SUBTITLE)
        found = _supports_subtitle(q, url, mode);
    }

  if (q->status.rc == QUVI_OK || q->status.rc == QUVI_ERROR_NO_SUPPORT)
    {
      if (type & QUVI_SUPPORTS_TYPE_MEDIA)
        found = _supports_media(q, url, mode);
    }
  return (found);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
