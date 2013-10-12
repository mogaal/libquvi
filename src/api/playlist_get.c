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

/** @file playlist_get.c */

#include "config.h"

#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_playlist_s.h"

/* Advances the current media pointer to the first media if undefined. */
static void _chk_curr_media(_quvi_playlist_t qp, _quvi_playlist_media_t *qpm)
{
  if (qp->curr.media == NULL)
    quvi_playlist_media_next(qp);

  if (qp->curr.media != NULL)
    *qpm = (_quvi_playlist_media_t) qp->curr.media->data;
}

static const gchar empty[] = "";

static QuviError _playlist_get(_quvi_playlist_t qp,
                               QuviPlaylistProperty n, ...)
{
  _quvi_playlist_media_t qpm;
  QuviError rc;
  gdouble *dp;
  va_list arg;
  gchar **sp;
  glong *lp;
  gint type;

  va_start(arg, n);
  type = QUVI_PLAYLIST_PROPERTY_TYPE_MASK & (gint) n;

  dp = NULL;
  sp = NULL;
  lp = NULL;

  rc = QUVI_OK;
  qpm = NULL;

  switch (type)
    {
    case QUVI_PLAYLIST_PROPERTY_TYPE_STRING:
      sp = va_arg(arg, gchar**);
      if (sp == NULL)
        rc = QUVI_ERROR_INVALID_ARG;
      break;
    case QUVI_PLAYLIST_PROPERTY_TYPE_LONG:
      lp = va_arg(arg, glong*);
      if (lp == NULL)
        rc = QUVI_ERROR_INVALID_ARG;
      break;
    case QUVI_PLAYLIST_PROPERTY_TYPE_DOUBLE:
      dp = va_arg(arg, gdouble*);
      if (dp == NULL)
        rc = QUVI_ERROR_INVALID_ARG;
      break;
    default:
      rc = QUVI_ERROR_INVALID_ARG;
      break;
    }
  va_end(arg);

  if (rc != QUVI_OK)
    return (rc);

  switch (n)
    {
    case QUVI_PLAYLIST_PROPERTY_THUMBNAIL_URL:
      *sp = qp->url.thumbnail->str;
      break;

    case QUVI_PLAYLIST_PROPERTY_TITLE:
      *sp = qp->title->str;
      break;

    case QUVI_PLAYLIST_PROPERTY_ID:
      *sp = qp->id.playlist->str;
      break;

      /*
       * (Playlist) Media properties.
       *
       * quvi_playlist_get is expected to return the property values for
       * the first media the playlist script returned, just like
       * quvi_media_get does with media scripts. We must mimic this
       * behaviour by first attempting to move to the first item in the
       * playlist media list.
       *
       * Unlike with quvi_media_get, playlist scripts are allowed to return
       * nothing. Media scripts are expected to return at least one media
       * stream.
       *
       * This means that if the playlist script did not return any such
       * properties (e.g. media URL):
       *  - returned strings must be set to empty ("")
       *  - returned numeric values must be set to 0
       */

    case QUVI_PLAYLIST_MEDIA_PROPERTY_TITLE:
      _chk_curr_media(qp, &qpm);
      *sp = (qpm != NULL) ? qpm->title->str : (gchar*) empty;
      break;

    case QUVI_PLAYLIST_MEDIA_PROPERTY_URL:
      _chk_curr_media(qp, &qpm);
      *sp = (qpm != NULL) ? qpm->url->str : (gchar*) empty;
      break;

    case QUVI_PLAYLIST_MEDIA_PROPERTY_DURATION_MS:
      _chk_curr_media(qp, &qpm);
      *dp = (qpm != NULL) ? qpm->duration_ms : 0;
      break;

    default:
      rc = QUVI_ERROR_INVALID_ARG;
      break;
    }
  return (rc);
}

/** @brief Return a playlist property
@sa @ref parse_playlist
@ingroup playlistprop
*/
void quvi_playlist_get(quvi_playlist_t handle,
                       QuviPlaylistProperty property, ...)
{
  _quvi_playlist_t qp;
  va_list arg;
  gpointer p;
  _quvi_t q;

  /* If G_DISABLE_CHECKS is defined then the check is not performed. */
  g_return_if_fail(handle != NULL);

  va_start(arg, property);
  p = va_arg(arg, gpointer);
  va_end(arg);

  qp = (_quvi_playlist_t) handle;
  q = qp->handle.quvi;

  q->status.rc = _playlist_get(qp, property, p);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
