ACLOCAL_AMFLAGS=-I m4
EXTRA_DIST=	libquvi.pc.in gen-ver.sh VERSION

pkgconfigdir=	$(libdir)/pkgconfig
pkgconfig_DATA=	libquvi.pc

SUBDIRS=	src examples doc

.PHONY: doc distdoc ChangeLog VERSION
doc:
if HAVE_ND
	@test -d $(top_builddir)/doc/libquvi-@VERSION@ || \
		mkdir $(top_builddir)/doc/libquvi-@VERSION@
	@test -d $(top_builddir)/doc/nd_temp || \
		mkdir $(top_builddir)/doc/nd_temp
	@@ND@ -i $(top_srcdir)/doc/ -i $(top_srcdir)/src/ \
		-o $(ND_FORMAT) $(top_builddir)/doc/libquvi-@VERSION@/ \
		-p $(top_builddir)/doc/nd_temp/ && echo See doc/libquvi-@VERSION@
else
	@echo "error: configure did not find NaturalDocs" && exit 1
endif
distdoc: doc
	@tar chof - $(top_builddir)/doc/libquvi-@VERSION@ | \
			GZIP=--best gzip -c >libquvi-doc-@VERSION@.tar.gz \
			&& echo Created libquvi-doc-@VERSION@.tar.gz

VERSION:
	@if test -d "$(top_srcdir)/.git" -o -f "$(top_srcdir)/.git"; then \
		"$(top_srcdir)/gen-ver.sh" "$(top_srcdir)" > $@; \
	fi

ChangeLog:
	@if test -d "$(top_srcdir)/.git" -o -f "$(top_srcdir)/.git"; then \
		git log --stat --name-only --date=short --abbrev-commit > $@; \
	fi

dist-hook: ChangeLog VERSION
