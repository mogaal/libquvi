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

#ifndef qversion_h
#define qversion_h

/** @file qversion.h */

/** @enum QuviVersion */
typedef enum
{
  QUVI_VERSION,
  QUVI_VERSION_CONFIGURATION,
  QUVI_VERSION_BUILD_CC_CFLAGS,
  QUVI_VERSION_BUILD_TARGET,
  QUVI_VERSION_BUILD_TIME,
  QUVI_VERSION_SCRIPTS_CONFIGURATION, /**< libquvi-scripts build options */
  QUVI_VERSION_SCRIPTS  /**< libquvi-scripts version */
} QuviVersion;

#endif /* qversion_h */

/* vim: set ts=2 sw=2 tw=72 expandtab: */
