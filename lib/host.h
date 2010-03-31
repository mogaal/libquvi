/* 
* Copyright (C) 2009,2010 Toni Gundogdu.
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

#ifndef host_h
#define host_h

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quvi/quvi.h"
#include "internal.h"
#include "curl.h"
#include "util.h"

/* Host specific handler functions. */

#define _host(n) \
    extern const char domain_##n[]; \
    extern const char formats_##n[]; \
    QUVIcode handle_##n(const char *, _quvi_video_t)

_host(youtube);     /* youtube.c */
_host(break);       /* break.c */
_host(cctv);        /* cctv.c */
_host(evisor);      /* evisor.c */
_host(clipfish);    /* You get the idea. */
_host(funnyhub);
_host(golem);
_host(google);
_host(liveleak);
_host(myubo);
_host(sevenload);
_host(vimeo);
_host(spiegel);
_host(dailymotion);
_host(buzzhumor);
#ifdef ENABLE_BROKEN
_host(ehrensenf);
_host(spyfilms);
#endif
#ifdef ENABLE_SMUT
_host(tube8);
_host(xvideos);
_host(youjizz);
#endif
/* Add new ones below. */

#undef _host

/* Used in implementation files. */

#define _host_constants(n,d,f) \
    const char domain_##n[]  = d; \
    const char formats_##n[] = f

#define _host_re(n,r) \
    static const char n[] = r

#define _host_id(id) \
    do { setvid(video->host_id, "%s", id); } while(0)

#endif


