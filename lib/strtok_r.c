/* 
* Copyright (C) 2009 Toni Gundogdu.
* Copyright (C) 1991,1996-1999,2001,2004,2007 Free Software Foundation, Inc.
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

#include <string.h>
#include <stdlib.h>

#ifdef TEST_STRTOK_R
#include <stdio.h>
#endif

#include "platform.h"

/** Copied from GNU C Library with minor changes. */
char *
strtok_r (char *s, const char *delim, char **saveptr) {
    char *t;

    if (!s)
        s = *saveptr;

    s += strspn(s, delim);

    if (*s == '\0') {
        *saveptr = s;
        return (0);
    }

    t = s;
    if ( !(s = strpbrk(t, delim)) )
        *saveptr = strchr(t, '\0');
    else {
        *s = '\0';
        *saveptr = s+1;
    }
    return (t);
}

#ifdef TEST_STRTOK_R
int
main (int argc, char **argv) {
    char *t, *tmp, *dup;
    if (argc < 3) {
        fprintf(stderr, "usage: strtok_r [string] [delim]\n");
        return (0);
    }
    dup = strdup(argv[1]);
    t = strtok_r(dup, argv[2], &tmp);
    while (t) {
        puts(t);
        t = strtok_r(0, argv[2], &tmp);
    }
    free(dup);
    return (0);
}
#endif


