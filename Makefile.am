ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
libquvi.pc.in \
libquvi.ver \
ChangeLog.pre-gitlog

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libquvi.pc

SUBDIRS = src examples share scripts doc

if ENABLE_TESTS
SUBDIRS += tests

check: test

test:
	@(cd tests; $(MAKE) all test)
endif

.PHONY: doc dist-doc
doc:
if HAVE_ND
	@test -d $(top_builddir)/doc/quvi-@VERSION@ || \
		mkdir $(top_builddir)/doc/quvi-@VERSION@
	@test -d $(top_builddir)/doc/nd_temp || \
		mkdir $(top_builddir)/doc/nd_temp
	@@ND@ -i $(top_srcdir)/doc/ -i $(top_srcdir)/src/ \
		-o $(ND_FORMAT) $(top_builddir)/doc/quvi-@VERSION@/ \
		-p $(top_builddir)/doc/nd_temp/ && echo See doc/quvi-@VERSION@
else
	@echo "error: configure did not find NaturalDocs" && exit 1
endif
dist-doc: doc
	@tar chof - $(top_builddir)/doc/quvi-@VERSION@ | \
			GZIP=--best gzip -c >quvi-doc-@VERSION@.tar.gz \
			&& echo Created quvi-doc-@VERSION@.tar.gz

ChangeLog:
	@if test -f $(top_srcdir)/.git/HEAD; then \
		@GIT@ log --stat --after="Sun Nov 14 11:03:50 2010" > $@; \
	fi

dist: ChangeLog

.PHONY: ChangeLog
