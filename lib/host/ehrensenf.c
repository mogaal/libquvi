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

#include "host.h"

_host_constants(ehrensenf, "ehrensenf.de", "flv");

_host_re(re_date,  "(\\d+)\\.(\\d+)\\.(\\d+)");

QUVIcode
handle_ehrensenf(const char *url, _quvi_video_t video) {
    char *content, *d, *m, *y;
    QUVIcode rc;

    /* host id */
    _host_id("ehrensenf");

    /* common */
    rc = parse_page_common(url, video, &content, NULL, NULL);

    if (rc != QUVI_OK)
        return (rc);

    /* date */
    rc = regexp_capture(
        video->quvi,
        content,
        re_date,
        0,
        0,
        &d,
        &m,
        &y,
        (void *) 0
    );

    _free(content);

    if (rc != QUVI_OK)
        return (rc);

    /* id and title */
    setvid(video->id,
        "%04d-%02d-%02d", atoi(y), atoi(m), atoi(d));

    _free(d);
    _free(m);
    _free(y);

    setvid(video->title, "%s", video->id);

    /* link */
    rc = add_video_link(&video->link,
        "http://www.ehrensenf.de/misc/"
        "load-balancing/lb.php?file=%s.flv",
            video->id);

    return (rc);
}


