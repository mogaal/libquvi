
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

#ifndef quvi_h
#define quvi_h

#include <stdlib.h>
#include <stdint.h> /* C99 header */

/*
 * Enum: Version
 *
 * QUVI_VERSION         - Version string
 * QUVI_VERSION_LONG    - Version string, build date, etc.
 * QUVI_VERSION_SCRIPTS - libquvi-scripts version
 */
typedef enum
{
  QUVI_VERSION = 0x00,
  QUVI_VERSION_LONG,
  QUVI_VERSION_SCRIPTS
} QUVIversion;

/*
 * Enum: Return codes
 *
 * QUVI_OK        - No error
 * QUVI_MEM       - Memory allocation error
 * QUVI_BADHANDLE - Bad handle
 * QUVI_INVARG    - Invalid function argument
 * QUVI_CURLINIT  - libcurl initialization failure
 * QUVI_LAST      - Last element in list
 * QUVI_ABORTEDBYCALLBACK - Aborted by callback function
 * QUVI_LUAINIT      - liblua initialization failure
 * QUVI_NOLUAWEBSITE - Failed to find any webscripts
 * QUVI_NOLUAUTIL - Failed to find the utility scripts
 * QUVI_NOSUPPORT - libquvi cannot handle the URL
 * QUVI_CALLBACK  - Network callback error occurred
 * QUVI_ICONV     - libiconv error occurred
 * QUVI_LUA       - liblua (or webscript) error occurred
 * QUVI_PCRE      - Deprecated since 0.2.9, removed in 0.4.0
 * QUVI_CURL      - Deprecated since 0.2.16, removed in 0.4.0,
 *                    use <QUVI_CALLBACK> instead
 */
typedef enum
{
  /* quvi_strerror returns static strings with these: */
  QUVI_OK = 0x00,
  QUVI_MEM,
  QUVI_BADHANDLE,
  QUVI_INVARG,
  QUVI_CURLINIT,
  QUVI_LAST,
  QUVI_ABORTEDBYCALLBACK,
  QUVI_LUAINIT,
  QUVI_NOLUAWEBSITE,
  QUVI_NOLUAUTIL,
  _INTERNAL_QUVI_LAST,
  /* quvi_strerror returns dynamically created error strings with these: */
  QUVI_NOSUPPORT = 0x41,
  QUVI_CALLBACK,
  QUVI_ICONV,
  QUVI_LUA
  /* Add new ones below */
} QUVIcode;

/*
 * Enum: Status changes
 *
 * QUVISTATUS_FETCH   - Status changed to 'fetch' from URL
 * QUVISTATUS_VERIFY  - Status changed to 'verify' URL
 * QUVISTATUS_RESOLVE - Status changed to 'resolve' URL
 * QUVISTATUS_SHORTENED - Deprecated since 0.2.16, removed in 0.4.0,
 *                          use <QUVISTATUS_RESOLVE> instead
 */
typedef enum
{
  QUVISTATUS_FETCH = 0x00,
  QUVISTATUS_VERIFY,
  QUVISTATUS_RESOLVE
  /* Add new ones below */
} QUVIstatus;

/*
 * Enum: Status change types
 *
 * QUVISTATUSTYPE_PAGE     - Status type changed to 'page'
 * QUVISTATUSTYPE_CONFIG   - Status type changed to 'config'
 * QUVISTATUSTYPE_PLAYLIST - Status type changed to 'playlist'
 * QUVISTATUSTYPE_DONE     - Status type changed to 'done'
 */
typedef enum
{
  QUVISTATUSTYPE_PAGE = 0x00,
  QUVISTATUSTYPE_CONFIG,
  QUVISTATUSTYPE_PLAYLIST,
  QUVISTATUSTYPE_DONE
  /* Add new ones below */
} QUVIstatusType;

/*
 * Enum: Options
 *
 * QUVIOPT_FORMAT          - Requested format
 * QUVIOPT_NOVERIFY        - Do not verify (HTTP) media URLs
 * QUVIOPT_STATUSFUNCTION  - Callback function for status updates
 * QUVIOPT_NORESOLVE       - Do not attempt to resolve URLs to new location
 * QUVIOPT_CATEGORY        - Bit pattern of OR'd webscript categories
 * QUVIOPT_FETCHFUNCTION   - Callback function for fetching URLs
 * QUVIOPT_RESOLVEFUNCTION - Callback function for resolving URL redirections
 * QUVIOPT_VERIFYFUNCTION  - Callback function for verifying media URLs
 * QUVIOPT_WRITEFUNCTION   - Deprecated since 0.2.17, removed in 0.4.0
 * QUVIOPT_NOSHORTENED     - Deprecated since 0.2.16, removed in 0.4.0,
 *                            use <QUVIOPT_NORESOLVE> instead
 */
typedef enum
{
  QUVIOPT_FORMAT = 0x00,
  QUVIOPT_NOVERIFY,
  QUVIOPT_STATUSFUNCTION,
  QUVIOPT_NORESOLVE = 0x4,
  QUVIOPT_CATEGORY,
  QUVIOPT_FETCHFUNCTION,
  QUVIOPT_RESOLVEFUNCTION,
  QUVIOPT_VERIFYFUNCTION
  /* Add new ones below */
} QUVIoption;

/*
 * Enum: Webscript protocol categories
 *
 * QUVIPROTO_HTTP - Protocol category HTTP
 * QUVIPROTO_MMS  - Protocol category MMS
 * QUVIPROTO_RTSP - Protocol category RTSP
 * QUVIPROTO_RTMP - Protocol category RTMP
 * QUVIPROTO_ALL  - All protocol categories
 */
typedef enum
{
  QUVIPROTO_HTTP = 0x1,
  QUVIPROTO_MMS  = 0x2,
  QUVIPROTO_RTSP = 0x4,
  QUVIPROTO_RTMP = 0x8,
  QUVIPROTO_ALL  =
  (QUVIPROTO_HTTP|QUVIPROTO_MMS|QUVIPROTO_RTSP|QUVIPROTO_RTMP)
} QUVIcategory;

#define QUVIINFO_VOID       0x100000 /**< void type */
#define QUVIINFO_LONG       0x200000 /**< long type */
#define QUVIINFO_STRING     0x300000 /**< string type */
#define QUVIINFO_DOUBLE     0x400000 /**< double type */
#define QUVIINFO_TYPEMASK   0xf00000 /**< type mask */

/*
 * Enum: Info types
 *
 * QUVIINFO_CURL         - libcurl session handle
 * QUVIINFO_RESPONSECODE - Last response code returned by server
 * QUVIINFO_CURLCODE     - Deprecated since 0.2.16, removed in 0.4.0
 * QUVIINFO_HTTPCODE     - Deprecated since 0.2.16, removed in 0.4.0
 */
typedef enum
{
  QUVIINFO_NONE         = 0x00,
  QUVIINFO_CURL         = QUVIINFO_VOID+1,
  QUVIINFO_RESPONSECODE = QUVIINFO_LONG+3
  /* Add new ones below */
} QUVIinfo;

#define QUVIPROPERTY_STRING     0x100000 /**< string type */
#define QUVIPROPERTY_LONG       0x200000 /**< long type */
#define QUVIPROPERTY_DOUBLE     0x300000 /**< double type */
#define QUVIPROPERTY_VOID       0x400000 /**< void type */
#define QUVIPROPERTY_TYPEMASK   0xf00000 /**< type mask */

/*
 * Enum: Media properties
 *
 * QUVIPROP_HOSTID    - Host (or website) ID, typically domain name
 * QUVIPROP_PAGEURL   - Page URL
 * QUVIPROP_PAGETITLE - Page title
 * QUVIPROP_MEDIAID   - Media ID
 * QUVIPROP_MEDIAURL  - Media URL
 * QUVIPROP_MEDIACONTENTLENGTH - Media content length in bytes
 * QUVIPROP_MEDIACONTENTTYPE   - Media content-type
 * QUVIPROP_FILESUFFIX   - File suffix, parsed from the content-type
 * QUVIPROP_RESPONSECODE - Last response code returned by server
 * QUVIPROP_FORMAT    - _Requested_ format, see <QUVIOPT_FORMAT>
 * QUVIPROP_STARTTIME - Start time for media, if any
 * QUVIPROP_MEDIATHUMBNAILURL - Media cover/thumbnail URL, if any
 * QUVIPROP_MEDIADURATION - Media duration in msecs, if any (0)
 * QUVIPROP_VIDEOID - Deprecated since 0.2.15, use <QUVIPROP_MEDIAID> instead
 * QUVIPROP_VIDEOURL - Deprecated since 0.2.15, use <QUVIPROP_MEDIAURL> instead
 * QUVIPROP_VIDEOFILELENGTH - Deprecated since 0.2.15,
 *                              use <QUVIPROP_MEDIACONTENTLENGTH> instead
 * QUVIPROP_VIDEOFILESUFFIX - Deprecated since 0.2.15,
 *                              use <QUVIPROP_FILESUFFIX> instead
 * QUVIPROP_VIDEOFORMAT - Deprecated since 0.2.15,
 *                          use <QUVIPROP_FORMAT> instead
 * QUVIPROP_HTTPCODE    - Deprecated since 0.2.16,
 *                          use <QUVIPROP_RESPONSECODE> instead
 */
typedef enum
{
  QUVIPROP_NONE               = 0x00,
  QUVIPROP_HOSTID             = QUVIPROPERTY_STRING+1,
  QUVIPROP_PAGEURL            = QUVIPROPERTY_STRING+2,
  QUVIPROP_PAGETITLE          = QUVIPROPERTY_STRING+3,
  QUVIPROP_MEDIAID            = QUVIPROPERTY_STRING+4,
  QUVIPROP_MEDIAURL           = QUVIPROPERTY_STRING+5,
  QUVIPROP_MEDIACONTENTLENGTH = QUVIPROPERTY_DOUBLE+6,
  QUVIPROP_MEDIACONTENTTYPE   = QUVIPROPERTY_STRING+7,
  QUVIPROP_FILESUFFIX         = QUVIPROPERTY_STRING+8,
  QUVIPROP_RESPONSECODE       = QUVIPROPERTY_LONG+9,
  QUVIPROP_FORMAT             = QUVIPROPERTY_STRING+10,
  QUVIPROP_STARTTIME          = QUVIPROPERTY_STRING+11,
  QUVIPROP_MEDIATHUMBNAILURL  = QUVIPROPERTY_STRING+12,
  QUVIPROP_MEDIADURATION      = QUVIPROPERTY_DOUBLE+13
  /* Add new ones below */
} QUVIproperty;

/*
 * Enum: Ident properties
 *
 * QUVI_IDENT_PROPERTY_URL        - URL
 * QUVI_IDENT_PROPERTY_DOMAIN     - Domain
 * QUVI_IDENT_PROPERTY_FORMATS    - Formats
 * QUVI_IDENT_PROPERTY_CATEGORIES - Protocol categories
 */
typedef enum
{
  QUVI_IDENT_PROPERTY_NONE = 0x00,
  QUVI_IDENT_PROPERTY_URL  = QUVIPROPERTY_STRING+1,
  QUVI_IDENT_PROPERTY_DOMAIN     = QUVIPROPERTY_STRING+2,
  QUVI_IDENT_PROPERTY_FORMATS    = QUVIPROPERTY_STRING+3,
  QUVI_IDENT_PROPERTY_CATEGORIES = QUVIPROPERTY_STRING+4
} QUVIidentProperty;

/*
 * Typedef: Handles
 *
 * quvi_t       - Session handle
 * quvi_media_t - Media handle
 * quvi_ident_t - Ident handle
 * quvi_video_t - Deprecated since 0.2.15, use quvi_media_t instead
 */
typedef void *quvi_t;
typedef void *quvi_media_t;
typedef void *quvi_ident_t;

/*
 * Typedef: Callbacks
 *
 * quvi_callback_status - Status callback function
 */
typedef int (*quvi_callback_status) (long, void*);

/*
 * Typedef: Other
 *
 * quvi_word - Word type
 * quvi_byte - Byte type
 */
typedef uint32_t quvi_word;
typedef uint8_t quvi_byte;

/*
 * Macro: Support macros
 *
 * quvi_lobyte - Return a low byte from a word type variable
 * quvi_hibyte - Return a high byte from a word type variable
 * quvi_loword - Return a low word from a long type variable
 * quvi_hiword - Return a high word from a long type variable
 */
#define quvi_lobyte(w) ((quvi_byte)((uint64_t)(w) & 0xff))
#define quvi_hibyte(w) ((quvi_byte)((uint64_t)(w) >> 8))
#define quvi_loword(l) ((quvi_word)((uint64_t)(l) & 0xffff))
#define quvi_hiword(l) ((quvi_word)((uint64_t)(l) >> 16))

#ifdef __cplusplus
extern "C" {
#endif

  QUVIcode quvi_init(quvi_t*);
  void     quvi_close(quvi_t*);

  QUVIcode quvi_setopt(quvi_t, QUVIoption, ...);
  QUVIcode quvi_getinfo(quvi_t, QUVIinfo, ...);

  QUVIcode quvi_parse(quvi_t, char *url, quvi_media_t*);
  void     quvi_parse_close(quvi_media_t*);

  QUVIcode quvi_getprop(quvi_media_t, QUVIproperty, ...);

  /* Support functions */
  QUVIcode quvi_query_formats(quvi_t, char *url, char **formats);

  QUVIcode quvi_next_media_url(quvi_media_t);

  QUVIcode quvi_supported(quvi_t, char *url);
  QUVIcode quvi_supported_ident(quvi_t, char *url, quvi_ident_t*);
  QUVIcode quvi_ident_getprop(quvi_ident_t, QUVIidentProperty, ...);
  void     quvi_supported_ident_close(quvi_ident_t*);

  QUVIcode quvi_next_supported_website(quvi_t, char **domain, char **formats);
  QUVIcode quvi_next_host(char **domain, char **formats);

  char *quvi_strerror(quvi_t, QUVIcode);
  char *quvi_version(QUVIversion);
  void quvi_free(void*);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* quvi_h */

/* vim: set ts=2 sw=2 tw=72 expandtab: */
