

my $expected_title = "CITY OF SCARS";
my $expected_id    = "xdpig1";
my $expected_length= 111422543;
my $expected_suffix= "flv";

my $r = quviTest::run(
"http://dai.ly/cityofscars",
"--page-title",
    $expected_title,
"--video-id",
    $expected_id,
"--file-length",
    $expected_length,
"--file-suffix",
    $expected_suffix
);

exit $r if $r;

exit quviTest::run(
"http://goo.gl/18ol",
"--page-title",
    $expected_title,
"--video-id",
    $expected_id,
"--file-length",
    $expected_length,
"--file-suffix",
    $expected_suffix
);
