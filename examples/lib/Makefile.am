src=\
  chk.c\
  cleanup.c\
  enable_autoproxy.c\
  enable_cookies.c\
  enable_verbose.c\
  exit_if_error.c\
  status.c\
  var.c

hdr=examples.h

noinst_LTLIBRARIES=libconvenience_example.la
libconvenience_example_la_SOURCES=$(src) $(hdr)

libconvenience_example_la_CPPFLAGS=\
  -I$(top_srcdir)/examples/lib/\
  -I$(top_srcdir)/src/quvi-0.9/\
  $(libcurl_CFLAGS)\
  $(glib_CFLAGS)\
  $(AM_CPPFLAGS)

libconvenience_example_la_LDFLAGS=\
  $(AM_LDFLAGS)

libconvenience_example_la_LIBADD=\
  $(libcurl_LIBS)\
  $(glib_LIBS)

# vim: set ts=2 sw=2 tw=72 expandtab:
