
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

JSON = \
data/resolve/inscript_redirect.json \
data/resolve/shortened.json \
data/format/default/megavideo.json \
data/format/default/charlierose.json \
data/format/default/liveleak.json \
data/format/default/publicsenat.json \
data/format/default/tmunderground.json \
data/format/default/gaskrank.json \
data/format/default/spiegel.json \
data/format/default/funnyordie.json \
data/format/default/funnyhub.json \
data/format/default/academicearth.json \
data/format/default/theonion.json \
data/format/default/ted.json \
data/format/default/tagtele.json \
data/format/default/break.json \
data/format/default/youtube.json \
data/format/default/golem.json \
data/format/default/google.json \
data/format/default/soundcloud.json \
data/format/default/globo.json \
data/format/default/cbsnews.json \
data/format/default/sapo.json \
data/format/default/tvlux.json \
data/format/default/bloomberg.json \
data/format/default/buzzhumor.json \
data/format/default/vimeo.json \
data/format/other/cbsnews_mp4_240p.json \
data/format/other/cbsnews_m4v_216p.json \
data/format/other/ted_best.json \
data/format/other/spiegel_1400k.json \
data/format/other/spiegel_best.json \
data/format/other/youtube_mp4_720p.json \
data/format/other/cbsnews_mp4_480p.json \
data/format/other/youtube_mp4_360p.json \
data/format/other/youtube_mp4_3072p.json \
data/format/other/youtube_mp4_1080p.json \
data/format/other/cbsnews_m4v_360p.json \
data/format/other/spiegel_576k.json \
data/format/other/golem_ipod.json \
data/format/other/ted_hd.json \
data/format/other/cbsnews_best.json \
data/format/other/cbsnews_mp4_180p.json \
data/format/other/cbsnews_m4v_480p.json \
data/format/other/golem_high.json \
data/format/other/youtube_best.json \
data/format/other/youtube_flv_480p.json \
data/format/other/vimeo_best.json \
data/format/other/spiegel_928k.json \
data/format/other/youtube_flv_240p.json \
data/format/other/golem_best.json \
data/format/other/youtube_flv_360p.json \
data/format/other/vimeo_hd.json

JSON_IGNORE = \
data/format/default/ignore/length_bytes/dailymotion.json \
data/format/other/ignore/length_bytes/dailymotion_hq.json \
data/format/other/ignore/length_bytes/dailymotion_hd.json \
data/format/other/ignore/length_bytes/dailymotion_best.json

JSON_TODO = \
data/format/default/todo/collegehumor.json

JSON_NLFY = \
data/format/default/nlfy/sevenload.json \
data/format/default/nlfy/clipfish.json

JSON_NSFW = \
data/format/default/nsfw/xvideos.json \
data/format/default/nsfw/tube8.json \
data/format/default/nsfw/keezmovies.json \
data/format/default/nsfw/xhamster.json \
data/format/default/nsfw/youjizz.json

README = \
README \
data/format/default/ignore/README \
data/format/default/todo/README \
data/format/default/non-http/README \
data/format/other/ignore/README

EXTRA_DIST = \
 $(README) $(TESTS_LIB) \
 $(TESTS_T) $(TESTS_NSFW_T) $(TESTS_D_T) $(TESTS_POD_T) \
 $(JSON) $(JSON_IGNORE) $(JSON_TODO) $(JSON_NLFY) $(JSON_NSFW)


PROVE_OPTS_COMMON = --nocolor

if ENABLE_VERBOSE_TESTS
PROVE_OPTS_COMMON += -v
endif

# Options for those tests that use Test::Quvi.

PROVE_OPTS = -I $(top_srcdir)/tests/lib/
PROVE_OPTS += $(PROVE_OPTS_COMMON)

TESTQUVI_OPTS = \
 -q $(top_builddir)/src/quvi \
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

TESTQUVI_OPTS_POD = -p $(top_srcdir)/man1

test: all
	@ ok=1; for t in $(TESTS_T); do \
		( $(PROVE) $(PROVE_OPTS) $(top_srcdir)/tests/$$t \
		:: $(TESTQUVI_OPTS) ) || ok=; \
	done; \
	for t in $(TESTS_POD_T); do \
		( $(PROVE) $(PROVE_OPTS_POD) $(top_srcdir)/tests/$$t \
		:: $(TESTQUVI_OPTS_POD) ) || ok=; \
	done; \
	if test -z "$$ok"; then exit 1; fi
