/* libquvi
 * Copyright (C) 2013  Toni Gundogdu <legatvs@gmail.com>
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

#ifndef qsubtprop_h
#define qsubtprop_h

/** @file qsubtprop.h */

/** @enum QuviSubtitleType */
typedef enum
{
  QUVI_SUBTITLE_TYPE_NONE,
  QUVI_SUBTITLE_TYPE_TTS,
  QUVI_SUBTITLE_TYPE_CC
} QuviSubtitleType;

/** @enum QuviSubtitleFormat (internal) */
typedef enum
{
  QUVI_SUBTITLE_FORMAT_NONE,
  QUVI_SUBTITLE_FORMAT_TT
} QuviSubtitleFormat;

/** @cond NODOC */
#define QUVI_SUBTITLE_PROPERTY_TYPE_STRING 0x100000
#define QUVI_SUBTITLE_PROPERTY_TYPE_LONG   0x200000
#define QUVI_SUBTITLE_PROPERTY_TYPE_DOUBLE 0x300000
#define QUVI_SUBTITLE_PROPERTY_TYPE_VOID   0x400000
#define QUVI_SUBTITLE_PROPERTY_TYPE_MASK   0xf00000
/** @endcond */

/** @enum QuviSubtitleTypeProperty */
typedef enum
{
  /* double */
  QUVI_SUBTITLE_TYPE_PROPERTY_FORMAT = QUVI_SUBTITLE_PROPERTY_TYPE_DOUBLE+1,
  QUVI_SUBTITLE_TYPE_PROPERTY_TYPE
} QuviSubtitleTypeProperty;

/** @enum QuviSubtitleLangProperty */
typedef enum
{
  /* string */
  QUVI_SUBTITLE_LANG_PROPERTY_TRANSLATED = QUVI_SUBTITLE_PROPERTY_TYPE_STRING+1,
  QUVI_SUBTITLE_LANG_PROPERTY_ORIGINAL,
  QUVI_SUBTITLE_LANG_PROPERTY_CODE,
  QUVI_SUBTITLE_LANG_PROPERTY_URL,
  QUVI_SUBTITLE_LANG_PROPERTY_ID
} QuviSubtitleLangProperty;

#endif /* qsubtprop_h */

/* vim: set ts=2 sw=2 tw=72 expandtab: */
