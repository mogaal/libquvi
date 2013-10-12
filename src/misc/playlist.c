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

#include "config.h"

#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_playlist_s.h"
/* -- */
#include "misc/unescape.h"
#include "misc/playlist.h"
#include "misc/slst.h"

gpointer m_playlist_new(_quvi_t q, const gchar *url)
{
  _quvi_playlist_t qp = g_new0(struct _quvi_playlist_s, 1);
  /* URL */
  qp->url.thumbnail = g_string_new(NULL);
  qp->url.input = g_string_new(url);
  m_unescape_url(qp->url.input);
  /* ID */
  qp->id.playlist = g_string_new(NULL);
  /* Handle */
  qp->handle.quvi = q;
  /* Other */
  qp->title = g_string_new(NULL);
  return (qp);
}

void m_playlist_media_free(_quvi_playlist_media_t qpm)
{
  if (qpm == NULL)
    return;

  g_string_free(qpm->title, TRUE);
  qpm->title = NULL;

  g_string_free(qpm->url, TRUE);
  qpm->url = NULL;

  g_free(qpm);
  qpm = NULL;
}

static void _playlist_media_free(gpointer p, gpointer userdata)
{
  m_playlist_media_free(p);
}

void m_playlist_free(_quvi_playlist_t qp)
{
  if (qp == NULL)
    return;

  m_slist_free_full(qp->media, _playlist_media_free);
  qp->media = NULL;

  /* URL */

  g_string_free(qp->url.thumbnail, TRUE);
  qp->url.thumbnail = NULL;

  g_string_free(qp->url.input, TRUE);
  qp->url.input = NULL;

  /* ID */

  g_string_free(qp->id.playlist, TRUE);
  qp->id.playlist = NULL;

  /* Other */

  g_string_free(qp->title, TRUE);
  qp->title = NULL;

  g_free(qp);
  qp = NULL;
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
