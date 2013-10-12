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

#ifndef _quvi_subtitle_s_h
#define _quvi_subtitle_s_h

/* subtitle */

struct _quvi_subtitle_s
{
  struct
  {
    GString *input;
  } url;
  struct
  {
    _quvi_t quvi;
  } handle;
  struct
  {
    GSList *type;
  } curr;
  GSList *types; /* of _quvi_subtitle_type_s */
};

typedef struct _quvi_subtitle_s *_quvi_subtitle_t;

/* data */

struct _quvi_subtitle_type_s
{
  struct
  {
    _quvi_t quvi;
  } handle;
  struct
  {
    GSList *language;
  } curr;
  GSList *languages; /* of _quvi_subtitle_lang_s */
  gdouble format;
  gdouble type;
};

typedef struct _quvi_subtitle_type_s *_quvi_subtitle_type_t;

/* lang */

struct _quvi_subtitle_lang_s
{
  struct
  {
    _quvi_t quvi;
  } handle;
  GString *translated;
  GString *original;
  gdouble format;
  GString *code;
  GString *url;
  GString *id;
};

typedef struct _quvi_subtitle_lang_s *_quvi_subtitle_lang_t;

#endif /* _quvi_subtitle_s_h */

/* vim: set ts=2 sw=2 tw=72 expandtab: */
