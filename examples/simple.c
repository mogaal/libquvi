/* libquvi
 * Copyright (C) 2009-2011  Toni Gundogdu <legatvs@gmail.com>
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

/* simple.c - basic example */

#include <stdio.h>
#include <quvi/quvi.h>

#include "common.h"

static int status_callback(long param, void *data)
{
  quvi_word status, type;

  status = quvi_loword(param);
  type = quvi_hiword(param);

  printf("status: %d, type: %d\n", status, type);

  return (0);
}

int main(int argc, char **argv)
{
  quvi_media_t m;               /* Media handle */
  QUVIcode rc;                  /* quvi return code */
  char *url;                    /* Holds parsed media stream URL */
  quvi_t q;                     /* Session handle */

  /* Start a new session. */
  rc = quvi_init(&q);
  check_error(q, rc);

  /* Set session options. */
  quvi_setopt(q, QUVIOPT_STATUSFUNCTION, &status_callback);
  quvi_setopt(q, QUVIOPT_NOVERIFY, 1L); /* Do not verify media stream URL */

  /* Parse media details from the specified URL. */
  rc = quvi_parse(q, "http://vimeo.com/1485507", &m);
  check_error(q, rc);

  /* Access the parsed media details. */
  quvi_getprop(m, QUVIPROP_MEDIAURL, &url);
  puts(url);

  /* When done with the parsed details, free them. */
  quvi_parse_close(&m);

  /* When done, close the session. */
  quvi_close(&q);

  return (0);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
