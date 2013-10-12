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

#include <locale.h>
#include <stdlib.h>
#include <glib.h>
#include <quvi.h>

#include "examples.h"

static void usage()
{
  g_printerr("Usage: playlist <URL>\n");
  exit(0);
}

typedef quvi_callback_status qcs;

gint main(gint argc, gchar **argv)
{
  gchar *url = NULL;
  gint i = 1;

  g_assert(qp == NULL);
  g_assert(q == NULL);

  setlocale(LC_ALL, "");

  if (argc < 2)
    usage();

  q = quvi_new();
  examples_exit_if_error();

  for (; i<argc; ++i)
    {
      if (g_strcmp0("-v", argv[i]) == 0)
        examples_enable_verbose();
      else if (g_strcmp0("-a", argv[i]) == 0)
        examples_enable_autoproxy();
      else
        url = argv[i];
    }

  if (url == NULL)
    usage();

  quvi_set(q, QUVI_OPTION_CALLBACK_STATUS, (qcs) examples_status);

  qp = quvi_playlist_new(q, url);
  examples_exit_if_error();
  {
    gchar *p_title, *p_thumb_url, *p_id;
    gchar *m_url, *m_title;
    gdouble m_dur_ms;

    quvi_playlist_get(qp, QUVI_PLAYLIST_PROPERTY_THUMBNAIL_URL, &p_thumb_url);
    g_print("thumb=%s\n", p_thumb_url);

    quvi_playlist_get(qp, QUVI_PLAYLIST_PROPERTY_TITLE, &p_title);
    g_print("title=%s\n", p_title);

    quvi_playlist_get(qp, QUVI_PLAYLIST_PROPERTY_ID, &p_id);
    g_print("id=%s\n", p_id);

    while (quvi_playlist_media_next(qp) == QUVI_TRUE)
      {
        quvi_playlist_get(qp,
                          QUVI_PLAYLIST_MEDIA_PROPERTY_DURATION_MS, &m_dur_ms);

        quvi_playlist_get(qp, QUVI_PLAYLIST_MEDIA_PROPERTY_TITLE, &m_title);
        quvi_playlist_get(qp, QUVI_PLAYLIST_MEDIA_PROPERTY_URL, &m_url);

        g_print("title=%s, duration=%.0f, url=%s\n", m_title, m_dur_ms, m_url);
      }
  }

  examples_cleanup();

  g_assert(qp == NULL);
  g_assert(q == NULL);

  return (0);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
