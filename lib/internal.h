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

#ifndef internal_h
#define internal_h

#include <lua.h>

#include "platform.h"
#include "llst.h"

#define makelong(low,high) \
    ((long)   (((quvi_word)((uint64_t)(low)  & 0xffff)) | \
    ((uint64_t)((quvi_word)((uint64_t)(high) & 0xffff))) << 16))

#define makeword(low,high) \
    ((quvi_word)(((quvi_byte)((uint64_t)(low)  & 0xff)) | \
    ((quvi_word)((quvi_byte) ((uint64_t)(high) & 0xff))) << 8))

#define _free(p) \
    do { \
        if (p) { free(p); p=0; } \
    } while(0)

struct _quvi_s
{
  char *format;
  long no_verify;
  long no_shortened;
  long category;
  quvi_callback_status status_func;
  quvi_callback_write write_func;
  void *curl;
  long httpcode;
  long curlcode;
  char *errmsg;
  llst_node_t util_scripts;
  llst_node_t website_scripts;
  llst_node_t curr_next_host;
  lua_State *lua;
};

typedef struct _quvi_s *_quvi_t;

struct _quvi_video_link_s
{
  char *url;
  char *suffix;
  char *content_type;
  double length;
};

typedef struct _quvi_video_link_s *_quvi_video_link_t;

struct _quvi_video_s
{
  _quvi_t quvi;
  char *id;
  char *title;
  char *start_time;
  char *thumbnail_url;
  double duration;
  char *charset;
  char *page_link;
  llst_node_t link;             /* holds all essential to video links */
  llst_node_t curr;             /* current (link) node */
  char *host_id;
  char *redirect_url;
};

typedef struct _quvi_video_s *_quvi_media_t;

struct _quvi_lua_script_s
{
  char *basename;
  char *path;
};

typedef struct _quvi_lua_script_s *_quvi_lua_script_t;

#endif
/* vim: set ts=2 sw=2 tw=72 expandtab: */
