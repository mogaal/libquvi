if ENABLE_QUVI
EXTRA_DIST = \
    cmdline.ggo

src = \
    quvi.c \
    cmdline.c

hdr = \
    cmdline.h

if NO_ASPRINTF
src += $(top_srcdir)/lib/vasprintf.c $(top_srcdir)/lib/asprintf.c
endif

bin_PROGRAMS = quvi
quvi_SOURCES = $(src) $(hdr)

quvi_CFLAGS = \
    -I$(top_builddir)/include \
    -I$(top_srcdir)/lib \
    $(libcurl_CFLAGS) \
    $(libpcre_CFLAGS)

quvi_LDADD =  \
    $(top_builddir)/lib/libquvi.la \
    $(libcurl_LIBS) \
    $(libpcre_LIBS) \
    $(LIBICONV) \
    $(LTLIBICONV)
endif

