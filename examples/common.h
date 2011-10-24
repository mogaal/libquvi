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

void spew_e(const char *fmt, ...);
void check_error(quvi_t, QUVIcode);
void handle_resolve_status(quvi_word type);
void handle_fetch_status(quvi_word type, void *p);
void handle_verify_status(quvi_word type);

/* vim: set ts=2 sw=2 tw=72 expandtab: */
