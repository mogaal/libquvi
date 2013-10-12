/* libquvi
 * Copyright (C) 2012,2013  Toni Gundogdu <legatvs@gmail.com>
 *
 * This file is part of libquvi <http://quvi.sourceforge.net/>.
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General
 * Public License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef qdef_h
#define qdef_h

/** @file qdef.h */

typedef int (*quvi_callback_status)(long,void*,void*); /**< Status callback type */

typedef void *quvi_subtitle_export_t; /**< Subtitle export handle type */
typedef void *quvi_subtitle_type_t; /**< Subtitle type handle type */
typedef void *quvi_subtitle_lang_t; /**< Subtitle language handle type */
typedef void *quvi_http_metainfo_t; /**< HTTP meta-info type */
typedef void *quvi_file_ext_t; /**< File extension type */
typedef void *quvi_playlist_t; /**< Playlist script type */
typedef void *quvi_subtitle_t; /**< Subtitle handle type */
typedef void *quvi_resolve_t; /**< Resolve type */
typedef void *quvi_media_t; /**< Media type */
typedef void *quvi_scan_t; /**< Scan type */
typedef void *quvi_t; /**< Library handle type */

typedef uint32_t quvi_word; /**< Word type */
typedef uint8_t quvi_byte; /**< Byte type */

#define quvi_lobyte(w) ((quvi_byte)((uint64_t)(w) & 0xff)) /**< Return low byte */
#define quvi_hibyte(w) ((quvi_byte)((uint64_t)(w) >> 8)) /**< Return high byte */
#define quvi_loword(l) ((quvi_word)((uint64_t)(l) & 0xffff)) /**< Return low word */
#define quvi_hiword(l) ((quvi_word)((uint64_t)(l) >> 16)) /**< Return high word */

#endif /* qdef_h */

/* vim: set ts=2 sw=2 tw=72 expandtab: */
