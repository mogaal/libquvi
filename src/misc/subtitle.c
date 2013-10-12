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
#include "misc/unescape.h"
#include "misc/subtitle.h"
#include "misc/slst.h"

gpointer m_subtitle_new(_quvi_t q, const gchar *url)
{
  _quvi_subtitle_t qsub = g_new0(struct _quvi_subtitle_s, 1);
  /* URL */
  qsub->url.input = g_string_new(url);
  m_unescape_url(qsub->url.input);
  /* Handle */
  qsub->handle.quvi = q;
  return (qsub);
}

void m_subtitle_lang_free(_quvi_subtitle_lang_t qsl)
{
  if (qsl == NULL)
    return;

  g_string_free(qsl->translated, TRUE);
  qsl->translated = NULL;

  g_string_free(qsl->original, TRUE);
  qsl->original = NULL;

  g_string_free(qsl->code, TRUE);
  qsl->code = NULL;

  g_string_free(qsl->url, TRUE);
  qsl->url = NULL;

  g_string_free(qsl->id, TRUE);
  qsl->id = NULL;

  g_free(qsl);
  qsl = NULL;
}

static void _lang_free(gpointer p, gpointer userdata)
{
  m_subtitle_lang_free(p);
}

void m_subtitle_type_free(_quvi_subtitle_type_t qst)
{
  if (qst == NULL)
    return;

  m_slist_free_full(qst->languages, _lang_free);
  qst->languages = NULL;

  g_free(qst);
  qst = NULL;
}

static void _type_free(gpointer p, gpointer userdata)
{
  m_subtitle_type_free(p);
}

void m_subtitle_free(_quvi_subtitle_t qsub)
{
  if (qsub == NULL)
    return;

  m_slist_free_full(qsub->types, _type_free);
  qsub->types = NULL;

  g_string_free(qsub->url.input, TRUE);
  qsub->url.input = NULL;

  g_free(qsub);
  qsub = NULL;
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
