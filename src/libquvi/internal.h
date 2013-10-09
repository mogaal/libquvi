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

#ifndef internal_h
#define internal_h

#include <lua.h>

#include "platform.h"

#define _makelong(low,high) \
    ((long)   (((quvi_word)((uint64_t)(low)  & 0xffff)) | \
    ((uint64_t)((quvi_word)((uint64_t)(high) & 0xffff))) << 16))

#define _makeword(low,high) \
    ((quvi_word)(((quvi_byte)((uint64_t)(low)  & 0xff)) | \
    ((quvi_word)((quvi_byte) ((uint64_t)(high) & 0xff))) << 8))

#define _free(p) \
    do { if (p) free(p); p=0; } while(0)

#define _is_invarg(p) \
  do { if (p == NULL) return (QUVI_INVARG); } while (0)

#define _is_badhandle(h) \
  do { if (h == NULL) return (QUVI_BADHANDLE); } while (0)

#define _set_alloc_s(dst) \
    do { freprintf(&dst, "%s", va_arg(arg,char*)); } while(0); break

#define _set_s(value) \
    do { *sp = value ? value:(char*)empty; } while(0); break

#define _set_from_arg_n(dst) \
    do { dst = va_arg(arg,long); } while(0); break

#define _set_from_value_n(dst,value) \
    do { *dst = value; } while(0); break

#define _init_from_arg(dst,type) \
    do { \
        if ( !(dst = va_arg(arg,type)) ) \
            rc = QUVI_INVARG; \
    } while (0); break

#define _set_v(value) \
    do  { *vp = value ? value:NULL; } while(0); break

/* linked list handle */
struct _quvi_llst_node_s
{
  struct _quvi_llst_node_s *next;
  struct _quvi_llst_node_s *prev;
  void *data;
};

typedef struct _quvi_llst_node_s *_quvi_llst_node_t;

/* quvi handle */
struct _quvi_s
{
  quvi_callback_resolve resolve_func;
  quvi_callback_status status_func;
  quvi_callback_verify verify_func;
  quvi_callback_fetch fetch_func;
  _quvi_llst_node_t website_scripts;
  _quvi_llst_node_t curr_website;
  _quvi_llst_node_t util_scripts;
  long no_resolve;
  long no_verify;
  lua_State *lua;
  long resp_code;
  long category;
  long curlcode;
  char *format;
  char *errmsg;
  void *curl;
};

typedef struct _quvi_s *_quvi_t;

/* quvi media url handle */
struct _quvi_media_url_s
{
  char *content_type;
  double length;
  char *suffix;
  char *url;
};

typedef struct _quvi_media_url_s *_quvi_media_url_t;

/* quvi media handle */
struct _quvi_media_s
{
  _quvi_llst_node_t curr;             /* current (url) node */
  _quvi_llst_node_t url;             /* holds all essential to media urls */
  char *thumbnail_url;
  char *redirect_url;
  char *start_time;
  double duration;
  char *page_url;
  char *charset;
  char *host_id;
  _quvi_t quvi;
  char *title;
  char *id;
};

typedef struct _quvi_media_s *_quvi_media_t;

/* lua script handle */
struct _quvi_lua_script_s
{
  char *basename;
  char *path;
};

typedef struct _quvi_lua_script_s *_quvi_lua_script_t;

/* quvi net handle */
struct _quvi_net_s
{
  _quvi_llst_node_t features;
  long resp_code;
  char *errmsg;
  char *url;
  struct
  {
    char *content;
  } fetch;
  struct
  {
    char *url;
  } redirect;
  struct
  {
    char *content_type;
    double content_length;
  } verify;
};

typedef struct _quvi_net_s *_quvi_net_t;

/* quvi net property option handle */
struct _quvi_net_propfeat_s
{
  char *name;
  char *value;
};

typedef struct _quvi_net_propfeat_s *_quvi_net_propfeat_t;

/* quvi webscript ident handle */
struct _quvi_ident_s
{
  long categories;
  char *formats;
  char *domain;
  _quvi_t quvi;
  char *url;
};

typedef struct _quvi_ident_s *_quvi_ident_t;

#endif

/* vim: set ts=2 sw=2 tw=72 expandtab: */
