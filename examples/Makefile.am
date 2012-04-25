if WITH_EXAMPLES
noinst_PROGRAMS=	simple version parse supported

simple_SOURCES=		simple.c common.c common.h
simple_CPPFLAGS=	-I$(top_srcdir)/src/libquvi $(AM_CPPFLAGS)
simple_CFLAGS=		$(AM_CFLAGS)
simple_LDFLAGS=		$(top_builddir)/src/libquvi/libquvi.la $(AM_LDFLAGS)

version_SOURCES=	version.c
version_CPPFLAGS=	-I$(top_srcdir)/src/libquvi $(AM_CPPFLAGS)
version_CFLAGS=		$(AM_CFLAGS)
version_LDFLAGS=	$(top_builddir)/src/libquvi/libquvi.la $(AM_LDFLAGS)

parse_SOURCES=		parse.c
parse_CPPFLAGS=		-I$(top_srcdir)/src/libquvi $(AM_CPPFLAGS)
parse_CFLAGS=		$(AM_CFLAGS)
parse_LDFLAGS=		$(top_builddir)/src/libquvi/libquvi.la $(AM_LDFLAGS)

supported_SOURCES=	supported.c
supported_CPPFLAGS=	-I$(top_srcdir)/src/libquvi $(AM_CPPFLAGS)
supported_CFLAGS=	$(AM_CFLAGS)
supported_LDFLAGS=	$(top_builddir)/src/libquvi/libquvi.la $(AM_LDFLAGS)

if WITH_SOUP
noinst_PROGRAMS+=	callback_libsoup

callback_libsoup_SOURCES=callback_libsoup.c common.c common.h

callback_libsoup_CPPFLAGS=\
    -I$(top_builddir)/include -I$(top_srcdir)/include \
    $(libsoup_CFLAGS) $(AM_CPPFLAGS)

callback_libsoup_CFLAGS=	$(AM_CFLAGS)

callback_libsoup_LDFLAGS=\
    $(top_builddir)/src/libquvi/libquvi.la $(libsoup_LIBS) $(AM_LDFLAGS)
endif # WITH_SOUP

endif # WITH_EXAMPLES
