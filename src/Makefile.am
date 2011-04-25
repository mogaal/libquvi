if ENABLE_QUVI
EXTRA_DIST = \
    cmdline.ggo

src = \
quvi.c \
cmdline.c \
$(top_srcdir)/lib/strepl.c \
$(top_srcdir)/lib/llst.c

hdr = \
cmdline.h

if NO_ASPRINTF
src += $(top_srcdir)/lib/vasprintf.c $(top_srcdir)/lib/asprintf.c
endif

bin_PROGRAMS = quvi
quvi_SOURCES = $(src) $(hdr)

quvi_CPPFLAGS = \
    -I$(top_builddir)/include \
    -I$(top_srcdir)/lib \
    $(libcurl_CFLAGS) \
    $(AM_CPPFLAGS)

quvi_CFLAGS = \
    $(AM_CFLAGS)

quvi_LDFLAGS = \
    $(AM_LDFLAGS)

quvi_LDADD =  \
    $(top_builddir)/lib/libquvi.la \
    $(libcurl_LIBS) \
    $(LIBICONV) \
    $(LTLIBICONV)

endif

