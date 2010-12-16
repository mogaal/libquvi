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

#include <assert.h>
#include <string.h>

#include <curl/curl.h>

#include "quvi/quvi.h"
#include "internal.h"
#include "lua_wrap.h"
#include "util.h"
#include "curl_wrap.h"

#define is_invarg(p) \
    do { if (p == NULL) return (QUVI_INVARG); } while (0)

#define is_badhandle(h) \
    do { if (h == NULL) return (QUVI_BADHANDLE); } while (0)

/* quvi_init */

QUVIcode quvi_init(quvi_t * dst)
{
  _quvi_t quvi;

  is_invarg(dst);
  *dst = 0;

  quvi = calloc(1, sizeof(*quvi));
  if (!quvi)
    return (QUVI_MEM);

  *dst = (quvi_t) quvi;

#ifndef HOST_W32
  curl_global_init(CURL_GLOBAL_NOTHING);
#else
  curl_global_init(CURL_GLOBAL_WIN32);
#endif

  quvi->curl = curl_easy_init();
  if (!quvi->curl) {
    _free(quvi);
    return (QUVI_CURLINIT);
  }

  /* set defaults */
  quvi_setopt(quvi, QUVIOPT_FORMAT, "default");

  csetopt(CURLOPT_USERAGENT, "Mozilla/5.0");
  csetopt(CURLOPT_FOLLOWLOCATION, 1L);
  csetopt(CURLOPT_NOBODY, 0L);

  return (init_lua(quvi));
}

/* quvi_close */

void quvi_close(quvi_t * handle)
{
  _quvi_t *quvi;

  quvi = (_quvi_t *) handle;

  if (quvi && *quvi) {

    free_lua(quvi);
    assert((*quvi)->util_scripts == NULL);
    assert((*quvi)->website_scripts == NULL);

    _free((*quvi)->format);
    assert((*quvi)->format == NULL);

    _free((*quvi)->errmsg);
    assert((*quvi)->errmsg == NULL);

    curl_easy_cleanup((*quvi)->curl);
    (*quvi)->curl = NULL;

    _free(*quvi);
    assert((*quvi) == NULL);

    curl_global_cleanup();
  }
}

/* quvi_supported */

QUVIcode quvi_supported(quvi_t quvi, char *url)
{
  _quvi_video_t video;
  QUVIcode rc;

  is_invarg(url);
  is_badhandle(quvi);

  video = calloc(1, sizeof(*video));
  if (!video)
    return (QUVI_MEM);

  video->quvi = quvi;

  setvid(video->page_link, "%s", url);

  rc = find_host_script(video);

  quvi_parse_close((quvi_video_t) & video);

  return (rc);
}

/* quvi_parse */

QUVIcode quvi_parse(quvi_t quvi, char *url, quvi_video_t * dst)
{
  _quvi_video_t video;
  QUVIcode rc;

  is_invarg(dst);
  *dst = 0;
  is_invarg(url);
  is_badhandle(quvi);

  video = calloc(1, sizeof(*video));
  if (!video)
    return (QUVI_MEM);

  *dst = video;
  video->quvi = quvi;

  setvid(video->page_link, "%s", url);

  if (!video->quvi->no_shortened) {
    rc = is_shortened_url(video);
    if (rc != QUVI_OK)
      return (rc);
  }

  while (1) {
    rc = find_host_script_and_parse(video);
    if (rc != QUVI_OK)
      return (rc);
    else {
      if (strlen(video->redirect)) {    /* Found a redirect. */
        setvid(video->page_link, "%s", video->redirect);
        continue;
      } else
        break;
    }
  }

#ifdef HAVE_ICONV               /* Convert character set encoding to utf8. */
  if (video->charset)
    to_utf8(video);
#endif
  assert(video->title != NULL); /* must be set in the lua script */

  video->title = from_html_entities(video->title);

  if (!video->quvi->no_verify) {
    llst_node_t curr = video->link;
    while (curr) {
      rc = query_file_length(video->quvi, curr);
      if (rc != QUVI_OK)
        break;
      curr = curr->next;
    }
  }

  /* set current video link to first link in the list. */
  video->curr = video->link;

  return (rc);
}

/* quvi_parse_close */

void quvi_parse_close(quvi_video_t * handle)
{
  _quvi_video_t *video;

  video = (_quvi_video_t *) handle;

  if (video && *video) {
    llst_node_t curr = (*video)->link;

    while (curr) {
      _quvi_video_link_t l = (_quvi_video_link_t) curr->data;
      _free(l->url);
      _free(l->suffix);
      _free(l->content_type);
      curr = curr->next;
    }
    llst_free(&(*video)->link);

    _free((*video)->id);
    _free((*video)->title);
    _free((*video)->charset);
    _free((*video)->page_link);
    _free((*video)->host_id);
    _free((*video)->redirect);
    _free((*video)->starttime);

    _free(*video);
  }
}

static QUVIcode _setopt(_quvi_t quvi, QUVIoption opt, va_list arg)
{

#define _sets(opt) \
    do { \
        _free(opt); \
        asprintf(&opt, "%s", va_arg(arg,char *)); \
    } while(0); break

#define _seti(opt) \
    do { opt = va_arg(arg,int); } while(0); break

  switch (opt) {
  case QUVIOPT_FORMAT:
    _sets(quvi->format);
  case QUVIOPT_NOVERIFY:
    _seti(quvi->no_verify);
  case QUVIOPT_STATUSFUNCTION:
    quvi->status_func = va_arg(arg, quvi_callback_status);
    break;
  case QUVIOPT_WRITEFUNCTION:
    quvi->write_func = va_arg(arg, quvi_callback_write);
    break;
  case QUVIOPT_NOSHORTENED:
    _seti(quvi->no_shortened);
  default:
    return (QUVI_INVARG);
  }
#undef _sets
#undef _seti
  return (QUVI_OK);
}

/* quvi_setopt */

QUVIcode quvi_setopt(quvi_t quvi, QUVIoption opt, ...)
{
  va_list arg;
  QUVIcode rc;

  is_badhandle(quvi);

  va_start(arg, opt);
  rc = _setopt(quvi, opt, arg);
  va_end(arg);

  return (rc);
}

static const char empty[] = "";

static QUVIcode _getprop(_quvi_video_t video, QUVIproperty prop, ...)
{
  _quvi_video_link_t qvl;
  QUVIcode rc;
  va_list arg;
  double *dp;
  char **sp;
  long *lp;
  int type;

  qvl = (_quvi_video_link_t) video->curr->data;
  assert(qvl != 0);

  rc = QUVI_OK;
  dp = 0;
  sp = 0;
  lp = 0;

  va_start(arg, prop);
  type = QUVIPROPERTY_TYPEMASK & (int)prop;

#define _initv(var,type) \
    do { \
        if ( !(var = va_arg(arg,type)) ) \
            rc = QUVI_INVARG; \
    } while (0); break

  switch (type) {
  case QUVIPROPERTY_DOUBLE:
    _initv(dp, double *);
  case QUVIPROPERTY_STRING:
    _initv(sp, char **);
  case QUVIPROPERTY_LONG:
    _initv(lp, long *);
  default:
    rc = QUVI_INVARG;
  }
  va_end(arg);

  if (rc != QUVI_OK)
    return rc;

#define _sets(with) \
    do { *sp = with ? with:(char*)empty; } while(0); break

#define _setn(var,with) \
    do { *var = with; } while(0); break

  switch (prop) {
  case QUVIPROP_HOSTID:
    _sets(video->host_id);
  case QUVIPROP_PAGEURL:
    _sets(video->page_link);
  case QUVIPROP_PAGETITLE:
    _sets(video->title);
  case QUVIPROP_VIDEOID:
    _sets(video->id);
  case QUVIPROP_VIDEOURL:
    _sets(qvl->url);
  case QUVIPROP_VIDEOFILELENGTH:
    _setn(dp, qvl->length);
  case QUVIPROP_VIDEOFILECONTENTTYPE:
    _sets(qvl->content_type);
  case QUVIPROP_VIDEOFILESUFFIX:
    _sets(qvl->suffix);
  case QUVIPROP_HTTPCODE:
    _setn(lp, video->quvi->httpcode);
  case QUVIPROP_VIDEOFORMAT:
    _sets(video->quvi->format);
  case QUVIPROP_STARTTIME:
    _sets(video->starttime);
  default:
    rc = QUVI_INVARG;
  }

  return (rc);
}

static QUVIcode _getinfo(_quvi_t quvi, QUVIinfo info, ...)
{
  QUVIcode rc;
  va_list arg;
  double *dp;
  char **sp;
  void **vp;
  long *lp;
  int type;

  rc = QUVI_OK;
  dp = 0;
  sp = 0;
  vp = 0;
  lp = 0;

  va_start(arg, info);
  type = QUVIINFO_TYPEMASK & (int)info;

  switch (type) {
  case QUVIINFO_DOUBLE:
    _initv(dp, double *);
  case QUVIINFO_STRING:
    _initv(sp, char **);
  case QUVIINFO_LONG:
    _initv(lp, long *);
  case QUVIINFO_VOID:
    _initv(vp, void **);
  default:
    rc = QUVI_INVARG;
  }
  va_end(arg);

  if (rc != QUVI_OK)
    return rc;

#define _setv(with) \
    do  { *vp = with ? with:NULL; } while(0); break

  switch (info) {
  case QUVIINFO_CURL:
    _setv(quvi->curl);
  case QUVIINFO_CURLCODE:
    _setn(lp, quvi->curlcode);
  case QUVIINFO_HTTPCODE:
    _setn(lp, quvi->httpcode);
  default:
    rc = QUVI_INVARG;
  }

  return (rc);
}

#undef _setv
#undef _setn
#undef _sets
#undef _initv

/* quvi_getinfo */

QUVIcode quvi_getinfo(quvi_t quvi, QUVIinfo info, ...)
{
  va_list arg;
  void *p;

  is_badhandle(quvi);

  va_start(arg, info);
  p = va_arg(arg, void *);
  va_end(arg);

  return (_getinfo(quvi, info, p));
}

/* quvi_getprop */

QUVIcode quvi_getprop(quvi_video_t video, QUVIproperty prop, ...)
{
  va_list arg;
  void *p;

  is_badhandle(video);

  va_start(arg, prop);
  p = va_arg(arg, void *);
  va_end(arg);

  return (_getprop(video, prop, p));
}

/* quvi_next_videolink */

QUVIcode quvi_next_videolink(quvi_video_t handle)
{
  _quvi_video_t video;

  is_badhandle(handle);

  video = (_quvi_video_t) handle;

  /* start from the first */
  if (!video->curr) {
    video->curr = video->link;
    return (QUVI_OK);
  }

  /* move to the next */
  video->curr = video->curr->next;
  if (!video->curr) {
    video->curr = video->link;  /* reset */
    return (QUVI_LAST);
  }

  return (QUVI_OK);
}

static llst_node_t curr_host = NULL;

/* quvi_next_supported_website */

QUVIcode
quvi_next_supported_website(quvi_t handle, char **domain, char **formats)
{
  struct lua_ident_s ident;
  _quvi_t quvi;
  QUVIcode rc;

  is_badhandle(handle);
  quvi = (_quvi_t) handle;

  is_invarg(domain);
  is_invarg(formats);

  if (!quvi->website_scripts)
    return (QUVI_NOLUAWEBSITE);

  if (!curr_host)
    curr_host = quvi->website_scripts;
  else {
    curr_host = curr_host->next;
    if (!curr_host)
      return (QUVI_LAST);
  }

  ident.quvi = quvi;
  ident.url = NULL;
  ident.domain = NULL;
  ident.formats = NULL;

  rc = run_ident_func(&ident, curr_host);

  *domain = ident.domain;
  *formats = ident.formats;

  /*
   * Scripts will return QUVI_NOSUPPORT because
   * of the "ident.url=0" above. This is OK since
   * we ignore the "will_handle" value altogether.
   */
  return (rc == QUVI_NOSUPPORT ? QUVI_OK : rc);
}

/* quvi_next_host, NOTE: deprecated. */

QUVIcode quvi_next_host(char **domain, char **formats)
{
  *domain = *formats = NULL;
  return (QUVI_LAST);
}

#undef is_badhandle
#undef is_invarg

/* quvi_strerror */

char *quvi_strerror(quvi_t handle, QUVIcode code)
{
  static const char *errormsgs[] = {
    "no error",
    "memory allocation failed",
    "bad handle argument to function",
    "invalid argument to function",
    "curl initialization failed",
    "end of list iteration",
    "aborted by callback",
    "lua initilization failed",
    "lua website scripts not found",
    "lua util scripts not found",
    "invalid error code (internal _INTERNAL_QUVI_LAST)"
  };

  _quvi_t quvi = (_quvi_t) handle;

  if (quvi) {
    if (code > _INTERNAL_QUVI_LAST)
      return (quvi->errmsg);
  } else {
    if (code > _INTERNAL_QUVI_LAST)
      code = _INTERNAL_QUVI_LAST;
  }

  return ((char *)errormsgs[code]);
}

/* quvi_version */

char *quvi_version(QUVIversion type)
{
  static const char version[] = PACKAGE_VERSION;
  static const char version_long[] =
#ifdef GIT_DESCRIBE
      GIT_DESCRIBE
#else
      PACKAGE_VERSION
#endif
#ifdef BUILD_DATE
      " built on " BUILD_DATE
#endif
      " for " CANONICAL_TARGET " ("
#ifdef HAVE_ICONV
      "i"
#endif
#ifdef ENABLE_BROKEN
      "b"
#endif
#ifdef ENABLE_NSFW
      "n"
#endif
      ")";

  if (type == QUVI_VERSION_LONG)
    return ((char *)version_long);
  return ((char *)version);
}

/* quvi_free */

void quvi_free(void *ptr)
{
  if (ptr != NULL)
    free(ptr);
}
