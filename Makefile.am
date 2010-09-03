ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
libquvi.pc.in \
libquvi.ver \
doxygen.conf.in \
quvi.pod \
KNOWN_ISSUES

if WITH_MAN
dist_man_MANS = quvi.1
endif

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libquvi.pc

SUBDIRS = include lib src examples share doc

if ENABLE_TESTS
SUBDIRS += tests
endif

.PHONY: man dox cmdline
man:
if HAVE_POD2MAN
	@POD2MAN@ -c "quvi manual" -n quvi \
		 -s 1 -r @VERSION@ $(top_srcdir)/quvi.pod $(top_builddir)/quvi.1
else
	@echo "error: configure did not find pod2man command"
endif
if HAVE_POD2HTML
	@POD2HTML@ --title="quvi manual" \
		--infile=$(top_srcdir)/quvi.pod \
        --outfile=$(top_builddir)/quvi.1.html
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

distclean-local: local-clean
