ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
libquvi.pc.in \
libquvi.ver \
doxygen.conf.in \
KNOWN_ISSUES \
ChangeLog.pre-gitlog

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libquvi.pc

SUBDIRS = include lib man1 src examples share doc

if ENABLE_TESTS
SUBDIRS += tests
endif

.PHONY: pod2man pod2html dox cmdline
pod2man:
if HAVE_POD2MAN
	@POD2MAN@ -c "quvi manual" -n quvi \
		 -s 1 -r @VERSION@ $(top_srcdir)/man1/quvi.1.pod $(top_builddir)/man1/quvi.1
else
	@echo "error: configure did not find pod2man command"
endif
pod2html:
if HAVE_POD2HTML
	@POD2HTML@ --title="quvi manual" \
		--infile=$(top_srcdir)/man1/quvi.1.pod \
        --outfile=$(top_builddir)/man1/quvi.1.html
else
	@echo "error: configure did not find pod2html command"
endif
dox:
if HAVE_DOXYGEN
	@$(DOXYGEN) $(top_builddir)/doxygen.conf
	@echo doxygen generated pages can be found in doc/ subdir
else
	@echo configure did not find doxygen
endif
cmdline:
if HAVE_GENGETOPT
	@GENGETOPT@ < $(top_srcdir)/src/cmdline.ggo -C --unamed-opts=URL \
		--output-dir $(top_srcdir)/src --no-version
else
	@echo "error: configure did not find 'gengetopt' command"
endif

local-clean:
	@rm -rf *.tmp *.core *.html

ChangeLog:
	@if test -f $(top_srcdir)/.git/HEAD; then \
		git log --stat --after="Sun Nov 14 11:03:50 2010" > $@; \
	fi

dist: ChangeLog

.PHONY: ChangeLog

distclean-local: local-clean
