/* quvi
 * Copyright (C) 2010  Toni Gundogdu <legatvs@gmail.com>
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

#ifndef lua_wrap_h
#define lua_wrap_h

int init_lua(_quvi_t);

void free_lua(_quvi_t *);

struct lua_ident_s
{
  _quvi_t quvi;
  char *url;
  char *domain;
  char *formats;
  long categories;
};

typedef struct lua_ident_s *lua_ident_t;

QUVIcode run_lua_suffix_func(_quvi_t, _quvi_video_link_t);

QUVIcode run_lua_charset_func(_quvi_media_t, const char *);

QUVIcode run_ident_func(lua_ident_t, llst_node_t);

QUVIcode find_host_script_and_parse(_quvi_media_t);

QUVIcode find_host_script(_quvi_media_t);

#endif
/* vim: set ts=2 sw=2 tw=72 expandtab: */
