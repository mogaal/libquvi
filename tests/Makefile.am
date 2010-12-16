
EXTRA_DIST = quviTest.pm.in $(TESTS) $(TESTS_POD) $(TESTS_NSFW)

if HAVE_PERL

TESTS_ENVIRONMENT = $(PERL) -w -mstrict -mquviTest -I $(top_builddir)/tests

TESTS =\
nosupport-eval.pl\
support-shortened.pl\
support-break.pl\
support-cbsnews.pl\
support-charlierose.pl\
support-clipfish.pl\
support-collegehumor.pl\
support-dailymotion.pl\
support-gaskrank.pl\
support-funnyhub.pl\
support-funnyordie.pl\
support-golem.pl\
support-google.pl\
support-liveleak.pl\
support-sapo.pl\
support-sevenload.pl\
support-spiegel.pl\
support-theonion.pl\
support-vimeo.pl\
support-youtube.pl\
support-academicearth.pl

# NOTE: academicearth.pl has to come after youtube.
# The former uses youtube for hosting some of its videos.

TESTS_BROKEN =\
support-bloomberg.pl\
support-cctv.pl

if ENABLE_BROKEN
TESTS += $(TESTS_BROKEN)
endif

TESTS_POD =\
pod-check.pl

if HAVE_PERL_POD_CHECKER
TESTS += $(TESTS_POD)
endif

TESTS_NSFW =\
support-tube8.pl\
support-xvideos.pl\
support-youjizz.pl

if ENABLE_NSFW
TESTS += $(TESTS_NSFW)
endif

else # HAVE_PERL
	@echo SKIP: perl not found.
endif # HAVE_PERL


