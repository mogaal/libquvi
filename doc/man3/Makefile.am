MAN3_TXT=libquvi.3.txt
DOC_MAN3=libquvi.3

MAN_TXT=$(MAN3_TXT)

ASCIIDOC_OPTS=\
  -f $(top_srcdir)/doc/asciidoc.conf\
  -a pkg_version="$(VERSION)"

if HAVE_A2X
$(DOC_MAN3):
	$(AM_V_GEN)$(A2X) -f manpage --asciidoc-opts="$(ASCIIDOC_OPTS)" \
		-D $(builddir) $(srcdir)/$@.txt
doc: $(DOC_MAN3)
else
doc:
	@echo configure did not find a2x
endif # HAVE_A2X

MOSTLYCLEANFILES=$(DOC_MAN3)
dist_man_MANS=$(DOC_MAN3)
EXTRA_DIST=$(MAN_TXT)

# vim: set ts=2 sw=2 tw=72 expandtab:
