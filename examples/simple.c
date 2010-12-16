/* 
* Copyright (C) 2009 Toni Gundogdu.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
* simple.c -- a simplistic libquvi example.
* Ignores errors, see src/quvi.c for a more complete example.
*/

#include <stdio.h>
#include <quvi/quvi.h>

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
  quvi_t q;                     /* library handle */
  quvi_video_t v;               /* video handle */
  char *lnk;                    /* holds parsed video link */

  quvi_init(&q);
  quvi_setopt(q, QUVIOPT_STATUSFUNCTION, &status_callback);
  quvi_parse(q, "http://www.youtube.com/watch?v=DeWsZ2b_pK4", &v);
  quvi_getprop(v, QUVIPROP_VIDEOURL, &lnk);
  puts(lnk);
  quvi_parse_close(&v);
  quvi_close(&q);

  return (0);
}
