/* quvi
 * Copyright (C) 2009-2011  Toni Gundogdu <legatvs@gmail.com>
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

/* quvi.c - query media tool. */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include <curl/curl.h>

#include "platform.h"

#include "quvi/quvi.h"
#include "quvi/llst.h"

#include "cmdline.h"

#define _free(p) \
    do { if (p) { free(p); p=0; } } while (0)

/* strepl.c */
extern char *strepl(const char *s, const char *what, const char *with);

static int verbose_flag = 1;
static quvi_t quvi = NULL;
static CURL *curl = NULL;

typedef struct gengetopt_args_info *opts_t;
static opts_t opts = NULL;

static quvi_llst_node_t inputs = NULL;

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

static void dump_error(quvi_t quvi, QUVIcode rc)
{
  fprintf(stderr, "error: %s\n", quvi_strerror(quvi, rc));
}

static void handle_resolve_status(quvi_word type)
{
  if (type == QUVISTATUSTYPE_DONE)
    spew_qe("done.\n");
  else
    spew_qe(":: Check for URL redirection ...");
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
      spew_qe(":: Verify media URL ...");
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
    case QUVISTATUS_RESOLVE:
      handle_resolve_status(type);
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

static void license()
{
  printf("%s *\n%s *\n%s *\n%s\n",
         LICENSE_1, LICENSE_2, LICENSE_3, LICENSE_4);
  exit(0);
}

#undef LICENSE_4
#undef LICENSE_3
#undef LICENSE_2
#undef LICENSE_1

static void version()
{
  printf("quvi version %s\n", quvi_version(QUVI_VERSION_LONG));
  exit(0);
}

static void dump_host(char *domain, char *formats)
{
  printf("%s\t%s\n", domain, formats);
  quvi_free(domain);
  quvi_free(formats);
}

/* Wraps quvi_supported_ident. */
static void supported(quvi_t quvi)
{
#ifndef _QUVI_SUPPORTED
  quvi_ident_t ident;
  char *formats;
#endif
  QUVIcode rc;
  int i;

  rc = QUVI_NOSUPPORT;

  for (i=0; i<opts->inputs_num; ++i)
    {
      rc = quvi_supported_ident(quvi, (char*)opts->inputs[i], &ident);
      if (rc == QUVI_OK)
        {
          quvi_ident_getprop(ident, QUVI_IDENT_PROPERTY_FORMATS, &formats);
          spew("%10s : %s\n", formats, (char *)opts->inputs[i]);
          quvi_supported_ident_close(&ident);
        }
      else
        dump_error(quvi,rc);
    }

  exit(rc);
}

static const char depr_msg[] =
  "Warning:\n"
  "   '--format list' is deprecated and will be removed in\n"
  "   quvi 0.2.20. Use --query-formats instead.";

static void format_help(quvi_t quvi)
{
  int quit = 0;

  if (strcmp(opts->format_arg, "help") == 0)
    {
      printf(
        "Usage:\n"
        "   --format arg                get format arg of media\n"
        "   --format list               print domains with formats\n"
        "   --format list arg           match arg to supported domain names\n"
        "Examples:\n"
        "   --format list youtube       print youtube formats\n"
        "   --format fmt34_360p         get format fmt34_360p of media\n"
        "%s\n",
        depr_msg);
      quit = 1;
    }

  else if (strcmp(opts->format_arg, "list") == 0)
    {
      int done = 0;
      char *d, *f;

      while(!done)
        {
          const int rc =
            quvi_next_supported_website(quvi, &d, &f);

          switch (rc)
            {

            case QUVI_OK:
            {
              int print = 1;

              /* -f list <pattern> */
              if (opts->inputs_num > 0)
                print = strstr(d, (char *)opts->inputs[0]) != 0;

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
      spew_qe("%s\n", depr_msg);
      quit = 1;
    }

  if (quit)
    exit(0);
}

/* Query which formats are available for the URL */
static void query_formats(quvi_t quvi)
{
  QUVIcode rc;
  int i;

  if (opts->inputs_num < 1)
    {
      spew_qe("error: no input URLs\n");
      exit (QUVI_INVARG);
    }

  for (i=0; i<opts->inputs_num; ++i)
    {
      char *formats = NULL;

      rc = quvi_query_formats(quvi, (char*)opts->inputs[i], &formats);
      if (rc == QUVI_OK)
        {
          spew("%10s : %s\n", formats, opts->inputs[i]);
          quvi_free(formats);
        }
      else
        dump_error(quvi,rc);
    }

  exit(rc);
}

/* dumps all supported hosts to stdout. */
static void support(quvi_t quvi)
{
  int done = 0;

  if (opts->inputs_num > 0)
    supported(quvi);

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

  exit(0);
}

static void invoke_exec(quvi_media_t media)
{
  char *cmd, *media_url,  *q_media_url;
  int rc;

  quvi_getprop(media, QUVIPROP_MEDIAURL, &media_url);

  asprintf(&q_media_url, "\"%s\"", media_url);

  cmd = strdup(opts->exec_arg);
  cmd = strepl(cmd, "%u", q_media_url);

  _free(q_media_url);

  rc = system(cmd);

  switch (rc)
    {
    case 0:
      break;
    case -1:
      spew_e("error: failed to execute `%s'\n", cmd);
      break;
    default:
      spew_e("error: child exited with: %d\n", rc >> 8);
      break;
    }

  _free(cmd);
}

struct parsed_url_s
{
  char *media_url;
  char *content_type;
  double content_length;
  char *file_suffix;
};

typedef struct parsed_url_s *parsed_url_t;

static void dump_media_url_xml(parsed_url_t p, int i)
{
  char *media_url = curl_easy_escape(curl, p->media_url, 0);

  spew("   <link id=\"%d\">\n", i);

  if (p->content_length)
    spew("       <length_bytes>%.0f</length_bytes>\n", p->content_length);

  if (strlen(p->content_type))
    spew("       <content_type>%s</content_type>\n", p->content_type);

  if (strlen(p->file_suffix))
    spew("       <file_suffix>%s</file_suffix>\n", p->file_suffix);

  spew("       <url>%s</url>\n"
       "   </link>\n",
       media_url ? media_url : p->media_url);

  _free(media_url);
}

static void dump_media_url_old(parsed_url_t p, int i)
{
  spew("link %02d  : %s\n", i, p->media_url);

  if (p->content_length)
    spew(":: length: %.0f\n", p->content_length);

  if (strlen(p->file_suffix))
    spew(":: suffix: %s\n", p->file_suffix);

  if (strlen(p->content_type))
    spew(":: content-type: %s\n", p->content_type);
}

static void dump_media_url_json(parsed_url_t p, int i, int prepend_newln)
{
  if (prepend_newln)
    spew(",\n");

  spew("    {\n"
       "      \"id\": \"%d\",\n", i);

  if (p->content_length)
    spew("      \"length_bytes\": \"%.0f\",\n", p->content_length);

  if (strlen(p->content_type))
    spew("      \"content_type\": \"%s\",\n", p->content_type);

  if (strlen(p->file_suffix))
    spew("      \"file_suffix\": \"%s\",\n", p->file_suffix);

  spew("      \"url\": \"%s\"\n"
       "    }",
       p->media_url);
}

static void dump_media_urls(quvi_media_t media)
{
  int json_flag=0, i=1;
  do
    {
      struct parsed_url_s p;

      memset(&p, 0, sizeof(&p));

      quvi_getprop(media, QUVIPROP_MEDIAURL, &p.media_url);
      quvi_getprop(media, QUVIPROP_MEDIACONTENTTYPE, &p.content_type);
      quvi_getprop(media, QUVIPROP_MEDIACONTENTLENGTH, &p.content_length);
      quvi_getprop(media, QUVIPROP_FILESUFFIX, &p.file_suffix);

      if (opts->xml_given)
        dump_media_url_xml(&p,i);
      else if (opts->old_given)
        dump_media_url_old(&p,i);
      else
        {
          dump_media_url_json(&p, i, i>1);
          json_flag = 1;
        }
      ++i;
    }
  while (quvi_next_media_url(media) == QUVI_OK);

  if (json_flag)
    spew("\n");
}

struct parsed_s
{
  char *page_url;
  char *page_title;
  char *media_id;
  char *format;
  char *host;
  char *start_time;
  char *thumb_url;
  double duration;
};

typedef struct parsed_s *parsed_t;

static void dump_media_xml(parsed_t p)
{
  char *e_page_url, *e_thumb_url;

  e_page_url = curl_easy_escape(curl, p->page_url, 0);
  e_thumb_url = curl_easy_escape(curl, p->thumb_url, 0);

  spew("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
       "<media id=\"%s\" host=\"%s\">\n"
       "   <format_requested>%s</format_requested>\n"
       "   <page_title>%s</page_title>\n"
       "   <page_url>%s</page_url>\n",
       p->media_id,
       p->host,
       p->format,
       p->page_title,
       e_page_url ? e_page_url : "");

  if (strlen(p->start_time))
    spew("   <start_time>%s</start_time>\n", p->start_time);

  if (e_thumb_url && strlen(e_thumb_url))
    spew("   <thumbnail_url>%s</thumbnail_url>\n", e_thumb_url);

  if (p->duration)
    spew("   <duration>%.0f</duration>\n", p->duration);

  if (e_page_url)
    {
      curl_free(e_page_url);
      e_page_url = NULL;
    }

  if (e_thumb_url)
    {
      curl_free(e_thumb_url);
      e_thumb_url = NULL;
    }
}

static void dump_media_old(parsed_t p)
{
  spew(" > Dump media:\n"
       "host    : %s\n"
       "url     : %s\n"
       "title   : %s\n"
       "id      : %s\n"
       "format  : %s (requested)\n",
       p->host,
       p->page_url,
       p->page_title,
       p->media_id,
       p->format);

  if (strlen(p->start_time))
    spew("start time: %s\n", p->start_time);

  if (strlen(p->thumb_url))
    spew("thumbnail url: %s\n", p->thumb_url);

  if (p->duration)
    spew("duration: %.0f\n", p->duration);
}

static void dump_media_json(parsed_t p)
{
  char *t;

  t = strdup(p->page_title);
  t = strepl(t, "\"", "\\\"");

  spew("{\n"
       "  \"host\": \"%s\",\n"
       "  \"page_title\": \"%s\",\n"
       "  \"page_url\": \"%s\",\n"
       "  \"id\": \"%s\",\n"
       "  \"format_requested\": \"%s\",\n",
       p->host,
       t,
       p->page_url,
       p->media_id,
       p->format);

  if (strlen(p->start_time))
    spew("  \"start_time\": \"%s\",\n", p->start_time);

  if (strlen(p->thumb_url))
    spew("  \"thumbnail_url\": \"%s\",\n", p->thumb_url);

  if (p->duration)
    spew("  \"duration\": \"%.0f\",\n", p->duration);

  spew("  \"link\": [\n");

  _free(t);
}

static void dump_media(quvi_media_t media)
{
  struct parsed_s p;

  memset(&p, 0, sizeof(p));

  quvi_getprop(media, QUVIPROP_HOSTID, &p.host);
  quvi_getprop(media, QUVIPROP_PAGEURL, &p.page_url);
  quvi_getprop(media, QUVIPROP_PAGETITLE, &p.page_title);
  quvi_getprop(media, QUVIPROP_MEDIAID, &p.media_id);
  quvi_getprop(media, QUVIPROP_FORMAT, &p.format);
  quvi_getprop(media, QUVIPROP_STARTTIME, &p.start_time);
  quvi_getprop(media, QUVIPROP_MEDIATHUMBNAILURL, &p.thumb_url);
  quvi_getprop(media, QUVIPROP_MEDIADURATION, &p.duration);

  if (opts->xml_given)
    dump_media_xml(&p);
  else if (opts->old_given)
    dump_media_old(&p);
  else
    dump_media_json(&p);

  dump_media_urls(media);

  if (opts->xml_given)
    spew("</media>\n");
  else if (opts->old_given) ;
  else
    spew("  ]\n}\n");
}

static quvi_t init_quvi()
{
  QUVIcode rc;
  quvi_t quvi;

  if ((rc = quvi_init(&quvi)) != QUVI_OK)
    {
      dump_error(quvi, rc);
      exit(rc);
    }
  assert(quvi != 0);

  /* Set quvi options. */

  if (opts->format_given)
    quvi_setopt(quvi, QUVIOPT_FORMAT, opts->format_arg);

  if (opts->no_shortened_given)
    {
      spew_e("warning: --no-shortened is deprecated, "
             "use --no-resolve instead\n");
    }
  quvi_setopt(quvi, QUVIOPT_NORESOLVE, opts->no_resolve_given);
  quvi_setopt(quvi, QUVIOPT_NOVERIFY, opts->no_verify_given);

  if (opts->category_all_given)
    quvi_setopt(quvi, QUVIOPT_CATEGORY, QUVIPROTO_ALL);
  else
    {
      long n = 0;
      if (opts->category_http_given)
        n |= QUVIPROTO_HTTP;
      if (opts->category_mms_given)
        n |= QUVIPROTO_MMS;
      if (opts->category_rtsp_given)
        n |= QUVIPROTO_RTSP;
      if (opts->category_rtmp_given)
        n |= QUVIPROTO_RTMP;
      if (n > 0)
        quvi_setopt(quvi, QUVIOPT_CATEGORY, n);
    }

  quvi_setopt(quvi, QUVIOPT_STATUSFUNCTION, status_callback);
  quvi_setopt(quvi, QUVIOPT_WRITEFUNCTION, write_callback);

  /* Use the quvi created cURL handle. */

  quvi_getinfo(quvi, QUVIINFO_CURL, &curl);
  assert(curl != 0);

  if (opts->agent_given)
    curl_easy_setopt(curl, CURLOPT_USERAGENT, opts->agent_arg);

  if (opts->proxy_given)
    curl_easy_setopt(curl, CURLOPT_PROXY, opts->proxy_arg);

  if (opts->no_proxy_given)
    curl_easy_setopt(curl, CURLOPT_PROXY, "");

  curl_easy_setopt(curl, CURLOPT_VERBOSE, opts->verbose_libcurl_given);

  curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT,
                   opts->connect_timeout_arg);

  return (quvi);
}

static void cleanup()
{
  quvi_llst_free(&inputs);
  assert(inputs == NULL);

  if (quvi)
    quvi_close(&quvi);
  assert(quvi == NULL);

  if (opts)
    {
      cmdline_parser_free(opts);
      _free(opts);
    }
  assert(opts == NULL);
}

static void read_from(FILE *f, int close)
{
  char b[256];

  if (!f)
    return;

  while (fgets(b, sizeof(b), f))
    {
      if (strlen(b) > 16)
        {
          const size_t n = strlen(b)-1;

          if (b[n] == '\n')
            b[n] = '\0';

          quvi_llst_append(&inputs, strdup(b));
        }
    }

  if (close)
    {
      fclose(f);
      f = NULL;
    }
}

static char *parse_url_scheme(const char *url)
{
  char *p, *r;

  p = strstr(url, ":/");
  if (!p)
    return (NULL);

  asprintf(&r, "%.*s", (int)(p - url), url);

  return (r);
}

static int is_url(const char *s)
{
  char *p = parse_url_scheme(s);
  if (p)
    {
      _free(p);
      return (1);
    }
  return (0);
}

static FILE* open_file(const char *path)
{
  FILE *f = fopen(path, "rt");
  if (!f)
#ifdef HAVE_STRERROR
    spew_e("error: %s: %s\n", path, strerror(errno));
#else
    perror("fopen");
#endif
  return (f);
}

static int read_input()
{
  if (opts->inputs_num == 0)
    read_from(stdin, 0);
  else
    {
      int i;
      for (i=0; i<opts->inputs_num; ++i)
        {
          if (!is_url(opts->inputs[i]))
            read_from(open_file(opts->inputs[i]), 1);
          else /* Must be an URL. */
            quvi_llst_append(&inputs, strdup(opts->inputs[i]));
        }
    }
  return (quvi_llst_size(inputs));
}

int main(int argc, char *argv[])
{
  const char *home, *no_config, *fname;
  QUVIcode rc, last_failure;
  quvi_llst_node_t curr;
  quvi_media_t media;
  int no_config_flag;
  int i, inputs_num;
  int errors;

  assert(quvi == NULL);
  assert(curl == NULL);
  assert(opts == NULL);
  assert(inputs == NULL);

  no_config = getenv("QUVI_NO_CONFIG");
  no_config_flag = 1;

  home = getenv("QUVI_HOME");
  if (!home)
    home = getenv("HOME");

#ifndef HOST_W32
  fname = "/.quvirc";
#else
  fname = "\\quvirc";
#endif

  atexit(cleanup);

  opts = calloc(1, sizeof(struct gengetopt_args_info));
  if (!opts)
    return(QUVI_MEM);

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

          if (cmdline_parser_config_file(path, opts, pp) == 0)
            {
              pp->initialize = 0;
              pp->override = 1;
              pp->check_required = 1;

              if (cmdline_parser_ext(argc, argv, opts, pp) == 0)
                no_config_flag = 0;
            }
          _free(pp);
        }

      _free(path);
    }

  if (no_config_flag)
    {
      if (cmdline_parser(argc, argv, opts) != 0)
        return (QUVI_INVARG);
    }

  if (opts->version_given)
    version(opts);

  if (opts->license_given)
    license(opts);

  verbose_flag = !opts->quiet_given;

  quvi = init_quvi();

  if (opts->query_formats_given)
    query_formats(quvi);

  if (opts->support_given)
    support(quvi);

  if (opts->format_given)
    format_help(quvi);

  /* User input */

  inputs_num = read_input();

  if (inputs_num == 0)
    {
      spew_qe("error: no input URLs\n");
      return (QUVI_INVARG);
    }

  last_failure = QUVI_OK;
  errors = 0;

  for (i=0, curr=inputs; curr; ++i)
    {
      char *url = quvi_llst_data(curr);
      rc = quvi_parse(quvi, url, &media);
      if (rc == QUVI_OK)
        {
          assert(media != 0);
          dump_media(media);

          if (opts->exec_given)
            {
              do
                {
                  invoke_exec(media);
                }
              while (quvi_next_media_url(media) == QUVI_OK);
            }
        }
      else
        {
          dump_error(quvi,rc);
          last_failure = rc;
          ++errors;
        }
      quvi_parse_close(&media);
      assert(media == 0);
      curr = quvi_llst_next(curr);
    }

  if (inputs_num > 1)
    {
      spew_qe("Results: %d OK, %d failed (last 0x%02x), exit with 0x%02x\n",
              inputs_num - errors, errors, last_failure, rc);
    }

  return (rc);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
