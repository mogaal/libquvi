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

#ifndef net_h

_quvi_net_t new_net_handle();

void free_net_handle(_quvi_net_t *n);

QUVIcode fetch_wrapper(_quvi_t, lua_State*, _quvi_net_t*);
QUVIcode resolve_wrapper(_quvi_t, const char*, char **);
QUVIcode verify_wrapper(_quvi_t, _quvi_llst_node_t);

#endif

/* vim: set ts=2 sw=2 tw=72 expandtab: */
