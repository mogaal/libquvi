src=\
  autoproxy.c\
  close.c\
  fetch.c\
  http_metainfo.c\
  init.c\
  reset.c\
  resolve.c\
  temp.c

hdr=\
  autoproxy.h\
  temp.h

noinst_LTLIBRARIES=libconvenience_curl.la
libconvenience_curl_la_SOURCES=$(src) $(hdr)

libconvenience_curl_la_CPPFLAGS=\
  -DLOCALEDIR=\""$(localedir)"\"\
  -I$(top_srcdir)/src/quvi-0.9/\
  -I$(top_srcdir)/src/\
  $(libproxy_CFLAGS)\
  $(libcurl_CFLAGS)\
  $(glib_CFLAGS)\
  $(AM_CPPFLAGS)

libconvenience_curl_la_CFLAGS=$(AM_CFLAGS)

libconvenience_curl_la_LDFLAGS=\
  $(AM_LDFLAGS)

libconvenience_curl_la_LIBADD=\
  $(libproxy_LIBS)\
  $(libcurl_LIBS)\
  $(glib_LIBS)

# vim: set ts=2 sw=2 tw=72 expandtab:
