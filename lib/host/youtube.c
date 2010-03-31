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

_host_constants(youtube,
    "youtube.com", "flv|3gp|mp4|hq|hd|fmt34|fmt17|fmt18|fmt35|fmt22");

_host_re(re_id,     "(?i)\"video_id\": \"(.*?)\"");
_host_re(re_title,  "(?i)<meta name=\"title\" content=\"(.*?)\"");
_host_re(re_t,      "(?i)\"t\": \"(.*?)\"");
_host_re(re_fmap,   "(?i)\"fmt_map\": \"(\\d+)");

QUVIcode
handle_youtube(const char *url, _quvi_video_t video) {
    char *content, *t, *fmap, *format, *lnk, *fmt_lnk;
    QUVIcode rc;

    /* host id */
    setvid(video->host_id, "%s", "youtube");

    /* common */
    rc = parse_page_common(url, video, &content, re_id, re_title);

    if (rc != QUVI_OK)
        return (rc);

    /* t */
    rc = regexp_capture(
        video->quvi,
        content,
        re_t,
        0,
        0,
        &t,
        (void *) 0
    );

    if (rc != QUVI_OK) {
        _free(content);
        return (rc);
    }

    /* fmt_map */
    rc = regexp_capture(
        video->quvi,
        content,
        re_fmap,
        0,
        0,
        &fmap,
        (void *) 0
    );

    /* video link */
    asprintf(&lnk,
        "http://youtube.com/get_video?video_id=%s&t=%s", video->id, t);

    _free(t);
    _free(content);

    /* format */
    format = video->quvi->format;
    fmt_lnk = 0;

    if (format) {
        char *fmt = 0;

        /*
        * Youtube/Google likes to rehash these from time to time.
        *
        * It's not uncommon that some formats are available only
        * for some videos. The following lists aliases for the
        * supported "fmt" strings, e.g. fmt22=hd.
        *
        * fmt22 = HD [1280x720]
        * fmt35 = HQ  [640x380]
        * fmt18 = mp4 [480x360]
        * fmt34 = -   [320x180] quality reportedly varies
        * fmt17 = 3gp [176x144]
        *
        * If --format is unused, we default to whatever youtube
        * defaults to by leaving "&fmt=" from the video link.
        */

        if (rc == QUVI_OK && !strcmp(format, "best") && fmap)
            fmt = fmap;
        else {
            if (!strcmp(format, "fmt18") || !strcmp(format, "mp4"))
                fmt = "18";
            else if (!strcmp(format, "fmt34"))
                fmt = "34";
            else if (!strcmp(format, "fmt35") || !strcmp(format, "hq"))
                fmt = "35";
            else if (!strcmp(format, "fmt22") || !strcmp(format, "hd"))
                fmt = "22";
            else if (!strcmp(format, "fmt17") || !strcmp(format, "3gp"))
                fmt = "17";
        }

        if (fmt) {
            _free(fmt_lnk);
            asprintf(&fmt_lnk, "%s&fmt=%s", lnk, fmt);
        }
    }

    _free(fmap);

    if (rc == QUVI_OK) {
        rc = add_video_link(&video->link, "%s",
            fmt_lnk
            ? fmt_lnk
            : lnk);
    }

    _free(lnk);

    return (QUVI_OK);
}


