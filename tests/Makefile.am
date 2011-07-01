
TESTS_T = \
t/format_default_nonhttp.t \
t/mem.t \
t/nosupport.t \
t/redirect.t \
t/format_default.t \
t/format_other.t \
t/shortened.t

TESTS_D_T = t/d/url.t
TESTS_POD_T = t/pod.t

TESTS_LIB = lib/Test/Quvi.pm

JSON = data

EXTRA_DIST = \
 $(README) $(TESTS_LIB) \
 $(TESTS_T) $(TESTS_NSFW_T) $(TESTS_D_T) $(TESTS_POD_T) \
 $(JSON)


PROVE_OPTS_COMMON = --nocolor

# Options for those tests that use Test::Quvi.

PROVE_OPTS = -I $(top_srcdir)/tests/lib/
PROVE_OPTS += $(PROVE_OPTS_COMMON)

TESTQUVI_OPTS = \
 -q $(top_builddir)/src/quvi/quvi \
 -d $(top_srcdir)/tests \
 -b $(top_srcdir)/share

if HAVE_VALGRIND
TESTQUVI_OPTS += -v $(VALGRIND)
endif

if ENABLE_TODO
TESTQUVI_OPTS += --todo
endif

if ENABLE_NLFY
TESTQUVI_OPTS += --nlfy
endif

if ENABLE_NSFW
TESTQUVI_OPTS += --nsfw
endif

# Options for those tests that *do not* use Test::Quvi.

# t/pod.t: GetOptions accepts a different (from those used in
# Test::Quvi) set of options.
PROVE_OPTS_POD = $(PROVE_OPTS_COMMON)

TESTQUVI_OPTS_POD = -p $(top_srcdir)/doc/man1

test: all
	@verbose=; test -n "$$TEST_VERBOSE" && verbose=-v; \
	ok=1; for t in $(TESTS_T); do \
		( $(PROVE) $$verbose $(PROVE_OPTS) $(top_srcdir)/tests/$$t \
		:: $(TESTQUVI_OPTS) ) || ok=; \
	done; \
	for t in $(TESTS_POD_T); do \
		( $(PROVE) $$verbose $(PROVE_OPTS_POD) $(top_srcdir)/tests/$$t \
		:: $(TESTQUVI_OPTS_POD) ) || ok=; \
	done; \
	if test -z "$$ok"; then exit 1; fi
