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
    "youtube.com",
    /*
     * Not in any specific order, note that this "array" contains
     * also (for a number of historical reasons) the aliases for
     * (some of) the fmt IDs.
     */
    "flv|3gp|mp4|hq|hd|fmt34|fmt17|fmt18|fmt35|fmt22|fmt37");

_host_re(re_id,     "(?i)&video_id=(.*?)&");
_host_re(re_title,  "(?i)<meta name=\"title\" content=\"(.*?)\"");
_host_re(re_t,      "(?i)&t=(.*?)&");
_host_re(re_fmap,   "(?i)&fmt_map=(\\d+)");

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

    t = unescape(video->quvi, t);

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

    if (rc != QUVI_OK)
        fmap = 0; /* This is still OK, just make sure it's a NULL. */

    /* video link */
    asprintf(&lnk,
        "http://youtube.com/get_video?video_id=%s&t=%s", video->id, t);

    _free(t);
    _free(content);

    /* format */
    format  = video->quvi->format;
    fmt_lnk = 0;

    if (format) {

        /*
         * See the format array notes above. Note that not all formats
         * are available for all videos. For example, some of the older
         * videos have only some of the formats available.
         *
         * The following aliases that are supported for historical
         * reasons only:
         *
         * Alias   ID
         *   hd .. fmt22 (1280x720)
         *   hq .. fmt35  (640x380)
         *  mp4 .. fmt18  (480x360)
         *  3gp .. fmt17  (176x144)
         *
         * The resolutions may vary. We have no accurate or reliable
         * info available.
         */

        char *fmt = 0;

        if (rc == QUVI_OK && !strcmp(format, "best") && fmap)
            fmt = fmap;
        else {
            if (!strcmp(format, "fmt18") || !strcmp(format, "mp4"))
                fmt = "18";
            else if (!strcmp(format, "fmt37"))
                fmt = "37";
            else if (!strcmp(format, "fmt34"))
                fmt = "34";
            else if (!strcmp(format, "fmt35") || !strcmp(format, "hq"))
                fmt = "35";
            else if (!strcmp(format, "fmt22") || !strcmp(format, "hd"))
                fmt = "22";
            else if (!strcmp(format, "fmt17") || !strcmp(format, "3gp"))
                fmt = "17";
        }

        if (fmt)
            asprintf(&fmt_lnk, "%s&fmt=%s", lnk, fmt);
    }

    _free(fmap);

    if (rc == QUVI_OK) {
        rc = add_video_link(&video->link, "%s",
            fmt_lnk
            ? fmt_lnk
            : lnk);
    }

    _free(fmt_lnk);
    _free(lnk);

    return (QUVI_OK);
}


