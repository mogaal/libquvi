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

/** @file set.c */

#include "config.h"

#include <glib.h>
#include <curl/curl.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"

static QuviError _set(_quvi_t q, const QuviOption o, va_list arg)
{
  switch (o)
    {
    case QUVI_OPTION_ALLOW_COOKIES:
      q->opt.allow_cookies = (gboolean) va_arg(arg, glong) >0;
      break;

    case QUVI_OPTION_AUTOPROXY:
      q->opt.autoproxy = (gboolean) va_arg(arg, glong) >0;
      break;

    case QUVI_OPTION_USER_AGENT:
      /* Save for later use: c_reset will reuse this value. */
      g_string_assign(q->opt.user_agent, va_arg(arg, gchar*));
      /* Apply for immediate effect. */
      curl_easy_setopt(q->handle.curl, CURLOPT_USERAGENT,
                       q->opt.user_agent->str);
      break;

    case QUVI_OPTION_CALLBACK_STATUS:
      q->cb.status = va_arg(arg, quvi_callback_status);
      break;

    case QUVI_OPTION_CALLBACK_STATUS_USERDATA:
      q->cb.userdata.status = va_arg(arg, void*);
      break;

    default:
      return (QUVI_ERROR_INVALID_ARG);
    }

  return (QUVI_OK);
}

/** @brief Set library handle option
@sa @ref getting_started
@ingroup lib
*/
void quvi_set(quvi_t handle, QuviOption option, ...)
{
  va_list arg;
  _quvi_t q;

  /* If G_DISABLE_CHECKS is defined then the check is not performed. */
  g_return_if_fail(handle != NULL);

  q = (_quvi_t) handle;

  va_start(arg, option);
  q->status.rc = _set(handle, option, arg);
  va_end(arg);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
