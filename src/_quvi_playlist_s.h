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

#ifndef _quvi_playlist_s_h
#define _quvi_playlist_s_h

struct _quvi_playlist_s
{
  struct
  {
    GString *thumbnail;
    GString *input;
  } url;
  struct
  {
    GString *playlist;
  } id;
  struct
  {
    _quvi_t quvi;
  } handle;
  struct
  {
    GSList *media;
  } curr;
  GString *title;
  GSList *media;
};

typedef struct _quvi_playlist_s *_quvi_playlist_t;

struct _quvi_playlist_media_s
{
  gdouble duration_ms;
  GString *title;
  GString *url;
};

typedef struct _quvi_playlist_media_s *_quvi_playlist_media_t;

#endif /* _quvi_playlist_s_h */

/* vim: set ts=2 sw=2 tw=72 expandtab: */
