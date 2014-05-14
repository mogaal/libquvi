/* libquvi
 * Copyright (C) 2009-2010  Toni Gundogdu <legatvs@gmail.com>
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

#include <stdio.h>
#include <stdarg.h>

#include "platform.h"

int asprintf(char **dst, const char *fmt, ...)
{
  va_list ap;
  int rc;

  va_start(ap, fmt);
  rc = vasprintf(dst, fmt, ap);
  va_end(ap);

  return (rc);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
