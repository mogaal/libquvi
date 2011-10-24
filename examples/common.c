/* libquvi
 * Copyright (C) 2011  Toni Gundogdu <legatvs@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

/* common.c - common functions used by the examples */

#include <stdio.h>
#include <stdarg.h>
#include <quvi/quvi.h>

#include "common.h"

void spew_e(const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
}

void check_error(quvi_t q, QUVIcode rc)
{
  if (rc == QUVI_OK)
    return;

  fprintf(stderr, "error: %s\n", quvi_strerror(q, rc));

  if (q != NULL)
    quvi_close(&q);

  exit(1);
}

void handle_resolve_status(quvi_word type)
{
  if (type == QUVISTATUSTYPE_DONE)
    spew_e("done.\n");
  else
    spew_e(":: Check for URL redirection ...");
}

void handle_fetch_status(quvi_word type, void *p)
{
  switch (type)
    {
    default:
      spew_e(":: Fetch %s ...", (char *)p);
      break;
    case QUVISTATUSTYPE_CONFIG:
      spew_e(":: Fetch config ...");
      break;
    case QUVISTATUSTYPE_PLAYLIST:
      spew_e(":: Fetch playlist ...");
      break;
    case QUVISTATUSTYPE_DONE:
      spew_e("done.\n");
      break;
    }
}

void handle_verify_status(quvi_word type)
{
  switch (type)
    {
    default:
      spew_e(":: Verify media URL ...");
      break;
    case QUVISTATUSTYPE_DONE:
      spew_e("done.\n");
      break;
    }
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
