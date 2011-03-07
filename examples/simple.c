/* quvi
 * Copyright (C) 2009,2010,2011  Toni Gundogdu <legatvs@gmail.com>
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

/* simple.c -- A very basic example. See src/quvi.c for a more complete
 * example. */

#include <stdio.h>
#include <quvi/quvi.h>

static void check_error(quvi_t q, QUVIcode rc)
{
  if (rc == QUVI_OK)
    return;

  fprintf(stderr, "error: %s\n", quvi_strerror(q, rc));

  if (q != NULL)
    quvi_close(&q);

  exit(1);
}

/* See src/quvi.c for a more complete example of status callback
 * function */
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
  quvi_video_t v;               /* Video handle */
  QUVIcode rc;                  /* quvi return code */
  char *lnk;                    /* Holds parsed video link */
  quvi_t q;                     /* Session handle */

  /* Start a new session. */
  rc = quvi_init(&q);
  check_error(q, rc);

  /* Set session options. */
  quvi_setopt(q, QUVIOPT_STATUSFUNCTION, &status_callback);
  quvi_setopt(q, QUVIOPT_NOVERIFY, 1L); /* Do not verify video link. */

  /* Parse video details from the specified URL. */
  rc = quvi_parse(q, "http://vimeo.com/1485507", &v);
  check_error(q, rc);

  /* Access the parsed video details. */
  quvi_getprop(v, QUVIPROP_VIDEOURL, &lnk);
  puts(lnk);

  /* When done with the parsed details, free them. */
  quvi_parse_close(&v);

  /* When done, close the session. */
  quvi_close(&q);

  return (0);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
