.PHONY: doc distdoc-html distdoc-latex distdoc-pdf

if HAVE_DOXYGEN
doc:
	@$(DOXYGEN) Doxyfile
distdoc-html: doc
	@tar chof - libquvi-@VERSION@/html | \
			XZ_OPT=-6 xz -c >@top_builddir@/libquvi-htmldocs-@VERSION@.tar.xz \
			&& echo Created libquvi-htmldocs-@VERSION@.tar.xz
distdoc-pdf: doc
	@make -C libquvi-@VERSION@/latex/ \
		&& tar chof - libquvi-@VERSION@/latex/refman.pdf | \
			XZ_OPT=-6 xz -c >@top_builddir@/libquvi-pdfdocs-@VERSION@.tar.xz \
      && rm -f libquvi-@VERSION@/latex/refman.pdf \
			&& echo Created libquvi-pdfdocs-@VERSION@.tar.xz
distdoc-latex: distdoc-pdf
	@tar chof - libquvi-@VERSION@/latex | \
			XZ_OPT=-6 xz -c >@top_builddir@/libquvi-latexdocs-@VERSION@.tar.xz \
			&& echo Created libquvi-latexdocs-@VERSION@.tar.xz
else
doc:
distdoc-html: doc
distdoc-pdf: doc
distdoc-latex: distdoc-pdf
	@echo configure did not find doxygen
endif

clean-local:
	@rm -rf libquvi-@VERSION@

# vim: set ts=2 sw=2 tw=72 expandtab:
