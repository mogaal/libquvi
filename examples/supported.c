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

#include <stdio.h>
#include <quvi/quvi.h>

static void print_supported(quvi_t q)
{
  char *d,*f;
  while (quvi_next_supported_website(q, &d, &f) == QUVI_OK)
    {
      puts(d);
      /* Ignore returned "formats" string as it has no longer use in
       * libquvi 0.4. */
      quvi_free(d);
      quvi_free(f);
    }
}

int main(int argc, char **argv)
{
  QUVIcode rc;
  quvi_t q;

  rc = quvi_init(&q);
  if (rc != QUVI_OK)
    {
      puts(quvi_strerror(q, rc));
      return (rc);
    }

#ifdef _1
  quvi_setopt(q, QUVIOPT_CATEGORY, QUVIPROTO_HTTP); /* Include HTTP only */
#endif
  print_supported(q);
  quvi_close(&q);

  return (0);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
