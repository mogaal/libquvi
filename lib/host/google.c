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

_host_constants(google, "video.google.", "flv|mp4");

_host_re(re_id,     "(?i)docid:'(.*?)'");
_host_re(re_title,  "(?i)<title>(.*?)</");
_host_re(re_lnk,    "(?i)videourl\\W+x3d(.*?)\\W+x26");
_host_re(re_mp4,    "(?i)href=(.*?)=ck1");

QUVIcode
handle_google(const char *url, _quvi_video_t video) {
    char *content, *lnk, *format;
    QUVIcode rc;

    /* host id */
    _host_id("google");

    /* common */
    rc = parse_page_common(url, video, &content, re_id, re_title);

    if (rc != QUVI_OK)
        return (rc);

    /* video link */
    lnk = 0;

    format = video->quvi->format;

    if (format) {
        if (!strcmp(format, "mp4") || !strcmp(format, "best")) {
            char *mp4;

            rc = regexp_capture(
                video->quvi,
                content,
                re_mp4,
                0,
                0,
                &mp4,
                (void *) 0
            );

            if (rc == QUVI_OK && mp4) {
                asprintf(&lnk, "%s=ck1", mp4);
                _free(mp4);
            }
        }
    }

    if (!lnk) { /* default to flv */
        rc = regexp_capture(
            video->quvi,
            content,
            re_lnk,
            0,
            0,
            &lnk,
            (void *) 0
        );

        if (rc != QUVI_OK)
            return (rc);
    }

    _free(content);

    lnk = unescape(video->quvi, lnk); /* orig. lnk freed by unescape */
    rc = add_video_link(&video->link, "%s", lnk);

    _free(lnk);

    return (rc);
}


