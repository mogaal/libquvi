
if HAVE_GNU_LD
EXTRA_LIBQUVI_LDFLAGS += -Wl,--version-script=$(top_srcdir)/libquvi.ver
endif

src = \
    quvi.c \
    util.c \
    curl_wrap.c \
    lua_wrap.c \
    strepl.c \
    llst.c

if NO_ASPRINTF
src += vasprintf.c asprintf.c
endif

hdr = \
    curl_wrap.h \
    lua_wrap.h \
    internal.h \
    util.h \
    platform.h \
    llst.h

lib_LTLIBRARIES = libquvi.la
libquvi_la_SOURCES = $(src) $(hdr)

libquvi_la_CPPFLAGS = \
    -I$(top_builddir)/include \
    -DDATADIR='"$(datadir)"' \
    -DPKGDATADIR='"$(pkgdatadir)"' \
    $(libcurl_CFLAGS) \
    $(liblua_CFLAGS) \
    $(AM_CPPFLAGS)

libquvi_la_CFLAGS = \
    $(AM_CFLAGS)

libquvi_la_LDFLAGS = \
    $(libcurl_LIBS) \
    $(liblua_LIBS) \
    $(LIBICONV) \
    $(LTLIBICONV) \
    -version-info $(QUVI_CURRENT):$(QUVI_REVISION):$(QUVI_AGE) \
    $(EXTRA_LIBQUVI_LDFLAGS) \
    $(AM_LDFLAGS)

