src=\
  fetch.c\
  handle.c\
  http_metainfo.c\
  resolve.c

hdr=\
  fetch.h\
  handle.h\
  resolve.h

noinst_LTLIBRARIES=libconvenience_net.la
libconvenience_net_la_SOURCES=$(src) $(hdr)

libconvenience_net_la_CPPFLAGS=\
  -DLOCALEDIR=\""$(localedir)"\"\
  -I$(top_srcdir)/src/quvi-0.9/\
  -I$(top_srcdir)/src/\
  $(liblua_CFLAGS)\
  $(glib_CFLAGS)\
  $(AM_CPPFLAGS)

libconvenience_net_la_CFLAGS=$(AM_CFLAGS)

libconvenience_net_la_LDFLAGS=\
  $(AM_LDFLAGS)

libconvenience_net_la_LIBADD=\
  $(liblua_LIBS)\
  $(glib_LIBS)

# vim: set ts=2 sw=2 tw=72 expandtab:
