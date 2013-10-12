/* libquvi
 * Copyright (C) 2012-2013  Toni Gundogdu <legatvs@gmail.com>
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

#ifndef qscript_h
#define qscript_h

/** @file qscript.h */

/** @cond NODOC */
#define QUVI_SCRIPT_PROPERTY_TYPE_STRING 0x100000
#define QUVI_SCRIPT_PROPERTY_TYPE_LONG   0x200000
#define QUVI_SCRIPT_PROPERTY_TYPE_DOUBLE 0x300000
#define QUVI_SCRIPT_PROPERTY_TYPE_VOID   0x400000
#define QUVI_SCRIPT_PROPERTY_MASK        0xf00000
/** @endcond */

/** @enum QuviScriptType */
typedef enum
{
  QUVI_SCRIPT_TYPE_SUBTITLE_EXPORT,
  QUVI_SCRIPT_TYPE_SUBTITLE,
  QUVI_SCRIPT_TYPE_PLAYLIST,
  QUVI_SCRIPT_TYPE_MEDIA,
  QUVI_SCRIPT_TYPE_SCAN
} QuviScriptType;

/** @enum QuviScriptProperty */
typedef enum
{
  QUVI_SCRIPT_PROPERTY_EXPORT_FORMAT = QUVI_SCRIPT_PROPERTY_TYPE_STRING+1,
  /**< Unique to export scripts, e.g. @ref subex_script */
  QUVI_SCRIPT_PROPERTY_FILEPATH,
  QUVI_SCRIPT_PROPERTY_FILENAME,
  QUVI_SCRIPT_PROPERTY_DOMAINS, /**< Unique to @ref m_script and @ref pl_script */
  QUVI_SCRIPT_PROPERTY_SHA1
} QuviScriptProperty;

#endif /* qscript_h */

/* vim: set ts=2 sw=2 tw=72 expandtab: */
