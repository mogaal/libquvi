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

_host_constants(break, "break.com", "flv");

_host_re(re_id,     "(?i)contentid='(.*?)'");
_host_re(re_title,  "(?i)id=\"vid_title\" content=\"(.*?)\"");
_host_re(re_fpath,  "(?i)contentfilepath='(.*?)'");
_host_re(re_fname,  "(?i)filename='(.*?)'");
_host_re(re_fhash,  "(?i)flashvars.icon = \"(.*?)\"");

QUVIcode
handle_break(const char *url, _quvi_video_t video) {
    char *content, *fpath, *fname, *fhash;
    QUVIcode rc;

    /* host id */
    _host_id("break");

    /* common */
    rc = parse_page_common(url, video, &content, re_id, re_title);

    if (rc != QUVI_OK)
        return (rc);

    /* fpath */
    rc = regexp_capture(
        video->quvi,
        content,
        re_fpath,
        0,
        0,
        &fpath,
        (void *) 0
    );

    if (rc != QUVI_OK) {
        _free(content);
        return (rc);
    }

    /* fname */
    rc = regexp_capture(
        video->quvi,
        content,
        re_fname,
        0,
        0,
        &fname,
        (void *) 0
    );

    if (rc != QUVI_OK) {
        _free(content);
        _free(fpath);
        return (rc);
    }

    /* fhash */
    rc = regexp_capture(
        video->quvi,
        content,
        re_fhash,
        0,
        0,
        &fhash,
        (void *) 0
    );

    _free(content);

    if (rc != QUVI_OK) {
        _free(fpath);
        _free(fname);
        return (rc);
    }

    /* video link */
    add_video_link(&video->link,
        "http://video1.break.com/dnet/media/%s/%s.flv?%s",
            fpath, fname, fhash);

    _free(fpath);
    _free(fname);
    _free(fhash);

    return (QUVI_OK);
}


