/* libquvi
 * Copyright (C) 2012  Toni Gundogdu <legatvs@gmail.com>
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

#ifndef qplaylistprop_h
#define qplaylistprop_h

/** @file qplaylistprop.h */

/** @cond NODOC */
#define QUVI_PLAYLIST_PROPERTY_TYPE_STRING 0x100000
#define QUVI_PLAYLIST_PROPERTY_TYPE_LONG   0x200000
#define QUVI_PLAYLIST_PROPERTY_TYPE_DOUBLE 0x300000
#define QUVI_PLAYLIST_PROPERTY_TYPE_VOID   0x400000
#define QUVI_PLAYLIST_PROPERTY_TYPE_MASK   0xf00000
/** @endcond */

/** @enum QuviPlaylistProperty */
typedef enum
{
  /* string */
  QUVI_PLAYLIST_PROPERTY_THUMBNAIL_URL = QUVI_PLAYLIST_PROPERTY_TYPE_STRING+1,
  QUVI_PLAYLIST_PROPERTY_TITLE,
  QUVI_PLAYLIST_PROPERTY_ID,
  QUVI_PLAYLIST_MEDIA_PROPERTY_TITLE,
  QUVI_PLAYLIST_MEDIA_PROPERTY_URL,
  /* double */
  QUVI_PLAYLIST_MEDIA_PROPERTY_DURATION_MS =
  ((QUVI_PLAYLIST_MEDIA_PROPERTY_URL - QUVI_PLAYLIST_PROPERTY_TYPE_STRING)
  + QUVI_PLAYLIST_PROPERTY_TYPE_DOUBLE + 0x20)
} QuviPlaylistProperty;

#endif /* qplaylistprop_h */

/* vim: set ts=2 sw=2 tw=72 expandtab: */
