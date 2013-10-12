SUBDIRS=lib
noinst_PROGRAMS=$(TEST_PROGS)

TEST_OPTS=-k -m=thorough --verbose #--g-fatal-warnings

testsuite_common_cppflags=\
  -I$(top_srcdir)/tests/lib\
  -I$(top_srcdir)/src/quvi-0.9/\
  $(libcurl_CFLAGS)\
  $(glib_CFLAGS)\
  $(AM_CPPFLAGS)

testsuite_common_ldflags=\
  $(AM_LDFLAGS)

testsuite_common_ldadd=\
  $(top_builddir)/tests/lib/libconvenience_test.la\
  $(top_builddir)/src/libquvi-0.9.la

TEST_PROGS=script

script_SOURCES=script.c
script_CPPFLAGS=$(testsuite_common_cppflags)
script_LDFLAGS=$(testsuite_common_ldflags)
script_LDADD=$(testsuite_common_ldadd)
script_CFLAGS=$(AM_CFLAGS)

TEST_PROGS+=quvi

quvi_SOURCES=quvi.c
quvi_CPPFLAGS=$(testsuite_common_cppflags)
quvi_LDFLAGS=$(testsuite_common_ldflags)
quvi_LDADD=$(testsuite_common_ldadd)
quvi_CFLAGS=$(AM_CFLAGS)

# TEST_INTERNET tests

TEST_PROGS+=playlist

playlist_SOURCES=playlist.c
playlist_CPPFLAGS=$(testsuite_common_cppflags)
playlist_LDFLAGS=$(testsuite_common_ldflags)
playlist_LDADD=$(testsuite_common_ldadd)
playlist_CFLAGS=$(AM_CFLAGS)

TEST_PROGS+=supports

supports_SOURCES=supports.c
supports_CPPFLAGS=$(testsuite_common_cppflags)
supports_LDFLAGS=$(testsuite_common_ldflags)
supports_LDADD=$(testsuite_common_ldadd)
supports_CFLAGS=$(AM_CFLAGS)

TEST_PROGS+=resolve

resolve_SOURCES=resolve.c
resolve_CPPFLAGS=$(testsuite_common_cppflags)
resolve_LDFLAGS=$(testsuite_common_ldflags)
resolve_LDADD=$(testsuite_common_ldadd)
resolve_CFLAGS=$(AM_CFLAGS)

TEST_PROGS+=media

media_SOURCES=media.c
media_CPPFLAGS=$(testsuite_common_cppflags)
media_CFLAGS=$(AM_CFLAGS)
media_LDFLAGS=$(testsuite_common_ldflags)
media_LDADD=$(testsuite_common_ldadd)

TEST_PROGS+=scan

scan_SOURCES=scan.c
scan_CPPFLAGS=$(testsuite_common_cppflags)
scan_LDFLAGS=$(testsuite_common_ldflags)
scan_LDADD=$(testsuite_common_ldadd)
scan_CFLAGS=$(AM_CFLAGS)

TEST_PROGS+=goto

goto_SOURCES=goto.c
goto_CPPFLAGS=$(testsuite_common_cppflags)
goto_LDFLAGS=$(testsuite_common_ldflags)
goto_LDADD=$(testsuite_common_ldadd)
goto_CFLAGS=$(AM_CFLAGS)

TEST_PROGS+=http_metainfo

http_metainfo_SOURCES=http_metainfo.c
http_metainfo_CPPFLAGS=$(testsuite_common_cppflags)
http_metainfo_LDFLAGS=$(testsuite_common_ldflags)
http_metainfo_LDADD=$(testsuite_common_ldadd)
http_metainfo_CFLAGS=$(AM_CFLAGS)

TEST_PROGS+=subtitle

subtitle_SOURCES=subtitle.c
subtitle_CPPFLAGS=$(testsuite_common_cppflags)
subtitle_LDFLAGS=$(testsuite_common_ldflags)
subtitle_LDADD=$(testsuite_common_ldadd)
subtitle_CFLAGS=$(AM_CFLAGS)

test: all
	@ok=1; for t in $(TEST_PROGS); do \
		gtester $(TEST_OPTS) $(top_builddir)/tests/$$t -o $$t.xml \
		|| { \
			echo NOTE: Check $(abs_builddir)/$$t.xml for details; ok=; \
		} \
	done; \
	if test -z "$$ok"; then exit 1; fi

if HAVE_VALGRIND
VG_OPTS=-q --tool=memcheck --leak-check=full\
  --leak-resolution=high --num-callers=20\
  --track-origins=yes --error-exitcode=1
endif

test-mem: all
if HAVE_VALGRIND
	@ok=1; for t in $(TEST_PROGS); do \
		f=$(top_builddir)/tests/$$t; \
		l=$$f.vgdump; \
		G_SLICE=always-malloc G_DEBUG=gc-friendly \
			libtool --mode=execute $(VALGRIND) $(VG_OPTS) --log-file=$$l $$f \
		|| { \
			ok=; echo "LEAK detected in $$f.  Check $$l for details."; \
		} \
	done; \
	if test -z "$$ok"; then exit 1; fi
else
	@echo configure did not find valgrind
endif

clean-local:
	@for t in $(TEST_PROGS); do rm -f $$t.xml $$t.html $$t.vgdump; done

EXTRA_DIST=find_tests.sh run_tests.sh

# vim: set ts=2 sw=2 tw=72 expandtab:
