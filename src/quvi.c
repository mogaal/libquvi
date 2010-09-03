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

/*
* quvi.c - query video tool.
*/

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

#include <curl/curl.h>
#include <pcre.h>

#include "platform.h"

#include "quvi/quvi.h"
#include "cmdline.h"

static int verbose_flag = 1;

static void
spew (const char *fmt, ...) {
    va_list ap;

    if (!verbose_flag)
        return;

    va_start (ap, fmt);
    vfprintf (stderr, fmt, ap);
    va_end (ap);
}

static void
spew_e (const char *fmt, ...) {
    va_list ap;
    va_start (ap, fmt);
    vfprintf (stdout, fmt, ap);
    va_end (ap);
}

typedef struct gengetopt_args_info opts_s;

static int
status_callback(long param, void *data) {
    quvi_word status,type;

    status = quvi_loword(param);
    type  = quvi_hiword(param);

    switch (status) {
    case QUVISTATUS_FETCH: /* handle fetch */
        switch (type) {
        default: spew(":: Fetch %s ...", (char *)data); break;
        case QUVISTATUSTYPE_CONFIG  : spew(":: Fetch config ..."); break;
        case QUVISTATUSTYPE_PLAYLIST: spew(":: Fetch playlist ..."); break;
        case QUVISTATUSTYPE_DONE    : spew("done.\n"); break;
        }
        break;

    case QUVISTATUS_VERIFY: /* handle verify */
        switch (type) {
        default: spew(":: Verify video link ..."); break;
        case QUVISTATUSTYPE_DONE: spew("done.\n"); break;
        }
        break;
    }

    fflush (stderr);

    return (0);
}

static size_t
write_callback (void *p, size_t size, size_t nmemb, void *data) {
    size_t r = quvi_write_callback_default(p, size, nmemb, data);
    /* Could do something useful here. */
#ifdef _1_
    puts(__func__);
#endif
    return r;
}

static const char notice[] =
"Copyright (C) 2009,2010 Toni Gundogdu. License: GNU GPL version  3  or later\n"
"This is free software; see the  source for  copying conditions.  There is NO\n"
"warranty;  not even for MERCHANTABILITY or FITNESS FOR A  PARTICULAR PURPOSE.";

static void
license (opts_s opts) {
    printf("%s\n",notice);
    cmdline_parser_free(&opts);
    exit (0);
}

static void
version (opts_s opts) {
    printf("quvi, version %s\n", quvi_version(QUVI_VERSION_LONG));
    cmdline_parser_free(&opts);
    exit (0);
}

static void
supports(quvi_t quvi, opts_s opts) {
    int done = 0;

    while (!done) {
        char *domain, *formats;
        QUVIcode rc = quvi_next_supported_website (quvi, &domain, &formats);

        switch (rc) {
        case QUVI_OK:
            printf("%s\t%s\n", domain, formats);
            quvi_free(domain);
            quvi_free(formats);
            break;
        case QUVI_LAST: done = 1; break;
        default       : fprintf(stderr, "%s\n", quvi_strerror(quvi, rc)); break;
        }
    }

    quvi_close(&quvi);
    cmdline_parser_free(&opts);

    exit (0);
}

static void
dump_video_links (quvi_video_t video, opts_s opts, CURL *curl) {
    const int xml = opts.xml_given;
    int i = 0;
    do {
        char *video_url, *file_suffix, *file_ct;
        double file_length;

        quvi_getprop(video, QUVIPROP_VIDEOURL, &video_url);
        quvi_getprop(video, QUVIPROP_VIDEOFILECONTENTTYPE, &file_ct);
        quvi_getprop(video, QUVIPROP_VIDEOFILESUFFIX, &file_suffix);
        quvi_getprop(video, QUVIPROP_VIDEOFILELENGTH, &file_length);

        if (!xml) {
            spew_e (
                "link %02d  : %s\n"
                ":: length: %.0f\n:: suffix: %s\n:: content-type: %s\n\n",
                ++i, video_url, file_length, file_suffix, file_ct
            );
        }
        else {
            char *ct  = curl_easy_escape (curl, file_ct, 0);
            char *url = curl_easy_escape (curl, video_url, 0);
            spew_e (
                "   <link id=\"%d\">\n"
                "       <length_bytes>%.0f</length_bytes>\n"
                "       <content_type>%s</content_type>\n"
                "       <file_suffix>%s</file_suffix>\n"
                "       <url>%s</url>\n"
                "   </link>\n",
                ++i, file_length, ct, file_suffix, url
            );
            curl_free (url);
            url = NULL;
            curl_free (ct);
            ct = NULL;
        }
    } while (quvi_next_videolink(video) == QUVI_OK);
}

static void
dump_video(quvi_video_t video, opts_s opts, CURL *curl) {
    char *page_link, *page_title, *video_id, *format;
#ifdef _1_
    long httpcode;
#endif

    quvi_getprop(video, QUVIPROP_PAGEURL, &page_link);
    quvi_getprop(video, QUVIPROP_PAGETITLE, &page_title);
    quvi_getprop(video, QUVIPROP_VIDEOID, &video_id);
    quvi_getprop(video, QUVIPROP_VIDEOFORMAT, &format);
#ifdef _1_
    quvi_getprop(video, QUVIPROP_HTTPCODE, &httpcode);
#endif

    if (!opts.xml_given) {
        spew_e (
            " > Dump video:\n"
            "url     : %s\n"
            "title   : %s\n"
            "id      : %s\n"
            "format  : %s (requested)\n",
            page_link, page_title, video_id, format
        );
    }
    else {
        char *id    = curl_easy_escape (curl, video_id, 0);
        char *title = curl_easy_escape (curl, page_title, 0);
        char *url   = curl_easy_escape (curl, page_link, 0);
        spew_e (
        "<?xml version='1.0' encoding='utf-8'?>\n"
        "<video id=\"%s\">\n"
        "   <format_requested>%s</format_requested>\n"
        "   <page_title>%s</page_title>\n"
        "   <page_url>%s</page_url>\n",
        id, format, title, url
        );
        curl_free (url);
        url = NULL;
        curl_free (title);
        title = NULL;
        curl_free (id);
        id = NULL;
    }

    dump_video_links (video, opts, curl);

    if (opts.xml_given)
        spew_e ("</video>\n");

#ifdef _0
    spew("httpcode: %ld (last)\n", httpcode);
#endif
}

static void
expect_error (const char *what, const char *expected, const char *got) {
    fprintf(stderr,
        "error: %s:\n  expected: \"%s\"\n  got: \"%s\"\n\n",
            what, expected, got);
}

static void
expect_error_d (const char *what, const double expected, const double got) {
    fprintf(stderr,
        "error: %s:\n  expected: \"%.0f\"\n  got: \"%.0f\"\n\n",
            what, expected, got);
}

static int
check_values(quvi_video_t video, opts_s opts) {
    int rc = 0;

    if (opts.page_title_given) {
        char *title, *expect=opts.page_title_arg;
        quvi_getprop(video, QUVIPROP_PAGETITLE, &title);
        rc = strcmp(expect, title) != 0;
        if (rc)
            expect_error("page title", expect, title);
    }

    if (opts.video_id_given && !rc) {
        char *id, *expect=opts.video_id_arg;
        quvi_getprop(video, QUVIPROP_VIDEOID, &id);
        rc = strcmp(expect, id) != 0;
        if (rc)
            expect_error("video id", expect, id);
    }

    if (opts.file_suffix_given && !rc) {
        char *suffix, *expect=opts.file_suffix_arg;
        quvi_getprop(video, QUVIPROP_VIDEOFILESUFFIX, &suffix);
        rc = strcmp(expect, suffix) != 0;
        if (rc)
            expect_error("file suffix", expect, suffix);
    }

    if (opts.file_length_given && !rc) {
        double length, expect=opts.file_length_arg;
        quvi_getprop(video, QUVIPROP_VIDEOFILELENGTH, &length);
        rc = expect != length;
        if (rc)
            expect_error_d("file length", expect, length);
    }

    return (rc);
}

static void
dump_error(quvi_t quvi, QUVIcode rc, opts_s opts) {
    long httpcode, curlcode;

    quvi_getinfo(quvi, QUVIINFO_HTTPCODE, &httpcode);
    quvi_getinfo(quvi, QUVIINFO_CURLCODE, &curlcode);

    fprintf(stderr, "\nerror: %s (http/%ld, curl/%ld)\n",
        quvi_strerror(quvi,rc),
        httpcode, curlcode);

    if (!opts.test_all_given) {
        quvi_close(&quvi);
        cmdline_parser_free(&opts);
        exit (rc);
    }
}

static const char *tests[] = {
"http://www.dailymotion.com/hd/video/x9fkzj_battlefield-1943-coral-sea-trailer_videogames",
"http://www.spiegel.de/video/video-1012582.html",
"http://vimeo.com/1485507",
"http://en.sevenload.com/videos/IUL3gda-Funny-Football-Clips",
"http://www.myubo.com/page/media_detail.html?movieid=1308f0fb-47c6-40c5-a6f9-1324dac12896",
"http://www.liveleak.com/view?i=704_1228511265",
"http://video.google.com/videoplay?docid=-8669127848070159803",
"http://video.golem.de/internet/2174/firefox-3.5-test.html",
"http://www.funnyhub.com/videos/pages/crazy-hole-in-one.html",
"http://www.clipfish.de/video/3100131/matratzendomino/",
"http://www.youtube.com/watch?v=DeWsZ2b_pK4",
"http://break.com/index/beach-tackle-whip-lash.html",
"http://www.evisor.tv/tv/rennstrecken/1-runde-oschersleben-14082008--6985.htm",
"http://www.buzzhumor.com/videos/32561/Girl_Feels_Shotgun_Power",
"http://www.funnyordie.com/videos/776d200b1c/etiquette-ninjas-episode-5-dicks-on-elevators",
"http://www.charlierose.com/view/interview/11125",
"http://www.theonion.com/video/time-announces-new-version-of-magazine-aimed-at-ad,17950/",

#ifdef ENABLE_BROKEN
"http://space.tv.cctv.com/video/VIDE1212909276513233", /* single-segment */
"http://space.tv.cctv.com/video/VIDE1247468077860061", /* multi-segment */
#endif

#ifdef ENABLE_SMUT
"http://www.tube8.com/fetish/japanese-melon-gal-censored/186133/",
"http://www.xvideos.com/video243887/devi_emmerson_body_painting",
"http://www.youjizz.com/videos/glamour-girls---melissa-125602.html",
#endif

NULL
};

static void
match_test (quvi_t quvi, opts_s opts, CURL *curl) {
    unsigned register int i;
    const char *errmsg;
    int err_offset, rc;

    for (i=0; tests[i]; ++i) {
        pcre *re;

        re = pcre_compile(
            opts.test_arg,
            PCRE_CASELESS,
            &errmsg,
            &err_offset,
            NULL
        );

        if (!re) {
            fprintf(stderr, "%s\n", errmsg);
            cmdline_parser_free(&opts);
            exit (QUVI_PCRE);
        }

        rc = pcre_exec(
            re,
            0,
            tests[i],
            strlen(tests[i]),
            0,
            0,
            0,
            0
        );

        pcre_free(re);

        if (rc >= 0) /* match */ {
            quvi_video_t v;
            QUVIcode _rc;

            _rc = quvi_parse(quvi, (char *)tests[i], &v);
            if (_rc != QUVI_OK)
                dump_error(quvi, _rc, opts);

            dump_video (v, opts, curl);
            rc = check_values(v, opts);
            quvi_parse_close(&v);

            cmdline_parser_free(&opts);
            quvi_close(&quvi);
            exit (rc);
        }
        else if (rc == PCRE_ERROR_NOMATCH) {
            continue;
        }
        else {
            fprintf(stderr, "error: pcre_exec: rc = %d\n", rc);
            cmdline_parser_free(&opts);
            exit (QUVI_PCRE);
        }
    }

    fprintf(stderr, "error: nothing matched `%s'\n", opts.test_arg);
    cmdline_parser_free(&opts);
    quvi_close(&quvi);
    exit (QUVI_PCRE);
}

static void
test_all (quvi_t quvi, opts_s opts, CURL *curl) {
    register unsigned int i,m;
    quvi_video_t video;
    QUVIcode rc;

    spew(":: Run built-in video link tests.\n");
    for (m=0; tests[m]; ++m);

    for (i=0; i<m; ++i) {
        spew(" > Test #%02d/%02d:\n", i+1, m);

        rc = quvi_parse(quvi, (char *)tests[i], &video);
        if (rc != QUVI_OK)
            dump_error(quvi, rc, opts);
        else {
            if (opts.dump_given)
                dump_video (video, opts, curl);
        }
        quvi_parse_close(&video);
    }
    spew(":: Tests done.\n");
}

static quvi_t
init_quvi(opts_s opts, CURL **curl) {
    QUVIcode rc;
    quvi_t quvi;

    if ( (rc = quvi_init(&quvi)) != QUVI_OK )
        dump_error(quvi, rc, opts);
    assert(quvi != 0);

    /* Set quvi options. */

    if (opts.format_given)
        quvi_setopt(quvi, QUVIOPT_FORMAT, opts.format_arg);

    if (opts.no_verify_given)
        quvi_setopt(quvi, QUVIOPT_NOVERIFY, 1L);

    quvi_setopt(quvi, QUVIOPT_STATUSFUNCTION, status_callback);
    quvi_setopt(quvi, QUVIOPT_WRITEFUNCTION, write_callback);

    /* We can use the quvi created cURL handle for our means. */

    quvi_getinfo(quvi, QUVIINFO_CURL, curl);
    assert(*curl != 0);

    if (opts.agent_given)
        curl_easy_setopt (*curl, CURLOPT_USERAGENT, opts.agent_arg);

    if (opts.proxy_given)
        curl_easy_setopt (*curl, CURLOPT_PROXY, opts.proxy_arg);

    if (opts.debug_given)
        curl_easy_setopt (*curl, CURLOPT_VERBOSE, 1L);

    curl_easy_setopt (*curl,
        CURLOPT_CONNECTTIMEOUT, opts.connect_timeout_arg);

    return (quvi);
}

int
main (int argc, char *argv[]) {
    const char *url, *home, *no_config, *fname;
    register unsigned int i;
    quvi_video_t video;
    int no_config_flag;
    opts_s opts;
    QUVIcode rc;
    quvi_t quvi;
    CURL *curl;

    curl      = NULL;
    url       = NULL;
    no_config = getenv("QUVI_NO_CONFIG");
    home      = getenv("QUVI_HOME");
    if (!home)
        home = getenv("HOME");
    no_config_flag   = 1;

#ifndef HOST_W32
    fname = "/.quvirc";
#else
    fname = "\\quvirc";
#endif

    /* Init cmdline parser. */

    if (home && !no_config) {
        char *path;
        FILE *f;

        asprintf(&path, "%s%s", home, fname);
        f = fopen(path, "r");

        if (f != NULL) {
            struct cmdline_parser_params *pp;

            fclose(f);
            f = NULL;

            pp = cmdline_parser_params_create();
            pp->check_required = 0;

            if (cmdline_parser_config_file(path, &opts, pp) == 0) {
                pp->initialize     = 0;
                pp->override       = 1;
                pp->check_required = 1;

                if (cmdline_parser_ext(argc, argv, &opts, pp) == 0)
                    no_config_flag = 0;
            }
            free(pp);
            pp = NULL;
        }

        free(path);
        path = NULL;
    }

    if (no_config_flag) {
        if (cmdline_parser(argc, argv, &opts) != 0)
            return (QUVI_INVARG);
    }

    /* Run and forget options. */

    if (opts.version_given)
        version(opts);

    if (opts.license_given)
        license(opts);

    /* Other */

    verbose_flag = !opts.quiet_given;

    /* Init quvi. */

    quvi = init_quvi (opts, &curl);

    /* --hosts */
    if (opts.hosts_given)
        supports(quvi, opts);

    /* Handle input. */
    if (opts.test_all_given)
        test_all (quvi, opts, curl);
    else if (opts.test_given)
        match_test(quvi, opts, curl);
    else
    {
        if (opts.inputs_num == 0)
            fprintf(stderr, "error: no input links\n");

        for (i=0; i<opts.inputs_num; ++i)
        {
            if ( (rc = quvi_parse(
                    quvi,
                    (char *)opts.inputs[i],
                    &video))
                != QUVI_OK )
            {
                dump_error(quvi, rc, opts);
            }

            assert(video != 0);
            dump_video(video, opts, curl);

            quvi_parse_close(&video);
            assert(video == 0);
        }
    }

    /* Cleanup. */

    quvi_close(&quvi);
    assert(quvi == 0);

    cmdline_parser_free(&opts);

    return (QUVI_OK);
}


