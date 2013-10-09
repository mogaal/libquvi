/* libquvi
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

#include "config.h"

#include <assert.h>
#include <string.h>

#include "quvi/quvi.h"
#include "quvi/net.h"
#include "quvi/llst.h"

#include "internal.h"
#include "lua_wrap.h"
#include "curl_wrap.h"
#include "util.h"
#include "net_wrap.h"

/*
 * Title: quvi API
 *
 * See also <Overview>.
 */

/*
 * Function: quvi_init
 *
 * Creates a new session.
 *
 * Parameters:
 *  session - New session handle (receives)
 *
 * Returns:
 *  Non-zero value if an error occurred.
 *
 * See Also:
 *  * <quvi_close>
 *  * <quvi_strerror>
 *
 * Example:
 * (start code)
 * QUVIcode rc;
 * quvi_t q;
 * rc = quvi_init(&q);
 * if (rc != QUVI_OK)
 *   {
 *     fprintf(stderr, "%s\n", quvi_strerror(q,rc));
 *     return (rc);
 *   }
 * quvi_close(&q);
 * (end)
 */
QUVIcode quvi_init(quvi_t *session)
{
  _quvi_t quvi;
  QUVIcode rc;

  _is_invarg(session);
  *session = NULL;

  quvi = calloc(1, sizeof(*quvi));
  if (!quvi)
    return (QUVI_MEM);

  *session = (quvi_t) quvi;

  rc = curl_init(quvi);
  if (rc != QUVI_OK)
    {
      _free(quvi);
      return (rc);
    }

  /* Set quvi defaults. */
  quvi_setopt(quvi, QUVIOPT_FORMAT, "default");
  quvi_setopt(quvi, QUVIOPT_CATEGORY, QUVIPROTO_ALL);

  return (init_lua(quvi));
}

/*
 * Function: quvi_close
 *
 * Closes a previously started session.
 *
 * Parameters:
 *  session - Session handle
 *
 * See Also:
 *  <quvi_init>
 */
void quvi_close(quvi_t *session)
{
  _quvi_t *quvi = (_quvi_t*) session;

  if (quvi && *quvi)
    {
      free_lua(quvi);
      assert((*quvi)->util_scripts == NULL);
      assert((*quvi)->website_scripts == NULL);

      _free((*quvi)->format);
      assert((*quvi)->format == NULL);

      _free((*quvi)->errmsg);
      assert((*quvi)->errmsg == NULL);

      curl_close(*quvi);
      assert((*quvi)->curl == NULL);

      _free(*quvi);
      assert((*quvi) == NULL);
    }
}

static QUVIcode _setopt(_quvi_t quvi, QUVIoption opt, va_list arg)
{
  switch (opt)
    {
    case QUVIOPT_FORMAT:
      _set_alloc_s(quvi->format);
    case QUVIOPT_NOVERIFY:
      _set_from_arg_n(quvi->no_verify);
    case QUVIOPT_STATUSFUNCTION:
      quvi->status_func = va_arg(arg, quvi_callback_status);
      break;
    case QUVIOPT_NORESOLVE:
      _set_from_arg_n(quvi->no_resolve);
    case QUVIOPT_CATEGORY:
      _set_from_arg_n(quvi->category);
    case QUVIOPT_FETCHFUNCTION:
      quvi->fetch_func = va_arg(arg, quvi_callback_fetch);
      break;
    case QUVIOPT_RESOLVEFUNCTION:
      quvi->resolve_func = va_arg(arg, quvi_callback_resolve);
      break;
    case QUVIOPT_VERIFYFUNCTION:
      quvi->verify_func = va_arg(arg, quvi_callback_verify);
      break;
    default:
      return (QUVI_INVARG);
    }
  return (QUVI_OK);
}

/*
 * Function: quvi_setopt
 *
 * Sets a session option.
 *
 * Parameters:
 *  session - Session handle
 *  option  - Option ID
 *  ...     - Parameter
 *
 * Returns:
 *  Non-zero value if an error occurred.
 *
 * Example:
 * (start code)
 * quvi_setopt(q, QUVIOPT_FORMAT, "best");
 * quvi_setopt(q, QUVIOPT_CATEGORY, QUVIPROTO_HTTP|QUVIPROTO_RTMP);
 * (end)
 */
QUVIcode quvi_setopt(quvi_t session, QUVIoption option, ...)
{
  va_list arg;
  QUVIcode rc;

  _is_badhandle(session);

  va_start(arg, option);
  rc = _setopt(session, option, arg);
  va_end(arg);

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
  dp = NULL;
  sp = NULL;
  vp = NULL;
  lp = NULL;

  va_start(arg, info);
  type = QUVIINFO_TYPEMASK & (int)info;

  switch (type)
    {
    case QUVIINFO_DOUBLE:
      _init_from_arg(dp, double *);
    case QUVIINFO_STRING:
      _init_from_arg(sp, char **);
    case QUVIINFO_LONG:
      _init_from_arg(lp, long *);
    case QUVIINFO_VOID:
      _init_from_arg(vp, void **);
    default:
      rc = QUVI_INVARG;
    }
  va_end(arg);

  if (rc != QUVI_OK)
    return (rc);

  switch (info)
    {
    case QUVIINFO_CURL:
      _set_v(quvi->curl);
    case QUVIINFO_RESPONSECODE:
      _set_from_value_n(lp, quvi->resp_code);
    default:
      rc = QUVI_INVARG;
    }

  return (rc);
}

/*
 * Function: quvi_getinfo
 *
 * Returns session info.
 *
 * Parameters:
 *  session - Session handle
 *  info    - Info ID
 *  ...     - Parameter
 *
 * Returns:
 *  Non-zero value if an error occurred.
 *
 * Example:
 * (start code)
 * long resp_code;
 * quvi_getinfo(q, QUVIINFO_RESPONSECODE, &resp_code);
 * (end)
 */
QUVIcode quvi_getinfo(quvi_t session, QUVIinfo info, ...)
{
  va_list arg;
  void *p;

  _is_badhandle(session);

  va_start(arg, info);
  p = va_arg(arg, void *);
  va_end(arg);

  return (_getinfo(session, info, p));
}

static QUVIcode resolve_unless_disabled(_quvi_media_t media)
{
  QUVIcode rc = QUVI_OK;

  if (!media->quvi->no_resolve)
    {
      char *redirect_url = NULL;

      rc = resolve_wrapper(media->quvi, media->page_url, &redirect_url);

      if (rc != QUVI_OK)
        return (rc);
      else
        {
          if (redirect_url)
            {
              freprintf(&media->page_url, "%s", redirect_url);
              _free(redirect_url);
            }
        }
    }
  return (rc);
}

typedef enum
{
  ParseFunc=0x00,
  QueryFormatsFunc
} FindScriptFunc;

static QUVIcode resolve_and_find_script(_quvi_media_t m,
                                        FindScriptFunc func,
                                        char **formats)
{
  QUVIcode rc = resolve_unless_disabled(m);

  if (rc != QUVI_OK)
    return (rc);

  while (1)
    {
      switch (func)
        {
        case ParseFunc:
        default:
          rc = find_host_script_and_parse(m);
          break;
        case QueryFormatsFunc:
          rc = find_host_script_and_query_formats(m, formats);
          break;
        }
      if (rc != QUVI_OK)
        return (rc);
      else
        {
          /* Check for "in-script redirection URL". Not to be confused
           * with "resolving redirection URLs". */
          if (strlen(m->redirect_url))
            {
              freprintf(&m->page_url, "%s", m->redirect_url);
              continue;
            }
          else
            break;
        }
    }
  return (rc);
}

/*
 * Function: quvi_parse
 *
 * Parses an URL.
 *
 * Parameters:
 *  session - Session handle
 *  url     - URL (null-terminated string)
 *  media   - Media handle (receives)
 *
 * Returns:
 *  Non-zero value if an error occurred.
 *
 * See Also:
 *  * <quvi_parse_close>
 *  * <quvi_close>
 *
 * Example:
 * (start code)
 * quvi_media_t m;
 * rc = quvi_parse(q, URL, &m);
 * if (rc != QUVI_OK)
 *   {
 *     ...
 *   }
 * quvi_parse_close(&m);
 * (end)
 */
QUVIcode quvi_parse(quvi_t session, char *url, quvi_media_t *media)
{
  _quvi_media_t m;
  QUVIcode rc;

  _is_invarg(media);
  *media = NULL;

  _is_invarg(url);
  _is_badhandle(session);

  m = calloc(1, sizeof(*m));
  if (!m)
    return (QUVI_MEM);

  m->quvi = session;
  *media  = m;

  freprintf(&m->page_url, "%s", url);

  rc = resolve_and_find_script(m, ParseFunc, NULL);
  if (rc != QUVI_OK)
    return (rc);

#ifdef HAVE_ICONV               /* Convert character set encoding to utf8. */
  if (m->charset)
    to_utf8(m);
#endif
  assert(m->title != NULL); /* must be set in the lua script */

  m->title = from_html_entities(m->title);

  if (!m->quvi->no_verify)
    {
      _quvi_llst_node_t curr = m->url;
      while (curr)
        {
          rc = verify_wrapper(m->quvi, curr);
          if (rc != QUVI_OK)
            break;
          curr = curr->next;
        }
    }

  /* Make current media URL from the first URL. */
  m->curr = m->url;

  return (rc);
}

/*
 * Function: quvi_parse_close
 *
 * Releases a previously allocated media handle.
 *
 * Parameters:
 *  media - Media handle
 */
void quvi_parse_close(quvi_media_t *media)
{
  _quvi_media_t *m = (_quvi_media_t*) media;

  if (m && *m)
    {
      _quvi_llst_node_t curr = (*m)->url;

      while (curr)
        {
          _quvi_media_url_t l = (_quvi_media_url_t) curr->data;
          _free(l->content_type);
          _free(l->suffix);
          _free(l->url);
          curr = curr->next;
        }
      quvi_llst_free((quvi_llst_node_t)&(*m)->url);
      assert((*m)->url == NULL);

      _free((*m)->id);
      _free((*m)->title);
      _free((*m)->charset);
      _free((*m)->page_url);
      _free((*m)->host_id);
      _free((*m)->redirect_url);
      _free((*m)->start_time);
      _free((*m)->thumbnail_url);

      _free(*m);
      assert(*m == NULL);
    }
}

const char empty[] = "";

static QUVIcode _getprop(_quvi_media_t media, QUVIproperty prop, ...)
{
  _quvi_media_url_t qvl;
  QUVIcode rc;
  va_list arg;
  double *dp;
  char **sp;
  long *lp;
  int type;

  qvl = (_quvi_media_url_t) media->curr->data;
  assert(qvl != NULL);

  rc = QUVI_OK;
  dp = NULL;
  sp = NULL;
  lp = NULL;

  va_start(arg, prop);
  type = QUVIPROPERTY_TYPEMASK & (int)prop;

  switch (type)
    {
    case QUVIPROPERTY_DOUBLE:
      _init_from_arg(dp, double *);
    case QUVIPROPERTY_STRING:
      _init_from_arg(sp, char **);
    case QUVIPROPERTY_LONG:
      _init_from_arg(lp, long *);
    default:
      rc = QUVI_INVARG;
    }
  va_end(arg);

  if (rc != QUVI_OK)
    return (rc);

  switch (prop)
    {
    case QUVIPROP_HOSTID:
      _set_s(media->host_id);
    case QUVIPROP_PAGEURL:
      _set_s(media->page_url);
    case QUVIPROP_PAGETITLE:
      _set_s(media->title);
    case QUVIPROP_MEDIAID:
      _set_s(media->id);
    case QUVIPROP_MEDIAURL:
      _set_s(qvl->url);
    case QUVIPROP_MEDIACONTENTLENGTH:
      _set_from_value_n(dp, qvl->length);
    case QUVIPROP_MEDIACONTENTTYPE:
      _set_s(qvl->content_type);
    case QUVIPROP_FILESUFFIX:
      _set_s(qvl->suffix);
    case QUVIPROP_RESPONSECODE:
      _set_from_value_n(lp, media->quvi->resp_code);
    case QUVIPROP_FORMAT:
      _set_s(media->quvi->format);
    case QUVIPROP_STARTTIME:
      _set_s(media->start_time);
    case QUVIPROP_MEDIATHUMBNAILURL:
      _set_s(media->thumbnail_url);
    case QUVIPROP_MEDIADURATION:
      _set_from_value_n(dp, media->duration);
    default:
      rc = QUVI_INVARG;
    }

  return (rc);
}

/*
 * Function: quvi_getprop
 *
 * Returns a media property.
 *
 * Parameters:
 *  media    - Media handle
 *  property - Property ID
 *  ...      - Parameter
 *
 * Returns:
 *  Non-zero value if an error occurred.
 *
 * Example:
 * Error handling omitted for brewity.
 * (start code)
 * double media_content_length;
 * char *media_url;
 * quvi_media_t m;
 * quvi_t q;
 *
 * quvi_init(&q);
 * quvi_parse(q, URL, &m);
 * quvi_getprop(m, QUVIPROP_MEDIAURL, &media_url);
 * quvi_getprop(m, QUVIPROP_MEDIACONTENTLENGTH, &media_content_length);
 * quvi_parse_close(&m);
 * quvi_close(&q);
 * (end)
 */
QUVIcode quvi_getprop(quvi_media_t media, QUVIproperty property, ...)
{
  va_list arg;
  void *p;

  _is_badhandle(media);

  va_start(arg, property);
  p = va_arg(arg, void *);
  va_end(arg);

  return (_getprop(media, property, p));
}

/*
 * Title: Support
 */

/*
 * Function: quvi_query_formats
 *
 * Queries available formats to the URL. The query is done over an Internet
 * connection. It resolves any shortened URLs _unless_ <QUVIOPT_NORESOLVE> is
 * set explicitly with <quvi_setopt>. This function checks also if an URL is
 * supported, similarly to that <quvi_supported>.
 *
 * Unlike <quvi_supported>, <quvi_supported_ident> and
 * <quvi_next_supported_website> which all return a static list of format
 * string, <quvi_query_formats> constructs the list from the data returned
 * by the server.
 *
 * The exception to the above are the webscripts that support only _1_
 * format. They will always return the string 'default'.
 *
 * Parameters:
 *  session - Session handle
 *  url     - URL (null-terminated string)
 *  formats - Null-terminated string (receives)
 *
 * Returns:
 *  A null-terminated string or NULL. <quvi_free> it when done.
 *
 * See Also:
 *  * <quvi_init>
 *  * <quvi_close>
 *  * <quvi_free>
 *
 * Example:
 * Error handling omitted for brewity.
 * (start code)
 * char *formats;
 * quvi_t q;
 *
 * quvi_init(&q);
 * quvi_query_formats(q, URL, &formats);
 * puts(formats);
 * quvi_free(formats);
 * quvi_close(&q);
 * (end)
 */
QUVIcode quvi_query_formats(quvi_t session, char *url, char **formats)
{
  _quvi_media_t m;
  QUVIcode rc;

  _is_badhandle(session);
  _is_invarg(url);
  _is_invarg(formats);
  *formats = NULL;

  m = calloc(1, sizeof(*m));
  if (!m)
    return (QUVI_MEM);

  m->quvi = session;
  freprintf(&m->page_url, "%s", url);

  rc = resolve_and_find_script(m, QueryFormatsFunc, formats);

  quvi_parse_close((quvi_media_t)&m);

  return (rc);
}

/*
 * Function: quvi_next_media_url
 *
 * Iterates the media stream URLs.
 *
 * Parameters:
 *  media - Media handle
 *
 * Returns:
 *  Non-zero value if an error occurred or QUVI_LAST, otherwise QUVI_OK.
 *
 * See Also:
 *  <quvi_getprop>
 *
 * Example:
 * Error handling omitted for brewity.
 * (start code)
 * quvi_media_t m;
 * char *url;
 * quvi_t q;
 *
 * quvi_init(&q);
 * quvi_parse(q, URL, &m);
 * do
 *   {
 *     quvi_getprop(m, QUVIPROP_MEDIAURL, &url);
 *     puts(url);
 *   }
 * while (quvi_next_media_url(media) == QUVI_OK);
 * quvi_parse_close(&m);
 * quvi_close(&q);
 * (end)
 */
QUVIcode quvi_next_media_url(quvi_media_t media)
{
  _quvi_media_t m = (_quvi_media_t) media;

  _is_badhandle(media);

  /* start from the first */
  if (!m->curr)
    {
      m->curr = m->url;
      return (QUVI_OK);
    }

  /* move to the next */
  m->curr = m->curr->next;
  if (!m->curr)
    {
      m->curr = m->url;  /* reset */
      return (QUVI_LAST);
    }

  return (QUVI_OK);
}

/*
 * Function: quvi_supported
 *
 * Checks whether library supports the URL. Does not require an Internet
 * connection. Shortened URLs will fail with this function.
 *
 * Parameters:
 *  session - Session handle
 *  url     - URL (null-terminated string)
 *
 * Returns:
 *  Non-zero value if an error occurred or QUVI_NOSUPPORT, otherwise QUVI_OK.
 *
 * See Also:
 *  <quvi_supported_ident>
 */
QUVIcode quvi_supported(quvi_t session, char *url)
{
  return quvi_supported_ident(session, url, NULL);
}

static QUVIcode _ident_getprop(_quvi_ident_t i, QUVIidentProperty p, ...)
{
  QUVIcode rc;
  va_list arg;
  char **sp;
  long *lp;
#ifdef _UNUSED
  double *dp;
  void **vp;
#endif
  int type;

  rc = QUVI_OK;
  sp = NULL;
  lp = NULL;
#ifdef _UNUSED
  dp = NULL;
  vp = NULL;
#endif

  va_start(arg, p);
  type = QUVIPROPERTY_TYPEMASK & (int)p;

  switch (type)
    {
    case QUVIPROPERTY_STRING:
      _init_from_arg(sp, char**);
    case QUVIPROPERTY_LONG:
      _init_from_arg(lp, long*);
#ifdef _UNUSED
    case QUVIPROPERTY_DOUBLE:
      _init_from_arg(dp, double*);
    case QUVIPROPERTY_VOID:
      _init_from_arg(vp, void **);
#endif
    default:
      rc = QUVI_INVARG;
    }
  va_end(arg);

  if (rc != QUVI_OK)
    return (rc);

  switch (p)
    {
    case QUVI_IDENT_PROPERTY_URL:
      _set_s(i->url);
    case QUVI_IDENT_PROPERTY_FORMATS:
      _set_s(i->formats);
    case QUVI_IDENT_PROPERTY_DOMAIN:
      _set_s(i->domain);
    case QUVI_IDENT_PROPERTY_CATEGORIES:
      _set_from_value_n(lp, i->categories);
    default:
      rc = QUVI_INVARG;
    }
  return (rc);
}

/*
 * Function: quvi_supported_ident
 *
 * Otherwise identical to <quvi_supported> but returns the `ident' data.
 *
 * Parameters:
 *  session - Session handle
 *  url     - URL (null-terminated string)
 *  ident   - Ident handle (receives)
 *
 * Returns:
 *  Non-zero value if an error occurred.
 *
 * See Also:
 *  * <quvi_ident_getprop>
 *  * <quvi_supported_ident_close>
 *
 * Example:
 * Error handling omitted for brewity.
 * (start code)
 * quvi_ident_t ident;
 * quvi_t q;
 *
 * quvi_init(&q);
 * if (quvi_supported_ident(q, URL, &ident) == QUVI_OK)
 *   {
 *     char *formats;
 *     quvi_ident_getprop(ident, QUVI_IDENT_PROPERTY_FORMATS, &formats);
 *     puts(formats);
 *     quvi_supported_ident_close(&ident);
 *   }
 * quvi_close(&q);
 * (end)
 */
QUVIcode quvi_supported_ident(quvi_t session, char *url, quvi_ident_t *ident)
{
  _quvi_media_t m;
  QUVIcode rc;

  /* ident may be NULL */
  _is_badhandle(session);
  _is_invarg(url);

  m = calloc(1, sizeof(*m));
  if (!m)
    return (QUVI_MEM);

  m->quvi = session;
  freprintf(&m->page_url, "%s", url);

  rc = find_host_script(m, (_quvi_ident_t*) ident);

  quvi_parse_close((quvi_media_t)&m);

  return (rc);
}

/*
 * Function: quvi_supported_ident_close
 *
 * Releases a previously allocated ident handle.
 *
 * Parameters:
 *  ident - Ident handle
 *
 * See Also:
 *  <quvi_supported_ident>
 */
void quvi_supported_ident_close(quvi_ident_t *handle)
{
  _quvi_ident_t *ident = (_quvi_ident_t*) handle;

  if (ident && *ident)
    {
      _free((*ident)->domain);
      _free((*ident)->formats);
      _free((*ident)->url);
      _free((*ident));
      assert(*ident == NULL);
    }
}

/*
 * Function: quvi_ident_getprop
 *
 * Returns an ident property.
 *
 * Parameters:
 *  ident    - Ident handle
 *  property - Property ID
 *  ...      - Parameters
 *
 * Returns:
 *  Non-zero value if an error occurred.
 *
 * See Also:
 *  <quvi_supported_ident>
 */
QUVIcode quvi_ident_getprop(quvi_ident_t ident,
                            QUVIidentProperty property,
                            ...)
{
  va_list arg;
  void *p;

  _is_badhandle(ident);

  va_start(arg, property);
  p = va_arg(arg, void*);
  va_end(arg);

  return (_ident_getprop(ident, property, p));
}

/*
 * Function: quvi_next_supported_website
 *
 * Returns the next supported website.
 *
 * Parameters:
 *  session - Session handle
 *  domain  - Null-terminated string containing the domain _pattern_ (receives)
 *  formats - Null-terminated (static) string containing formats (receives)
 *
 * Returns:
 *  Non-zero value if an error occurred or QUVI_LAST, otherwise QUVI_OK.
 */
QUVIcode quvi_next_supported_website(quvi_t session,
                                     char **domain,
                                     char **formats)
{
  struct _quvi_ident_s ident;
  _quvi_t quvi;
  QUVIcode rc;

  _is_badhandle(session);
  quvi = (_quvi_t) session;

  _is_invarg(domain);
  _is_invarg(formats);

  if (!quvi->website_scripts)
    return (QUVI_NOLUAWEBSITE);

  if (!quvi->curr_website)
    quvi->curr_website = quvi->website_scripts;
  else
    {
      quvi->curr_website = quvi->curr_website->next;
      if (!quvi->curr_website)
        return (QUVI_LAST);
    }

  ident.quvi = quvi;
  ident.url = NULL;
  ident.domain = NULL;
  ident.formats = NULL;

  rc = run_ident_func(&ident, quvi->curr_website);

  if (rc == QUVI_NOSUPPORT)
    {
      /* The website scripts return QUVI_NOSUPPORT in all cases. This is
       * because of the undefined URL that we pass to them above (ident.url
       * = NULL). We are only interested in the `domain' and `formats'
       * information anyway, so this is OK. */
      if (ident.categories & quvi->category)
        {
          *domain = ident.domain;
          *formats = ident.formats;
          rc = QUVI_OK;
        }
      else
        {
          _free(ident.domain);
          _free(ident.formats);
          rc = quvi_next_supported_website(session, domain, formats);
        }
    }

  return (rc);
}

/*
 * Function: quvi_strerror
 *
 * Returns a corresponding error message to the return code.
 *
 * Parameters:
 *  session - Session handle
 *  code    - Return code
 *
 * Returns:
 *  A null-terminated string. Do _not_ attempt to <quvi_free> it.
 */
char *quvi_strerror(quvi_t session, QUVIcode code)
{
  static const char *errormsgs[] =
  {
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

  _quvi_t quvi = (_quvi_t) session;

  if (quvi)
    {
      if (code > _INTERNAL_QUVI_LAST)
        return (quvi->errmsg);
    }
  else
    {
      if (code > _INTERNAL_QUVI_LAST)
        code = _INTERNAL_QUVI_LAST;
    }

  return ((char *)errormsgs[code]);
}

/*
 * Function: quvi_version
 *
 * Returns a quvi version string.
 *
 * Parameters:
 *  id - Version ID
 *
 * Returns:
 *  A null-terminated string. Do _not_ attempt to <quvi_free> it.
 */
char *quvi_version(QUVIversion id)
{
  static const char version[] =
#ifdef VN
    VN
#else
    PACKAGE_VERSION
#endif
    ;
  static const char version_long[] =
#ifdef VN
    VN
#else
    PACKAGE_VERSION
#endif
    " for " CANONICAL_TARGET;

  switch (id)
    {
    case QUVI_VERSION_SCRIPTS:
      return (read_scripts_version());
    case QUVI_VERSION_LONG:
      return ((char*)version_long);
    default:
      break;
    }

  return ((char *)version);
}

/*
 * Function: quvi_free
 *
 * Frees allocated memory.
 *
 * Parameters:
 *  pointer - Pointer to data
 */
void quvi_free(void *pointer)
{
  if (pointer != NULL)
    free(pointer);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
