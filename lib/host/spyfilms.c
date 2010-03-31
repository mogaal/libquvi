/* 
* Copyright (C) 2010 Toni Gundogdu.
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

_host_constants(spyfilms, "spyfilms.com", "flv|hd");

_host_re(re_title,  "(?i)fv_title:\\s+\"(.*?)\"");
_host_re(re_lnk,    "(?i)fv_flv:\\s+\"(.*?)\"");
_host_re(re_hd,     "(?i)fv_hd:\\s+\"(.*?)\"");
_host_re(re_reel_path, "(?i)fv_reel_path:\\s+\"(.*?)\"");
_host_re(re_dir_path, "(?i)fv_director_path:\\s+\"(.*?)\"");

static QUVIcode
video_id(_quvi_video_t video) {
    size_t size;
    char *id, *tmp;
    int i;

    id = calloc(1, strlen(video->title)*2+1);
    if (!id)
        return (QUVI_MEM);

    /* video title -> hex string. */
    for (i=0; i<strlen(video->title); ++i) {
        asprintf(&tmp, "%02x", video->title[i]);
        strcat(id, tmp);
        _free(tmp);
    }

    _free(video->id);

    size = strlen(id) ? 12:strlen(id);

    video->id = calloc(1, size+1);
    if (!video->id) {
        _free(id);
        return (QUVI_MEM);
    }

    /* copy the first 12 (or less) bytes of the hex string and use it
     * as the video id. */
    memcpy(video->id, id, size);
    video->id[size] = '\0';

    _free(id);

    return (QUVI_OK);
}

QUVIcode
handle_spyfilms(const char *url, _quvi_video_t video) {
    char *content, *lnk, *hd, *reel_path, *dir_path;
    QUVIcode rc;

    /* host id */
    _host_id("spyfilms");

    /* common */
    rc = parse_page_common(url, video, &content, 0, re_title);

    if (rc != QUVI_OK)
        return (rc);

    /* cleanup title */
    video->title = strepl(video->title, "+", " ");

    /* flv link */
    rc = regexp_capture(
        video->quvi,
        content,
        re_lnk,
        0,
        0,
        &lnk,
        (void *) 0
    );

    if (rc != QUVI_OK) {
        _free(content);
        return (rc);
    }

    /* hd link */
    if (!strcmp(video->quvi->format, "best")
        || !strcmp(video->quvi->format, "hd"))
    {
        int tmp;

        tmp = regexp_capture(
            video->quvi,
            content,
            re_hd,
            0,
            0,
            &hd,
            (void *) 0
        );

        if (tmp == QUVI_OK) {
            _free(lnk);
            lnk = hd;
        }
    }

    /* reel path */
    rc = regexp_capture(
        video->quvi,
        content,
        re_reel_path,
        0,
        0,
        &reel_path,
        (void *) 0
    );

    if (rc != QUVI_OK) {
        _free(content);
        _free(lnk);
        return (rc);
    }

    /* director path */
    rc = regexp_capture(
        video->quvi,
        content,
        re_dir_path,
        0,
        0,
        &dir_path,
        (void *) 0
    );

    _free(content);

    if (rc != QUVI_OK) {
        _free(reel_path);
        _free(lnk);
        return (rc);
    }

    /* video link */
    add_video_link(&video->link, "%s%s/%s", reel_path, dir_path, lnk);

    _free(reel_path);
    _free(dir_path);
    _free(lnk);

    /* video id: host does not appear to provide one. create own. */
    return (video_id(video));
}


