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

/** @file script_get.c */

#include "config.h"

#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_script_s.h"

/** @cond NODOC */
typedef _quvi_script_t _qs_t;
/** @endcond */

static QuviError _get(_quvi_t q, QuviScriptType stype,
                      QuviScriptProperty n, ...)
{
  QuviError rc;
  gpointer *vp;
  gdouble *dp;
  va_list arg;
  gchar **sp;
  glong *lp;
  gint type;
  _qs_t qs;

  switch (stype)
    {
    case QUVI_SCRIPT_TYPE_SUBTITLE_EXPORT:
      qs = (_qs_t) q->scripts.curr.subtitle_export->data;
      break;

    case QUVI_SCRIPT_TYPE_SUBTITLE:
      qs = (_qs_t) q->scripts.curr.subtitle->data;
      break;

    case QUVI_SCRIPT_TYPE_PLAYLIST:
      qs = (_qs_t) q->scripts.curr.playlist->data;
      break;

    case QUVI_SCRIPT_TYPE_MEDIA:
    default:
      qs = (_qs_t) q->scripts.curr.media->data;
      break;

    case QUVI_SCRIPT_TYPE_SCAN:
      qs = (_qs_t) q->scripts.curr.scan->data;
      break;
    }

  va_start(arg, n);
  type = QUVI_SCRIPT_PROPERTY_MASK & (gint) n;

  dp = NULL;
  sp = NULL;
  vp = NULL;
  lp = NULL;

  rc = QUVI_OK;

  switch (type)
    {
    case QUVI_SCRIPT_PROPERTY_TYPE_DOUBLE:
      dp = va_arg(arg, gdouble*);
      if (dp == NULL)
        rc = QUVI_ERROR_INVALID_ARG;
      break;

    case QUVI_SCRIPT_PROPERTY_TYPE_STRING:
      sp = va_arg(arg, gchar**);
      if (sp == NULL)
        rc = QUVI_ERROR_INVALID_ARG;
      break;

    case QUVI_SCRIPT_PROPERTY_TYPE_LONG:
      lp = va_arg(arg, glong*);
      if (lp == NULL)
        rc = QUVI_ERROR_INVALID_ARG;
      break;

    case QUVI_SCRIPT_PROPERTY_TYPE_VOID:
      vp = va_arg(arg, gpointer*);
      if (vp == NULL)
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
      /* Export (e.g. subtitle) */
    case QUVI_SCRIPT_PROPERTY_EXPORT_FORMAT:
      *sp = qs->export.format->str;
      break;

      /* Media, playlist. */
    case QUVI_SCRIPT_PROPERTY_DOMAINS:
      *sp = qs->domains->str;
      break;

      /* Any */
    case QUVI_SCRIPT_PROPERTY_FILEPATH:
      *sp = qs->fpath->str;
      break;
    case QUVI_SCRIPT_PROPERTY_FILENAME:
      *sp = qs->fname->str;
      break;
    case QUVI_SCRIPT_PROPERTY_SHA1:
      *sp = qs->sha1->str;
      break;

    default:
      rc = QUVI_ERROR_INVALID_ARG;
      break;
    }
  return (rc);
}

/** @brief Return a script property
@ingroup script
*/
void quvi_script_get(quvi_t handle, QuviScriptType type,
                     QuviScriptProperty id, ...)
{
  va_list arg;
  gpointer p;
  _quvi_t q;

  /* If G_DISABLE_CHECKS is defined then the check is not performed. */
  g_return_if_fail(handle != NULL);

  va_start(arg, id);
  p = va_arg(arg, gpointer);
  va_end(arg);

  q = (_quvi_t) handle;
  q->status.rc = _get(handle, type, id, p);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
