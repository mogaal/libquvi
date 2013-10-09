/* libquvi
 * Copyright (C) 2011  Toni Gundogdu <legatvs@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include <quvi/quvi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
  quvi_media_t m;
  QUVIcode rc;
  quvi_t q;

  rc = quvi_init(&q);
  if (rc != QUVI_OK)
    {
      puts(quvi_strerror(q, rc));
      quvi_close(&q); /* Must be closed. */
      return (rc);
    }

  quvi_setopt(q, QUVIOPT_NORESOLVE, 1L); /*!*/

  rc = quvi_parse(q, "http://dai.ly/cityofscars", &m);
  if (rc != QUVI_OK)
    puts(quvi_strerror(q, rc));
  else
    {
      char *s;
      quvi_getprop(m, QUVIPROP_PAGETITLE, &s);
      puts(s);
    }
  quvi_parse_close(&m);
  quvi_close(&q);

  return (0);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
