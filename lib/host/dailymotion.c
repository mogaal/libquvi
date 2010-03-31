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

_host_constants(dailymotion, "dailymotion.", "flv|hq|hd");

_host_re(re_id,    "(?i)video\\/(.*?)_");
_host_re(re_title, "(?i)title=\"(.*?)\"");
_host_re(re_path,  "(?i)%22(\\w\\w)URL%22%3A%22(.*?)%22");

QUVIcode
handle_dailymotion(const char *url, _quvi_video_t video) {
    static const char default_format[] = "sd";
    char *content, *best, *req_format;
    int ovector[30];
    QUVIcode rc;
    int offset;

    req_format =
        strcmp(video->quvi->format, "flv") == 0
        ? (char *)default_format
        : !is_format_supported(video->quvi->format, formats_dailymotion)
          ? (char *)default_format
          : video->quvi->format;

    /* host id */
    _host_id("dailymotion");

    /* common */
    rc = parse_page_common(url, video, &content, re_id, re_title);

    if (rc != QUVI_OK)
        return (rc);

    /*
     * can we dl this video? if it looks like a partner video,
     * walks like a partner video, then it's a partner video.
     * we don't care for those.
     */
    if (strstr(content, "SWFObject(\"http:") != 0) {
        _quvi_t quvi = video->quvi; /* seterr macro needs this. */
        seterr("looks like dailymotion partner video. refusing to continue");
        _free(content);
        return (QUVI_NOSUPPORT);
    }

    /* paths */
    offset = 0;
    best = NULL;
    do {
        char *path, *format_id;

        rc = regexp_capture(
            video->quvi,
            content+offset,
            re_path,
            ovector,
            sizeof(ovector)/sizeof(int),
            &format_id,
            &path,
            (void *) 0
        );

        if (rc == QUVI_OK) {

            path = strepl(path, "%5C", "");
            path = unescape(video->quvi, path);

            if (best != NULL)
                _free(best);

            asprintf(&best, "%s", path);

            if (strcmp(format_id, req_format) == 0)
                rc = add_video_link(&video->link, path);

            _free(path);
            _free(format_id);

            offset += ovector[1];
        }
    } while (rc == QUVI_OK);

    _free(content);

    if (best != NULL) {
        if (strcmp(req_format, "best") == 0)
            rc = add_video_link(&video->link, "%s", best);
        _free(best);
    }

    if (rc != QUVI_OK && !video->link)
        return (rc);

    return (QUVI_OK);
}

#ifdef _1_
static QUVIcode
parse(const _quvi_video_t video, char *paths) {
    static const char default_format[] = "spark";
    int best_width, best_flag;
    const char *format;
    char *best_link;
    char *s, *t;
    QUVIcode rc;

    format = 
        !strcmp(video->quvi->format, "flv")
        ? default_format /* flv -> spark which is dmotion's default fmt */
        : !is_format_supported(video->quvi->format, formats_dailymotion)
          ? default_format /* -> spark if requested fmt unsupported */
          : video->quvi->format;

    best_width = 0;
    best_flag  = !strcmp(format, "best");
    best_link   = 0;

    s = paths;
    t = strtok(s, "||");

    while (t) {
        char *path, *id;

        rc = regexp_capture(
            video->quvi,
            t,
            "(.*?)@@(.*?)$",
            0,
            0,
            &path,
            &id,
            (void *) 0
        );

        if (rc != QUVI_OK)
            return (rc);

        if (!best_flag) {
            if (!strcmp(id,format)) {
                rc = add_video_link(&video->link, "%s", path);
                _free(path);
                _free(id);
                return (rc);
            }
        }
        else { /* -f best */
            char *w, *h;

            rc = regexp_capture(
                video->quvi,
                path,
                "-(\\d+)x(\\d+)",
                0,
                0,
                &w,
                &h,
                (void *) 0
            );

            if (rc != QUVI_OK) {
                _free(path);
                _free(id);
                return (rc);
            }

            if (atoi(w) > best_width) {
                best_width = atoi(w);
                _free(best_link);
                asprintf(&best_link, "%s", path);
            }

            _free(w);
            _free(h);
        }

        _free(path);
        _free(id);

        t = strtok(0, "||");
    }

    rc = add_video_link(&video->link, "%s", best_link);

    _free(best_link);

    return (rc);
}

QUVIcode
handle_dailymotion(const char *url, _quvi_video_t video) {
    char *content, *paths;
    QUVIcode rc;

    /* host id */
    _host_id("dailymotion");

    /* common */
    rc = parse_page_common(url, video, &content, re_id, re_title);

    if (rc != QUVI_OK)
        return (rc);

    /* paths */
    rc = regexp_capture(
        video->quvi,
        content,
        re_paths,
        0,
        0,
        &paths,
        (void *) 0
    );

    _free(content);

    if (rc != QUVI_OK)
        return (rc);

    paths = unescape(video->quvi, paths); /* frees paths */

    rc = parse(video, paths);

    _free(paths);

    return (rc);
}
#endif


