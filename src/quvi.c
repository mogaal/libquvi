/* quvi
 * Copyright (C) 2009,2010  Toni Gundogdu <legatvs@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

/* quvi.c - query video tool. */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

#include <curl/curl.h>

#include "platform.h"

#include "quvi/quvi.h"
#include "cmdline.h"

/* strepl.c */
extern char *strepl(const char *s, const char *what, const char *with);

static int verbose_flag = 1;

/* prints to std(e)rr. */
static void spew_e(const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
}

/* respects (q)uiet, prints to std(e)rr. */
static void spew_qe(const char *fmt, ...)
{
  va_list ap;
  if (!verbose_flag)
    return;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
}

/* glorified printf. */
static void spew(const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stdout, fmt, ap);
  va_end(ap);
}

typedef struct gengetopt_args_info opts_s;

static void handle_shortened_status(quvi_word type)
{
  if (type == QUVISTATUSTYPE_DONE)
    spew_qe("done.\n");
  else
    spew_qe(":: Check for shortened URL ...");
}

static void handle_fetch_status(quvi_word type, void *p)
{
  switch (type)
    {
    default:
      spew_qe(":: Fetch %s ...", (char *)p);
      break;
    case QUVISTATUSTYPE_CONFIG:
      spew_qe(":: Fetch config ...");
      break;
    case QUVISTATUSTYPE_PLAYLIST:
      spew_qe(":: Fetch playlist ...");
      break;
    case QUVISTATUSTYPE_DONE:
      spew_qe("done.\n");
      break;
    }
}

static void handle_verify_status(quvi_word type)
{
  switch (type)
    {
    default:
      spew_qe(":: Verify video link ...");
      break;
    case QUVISTATUSTYPE_DONE:
      spew_qe("done.\n");
      break;
    }
}

static int status_callback(long param, void *data)
{
  quvi_word status, type;

  status = quvi_loword(param);
  type = quvi_hiword(param);

  switch (status)
    {

    case QUVISTATUS_SHORTENED:
      handle_shortened_status(type);
      break;

    case QUVISTATUS_FETCH:
      handle_fetch_status(type, data);
      break;

    case QUVISTATUS_VERIFY:
      handle_verify_status(type);
      break;
    }

  fflush(stderr);

  return (0);
}

static size_t write_callback(void *p, size_t size, size_t nmemb,
                             void *data)
{
  size_t r = quvi_write_callback_default(p, size, nmemb, data);
  /* Could do something useful here. */
#ifdef _0
  puts(__func__);
#endif
  return r;
}

/* Divided into smaller blocks. Otherwise -pedantic objects. */

#define LICENSE_1 \
"/* quvi\n" \
" * Copyright (C) 2009,2010,2011  Toni Gundogdu <legatvs@gmail.com>\n"

#define LICENSE_2 \
" * This library is free software; you can redistribute it and/or\n" \
" * modify it under the terms of the GNU Lesser General Public\n" \
" * License as published by the Free Software Foundation; either\n" \
" * version 2.1 of the License, or (at your option) any later version.\n"

#define LICENSE_3 \
" * This library is distributed in the hope that it will be useful,\n" \
" * but WITHOUT ANY WARRANTY; without even the implied warranty of\n" \
" * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n" \
" * Lesser General Public License for more details.\n"

#define LICENSE_4 \
" * You should have received a copy of the GNU Lesser General Public\n" \
" * License along with this library; if not, write to the Free Software\n" \
" * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA\n" \
" * 02110-1301  USA\n" " */"

static void license(opts_s opts)
{
  printf("%s *\n%s *\n%s *\n%s\n", LICENSE_1, LICENSE_2, LICENSE_3,
         LICENSE_4);
  cmdline_parser_free(&opts);
  exit(0);
}

#undef LICENSE_4
#undef LICENSE_3
#undef LICENSE_2
#undef LICENSE_1

static void version(opts_s opts)
{
  printf("quvi version %s\n", quvi_version(QUVI_VERSION_LONG));
  cmdline_parser_free(&opts);
  exit(0);
}

static void dump_host(char *domain, char *formats)
{
  printf("%s\t%s\n", domain, formats);
  quvi_free(domain);
  quvi_free(formats);
}

/* Wraps quvi_supported. */
static void supported(quvi_t quvi, opts_s opts)
{
  QUVIcode rc;
  int i;

  for (i = 0; i < opts.inputs_num; ++i)
    {
      rc = quvi_supported(quvi, (char *)opts.inputs[i]);
      if (rc == QUVI_OK)
        spew_qe("%s: OK\n", (char *)opts.inputs[i]);
      else
        spew_qe("error: %s\n", quvi_strerror(quvi, rc));
    }

  quvi_close(&quvi);
  cmdline_parser_free(&opts);

  exit(rc);
}

static void format_help(quvi_t quvi, opts_s opts)
{
  int quit = 0;

  if (strcmp(opts.format_arg, "help") == 0)
    {
      printf("Usage:\n"
             "   --format arg            get format arg\n"
             "   --format list           list websites and supported formats\n"
             "   --format list arg       match arg to websites, list formats for matches\n"
             "Examples:\n"
             "   --format mp4_360p       get format mp4_360p (youtube)\n"
             "   --format list youtube   list youtube formats\n"
             "   --format list dailym    list dailym(otion) formats\n"
            );
      quit = 1;
    }
  else if (strcmp(opts.format_arg, "list") == 0)
    {
      int done = 0;
      char *d, *f;

      while(!done)
        {
          const int rc = quvi_next_supported_website(quvi, &d, &f);
          switch (rc)
            {
            case QUVI_OK:
            {
              int print = 1;
              /* -f list <pattern> */
              if (opts.inputs_num > 0)
                print = strstr(d, (char *)opts.inputs[0]) != 0;
              /* -f list */
              if (print)
                printf("%s:\n  %s\n\n", d, f);
              quvi_free(d);
              quvi_free(f);
            }
            break;
            case QUVI_LAST:
              done = 1;
              break;
            default:
              spew_e("%s\n", quvi_strerror(quvi, rc));
              break;
            }
        }
      quit = 1;
    }

  if (quit)
    {
      quvi_close(&quvi);
      cmdline_parser_free(&opts);
      exit(0);
    }
}

/* dumps all supported hosts to stdout. */
static void support(quvi_t quvi, opts_s opts)
{
  int done = 0;

  if (opts.inputs_num > 0)
    supported(quvi, opts);

  while (!done)
    {
      char *domain, *formats;
      QUVIcode rc;

      rc = quvi_next_supported_website(quvi, &domain, &formats);

      switch (rc)
        {
        case QUVI_OK:
          dump_host(domain, formats);
          break;
        case QUVI_LAST:
          done = 1;
          break;
        default:
          spew_e("%s\n", quvi_strerror(quvi, rc));
          break;
        }
    }

  quvi_close(&quvi);
  cmdline_parser_free(&opts);

  exit(0);
}

static void invoke_exec(quvi_video_t video, const char *video_url,
                        opts_s opts)
{
  char *quoted_url, *arg;
  int rc;

  asprintf(&quoted_url, "\"%s\"", video_url);

  arg = strdup(opts.exec_arg);
  arg = strepl(arg, "%u", quoted_url);

  free(quoted_url);
  quoted_url = NULL;

  rc = system(arg);

  switch (rc)
    {
    case 0:
      break;
    case -1:
      spew_e("error: failed to execute `%s'\n", arg);
      break;
    default:
      spew_e("error: child exited with: %d\n", rc >> 8);
      break;
    }

  free(arg);
  arg = NULL;
}

static void
dump_video_link_xml(CURL * curl,
                    int i,
                    char *video_url,
                    double file_length, char *file_ct,
                    char *file_suffix)
{
  char *url;

  url = curl_easy_escape(curl, video_url, 0);

  spew("   <link id=\"%d\">\n"
       "       <length_bytes>%.0f</length_bytes>\n"
       "       <content_type>%s</content_type>\n"
       "       <file_suffix>%s</file_suffix>\n"
       "       <url>%s</url>\n"
       "   </link>\n",
       i, file_length, file_ct, file_suffix, url ? url : video_url);

  if (url)
    {
      curl_free(url);
      url = NULL;
    }
}

static void
dump_video_link_old(int i,
                    char *video_url,
                    double file_length, char *file_suffix,
                    char *file_ct)
{
  spew("link %02d  : %s\n"
       ":: length: %.0f\n:: suffix: %s\n:: content-type: %s\n\n",
       i, video_url, file_length, file_suffix, file_ct);
}

static void
dump_video_link_json(int i,
                     char *video_url,
                     double file_length, char *file_suffix,
                     char *file_ct)
{
  spew("    {\n"
       "      \"id\": \"%d\",\n"
       "      \"length_bytes\": \"%.0f\",\n"
       "      \"content_type\": \"%s\",\n"
       "      \"file_suffix\": \"%s\",\n"
       "      \"url\": \"%s\"\n"
       "    }%s\n",
       i, file_length, file_ct, file_suffix, video_url,
       i > 1 ? "," : "");
}

static void dump_video_links(quvi_video_t video, opts_s opts,
                             CURL * curl)
{
  int i = 0;
  do
    {
      char *video_url, *file_suffix, *file_ct;
      double file_length;

      quvi_getprop(video, QUVIPROP_VIDEOURL, &video_url);
      quvi_getprop(video, QUVIPROP_VIDEOFILECONTENTTYPE, &file_ct);
      quvi_getprop(video, QUVIPROP_VIDEOFILESUFFIX, &file_suffix);
      quvi_getprop(video, QUVIPROP_VIDEOFILELENGTH, &file_length);

      ++i;

      if (opts.xml_given)
        dump_video_link_xml(curl, i, video_url, file_length,
                            file_ct, file_suffix);

      else if (opts.old_given)
        dump_video_link_old(i, video_url, file_length, file_suffix,
                            file_ct);
      else
        dump_video_link_json(i, video_url, file_length, file_suffix,
                             file_ct);
    }
  while (quvi_next_videolink(video) == QUVI_OK);
}

static void
dump_video_xml(CURL * curl,
               char *video_id, char *host, char *format,
               char *page_title, char *page_link)
{
  char *url;

  url = curl_easy_escape(curl, page_link, 0);

  spew("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
       "<video id=\"%s\" host=\"%s\">\n"
       "   <format_requested>%s</format_requested>\n"
       "   <page_title>%s</page_title>\n"
       "   <page_url>%s</page_url>\n",
       video_id, host, format, page_title, url ? url : page_link);

  if (url)
    {
      curl_free(url);
      url = NULL;
    }

}

static void
dump_video_old(char *video_id, char *host, char *format,
               char *page_title, char *page_link)
{
  spew(" > Dump video:\n"
       "host    : %s\n"
       "url     : %s\n"
       "title   : %s\n"
       "id      : %s\n"
       "format  : %s (requested)\n",
       host, page_link, page_title, video_id, format);
}

static void
dump_video_json(char *video_id, char *host, char *format,
                char *page_title, char *page_link)
{
  char *t;

  t = strdup(page_title);
  t = strepl(t, "\"", "\\\"");

  spew("{\n"
       "  \"host\": \"%s\",\n"
       "  \"page_title\": \"%s\",\n"
       "  \"page_url\": \"%s\",\n"
       "  \"id\": \"%s\",\n"
       "  \"format_requested\": \"%s\",\n"
       "  \"link\": [\n", host, t, page_link, video_id, format);

  free(t);
  t = NULL;
}

static void dump_video(quvi_video_t video, opts_s opts, CURL * curl)
{
  char *page_link, *page_title, *video_id, *format, *host;

  quvi_getprop(video, QUVIPROP_HOSTID, &host);
  quvi_getprop(video, QUVIPROP_PAGEURL, &page_link);
  quvi_getprop(video, QUVIPROP_PAGETITLE, &page_title);
  quvi_getprop(video, QUVIPROP_VIDEOID, &video_id);
  quvi_getprop(video, QUVIPROP_VIDEOFORMAT, &format);

  if (opts.xml_given)
    dump_video_xml(curl, video_id, host, format, page_title, page_link);
  else if (opts.old_given)
    dump_video_old(video_id, host, format, page_title, page_link);
  else
    dump_video_json(video_id, host, format, page_title, page_link);

  dump_video_links(video, opts, curl);

  if (opts.xml_given)
    spew("</video>\n");
  else if (opts.old_given) ;
  else
    spew("  ]\n}\n");
}

static void
expect_error(const char *what, const char *expected, const char *got)
{
  fprintf(stderr, "error: %s:\n  expected: \"%s\"\n  got: \"%s\"\n\n",
          what, expected, got);
}

static void
expect_error_d(const char *what, const double expected,
               const double got)
{
  fprintf(stderr,
          "error: %s:\n  expected: \"%.0f\"\n  got: \"%.0f\"\n\n", what,
          expected, got);
}

static int check_values(quvi_video_t video, opts_s opts)
{
  char *title, *expect, *id, *suffix;
  double length;
  int rc;

  rc = 0;

  if (opts.page_title_given)
    {

      expect = opts.page_title_arg;

      quvi_getprop(video, QUVIPROP_PAGETITLE, &title);

      rc = strcmp(expect, title) != 0;
      if (rc)
        expect_error("page title", expect, title);
    }

  if (opts.video_id_given && !rc)
    {

      expect = opts.video_id_arg;

      quvi_getprop(video, QUVIPROP_VIDEOID, &id);

      rc = strcmp(expect, id) != 0;
      if (rc)
        expect_error("video id", expect, id);
    }

  if (opts.file_suffix_given && !rc)
    {

      expect = opts.file_suffix_arg;

      quvi_getprop(video, QUVIPROP_VIDEOFILESUFFIX, &suffix);

      rc = strcmp(expect, suffix) != 0;
      if (rc)
        expect_error("file suffix", expect, suffix);
    }

  if (opts.file_length_given && !rc)
    {

      double expect_d = opts.file_length_arg;

      quvi_getprop(video, QUVIPROP_VIDEOFILELENGTH, &length);

      rc = expect_d != length;
      if (rc)
        expect_error_d("file length", expect_d, length);
    }

  return (rc);
}

static const char *tests[] =
{
  "http://www.tagtele.com/videos/voir/64096",
  "http://www.tmunderground.com/watch/4f88b3a8e3d44c636cbd/The-Intermediate",
  "http://www.publicsenat.fr/vod/un-monde-de-bulles/speciale-journal-de-spirou/67141",
  "http://video.globo.com/Videos/Player/Noticias/0,,GIM1392245-7823-QUATRO+MEDICOS+SAO+PRESOS+POR+VENDER+E+USAR+PRODUTOS+FALSOS+NO+RIO,00.html",
  "http://www.cbsnews.com/video/watch/?id=7118769n",
  "http://videos.sapo.pt/hd4ZBIHG80zFviLc5YEa",
  "http://www.dailymotion.com/video/xdpig1_city-of-scars_shortfilms",
  "http://www.spiegel.de/video/video-1012582.html",
  "http://vimeo.com/1485507",
  "http://en.sevenload.com/videos/IUL3gda-Funny-Football-Clips",
  "http://www.liveleak.com/view?i=704_1228511265",
  "http://video.google.com/videoplay?docid=-6970952080219955808",
  "http://video.golem.de/internet/2174/firefox-3.5-test.html",
  "http://www.funnyhub.com/videos/pages/crazy-hole-in-one.html",
  "http://www.clipfish.de/video/3100131/matratzendomino/",
  "http://www.youtube.com/watch?v=9dgSa4wmMzk",
  "http://break.com/index/beach-tackle-whip-lash.html",
  "http://www.gaskrank.tv/tv/rennstrecken/1-runde-oschersleben-14082008--6985.htm",
  "http://www.buzzhumor.com/videos/32561/Girl_Feels_Shotgun_Power",
  "http://www.funnyordie.com/videos/776d200b1c/etiquette-ninjas-episode-5-dicks-on-elevators",
  "http://www.charlierose.com/view/interview/11125",
  "http://www.academicearth.org/lectures/building-dynamic-websites-http",
  /* uses "redirect". */
  "http://www.academicearth.org/lectures/intro-roman-architecture",
  "http://www.collegehumor.com/video:1942317",
  "http://www.theonion.com/video/time-announces-new-version-of-magazine-aimed-at-ad,17950/",
  "http://www.bloomberg.com/video/63722844/",
#ifdef ENABLE_NSFW
  "http://xhamster.com/movies/391958/perfect_babe_dancing.html",
  "http://www.keezmovies.com/video/evelyn-s",
  "http://www.tube8.com/fetish/japanese-melon-gal-censored/186133/",
  "http://www.xvideos.com/video243887/devi_emmerson_body_painting",
  "http://www.youjizz.com/videos/glamour-girls---melissa-125602.html",
#endif
#ifdef ENABLE_BROKEN
#endif

  NULL
};

static void dump_error(quvi_t quvi, QUVIcode rc, opts_s opts)
{
  if (rc != QUVI_NOSUPPORT)
    fprintf(stderr, "\n");

  fprintf(stderr, "error: %s\n", quvi_strerror(quvi, rc));

  if (!opts.test_all_given)
    {
      quvi_close(&quvi);
      cmdline_parser_free(&opts);
      exit(rc);
    }
}

static void match_test(quvi_t quvi, opts_s opts, CURL * curl)
{
  int i, no_match;
  QUVIcode rc;

  /* None of our tests (should) use shortened URLs. */
  quvi_setopt(quvi, QUVIOPT_NOSHORTENED, 1L);

  for (rc = QUVI_OK, no_match = 1, i = 0; tests[i]; ++i)
    {
      if (strstr(tests[i], opts.test_arg) != NULL)
        {
          quvi_video_t v;

          no_match = 0;
          rc = quvi_parse(quvi, (char *)tests[i], &v);
          if (rc != QUVI_OK)
            dump_error(quvi, rc, opts);

          dump_video(v, opts, curl);
          rc = check_values(v, opts);

          if (opts.exec_given && rc == QUVI_OK)
            {
              char *video_url = NULL;
              do
                {
                  quvi_getprop(v, QUVIPROP_VIDEOURL, &video_url);
                  invoke_exec(v, video_url, opts);
                }
              while (quvi_next_videolink(v) == QUVI_OK);
            }

          quvi_parse_close(&v);
        }
    }

  if (no_match)
    {
      fprintf(stderr, "error: nothing matched `%s'\n", opts.test_arg);
      rc = QUVI_NOSUPPORT;
    }

  cmdline_parser_free(&opts);
  quvi_close(&quvi);

  exit(rc);
}

static void test_all(quvi_t quvi, opts_s opts, CURL * curl)
{
  quvi_video_t video;
  QUVIcode rc;
  int i, m;

  /* None of our tests (should) use shortened URLs. */
  quvi_setopt(quvi, QUVIOPT_NOSHORTENED, 1L);

  spew_qe(":: Run built-in video link tests.\n");
  for (m = 0; tests[m]; ++m) ;

  for (i = 0; i < m; ++i)
    {
      spew_qe(" > Test #%02d/%02d:\n", i + 1, m);

      rc = quvi_parse(quvi, (char *)tests[i], &video);
      if (rc != QUVI_OK)
        dump_error(quvi, rc, opts);
      else
        {
          if (opts.dump_given)
            dump_video(video, opts, curl);
        }
      quvi_parse_close(&video);
    }
  spew_qe(":: Tests done.\n");
}

static quvi_t init_quvi(opts_s opts, CURL ** curl)
{
  QUVIcode rc;
  quvi_t quvi;

  if ((rc = quvi_init(&quvi)) != QUVI_OK)
    dump_error(quvi, rc, opts);
  assert(quvi != 0);

  /* Set quvi options. */

  if (opts.format_given)
    quvi_setopt(quvi, QUVIOPT_FORMAT, opts.format_arg);

  quvi_setopt(quvi, QUVIOPT_NOSHORTENED, opts.no_shortened_given);
  quvi_setopt(quvi, QUVIOPT_NOVERIFY, opts.no_verify_given);

  if (opts.category_all_given)
    quvi_setopt(quvi, QUVIOPT_CATEGORY, QUVIPROTO_ALL);
  else
    {
      long n = 0;
      if (opts.category_http_given)
        n |= QUVIPROTO_HTTP;
      if (opts.category_mms_given)
        n |= QUVIPROTO_MMS;
      if (opts.category_rtsp_given)
        n |= QUVIPROTO_RTSP;
      if (opts.category_rtmp_given)
        n |= QUVIPROTO_RTMP;
      if (n > 0)
        quvi_setopt(quvi, QUVIOPT_CATEGORY, n);
    }

  quvi_setopt(quvi, QUVIOPT_STATUSFUNCTION, status_callback);
  quvi_setopt(quvi, QUVIOPT_WRITEFUNCTION, write_callback);

  /* Use the quvi created cURL handle. */

  quvi_getinfo(quvi, QUVIINFO_CURL, curl);
  assert(*curl != 0);

  if (opts.agent_given)
    curl_easy_setopt(*curl, CURLOPT_USERAGENT, opts.agent_arg);

  if (opts.proxy_given)
    curl_easy_setopt(*curl, CURLOPT_PROXY, opts.proxy_arg);

  if (opts.no_proxy_given)
    curl_easy_setopt(*curl, CURLOPT_PROXY, "");

  curl_easy_setopt(*curl, CURLOPT_VERBOSE, opts.verbose_libcurl_given);

  curl_easy_setopt(*curl, CURLOPT_CONNECTTIMEOUT,
                   opts.connect_timeout_arg);

  return (quvi);
}

int main(int argc, char *argv[])
{
  const char *url, *home, *no_config, *fname;
  quvi_video_t video;
  int no_config_flag;
  opts_s opts;
  QUVIcode rc;
  quvi_t quvi;
  CURL *curl;
  int i;

  curl = NULL;
  url = NULL;

  no_config = getenv("QUVI_NO_CONFIG");

  home = getenv("QUVI_HOME");
  if (!home)
    home = getenv("HOME");

  no_config_flag = 1;

#ifndef HOST_W32
  fname = "/.quvirc";
#else
  fname = "\\quvirc";
#endif

  /* Init cmdline parser. */

  if (home && !no_config)
    {
      char *path;
      FILE *f;

      asprintf(&path, "%s%s", home, fname);
      f = fopen(path, "r");

      if (f != NULL)
        {
          struct cmdline_parser_params *pp;

          fclose(f);
          f = NULL;

          pp = cmdline_parser_params_create();
          pp->check_required = 0;

          if (cmdline_parser_config_file(path, &opts, pp) == 0)
            {
              pp->initialize = 0;
              pp->override = 1;
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

  if (no_config_flag)
    {
      if (cmdline_parser(argc, argv, &opts) != 0)
        return (QUVI_INVARG);
    }

  if (opts.version_given)
    version(opts);

  if (opts.license_given)
    license(opts);

  verbose_flag = !opts.quiet_given;

  quvi = init_quvi(opts, &curl);

  if (opts.support_given)
    support(quvi, opts);

  if (opts.format_given)
    format_help(quvi, opts);

  /* User input */

  if (opts.test_all_given)
    test_all(quvi, opts, curl);

  else if (opts.test_given)
    match_test(quvi, opts, curl);

  else
    {
      if (opts.inputs_num == 0)
        fprintf(stderr, "error: no input links\n");

      for (i = 0; i < opts.inputs_num; ++i)
        {

          rc = quvi_parse(quvi, (char *)opts.inputs[i], &video);

          if (rc != QUVI_OK)
            dump_error(quvi, rc, opts);

          assert(video != 0);
          dump_video(video, opts, curl);

          if (opts.exec_given)
            {
              char *video_url = NULL;
              do
                {
                  quvi_getprop(video, QUVIPROP_VIDEOURL, &video_url);
                  invoke_exec(video, video_url, opts);
                }
              while (quvi_next_videolink(video) == QUVI_OK);
            }

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

/* vim: set ts=2 sw=2 tw=72 expandtab: */
