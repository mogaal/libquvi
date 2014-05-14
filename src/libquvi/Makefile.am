SUBDIRS=	quvi
EXTRA_DIST=	depr_api.c

src=\
 quvi_api.c \
 net_api.c \
 llst_api.c \
 curl_wrap.c \
 lua_wrap.c \
 net_wrap.c \
 util.c \
 strepl.c

if NO_ASPRINTF
src+=	vasprintf.c asprintf.c
endif

hdr=\
 curl_wrap.h \
 lua_wrap.h \
 internal.h \
 util.h \
 platform.h \
 net_wrap.h

lib_LTLIBRARIES=	libquvi.la
libquvi_la_SOURCES=	$(src) $(hdr)

libquvi_la_CPPFLAGS=\
 $(libcurl_CFLAGS) \
 $(liblua_CFLAGS) \
 $(AM_CPPFLAGS)

libquvi_la_CFLAGS=\
 $(AM_CFLAGS)

libquvi_la_LDFLAGS=\
 $(libcurl_LIBS) \
 $(liblua_LIBS) \
 $(LIBICONV) \
 $(LTLIBICONV) \
 -version-info $(LIBQUVI_CURRENT):$(LIBQUVI_REVISION):$(LIBQUVI_AGE) \
 $(AM_LDFLAGS)
