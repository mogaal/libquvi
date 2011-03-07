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

#include "config.h"

#include <stdlib.h>
#include <memory.h>
#include <assert.h>

#include <curl/curl.h>

#include "quvi/quvi.h"
#include "internal.h"
#include "util.h"
#include "lua_wrap.h"
#include "curl_wrap.h"

static void *_realloc(void *p, const size_t size)
{
  if (p)
    return realloc(p, size);
  return malloc(size);
}

struct mem_s
{
  size_t size;
  char *p;
};

size_t
quvi_write_callback_default(void *p, size_t size, size_t nmemb,
                            void *data)
{
  struct mem_s *m = (struct mem_s *)data;
  const size_t rsize = size * nmemb;
  void *tmp = _realloc(m->p, m->size + rsize + 1);
  if (tmp)
    {
      m->p = (char *)tmp;
      memcpy(&(m->p[m->size]), p, rsize);
      m->size += rsize;
      m->p[m->size] = '\0';
    }
  return (rsize);
}

/* lua_wrap.c */
extern char *lua_get_field_s(lua_State *, const char *);

QUVIcode
fetch_to_mem(_quvi_video_t video, const char *url, lua_State * l,
             char **dst)
{
  char *fetch_type, *arbitrary_cookie, *user_agent;
  QUVIstatusType fetch_type_n;
  long respcode, conncode;
  CURLcode curlcode;
  struct mem_s mem;
  QUVIcode rc;

  if (!video)
    return (QUVI_BADHANDLE);

  if (!dst)
    return (QUVI_INVARG);

  *dst = NULL;
  fetch_type = NULL;
  user_agent = NULL;
  arbitrary_cookie = NULL;
  fetch_type_n = QUVISTATUSTYPE_PAGE;   /* default */

  /* Additional settings from LUA table */

  if (lua_istable(l, 2))
    {
      fetch_type = lua_get_field_s(l, "fetch_type");
      if (fetch_type)
        {
          if (strcmp(fetch_type, "config") == 0)
            fetch_type_n = QUVISTATUSTYPE_CONFIG;
          else if (strcmp(fetch_type, "playlist") == 0)
            fetch_type_n = QUVISTATUSTYPE_PLAYLIST;
        }
      arbitrary_cookie = lua_get_field_s(l, "arbitrary_cookie");
      user_agent = lua_get_field_s(l, "user_agent");
    }

  if (video->quvi->status_func)
    {
      if (video->quvi->
          status_func(makelong(QUVISTATUS_FETCH, fetch_type_n),
                      (void *)url) != QUVI_OK)
        {
          return (QUVI_ABORTEDBYCALLBACK);
        }
    }

  curl_easy_setopt(video->quvi->curl, CURLOPT_URL, url);
  curl_easy_setopt(video->quvi->curl, CURLOPT_ENCODING, "");

  memset(&mem, 0, sizeof(mem));
  curl_easy_setopt(video->quvi->curl, CURLOPT_WRITEDATA, &mem);

  curl_easy_setopt(video->quvi->curl, CURLOPT_WRITEFUNCTION,
                   video->quvi->write_func
                   ? (curl_write_callback) video->quvi->write_func
                   : (curl_write_callback) quvi_write_callback_default);

  if (arbitrary_cookie != NULL && *arbitrary_cookie != '\0')
    {
      curl_easy_setopt(video->quvi->curl, CURLOPT_COOKIE,
                       arbitrary_cookie);
    }

  if (user_agent != NULL && *user_agent != '\0')
    {
      curl_easy_setopt(video->quvi->curl, CURLOPT_USERAGENT, user_agent);
    }

  curlcode = curl_easy_perform(video->quvi->curl);
  respcode = 0;
  conncode = 0;
  rc = QUVI_OK;

  curl_easy_getinfo(video->quvi->curl, CURLINFO_RESPONSE_CODE,
                    &respcode);
  curl_easy_getinfo(video->quvi->curl, CURLINFO_HTTP_CONNECTCODE,
                    &conncode);

  if (curlcode == CURLE_OK && respcode == 200)
    {

      if (video->quvi->status_func)
        {

          if (video->quvi->status_func(makelong(QUVISTATUS_FETCH,
                                                QUVISTATUSTYPE_DONE),
                                       0) != QUVI_OK)
            {
              rc = QUVI_ABORTEDBYCALLBACK;
            }

        }

    }

  else
    {

      if (curlcode == CURLE_OK)
        {
          freprintf(&video->quvi->errmsg,
                    "server response code %ld (conncode=%ld)", respcode,
                    conncode);
        }

      else
        {
          freprintf(&video->quvi->errmsg,
                    "%s (curlcode=%d, conncode=%ld)",
                    curl_easy_strerror(curlcode), curlcode, conncode);
        }

      rc = QUVI_CURL;
    }

  if (mem.p)
    {
      *dst = mem.p;
      if (rc == QUVI_OK && !video->charset)       /* charset */
        run_lua_charset_func(video, mem.p);
    }

  video->quvi->httpcode = respcode;
  video->quvi->curlcode = curlcode;

  return (rc);
}

QUVIcode query_file_length(_quvi_t quvi, llst_node_t lnk)
{
  static const char *scheme = "http://";
  long respcode, conncode;
  _quvi_video_link_t qvl;
  CURLcode curlcode;
  struct mem_s mem;
  QUVIcode rc;
  char buf[8];

  if (!quvi)
    return (QUVI_BADHANDLE);

  if (!lnk)
    return (QUVI_BADHANDLE);

  qvl = (_quvi_video_link_t) lnk->data;
  assert(qvl != NULL);

  if (!qvl)
    return (QUVI_BADHANDLE);

  qvl->url = from_html_entities(qvl->url);

  /* We can currently check HTTP URLs only */
  memset(&buf, 0, sizeof(buf));
  if (strcmp(strncpy(buf, qvl->url, strlen(scheme)), scheme) != 0)
    return (QUVI_OK);           /* Skip video URL verification discreetly. */

  if (quvi->status_func)
    {
      if (quvi->status_func(makelong(QUVISTATUS_VERIFY, 0), 0) != QUVI_OK)
        {
          return (QUVI_ABORTEDBYCALLBACK);
        }
    }

  curl_easy_setopt(quvi->curl, CURLOPT_URL, qvl->url);
  curl_easy_setopt(quvi->curl, CURLOPT_NOBODY, 1L);     /* get -> head */

  memset(&mem, 0, sizeof(mem));
  curl_easy_setopt(quvi->curl, CURLOPT_WRITEDATA, &mem);

  curl_easy_setopt(quvi->curl, CURLOPT_WRITEFUNCTION, (quvi->write_func)
                   ? (curl_write_callback) quvi->write_func
                   : (curl_write_callback) quvi_write_callback_default);

  curlcode = curl_easy_perform(quvi->curl);

  curl_easy_setopt(quvi->curl, CURLOPT_HTTPGET, 1L);    /* reset: head -> get */

  respcode = 0;
  conncode = 0;
  rc = QUVI_OK;

  curl_easy_getinfo(quvi->curl, CURLINFO_RESPONSE_CODE, &respcode);
  curl_easy_getinfo(quvi->curl, CURLINFO_HTTP_CONNECTCODE, &conncode);

  if (curlcode == CURLE_OK)
    {

      if (respcode == 200 || respcode == 206)
        {

          const char *ct;

          curl_easy_getinfo(quvi->curl, CURLINFO_CONTENT_TYPE, &ct);

          _free(qvl->content_type);
          asprintf(&qvl->content_type, "%s", ct);

          curl_easy_getinfo(quvi->curl,
                            CURLINFO_CONTENT_LENGTH_DOWNLOAD, &qvl->length);

          if (quvi->status_func)
            {
              if (quvi->status_func(makelong
                                    (QUVISTATUS_VERIFY, QUVISTATUSTYPE_DONE),
                                    0) != QUVI_OK)
                {
                  rc = QUVI_ABORTEDBYCALLBACK;
                }
            }

          if (rc == QUVI_OK)
            {
              /* Content-Type -> suffix. */
              rc = run_lua_suffix_func(quvi, qvl);
            }
        }
      else
        {
          freprintf(&quvi->errmsg,
                    "server response code %ld (conncode=%ld)", respcode,
                    conncode);
          rc = QUVI_CURL;
        }
    }
  else
    {
      freprintf(&quvi->errmsg, "%s (curlcode=%d, conncode=%ld)",
                curl_easy_strerror(curlcode), curlcode, conncode);
      rc = QUVI_CURL;
    }

  quvi->httpcode = respcode;
  quvi->curlcode = curlcode;

  if (mem.p)
    _free(mem.p);

  return (rc);
}

/**
 * Check if URL is a shortened URL (e.g. dai.ly, goo.gl, etc.) and
 * replace the (shortened) video page URL with the redirect URL.
 */
QUVIcode is_shortened_url(_quvi_video_t video)
{
  long respcode, conncode;
  CURLcode curlcode;
  struct mem_s mem;
  QUVIcode rc;

  assert(video != NULL);

  if (video->quvi->status_func)
    {
      if (video->quvi->status_func(QUVISTATUS_SHORTENED, 0) != QUVI_OK)
        return (QUVI_ABORTEDBYCALLBACK);
    }

  memset(&mem, 0, sizeof(mem));
  curl_easy_setopt(video->quvi->curl, CURLOPT_WRITEDATA, &mem);

  curl_easy_setopt(video->quvi->curl, CURLOPT_WRITEFUNCTION,
                   (video->quvi->write_func)
                   ? (curl_write_callback) video->quvi->write_func
                   : (curl_write_callback) quvi_write_callback_default);

  curl_easy_setopt(video->quvi->curl, CURLOPT_URL, video->page_link);
  curl_easy_setopt(video->quvi->curl, CURLOPT_FOLLOWLOCATION, 0L);
  curl_easy_setopt(video->quvi->curl, CURLOPT_NOBODY, 1L);      /* get -> head */

  curlcode = curl_easy_perform(video->quvi->curl);

  curl_easy_setopt(video->quvi->curl, CURLOPT_FOLLOWLOCATION, 1L);      /* reset */

  curl_easy_setopt(video->quvi->curl, CURLOPT_HTTPGET, 1L);     /* reset: head -> get */

  respcode = 0;
  conncode = 0;
  rc = QUVI_OK;

  curl_easy_getinfo(video->quvi->curl, CURLINFO_RESPONSE_CODE,
                    &respcode);
  curl_easy_getinfo(video->quvi->curl, CURLINFO_HTTP_CONNECTCODE,
                    &conncode);

  if (curlcode == CURLE_OK)
    {

      if (respcode >= 301 && respcode <= 303)
        {

          /* A redirect. */

          char *url = NULL;

          curl_easy_getinfo(video->quvi->curl, CURLINFO_REDIRECT_URL, &url);
          freprintf(&video->page_link, "%s", url);

          rc = QUVI_OK;

        }
      /* respcode >= 301 && respcode <= 303 */
      else
        {
          /* Most likely not a shortened URL redirect. Let it pass. */
          rc = QUVI_OK;
        }

      if (video->quvi->status_func)
        {

          const long param =
            makelong(QUVISTATUS_SHORTENED, QUVISTATUSTYPE_DONE);

          rc = video->quvi->status_func(param, 0);

        }

    }
  else
    {

      freprintf(&video->quvi->errmsg, "%s (curlcode=%d, conncode=%ld)",
                curl_easy_strerror(curlcode), curlcode, conncode);

      rc = QUVI_CURL;
    }

  if (mem.p)
    _free(mem.p);

  video->quvi->httpcode = respcode;
  video->quvi->curlcode = curlcode;

  return (rc);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
