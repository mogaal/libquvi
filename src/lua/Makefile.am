src=\
  chk.c\
  exec_media_script_ident.c\
  exec_media_script_parse.c\
  exec_playlist_script_ident.c\
  exec_playlist_script_parse.c\
  exec_scan_script_parse.c\
  exec_subtitle_export_script_export.c\
  exec_subtitle_export_script_ident.c\
  exec_subtitle_script_ident.c\
  exec_subtitle_script_parse.c\
  getfield.c\
  init.c\
  load_util_script.c\
  match_url_to_media_script.c\
  match_url_to_playlist_script.c\
  match_url_to_subtitle_script.c\
  modify_pkgpath.c\
  quvi/base64/decode.c\
  quvi/base64/encode.c\
  quvi/crypto/copts.c\
  quvi/crypto/en_decrypt.c\
  quvi/crypto/err.c\
  quvi/crypto/hash.c\
  quvi/http/cookie.c\
  quvi/http/fetch.c\
  quvi/http/header.c\
  quvi/http/metainfo.c\
  quvi/http/resolve.c\
  quvi/opts.c\
  setfield.c\
  util/exec_util_convert_entities.c\
  util/exec_util_resolve_redirections.c\
  util/exec_util_to_file_ext.c

hdr=\
  chk.h\
  def.h\
  exec.h\
  getfield.h\
  load_util_script.h\
  quvi/crypto/err.h\
  quvi/crypto/opts.h\
  quvi/opts.h\
  setfield.h

noinst_LTLIBRARIES=libconvenience_lua.la
libconvenience_lua_la_SOURCES=$(src) $(hdr)

libconvenience_lua_la_CPPFLAGS=\
  -DLOCALEDIR=\""$(localedir)"\"\
  -I$(top_srcdir)/src/quvi-0.9/\
  -I$(top_srcdir)/src/\
  $(liblua_CFLAGS)\
  $(glib_CFLAGS)\
  $(AM_CPPFLAGS)

libconvenience_lua_la_CFLAGS=$(AM_CFLAGS)

libconvenience_lua_la_LDFLAGS=\
  $(AM_LDFLAGS)

libconvenience_lua_la_LIBADD=\
  $(liblua_LIBS)\
  $(glib_LIBS)

# vim: set ts=2 sw=2 tw=72 expandtab:
