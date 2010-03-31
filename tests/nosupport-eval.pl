my $rc = quviTest::run("http://invalid.host");
$rc = 0 if $rc == 0x41; # QUVI_NOSUPPORT (0x41) is what we expect.
exit $rc;
