my $rc = quviTest::run("http://invalid.host", "--no-shortened", "-q");
if ($rc == 0x41) { # QUVI_NOSUPPORT (0x41) is what we expect.
    $rc = quviTest::run("--support", "http://invalid.url", "-q");
    if ($rc == 0x41) { # Ditto. And the following should return QUVI_OK.
        { $rc = quviTest::run("--support", "http://youtube.com", "-q"); }
    }
}
exit $rc;
