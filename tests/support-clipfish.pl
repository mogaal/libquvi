unless ($ENV{TEST_RESTRICTED}) {
    print STDERR "SKIP: Country restricted. Set TEST_RESTRICTED to enable.\n";
    exit 0;
}
exit quviTest::run_t(
"clipfish",
"--page-title",
    "Matratzendomino - bett domino matratze matratzendomino",
"--video-id",
    "3100131",
"--file-length",
    "6529708",
"--file-suffix",
    "flv"
);
