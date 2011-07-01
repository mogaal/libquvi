
if ENABLE_EXAMPLES
noinst_PROGRAMS=    simple

simple_SOURCES=     simple.c common.c common.h
simple_CPPFLAGS=    -I$(top_builddir)/include $(AM_CPPFLAGS)
simple_CFLAGS=      $(AM_CFLAGS)
simple_LDFLAGS=     $(top_builddir)/lib/libquvi.la $(AM_LDFLAGS)

if WITH_SOUP
noinst_PROGRAMS += callback_libsoup

callback_libsoup_SOURCES=   callback_libsoup.c common.c common.h

callback_libsoup_CPPFLAGS=  \
    -I$(top_builddir)/include -I$(top_srcdir)/include \
    $(libsoup_CFLAGS) $(AM_CPPFLAGS)

callback_libsoup_CFLAGS=    $(AM_CFLAGS)

callback_libsoup_LDFLAGS=   \
    $(top_builddir)/lib/libquvi.la $(libsoup_LIBS) $(AM_LDFLAGS)
endif # WITH_SOUP

endif # ENABLE_EXAMPLES


