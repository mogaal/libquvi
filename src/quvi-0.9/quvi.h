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

#ifndef quvi_h
#define quvi_h

#ifndef QUVI_VERSION_API
#define QUVI_VERSION_API 090000 /* 0.9 */
#ifdef _1
#define QUVI_VERSION_API 100000 /* 1.0 */
#endif /* _1 */
#endif

#include <stdint.h>

#include <quvi/qplaylistprop.h>
#include <quvi/qhttpmiprop.h>
#include <quvi/qmediaprop.h>
#include <quvi/qsubtprop.h>
#include <quvi/qcallback.h>
#include <quvi/qversion.h>
#include <quvi/qscript.h>
#include <quvi/qoption.h>
#include <quvi/qerror.h>
#include <quvi/qsupp.h>
#include <quvi/qinfo.h>
#include <quvi/qbool.h>
#include <quvi/qdef.h>
#include <quvi/qfunc.h>

#endif /* quvi_h */

/* vim: set ts=2 sw=2 tw=72 expandtab: */
