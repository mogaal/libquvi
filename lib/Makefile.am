
if HAVE_GNU_LD
EXTRA_LIBQUVI_LDFLAGS += -Wl,--version-script=$(top_srcdir)/libquvi.ver
endif

src = \
quvi.c \
util.c \
curl.c \
strepl.c \
llst.c

if NO_ASPRINTF
src += vasprintf.c asprintf.c
endif

if NO_STRUPR
src += strupr.c
endif

if NO_STRTOK_R
src += strtok_r.c
endif

src_host = \
host/break.c \
host/cctv.c \
host/clipfish.c \
host/evisor.c \
host/funnyhub.c \
host/golem.c \
host/google.c \
host/liveleak.c \
host/myubo.c \
host/sevenload.c \
host/vimeo.c \
host/spiegel.c \
host/ehrensenf.c \
host/dailymotion.c \
host/spyfilms.c \
host/youtube.c \
host/buzzhumor.c

if ENABLE_SMUT
src_host += \
host/tube8.c \
host/xvideos.c \
host/youjizz.c
endif

hdr = \
curl.h \
host.h \
internal.h \
util.h \
platform.h \
llst.h

lib_LTLIBRARIES = libquvi.la
libquvi_la_SOURCES = $(src) $(src_host) $(hdr)

libquvi_la_CFLAGS = \
-I$(top_builddir)/include \
$(libcurl_CFLAGS) \
$(libpcre_CFLAGS)

libquvi_la_LDFLAGS = \
$(libcurl_LIBS) \
$(libpcre_LIBS) \
$(LIBICONV) \
$(LTLIBICONV) \
-version-info $(QUVI_CURRENT):$(QUVI_REVISION):$(QUVI_AGE) \
$(EXTRA_LIBQUVI_LDFLAGS)


