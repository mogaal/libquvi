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

/** @file media_get.c */

#include "config.h"

#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_media_s.h"

/* Advances the current stream pointer to the first stream if undefined. */
static void _chk_curr_stream(_quvi_media_t qm, _quvi_media_stream_t *qms)
{
  if (qm->curr.stream == NULL)
    {
      const QuviBoolean r = quvi_media_stream_next(qm);
      g_assert(r == QUVI_TRUE); /* If the stream list is still empty,
                                 * quvi_media_get is being used incorrectly.*/
    }
  g_assert(qm->curr.stream != NULL);
  *qms = (_quvi_media_stream_t) qm->curr.stream->data;
  g_assert(*qms != NULL);
}

static QuviError _media_get(_quvi_media_t qm, const QuviMediaProperty n, ...)
{
  _quvi_media_stream_t qms;
  QuviError rc;
  gdouble *dp;
  va_list arg;
  gchar **sp;
  glong *lp;
  gint type;

  va_start(arg, n);
  type = QUVI_MEDIA_PROPERTY_TYPE_MASK & (gint) n;

  dp = NULL;
  sp = NULL;
  lp = NULL;

  rc = QUVI_OK;
  qms = NULL;

  switch (type)
    {
    case QUVI_MEDIA_PROPERTY_TYPE_STRING:
      sp = va_arg(arg, gchar**);
      if (sp == NULL)
        rc = QUVI_ERROR_INVALID_ARG;
      break;
    case QUVI_MEDIA_PROPERTY_TYPE_LONG:
      lp = va_arg(arg, glong*);
      if (lp == NULL)
        rc = QUVI_ERROR_INVALID_ARG;
      break;
    case QUVI_MEDIA_PROPERTY_TYPE_DOUBLE:
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
    case QUVI_MEDIA_PROPERTY_TITLE:
      *sp = qm->title->str;
      break;
    case QUVI_MEDIA_PROPERTY_ID:
      *sp = qm->id->str;
      break;
    case QUVI_MEDIA_PROPERTY_START_TIME_MS:
      *dp = qm->start_time_ms;
      break;
    case QUVI_MEDIA_PROPERTY_THUMBNAIL_URL:
      *sp = qm->url.thumbnail->str;
      break;
    case QUVI_MEDIA_PROPERTY_DURATION_MS:
      *dp = qm->duration_ms;
      break;

      /*
       * Stream properties.
       *
       * NOTE: These must advance current stream pointer if it is still NULL.
       */

    case QUVI_MEDIA_STREAM_PROPERTY_VIDEO_ENCODING:
      _chk_curr_stream(qm, &qms);
      *sp = qms->video.encoding->str;
      break;
    case QUVI_MEDIA_STREAM_PROPERTY_AUDIO_ENCODING:
      _chk_curr_stream(qm, &qms);
      *sp = qms->audio.encoding->str;
      break;
    case QUVI_MEDIA_STREAM_PROPERTY_CONTAINER:
      _chk_curr_stream(qm, &qms);
      *sp = qms->container->str;
      break;
    case QUVI_MEDIA_STREAM_PROPERTY_URL:
      _chk_curr_stream(qm, &qms);
      *sp = qms->url->str;
      break;
    case QUVI_MEDIA_STREAM_PROPERTY_ID:
      _chk_curr_stream(qm, &qms);
      *sp = qms->id->str;
      break;
    case QUVI_MEDIA_STREAM_PROPERTY_VIDEO_BITRATE_KBIT_S:
      _chk_curr_stream(qm, &qms);
      *dp = qms->video.bitrate_kbit_s;
      break;
    case QUVI_MEDIA_STREAM_PROPERTY_AUDIO_BITRATE_KBIT_S:
      _chk_curr_stream(qm, &qms);
      *dp = qms->audio.bitrate_kbit_s;
      break;
    case QUVI_MEDIA_STREAM_PROPERTY_VIDEO_HEIGHT:
      _chk_curr_stream(qm, &qms);
      *dp = qms->video.height;
      break;
    case QUVI_MEDIA_STREAM_PROPERTY_VIDEO_WIDTH:
      _chk_curr_stream(qm, &qms);
      *dp = qms->video.width;
      break;

    default:
      rc = QUVI_ERROR_INVALID_ARG;
      break;
    }
  return (rc);
}

/** @brief Return a media property
@sa @ref parse_media
@ingroup mediaprop
@note Accessing any of the QUVI_MEDIA_STREAM_PROPERTY_* values using
this function will cause the library to advance to the first stream
in the list, this will conflict with @ref quvi_media_stream_next,
causing @ref quvi_media_stream_next to advance from the second stream,
not the first stream
*/
void quvi_media_get(quvi_media_t handle, QuviMediaProperty property, ...)
{
  _quvi_media_t qm;
  va_list arg;
  gpointer p;
  _quvi_t q;

  /* If G_DISABLE_CHECKS is defined then the check is not performed. */
  g_return_if_fail(handle != NULL);

  va_start(arg, property);
  p = va_arg(arg, gpointer);
  va_end(arg);

  qm = (_quvi_media_t) handle;
  q = qm->handle.quvi;

  q->status.rc = _media_get(qm, property, p);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
