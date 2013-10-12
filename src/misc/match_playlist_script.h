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

#ifndef m_match_playlist_script_h
#define m_match_playlist_script_h

typedef enum
{
  QM_MATCH_PS_SUPPORTED_OFFLINE,
  QM_MATCH_PS_SUPPORTED_ONLINE,
  QM_MATCH_PS_PARSE
} QuviMatchPlaylistScriptMode;

QuviError m_match_playlist_script(_quvi_t, _quvi_playlist_t*, const gchar*,
                                  const QuviMatchPlaylistScriptMode);

#endif /* m_match_playlist_script_h */

/* vim: set ts=2 sw=2 tw=72 expandtab: */
