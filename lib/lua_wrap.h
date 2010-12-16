/* 
* Copyright (C) 2010 Toni Gundogdu.
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

#ifndef lua_wrap_h
#define lua_wrap_h

int init_lua(_quvi_t);

void free_lua(_quvi_t *);

struct lua_ident_s {
  _quvi_t quvi;
  char *url;
  char *domain;
  char *formats;
};

typedef struct lua_ident_s *lua_ident_t;

QUVIcode run_lua_suffix_func(_quvi_t, _quvi_video_link_t);

QUVIcode run_lua_charset_func(_quvi_video_t, const char *);

QUVIcode run_ident_func(lua_ident_t, llst_node_t);

QUVIcode find_host_script_and_parse(_quvi_video_t);

QUVIcode find_host_script(_quvi_video_t);

#endif
