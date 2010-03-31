
# We will need utf8 for this.
quviTest::check_codeset();

my $rc = quviTest::run_t(
"cctv",
"--page-title",
    "[百家讲坛]王立群读史记之秦始皇（一）荆轲刺秦王(2008.06.08)百家讲坛,名栏目",
"--video-id",
    "VIDE1212909276513233",
"--file-length",
    "75987226",
"--file-suffix",
    "flv"
); # single-segment

$rc = quviTest::run("-t", "12474")  if !$rc; # multi-segment

exit $rc;
