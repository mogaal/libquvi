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

_host_constants(clipfish, "clipfish.de", "flv");

_host_re(re_id,     "(?i)\\/video\\/(.*?)\\/");
_host_re(re_title,  "(?i)<title>(.*?)</title>");
_host_re(re_lnk,    "(?i)&url=(.*?)&");

QUVIcode
handle_clipfish(const char *url, _quvi_video_t video) {
    char *content, *config_url, *config, *lnk;
    QUVIcode rc;

    /* host id */
    _host_id("clipfish");

    /* common */
    rc = parse_page_common(url, video, &content, re_id, re_title);
    _free(content);

    if (rc != QUVI_OK)
        return (rc);

    video->title = strepl(video->title, "Video: ", "");
    video->title = strepl(video->title, " - Clipfish", "");

    /* config */
    asprintf(&config_url,
        "http://www.clipfish.de/video_n.php?p=0|DE&vid=%s", video->id);

    rc = fetch_to_mem(video, config_url, QUVISTATUSTYPE_CONFIG, &config);
    _free(config_url);

    /* video link */
    rc = regexp_capture(
        video->quvi,
        config,
        re_lnk,
        0,
        0,
        &lnk,
        (void *) 0
    );

    _free(config);

    if (rc != QUVI_OK)
        return (rc);

    rc = add_video_link(&video->link, "%s", lnk);

    _free(lnk);

    return (rc);
}


