src=\
  env.c\
  qerr.c\
  re.c

hdr=tests.h

noinst_LTLIBRARIES=libconvenience_test.la
libconvenience_test_la_SOURCES=$(src) $(hdr)

libconvenience_test_la_CPPFLAGS=\
  -I$(top_srcdir)/tests/lib/\
  -I$(top_srcdir)/src/quvi-0.9/\
  $(libcurl_CFLAGS)\
  $(glib_CFLAGS)\
  $(AM_CPPFLAGS)

libconvenience_test_la_LDFLAGS=\
  $(AM_LDFLAGS)

libconvenience_test_la_LIBADD=\
  $(libcurl_LIBS)\
  $(glib_LIBS)

# vim: set ts=2 sw=2 tw=72 expandtab:
