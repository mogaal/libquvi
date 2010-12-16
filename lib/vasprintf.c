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

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

int vasprintf(char **dst, const char *fmt, va_list ap)
{
  va_list apc;
  int len;
  char *p;

  va_copy(apc, ap);
  len = vsnprintf(0, 0, fmt, apc) + 1;
  va_end(apc);

  *dst = p = malloc(len);
  if (!p)
    return (-1);

  return (vsnprintf(p, len, fmt, ap));
}
