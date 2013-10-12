src=crypto.c init.c
hdr=crypto.h

noinst_LTLIBRARIES=libconvenience_gcrypt.la
libconvenience_gcrypt_la_SOURCES=$(src) $(hdr)

libconvenience_gcrypt_la_CPPFLAGS=\
  -DLOCALEDIR=\""$(localedir)"\"\
  -I$(top_srcdir)/src/quvi-0.9/\
  -I$(top_srcdir)/src/\
  $(LIBGCRYPT_CFLAGS)\
  $(liblua_CFLAGS)\
  $(glib_CFLAGS)\
  $(AM_CPPFLAGS)

libconvenience_gcrypt_la_CFLAGS=$(AM_CFLAGS)

libconvenience_gcrypt_la_LDFLAGS=\
  $(AM_LDFLAGS)

libconvenience_gcrypt_la_LIBADD=\
  $(LIBGCRYPT_LIBS)\
  $(liblua_LIBS)\
  $(glib_LIBS)

# vim: set ts=2 sw=2 tw=72 expandtab:
