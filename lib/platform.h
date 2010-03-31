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

#ifndef platform_h
#define platform_h

#include "config.h"

#include <stdarg.h>

#ifndef HAVE_VASPRINTF
int vasprintf(char **dst, const char *fmt, va_list ap);
#endif

#ifndef HAVE_ASPRINTF
int asprintf(char **dst, const char *fmt, ...);
#endif

#ifndef HAVE_STRUPR
char *strupr(char *src);
#endif

#ifndef HAVE_STRTOK_R
char *strtok_r(char *str, const char *delim, char **saveptr);
#endif

#endif


