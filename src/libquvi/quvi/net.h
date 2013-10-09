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

#ifndef quvi_net_h
#define quvi_net_h

/*
 * Typedef: Handles
 *
 * quvi_net_t          - Network handle
 * quvi_net_propfeat_t - Network property handle
 */
typedef void *quvi_net_t;
typedef void *quvi_net_propfeat_t;

/*
 * Typedef: Callbacks
 *
 * quvi_callback_fetch   - Fetch callback function
 * quvi_callback_resolve - Resolve callback function
 * quvi_callback_verify  - Verify callback function
 */
typedef int (*quvi_callback_fetch) (quvi_net_t);
typedef int (*quvi_callback_resolve) (quvi_net_t);
typedef int (*quvi_callback_verify) (quvi_net_t);

/*
 * Enum: Network properties
 *
 * QUVI_NET_PROPERTY_URL         - URL
 * QUVI_NET_PROPERTY_REDIRECTURL - URL to another location
 * QUVI_NET_PROPERTY_CONTENT     - Content
 * QUVI_NET_PROPERTY_CONTENTTYPE - Content-type from returned HTTP header
 * QUVI_NET_PROPERTY_CONTENTLENGTH - Content-length from returned HTTP header
 * QUVI_NET_PROPERTY_RESPONSECODE  - Response code returned by the server
 * QUVI_NET_PROPERTY_FEATURES      - Features, e.g. arbitrary cookie
 */
typedef enum
{
  QUVI_NET_PROPERTY_NONE        = 0x00,
  QUVI_NET_PROPERTY_URL         = QUVIPROPERTY_STRING+1,
  QUVI_NET_PROPERTY_REDIRECTURL = QUVIPROPERTY_STRING+2,
  QUVI_NET_PROPERTY_CONTENT     = QUVIPROPERTY_STRING+3,
  QUVI_NET_PROPERTY_CONTENTTYPE = QUVIPROPERTY_STRING+4,
  QUVI_NET_PROPERTY_CONTENTLENGTH = QUVIPROPERTY_DOUBLE+5,
  QUVI_NET_PROPERTY_RESPONSECODE  = QUVIPROPERTY_LONG+6,
  QUVI_NET_PROPERTY_FEATURES    = QUVIPROPERTY_VOID+7
  /* Add new ones below */
} QUVInetProperty;

/*
 * Enum: Property features
 *
 * QUVI_NET_PROPERTY_FEATURE_NAME  - Name of the feature
 * QUVI_NET_PROPERTY_FEATURE_VALUE - Value of the feature
 */
typedef enum
{
  QUVI_NET_PROPERTY_FEATURE_NONE  = 0x00,
  QUVI_NET_PROPERTY_FEATURE_NAME  = QUVIPROPERTY_STRING+1,
  QUVI_NET_PROPERTY_FEATURE_VALUE = QUVIPROPERTY_STRING+2
  /* Add new ones below */
} QUVInetPropertyFeature;

/*
 * Enum: Property feature shorthands
 *
 * QUVI_NET_PROPERTY_FEATURE_ARBITRARYCOOKIE - Arbitrary cookie
 * QUVI_NET_PROPERTY_FEATURE_USERAGENT       - User-agent
 */
typedef enum
{
  QUVI_NET_PROPERTY_FEATURE_NAME_NONE = 0x00,
  QUVI_NET_PROPERTY_FEATURE_ARBITRARYCOOKIE,
  QUVI_NET_PROPERTY_FEATURE_USERAGENT,
  /* Add new ones below */
  _QUVI_NET_PROPERTY_FEATURE_NAME_LAST
} QUVInetPropertyFeatureName;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

  QUVIcode quvi_net_getprop(quvi_net_t net, QUVInetProperty property, ...);
  QUVIcode quvi_net_setprop(quvi_net_t net, QUVInetProperty property, ...);

  QUVIcode quvi_net_getprop_feat(quvi_net_propfeat_t handle,
                                 QUVInetPropertyFeature feature,
                                 ...);

  char *quvi_net_get_one_prop_feat(quvi_net_t net,
                                   QUVInetPropertyFeatureName name);

  QUVIcode quvi_net_seterr(quvi_net_t net, const char *fmt, ...);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* quvi_net_h */

/* vim: set ts=2 sw=2 tw=72 expandtab: */
