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

#ifndef _quvi_script_h
#define _quvi_script_h

struct _quvi_script_s
{
  struct
  {
    GString *format; /* Subtitle export, ... */
  } export;
  GString *domains; /* Media, playlist, ... */
  GString *fpath;
  GString *fname;
  GString *sha1;
};

typedef struct _quvi_script_s *_quvi_script_t;

#endif /* _quvi_script_h */

/* vim: set ts=2 sw=2 tw=72 expandtab: */
