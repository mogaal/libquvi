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

#ifndef qfunc_h
#define qfunc_h

#ifdef __cplusplus
extern "C" {
#endif

  quvi_t quvi_new(void);
  void quvi_free(quvi_t);

  void quvi_set(quvi_t, QuviOption, ...);
  void quvi_get(quvi_t, QuviInfo, ...);

  /* Script */

  QuviBoolean quvi_script_next(quvi_t, QuviScriptType);
  void quvi_script_get(quvi_t, QuviScriptType, QuviScriptProperty, ...);

  /* Media */

  quvi_media_t quvi_media_new(quvi_t, const char*);
  void quvi_media_free(quvi_media_t);

  void quvi_media_get(quvi_media_t, QuviMediaProperty, ...);

  QuviBoolean quvi_media_stream_next(quvi_media_t);
  void quvi_media_stream_reset(quvi_media_t);

  void quvi_media_stream_select(quvi_media_t, const char*);
  void quvi_media_stream_choose_best(quvi_media_t);

  /* Playlist */

  quvi_playlist_t quvi_playlist_new(quvi_t, const char*);
  void quvi_playlist_free(quvi_playlist_t);

  void quvi_playlist_get(quvi_playlist_t, QuviPlaylistProperty, ...);

  QuviBoolean quvi_playlist_media_next(quvi_playlist_t);
  void quvi_playlist_media_reset(quvi_playlist_t);

  /* Scan */

  quvi_scan_t quvi_scan_new(quvi_t, const char*);
  void quvi_scan_free(quvi_scan_t);

  const char *quvi_scan_next_media_url(quvi_scan_t);

  /* Resolve */

  quvi_resolve_t quvi_resolve_new(quvi_t, const char*);
  void quvi_resolve_free(quvi_resolve_t);

  QuviBoolean quvi_resolve_forwarded(quvi_resolve_t);
  const char *quvi_resolve_destination_url(quvi_resolve_t);

  /* HTTP meta-info */

  quvi_http_metainfo_t quvi_http_metainfo_new(quvi_t, const char*);
  void quvi_http_metainfo_free(quvi_http_metainfo_t);

  void quvi_http_metainfo_get(quvi_http_metainfo_t,
                              QuviHTTPMetaInfoProperty, ...);

  /* Subtitle */

  quvi_subtitle_t quvi_subtitle_new(quvi_t, const char*);
  void quvi_subtitle_free(quvi_subtitle_t);

  const quvi_subtitle_type_t quvi_subtitle_type_next(quvi_subtitle_t);
  const quvi_subtitle_lang_t quvi_subtitle_lang_next(quvi_subtitle_type_t);

  void quvi_subtitle_type_get(quvi_subtitle_type_t,
                              QuviSubtitleTypeProperty, ...);

  void quvi_subtitle_lang_get(quvi_subtitle_lang_t,
                              QuviSubtitleLangProperty, ...);

  void quvi_subtitle_type_reset(quvi_subtitle_t);
  void quvi_subtitle_lang_reset(quvi_subtitle_type_t);

  const quvi_subtitle_lang_t quvi_subtitle_select(quvi_subtitle_t, const char*);

  /* Subtitle - export */

  quvi_subtitle_export_t quvi_subtitle_export_new(quvi_subtitle_lang_t, char*);
  void quvi_subtitle_export_free(quvi_subtitle_export_t);

  const char *quvi_subtitle_export_data(quvi_subtitle_export_t);

  /* Convenience - file extension */

  quvi_file_ext_t quvi_file_ext_new(quvi_t, const char*);
  void quvi_file_ext_free(quvi_file_ext_t);

  const char *quvi_file_ext_get(quvi_file_ext_t);

  /* Convenience */

  QuviBoolean quvi_supports(quvi_t, const char*,
                            QuviSupportsMode, QuviSupportsType);

  const char *quvi_version(QuviVersion);

  const char *quvi_errmsg(quvi_t);
  QuviError quvi_errcode(quvi_t);
  QuviBoolean quvi_ok(quvi_t);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* qfunc_h */

/* vim: set ts=2 sw=2 tw=72 expandtab: */
