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

#ifndef qsupp_h
#define qsupp_h

/** @file qsupp.h */

/** @enum QuviSupportsMode */
typedef enum
{
  QUVI_SUPPORTS_MODE_OFFLINE, /**< Networkless check */
  QUVI_SUPPORTS_MODE_ONLINE   /**< Resolve URL prior to check */
} QuviSupportsMode;

/** @enum QuviSupportsType */
typedef enum
{
  QUVI_SUPPORTS_TYPE_PLAYLIST = 1<<0,
  QUVI_SUPPORTS_TYPE_SUBTITLE = 1<<1,
  QUVI_SUPPORTS_TYPE_MEDIA = 1<<2,
  QUVI_SUPPORTS_TYPE_ANY =
  (QUVI_SUPPORTS_TYPE_PLAYLIST
   |QUVI_SUPPORTS_TYPE_SUBTITLE
   |QUVI_SUPPORTS_TYPE_MEDIA)
} QuviSupportsType;

#endif /* qsupp_h */

/* vim: set ts=2 sw=2 tw=72 expandtab: */
