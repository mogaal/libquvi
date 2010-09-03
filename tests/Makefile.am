
EXTRA_DIST = quviTest.pm.in $(TESTS) $(TESTS_POD) $(TESTS_ADULT)

if HAVE_PERL

TESTS_ENVIRONMENT = $(PERL) -w -mstrict -mquviTest -I $(top_builddir)/tests

TESTS =\
nosupport-eval.pl\
support-break.pl\
support-charlierose.pl\
support-clipfish.pl\
support-dailymotion.pl\
support-evisor.pl\
support-funnyhub.pl\
support-funnyordie.pl\
support-golem.pl\
support-liveleak.pl\
support-myubo.pl\
support-sevenload.pl\
support-spiegel.pl\
support-theonion.pl\
support-vimeo.pl\
support-youtube.pl

TESTS_BROKEN =\
support-cctv.pl\
support-ehrensenf.pl\
support-spyfilms.pl

if ENABLE_BROKEN
TESTS += $(TESTS_BROKEN)
endif

TESTS_POD =\
pod-check.pl

if HAVE_PERL_POD_CHECKER
TESTS += $(TESTS_POD)
endif

TESTS_ADULT =\
support-tube8.pl\
support-xvideos.pl\
support-youjizz.pl

if ENABLE_SMUT
TESTS += $(TESTS_ADULT)
endif

else # HAVE_PERL
	@echo SKIP: perl not found.
endif # HAVE_PERL


