/* libquvi
 * Copyright (C) 2009-2010  Toni Gundogdu <legatvs@gmail.com>
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

#ifndef util_h
#define util_h

char *vafreprintf(char **dst, const char *fmt, va_list);
char *freprintf(char **dst, const char *fmt, ...);

QUVIcode to_utf8(_quvi_media_t);

char *strepl(const char *s, const char *what, const char *with);
int add_media_url(_quvi_llst_node_t * lst, const char *fmt, ...);
char *unescape(_quvi_t quvi, char *s);
char *from_html_entities(char *src);

#endif
/* vim: set ts=2 sw=2 tw=72 expandtab: */
