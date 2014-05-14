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

#include "config.h"

#include <string.h>

#include "quvi/quvi.h"
#include "quvi/net.h"
#include "quvi/llst.h"

#include "internal.h"
#include "util.h"

extern const char empty[]; /* quvi_api.c */

/* Title: quvi Network API */

static QUVIcode _net_getprop(_quvi_net_t n, QUVInetProperty p, ...)
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

  va_start(arg, p);
  type = QUVIPROPERTY_TYPEMASK & (int)p;

  switch (type)
    {
    case QUVIPROPERTY_DOUBLE:
      _init_from_arg(dp, double*);
    case QUVIPROPERTY_STRING:
      _init_from_arg(sp, char**);
    case QUVIPROPERTY_LONG:
      _init_from_arg(lp, long*);
    case QUVIPROPERTY_VOID:
      _init_from_arg(vp, void **);
    default:
      rc = QUVI_INVARG;
    }
  va_end(arg);

  if (rc != QUVI_OK)
    return (rc);

  switch (p)
    {
    case QUVI_NET_PROPERTY_URL:
      _set_s(n->url);
    case QUVI_NET_PROPERTY_REDIRECTURL:
      _set_s(n->redirect.url);
    case QUVI_NET_PROPERTY_CONTENT:
      _set_s(n->fetch.content);
    case QUVI_NET_PROPERTY_CONTENTTYPE:
      _set_s(n->verify.content_type);
    case QUVI_NET_PROPERTY_CONTENTLENGTH:
      _set_from_value_n(dp, n->verify.content_length);
    case QUVI_NET_PROPERTY_RESPONSECODE:
      _set_from_value_n(lp, n->resp_code);
    case QUVI_NET_PROPERTY_FEATURES:
      _set_v(n->features);
    default:
      rc = QUVI_INVARG;
    }
  return (rc);
}

/*
 * Function: quvi_net_getprop
 *
 * Returns a network property.
 *
 * Parameters:
 *  net      - Network handle
 *  property - Property ID
 *
 * Returns:
 *  Non-zero value if an error occurred.
 */
QUVIcode quvi_net_getprop(quvi_net_propfeat_t net,
                          QUVInetProperty property,
                          ...)
{
  va_list arg;
  void *p;

  _is_badhandle(net);

  va_start(arg, property);
  p = va_arg(arg, void*);
  va_end(arg);

  return (_net_getprop(net, property, p));
}

static QUVIcode _net_setprop(_quvi_net_t n, QUVInetProperty p, va_list arg)
{
  switch (p)
    {
    case QUVI_NET_PROPERTY_URL:
      _set_alloc_s(n->url);
    case QUVI_NET_PROPERTY_FEATURES:
      break; /* Ignored: read-only */
    case QUVI_NET_PROPERTY_REDIRECTURL:
      _set_alloc_s(n->redirect.url);
    case QUVI_NET_PROPERTY_CONTENT:
      _set_alloc_s(n->fetch.content);
    case QUVI_NET_PROPERTY_CONTENTTYPE:
      _set_alloc_s(n->verify.content_type);
    case QUVI_NET_PROPERTY_CONTENTLENGTH:
      _set_from_arg_n(n->verify.content_length);
    case QUVI_NET_PROPERTY_RESPONSECODE:
      _set_from_arg_n(n->resp_code);
    default:
      return (QUVI_INVARG);
    }
  return (QUVI_OK);
}

/*
 * Function: quvi_net_setprop
 *
 * Sets a network property.
 *
 * Parameters:
 *  net      - Network handle
 *  property - Property ID
 *  ...      - Parameter
 *
 * Returns:
 *  Non-zero value if an error occurred.
 */
QUVIcode quvi_net_setprop(quvi_net_t net, QUVInetProperty property, ...)
{
  va_list arg;
  QUVIcode rc;

  _is_badhandle(net);

  va_start(arg, property);
  rc = _net_setprop(net, property, arg);
  va_end(arg);

  return (rc);
}

static QUVIcode _net_getprop_feat(_quvi_net_propfeat_t n,
                                  QUVInetPropertyFeature feature,
                                  ...)
{
  QUVIcode rc;
  va_list arg;
  double *dp;
  char **sp;
#ifdef _UNUSED
  void **vp;
  long *lp;
#endif
  int type;

  rc = QUVI_OK;
  dp = 0;
  sp = 0;
#ifdef _UNUSED
  vp = 0;
  lp = 0;
#endif

  va_start(arg, feature);
  type = QUVIPROPERTY_TYPEMASK & (int)feature;

  switch (type)
    {
    case QUVIPROPERTY_DOUBLE:
      _init_from_arg(dp, double*);
    case QUVIPROPERTY_STRING:
      _init_from_arg(sp, char**);
    default:
      rc = QUVI_INVARG;
    }
  va_end(arg);

  if (rc != QUVI_OK)
    return (rc);

  switch (feature)
    {
    case QUVI_NET_PROPERTY_FEATURE_NAME:
      _set_s(n->name);
    case QUVI_NET_PROPERTY_FEATURE_VALUE:
      _set_s(n->value);
    default:
      rc = QUVI_INVARG;
    }
  return (rc);
}

/*
 * Function: quvi_net_getprop_feat
 *
 * Returns a network property feature.
 *
 * Parameters:
 *  handle  - Property feature handle
 *  feature - Property feature ID
 *  ...     - Parameter
 *
 * Returns:
 *  Non-zero value if an error occurred.
 */
QUVIcode quvi_net_getprop_feat(quvi_net_propfeat_t handle,
                               QUVInetPropertyFeature feature,
                               ...)
{
  va_list arg;
  void *p;

  _is_badhandle(handle);

  va_start(arg, feature);
  p = va_arg(arg, void*);
  va_end(arg);

  return (_net_getprop_feat(handle, feature, p));
}

/*
 * Function: quvi_net_seterr
 *
 * Sets a network error message.
 *
 * Parameters:
 *  net - Network handle
 *  fmt - Format string
 *  ... - Parameter
 *
 * Returns:
 *  Non-zero value if an error occurred.
 */
QUVIcode quvi_net_seterr(quvi_net_t net, const char *fmt, ...)
{
  _quvi_net_t n;
  va_list args;

  _is_badhandle(net);
  n = (_quvi_net_t) net;

  va_start(args, fmt);
  vafreprintf(&n->errmsg, fmt, args);

  return (QUVI_OK);
}

/* Title: Convenience functions */

extern const char *net_prop_feats[];

static const char *_feat_to_str(QUVInetPropertyFeatureName id)
{
  const char *s = NULL;

  if (id > QUVI_NET_PROPERTY_FEATURE_NAME_NONE
      && id < _QUVI_NET_PROPERTY_FEATURE_NAME_LAST)
    {
      s = net_prop_feats[id];
    }
  return (s);
}

/*
 * Function: quvi_net_get_one_prop_feat
 *
 * Returns the first matching property feature from the list. A
 * convenience function that allows finding the feature by an ID rather
 * than a string.
 *
 * Parameters:
 *  net  - Network handle
 *  name - Property feature ID
 *
 * Returns:
 *  A null-terminated string, otherwise NULL.
 *
 * See Also:
 *  <quvi_net_getprop_feat>
 */
char *quvi_net_get_one_prop_feat(quvi_net_t net,
                                 QUVInetPropertyFeatureName name)
{
  quvi_llst_node_t opt;

  quvi_net_getprop(net, QUVI_NET_PROPERTY_FEATURES, &opt);

  while (opt)
    {
      const char *feat_name, *feat_value, *s;
      quvi_net_propfeat_t popt;

      popt = (quvi_net_propfeat_t) quvi_llst_data(opt);

      quvi_net_getprop_feat(popt,
                            QUVI_NET_PROPERTY_FEATURE_NAME, &feat_name);

      quvi_net_getprop_feat(popt,
                            QUVI_NET_PROPERTY_FEATURE_VALUE, &feat_value);

      s = _feat_to_str(name);

      if (s && !strcmp(feat_name,s))
        return ((char*)feat_value);

      opt = quvi_llst_next(opt);
    }
  return (NULL);
}


/* vim: set ts=2 sw=2 tw=72 expandtab: */
