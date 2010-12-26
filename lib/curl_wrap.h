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

#ifndef curl_wrap_h
#define curl_wrap_h

QUVIcode
fetch_to_mem(_quvi_video_t video, const char *url, lua_State * l, char **dst);

QUVIcode query_file_length(_quvi_t, llst_node_t);

QUVIcode is_shortened_url(_quvi_video_t);

#endif
