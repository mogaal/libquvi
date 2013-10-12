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

#ifndef qinfo_h
#define qinfo_h

/** @file qinfo.h */

/** @cond NODOC */
#define QUVI_INFO_TYPE_VOID   0x100000
#define QUVI_INFO_TYPE_LONG   0x200000
#define QUVI_INFO_TYPE_STRING 0x300000
#define QUVI_INFO_TYPE_DOUBLE 0x400000
#define QUVI_INFO_TYPE_MASK   0xf00000
/** @endcond */

/** @enum QuviInfo */
typedef enum
{
  /* long */
  QUVI_INFO_RESPONSE_CODE = QUVI_INFO_TYPE_LONG+1,
    /**< Last server response code */
  /* void */
  QUVI_INFO_CURL_HANDLE =
  ((QUVI_INFO_RESPONSE_CODE - QUVI_INFO_TYPE_LONG) + QUVI_INFO_TYPE_VOID + 0x20)
    /**< Current @ref sess_handle */
} QuviInfo;

#endif /* qinfo_h */

/* vim: set ts=2 sw=2 tw=72 expandtab: */
