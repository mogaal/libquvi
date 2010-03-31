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

_host_constants(vimeo, "vimeo.com", "flv"); /* Make hd -> flv. */

_host_re(re_id,     "(?i)clip_id=(.*?)\"");
_host_re(re_title,  "(?i)<caption>(.*?)</caption>");
_host_re(re_sign,   "(?i)<request_signature>(.*?)</");
_host_re(re_exp,    "(?i)<request_signature_expires>(.*?)</");
_host_re(re_hd,     "(?i)<hd_button>(\\d+)</");

QUVIcode
handle_vimeo(const char *url, _quvi_video_t video) {
    char *content, *config_url, *config, *sign, *exp;
    char *format, *quality, *lnk;
    int hd_avail;
    QUVIcode rc;

    hd_avail = 0;

    /* host id */
    _host_id("vimeo");

    /* common */
    rc = parse_page_common(url, video, &content, re_id, NULL);

    _free(content);

    if (rc != QUVI_OK)
        return (rc);

    /* config */
    asprintf(&config_url,
        "http://vimeo.com/moogaloop/load/clip:%s", video->id);

    rc = fetch_to_mem(video, config_url, QUVISTATUSTYPE_CONFIG, &config);

    _free(config_url);

    if (rc != QUVI_OK)
        return (rc);

    /* title */
    _free(video->title);

    rc = regexp_capture(
        video->quvi,
        config,
        re_title,
        0,
        0,
        &video->title,
        (void *) 0
    );

    if (rc != QUVI_OK) {
        _free(config);
        return (rc);
    }

    /* req. signature. */
    rc = regexp_capture(
        video->quvi,
        config,
        re_sign,
        0,
        0,
        &sign,
        (void *) 0
    );

    if (rc == QUVI_OK) {
        /* req. signature expires */
        rc = regexp_capture(
            video->quvi,
            config,
            re_exp,
            0,
            0,
            &exp,
            (void *) 0
        );

        if (rc == QUVI_OK) {
            QUVIcode _rc; /* do not override rc */
            char *hd;

            _rc = regexp_capture(
                video->quvi,
                config,
                re_hd,
                0,
                0,
                &hd,
                (void *) 0
            );

            if (hd && !strcmp(hd, "1"))
                hd_avail = 1;

            _free(hd);
        }
    }

    _free(config);

    if (rc != QUVI_OK) {
        _free(exp);
        _free(sign);
        return (rc);
    }

    format  = video->quvi->format;

    quality = 
        (format && (!strcmp(format,"hd") || !strcmp(format,"best")) && hd_avail)
        ? "hd"
        : "sd";

    /* video link */
    asprintf(&lnk,
        "http://vimeo.com/moogaloop/play/clip:%s/%s/%s/?q=%s",
        video->id, sign, exp, quality);

    _free(sign);
    _free(exp);

    rc = add_video_link(&video->link, "%s", lnk);

    _free(lnk);

    return (QUVI_OK);
}


