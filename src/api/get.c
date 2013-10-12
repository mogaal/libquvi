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

/** @file get.c */

#include "config.h"

#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"

static QuviError _get(_quvi_t q, const QuviInfo n, ...)
{
  QuviError rc;
  gpointer *vp;
  va_list arg;
  gdouble *dp;
  gchar **sp;
  glong *lp;
  gint type;

  va_start(arg, n);
  type = QUVI_INFO_TYPE_MASK & (gint) n;

  rc = QUVI_OK;
  vp = NULL;
  lp = NULL;
  dp = NULL;
  sp = NULL;

  switch (type)
    {
    case QUVI_INFO_TYPE_LONG:
      lp = va_arg(arg, glong*);
      if (lp == NULL)
        rc = QUVI_ERROR_INVALID_ARG;
      break;
    case QUVI_INFO_TYPE_VOID:
      vp = va_arg(arg, gpointer*);
      if (vp == NULL)
        rc = QUVI_ERROR_INVALID_ARG;
      break;
    case QUVI_INFO_TYPE_DOUBLE:
      dp = va_arg(arg, gdouble*);
      if (dp == NULL)
        rc = QUVI_ERROR_INVALID_ARG;
      break;
    case QUVI_INFO_TYPE_STRING:
      sp = va_arg(arg, gchar**);
      if (sp == NULL)
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
    case QUVI_INFO_RESPONSE_CODE:
      *lp = q->status.resp_code;
      break;
    case QUVI_INFO_CURL_HANDLE:
      *vp = q->handle.curl;
      break;
    default:
      rc = QUVI_ERROR_INVALID_ARG;
      break;
    }
  return (rc);
}

/** @brief Return information about the library handle
@sa @ref getting_started
@ingroup lib
*/
void quvi_get(quvi_t handle, QuviInfo info, ...)
{
  va_list arg;
  gpointer p;
  _quvi_t q;

  /* If G_DISABLE_CHECKS is defined then the check is not performed. */
  g_return_if_fail(handle != NULL);

  va_start(arg, info);
  p = va_arg(arg, gpointer);
  va_end(arg);

  q = (_quvi_t) handle;
  q->status.rc = _get(q, info, p);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
