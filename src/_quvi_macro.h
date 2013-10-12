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

#ifndef quvi_macro_h
#define quvi_macro_h

#define q_makelong(low,high) \
    ((long)   (((quvi_word)((uint64_t)(low)  & 0xffff)) | \
    ((uint64_t)((quvi_word)((uint64_t)(high) & 0xffff))) << 16))

#define q_makeword(low,high) \
    ((quvi_word)(((quvi_byte)((uint64_t)(low)  & 0xff)) | \
    ((quvi_word)((quvi_byte) ((uint64_t)(high) & 0xff))) << 8))

#endif /* quvi_macro_h */

/* vim: set ts=2 sw=2 tw=72 expandtab: */
