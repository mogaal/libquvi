/* libquvi
 * Copyright (C) 2011  Toni Gundogdu <legatvs@gmail.com>
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

/* callback_libsoup.c - use libsoup instead of library default libcurl */

#include "config.h"

#include <stdio.h>
#include <string.h>

#include <quvi/quvi.h>
#include <quvi/net.h>

#ifdef HAVE_LIBSOUP_GNOME
#include <libsoup/soup-gnome.h>
#else
#include <libsoup/soup.h>
#endif

#include "common.h"

static QUVIcode status_callback(long param, void *data)
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

  return (QUVI_OK);
}

static void set_feat(quvi_net_t n, SoupMessage *m,
                     QUVInetPropertyFeatureName feature,
                     const char *hdr)
{
  char *s = quvi_net_get_one_prop_feat(n, feature);
  if (s)
    soup_message_headers_append(m->request_headers, hdr, s);
}

static void set_feats_from_lua_script(quvi_net_t n, SoupMessage *m)
{
  set_feat(n, m, QUVI_NET_PROPERTY_FEATURE_ARBITRARYCOOKIE, "Cookie");
  set_feat(n, m, QUVI_NET_PROPERTY_FEATURE_USERAGENT, "User-Agent");
}

#ifdef _0
/* Same as set_feats_from_lua_script above. quvi_net_get_one_prop_feat
 * is a convinience function that can be used instead unless the
 * application is expected handle >1 of the same features (e.g. multiple
 * instances of QUVI_NET_PROPERTY_FEATURE_ARBITRARYCOOKIE in the
 * features list. */
static void set_feats_from_lua_script_alt(quvi_net_t n, SoupMessage *m)
{
  quvi_llst_node_t feature;
  quvi_net_getprop(n, QUVI_NET_PROPERTY_FEATURES, &feature);

  while (feature)
    {
      char *feature_name, *feature_value;
      quvi_net_propfeat_t pfeat;

      pfeat = (quvi_net_propfeat_t) quvi_llst_data(feature);

      quvi_net_getprop_feat(pfeat,
                            QUVI_NET_PROPERTY_FEATURE_NAME, &feature_name);

      quvi_net_getprop_feat(pfeat,
                            QUVI_NET_PROPERTY_FEATURE_VALUE, &feature_value);

      if (strcmp(feature_name, "arbitrary_cookie") == 0)
        {
          soup_message_headers_append(
            m->request_headers, "Cookie", feature_value);
        }

      if (strcmp(feature_name, "user_agent") == 0)
        {
          soup_message_headers_append(
            m->request_headers, "User-Agent", feature_value);
        }

      feature = quvi_llst_next(feature);
    }
}
#endif

static SoupSession *session = NULL;

static void send_message(quvi_net_t n, SoupMessage **message,
                         guint *status, SoupMessageFlags msg_flags,
                         const int head_flag, const int lua_feats)
{
  char *url = NULL;

  quvi_net_getprop(n, QUVI_NET_PROPERTY_URL, &url);

  *message = soup_message_new(head_flag ? "HEAD":"GET", url);

  if (msg_flags)
    soup_message_set_flags(*message, msg_flags);

  /* In reality, the library sets these only for quvi.fetch. */
  if (lua_feats)
    set_feats_from_lua_script(n, *message);

  *status = soup_session_send_message(session, *message);

  quvi_net_setprop(n, QUVI_NET_PROPERTY_RESPONSECODE, *status);
}

static QUVIcode fetch_callback(quvi_net_t n)
{
  SoupMessage *m;
  guint status;

  send_message(n, &m, &status,
               0, /* Session flags */
               0, /* 'HEAD' flag */
               1); /* Set features from quvi.fetch */

  if (SOUP_STATUS_IS_SUCCESSFUL(status))
    {
      quvi_net_setprop(n, QUVI_NET_PROPERTY_CONTENT, m->response_body->data);
      return (QUVI_OK);
    }

  quvi_net_seterr(n, "%s (http/%d)", m->reason_phrase, status);

  return (QUVI_CALLBACK);
}

static QUVIcode resolve_callback(quvi_net_t n)
{
  SoupMessage *m;
  guint status;

  send_message(n, &m, &status,
               SOUP_MESSAGE_NO_REDIRECT, /* Session flags */
               0, /* 'HEAD' flag */
               0); /* Set features from quvi.fetch */

  if (SOUP_STATUS_IS_REDIRECTION(status))
    {
      const char *r_url =
        soup_message_headers_get_one(m->response_headers, "Location");

      quvi_net_setprop(n, QUVI_NET_PROPERTY_REDIRECTURL, r_url);
    }
  else if (!SOUP_STATUS_IS_SUCCESSFUL(status))
    {
      quvi_net_seterr(n, "%s (http/%d)", m->reason_phrase, status);
      return (QUVI_CALLBACK);
    }

  return (QUVI_OK);
}

static QUVIcode verify_callback(quvi_net_t n)
{
  SoupMessage *m;
  guint status;

  send_message(n, &m, &status,
               0, /* Session flags */
               1, /* 'HEAD' flag */
               0); /* Set features from quvi.fetch */

  if (SOUP_STATUS_IS_SUCCESSFUL(status))
    {
      goffset cl =
        soup_message_headers_get_content_length(m->response_headers);

      const char *ct =
        soup_message_headers_get_content_type(m->response_headers, NULL);

      quvi_net_setprop(n, QUVI_NET_PROPERTY_CONTENTTYPE, ct);
      quvi_net_setprop(n, QUVI_NET_PROPERTY_CONTENTLENGTH, cl);

      return (QUVI_OK);
    }

  quvi_net_seterr(n, "%s (http/%d)", m->reason_phrase, status);

  return (QUVI_CALLBACK);
}

static void dump_media(quvi_media_t m)
{
  char *m_url, *m_ct, *m_suffix;
  double m_cl;

  quvi_getprop(m, QUVIPROP_MEDIAURL, &m_url);
  quvi_getprop(m, QUVIPROP_MEDIACONTENTTYPE, &m_ct);
  quvi_getprop(m, QUVIPROP_MEDIACONTENTLENGTH, &m_cl);
  quvi_getprop(m, QUVIPROP_FILESUFFIX, &m_suffix);

  spew_e(
    "%s\n"
    "url: %s\n"
    "content-type: %s\n"
    "content-length: %.0f\n"
    "file suffix: %s\n",
    __func__, m_url, m_ct, m_cl, m_suffix);
}

static void help()
{
  printf(
    "Usage: callback_libsoup [--help|--log] [URL]\n\n"
    "  -h,--help    .. Print help and exit\n"
    "  -l,--logger  .. Enable logger\n\n"
    "Note: Unless URL is specified, the default URL will be used\n");
  exit(0);
}

int main (int argc, char *argv[])
{
  char *url = "http://is.gd/yFNPMR";
  quvi_media_t m;
  int log_flag;
  QUVIcode rc;
  quvi_t q;

  log_flag = 0;

  if (argc > 1)
    {
      int i;
      for (i=1; i<argc; ++i)
        {
          if (strcmp(argv[i], "-l") == 0
              || strcmp(argv[i], "--logger") == 0)
            {
              log_flag = 1;
            }
          else if (strcmp(argv[i], "-h") == 0
                   || strcmp(argv[i], "--help") == 0)
            {
              help();
            }
          else
            url = argv[i];
        }
    }

  rc = quvi_init(&q);
  check_error(q,rc);

  quvi_setopt(q, QUVIOPT_STATUSFUNCTION, &status_callback);
  quvi_setopt(q, QUVIOPT_FETCHFUNCTION, &fetch_callback);
  quvi_setopt(q, QUVIOPT_RESOLVEFUNCTION, &resolve_callback);
  quvi_setopt(q, QUVIOPT_VERIFYFUNCTION, &verify_callback);

  g_type_init();

#ifdef HAVE_LIBSOUP_GNOME
  session = soup_session_async_new_with_options(
              SOUP_SESSION_ADD_FEATURE_BY_TYPE,
              SOUP_TYPE_PROXY_RESOLVER_GNOME,
              NULL
            );
#else
  session = soup_session_async_new();
#endif

  if (log_flag)
    {
      SoupLogger *log = soup_logger_new(SOUP_LOGGER_LOG_HEADERS, -1);
      soup_session_add_feature(session, SOUP_SESSION_FEATURE(log));
      g_object_unref(log);
    }

  rc = quvi_parse(q, url, &m);
  check_error(q, rc);

  dump_media(m);

  quvi_parse_close(&m);
  quvi_close(&q);

  return (0);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
