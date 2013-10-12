src=\
  capture.c\
  match.c\
  match_media_script.c\
  match_playlist_script.c\
  match_subtitle_export_script.c\
  match_subtitle_script.c\
  media.c\
  playlist.c\
  resolve.c\
  scan_new.c\
  scan_scripts.c\
  script_free.c\
  slst.c\
  subtitle.c\
  subtitle_export.c\
  to_utf8.c\
  trim.c\
  unescape.c

hdr=\
  match_media_script.h\
  match_playlist_script.h\
  match_subtitle_export_script.h\
  match_subtitle_script.h\
  media.h\
  playlist.h\
  re.h\
  resolve.h\
  scan_new.h\
  script_free.h\
  slst.h\
  subtitle_export.h\
  subtitle.h\
  unescape.h

noinst_LTLIBRARIES=libconvenience_misc.la
libconvenience_misc_la_SOURCES=$(src) $(hdr)

libconvenience_misc_la_CPPFLAGS=\
  -DLOCALEDIR=\""$(localedir)"\"\
  -I$(top_srcdir)/src/quvi-0.9/\
  -I$(top_srcdir)/src/\
  $(glib_CFLAGS)\
  $(AM_CPPFLAGS)

libconvenience_misc_la_CFLAGS=$(AM_CFLAGS)

libconvenience_misc_la_LDFLAGS=\
  $(AM_LDFLAGS)

libconvenience_misc_la_LIBADD=\
  $(glib_LIBS)

# vim: set ts=2 sw=2 tw=72 expandtab:
