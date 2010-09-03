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

#include "config.h"

#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <assert.h>

#include <curl/curl.h>

#ifdef HAVE_ICONV
#include <iconv.h>
#endif

#include <pcre.h>

#include "quvi/quvi.h"
#include "internal.h"
#include "curl_wrap.h"
#include "util.h"

#ifdef COMPILE_UNUSED
QUVIcode
regexp_capture(
    _quvi_t quvi,
    const char *content,
    const char *regexp,
    int *dst_ovector,
    int ovector_count,
    ...)
{
    int ovector[128], i, count;
    const char *pcre_errmsg;
    const char **results;
    int error_offset;
    int pcre_code;
    char **dst;
    va_list ap;
    pcre *re;

    assert(content != 0);
    assert(regexp != 0);

    re = pcre_compile(
        regexp,
        0,
        &pcre_errmsg,
        &error_offset,
        0
    );

    if (!re) {
        seterr(pcre_errmsg);
        return (QUVI_PCRE);
    }

    pcre_code = pcre_exec(
        re,
        0,
        content,
        strlen(content),
        0,
        0,
        dst_ovector ? dst_ovector   : ovector,
        dst_ovector ? ovector_count : sizeof(ovector)/sizeof(int)
    );

    if (pcre_code == PCRE_ERROR_NOMATCH) {
        seterr("no match: %s", regexp);
        pcre_free(re);
        return (QUVI_PCRE);
    }
    else if (pcre_code < 0) {
        seterr("pcre_exec: pcre_code=%d", pcre_code);
        pcre_free(re);
        return (QUVI_PCRE);
    }

    count = pcre_code; /* string count */

    pcre_code = pcre_get_substring_list(
        content,
        dst_ovector ? dst_ovector : ovector,
        count,
        &results
    );

    pcre_free(re);

    if (pcre_code < 0) {
        seterr("pcre_get_substring_list: pcre_code=%d", pcre_code);
        return (QUVI_PCRE);
    }

    i = 1;
    va_start(ap, ovector_count);
    while ((dst = va_arg(ap, char **)) != 0
            && i < count)
    {
        *dst = strdup(results[i++]);
    }
    va_end(ap);

    pcre_free_substring_list(results);

    return (QUVI_OK);
}
#endif /* COMPILE_UNUSED */

#ifdef HAVE_ICONV
QUVIcode
to_utf8(_quvi_video_t video) {
    static const char to[] = "UTF-8";
    size_t insize, avail, iconv_code;
    char inbuf[1024], outbuf[1024];
    ICONV_CONST char *inptr;
    char *from, *wptr;
    iconv_t cd;
    _quvi_t quvi;

    assert(video != 0);
    assert(video->quvi != 0);
    assert(video->title != 0);
    assert(video->charset != 0);

    quvi    = video->quvi;
    wptr    = outbuf;
    inptr   = inbuf;
    avail   = sizeof(outbuf);
    insize  = strlen(video->title);

    if (insize >= sizeof(inbuf))
        insize = sizeof(inbuf);

    memset(wptr, 0, sizeof(outbuf));

    snprintf(inbuf, sizeof(inbuf),
        "%s", video->title);

    /* Try with TRANSLIT first. */
    asprintf(&from, "%s//TRANSLIT", video->charset);
    cd = iconv_open(to, from);

    /* If that fails, then without TRANSLIT. */
    if (cd == (iconv_t)-1) {
        _free(from);
        asprintf(&from, "%s", video->charset);
        cd = iconv_open(to, from);
    }

    if (cd == (iconv_t)-1) {
        if (errno == EINVAL)
            seterr("conversion from %s to %s unavailable", from, to);
        else {
#ifdef HAVE_STRERROR
            seterr("iconv_open: %s", strerror(errno));
#else
            perror("iconv_popen");
#endif
        }

        _free(from);

        return (QUVI_ICONV);
    }

    iconv_code = iconv(cd, &inptr, &insize, &wptr, &avail);
    iconv_close(cd);
    cd = 0;

    if (iconv_code == (size_t)-1) {
        seterr("converting characters from '%s' to '%s' failed", from, to);
        _free(from);
        return (QUVI_ICONV);
    }
    else
        setvid(video->title, "%s", outbuf);

    _free(from);

    return (QUVI_OK);
}
#endif

char *
unescape(_quvi_t quvi, char *s) {
    char *tmp, *ret;

    assert(quvi != 0);
    assert(quvi->curl != 0);

    tmp = curl_easy_unescape(quvi->curl, s, 0, NULL);
    assert(tmp != 0);
    ret = strdup(tmp);
    curl_free(tmp);

    free(s);

    return (ret);
}

int
is_format_supported(const char *fmt, const char *lst) {
    int pcre_code, error_offset;
    const char *pcre_errmsg;
    pcre *re;
    char *m;

    if (!strcmp(fmt, "best"))
        return (1);

    asprintf(&m, "\\b%s\\b", fmt);

    re = pcre_compile(
        m,
        PCRE_CASELESS,
        &pcre_errmsg,
        &error_offset,
        0
    );

    _free(m);

    if (!re) {
        /* Ignore errors. Return false (0), indicating
         * the specified format was not in the list. */
        return (0);
    }

    pcre_code = pcre_exec(
        re,
        0,
        lst,
        strlen(lst),
        0,
        0,
        0,
        0
    );

    pcre_free(re);

    return (pcre_code >= 0);
}

char *
from_html_entities(char *src) {
    struct lookup_s {
        const char *from;
        const char *to;
    };

    static const struct lookup_s conv[] = {
        {"&quot;", "\""},
        {"&#34;",  "\""},
        {"&amp;",  "&"},
        {"&#38;",  "&"},
        {"&apos;", "'"},
        {"&#39;",  "'"},
        {"&lt;",   "<"},
        {"&#60;",  "<"},
        {"&gt;",   ">"},
        {"&#62;",  ">"},
        {0,0}
    };

    int i;

    for (i=0; conv[i].from; ++i)
        src = strepl(src, conv[i].from, conv[i].to);

    return (src);
}

static int
new_video_link(_quvi_video_link_t *dst) {
    struct _quvi_video_link_s *qvl;

    qvl = calloc(1, sizeof(*qvl));
    if (!qvl)
        return (QUVI_MEM);

    *dst = qvl;

    return (QUVI_OK);
}

int
add_video_link(llst_node_t *lst, const char *fmt, ...) {
    _quvi_video_link_t qvl;
    va_list args;
    int rc;

    rc = new_video_link(&qvl);
    if (rc != QUVI_OK)
        return (rc);

    va_start(args, fmt);
    vasprintf((char **)&qvl->url, fmt, args);
    va_end(args);

    if (!qvl->url) {
        _free(qvl);
        return (QUVI_MEM);
    }

    return (llst_add(lst, qvl));
}


