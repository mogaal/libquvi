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

/** @file subtitle_lang_get.c */

#include "config.h"

#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_subtitle_s.h"

static QuviError _get(_quvi_subtitle_lang_t qsl,
                      const QuviSubtitleLangProperty n, ...)
{
  QuviError rc;
  gdouble *dp;
  va_list arg;
  gchar **sp;
  glong *lp;
  gint type;

  va_start(arg, n);
  type = QUVI_SUBTITLE_PROPERTY_TYPE_MASK & (gint) n;

  rc = QUVI_OK;
  dp = NULL;
  sp = NULL;
  lp = NULL;

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
      /*
       * NOTE: Advances to the first item in the 'language' list if the
       * current pointer is still set to NULL.
       */

    case QUVI_SUBTITLE_LANG_PROPERTY_TRANSLATED:
      *sp = qsl->translated->str;
      break;

    case QUVI_SUBTITLE_LANG_PROPERTY_ORIGINAL:
      *sp = qsl->original->str;
      break;

    case QUVI_SUBTITLE_LANG_PROPERTY_CODE:
      *sp = qsl->code->str;
      break;

    case QUVI_SUBTITLE_LANG_PROPERTY_URL:
      *sp = qsl->url->str;
      break;

    case QUVI_SUBTITLE_LANG_PROPERTY_ID:
      *sp = qsl->id->str;
      break;

    default:
      rc = QUVI_ERROR_INVALID_ARG;
      break;
    }
  return (rc);
}

/** @brief Return a subtitle property
@sa @ref parse_subtitle
@ingroup subprop
*/
void quvi_subtitle_lang_get(quvi_subtitle_lang_t handle,
                            QuviSubtitleLangProperty n, ...)
{
  _quvi_subtitle_lang_t qsl;
  va_list arg;
  gpointer p;
  _quvi_t q;

  /* If G_DISABLE_CHECKS is defined then the check is not performed. */
  g_return_if_fail(handle != NULL);

  va_start(arg, n);
  p = va_arg(arg, gpointer);
  va_end(arg);

  qsl = (_quvi_subtitle_lang_t) handle;
  q = qsl->handle.quvi;

  q->status.rc = _get(qsl, n, p);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
