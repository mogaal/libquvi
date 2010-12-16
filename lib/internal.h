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

#define seterr(args...) \
    do { \
        _free(quvi->errmsg); \
        asprintf(&quvi->errmsg, args); \
    } while(0)

#define setvid(prop,args...) \
    do { \
        _free(prop); \
        asprintf(&prop, args); \
    } while(0)

#define csetopt(opt,param) \
    curl_easy_setopt(quvi->curl,opt,param)

struct _quvi_s {
  char *format;
  int no_verify;
  int no_shortened;
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

struct _quvi_video_link_s {
  char *url;
  char *suffix;
  char *content_type;
  double length;
};

typedef struct _quvi_video_link_s *_quvi_video_link_t;

struct _quvi_video_s {
  _quvi_t quvi;
  char *id;
  char *title;
  char *starttime;
  char *charset;
  char *page_link;
  llst_node_t link;             /* holds all essential to video links */
  llst_node_t curr;             /* current (link) node */
  char *host_id;
  char *redirect;
};

typedef struct _quvi_video_s *_quvi_video_t;

struct _quvi_lua_script_s {
  char *basename;
  char *path;
};

typedef struct _quvi_lua_script_s *_quvi_lua_script_t;

#endif
