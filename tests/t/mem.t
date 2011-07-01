
use warnings;
use strict;

use Test::More;

eval "use JSON::XS";
plan skip_all => "JSON::XS required for testing" if $@;

use Test::Quvi;

my $q = Test::Quvi->new;
my $c = $q->get_config;

plan skip_all =>
  'valgrind required for testing, specify with --valgrind-path'
  unless $c->{valgrind_path};

plan tests => 11;

# invalid option, make an exception to redirect to /dev/null),
# gengetopt produced code checks and exits if this error occurs.
my ($r) = $q->run_with_valgrind('--invalid', '2>/dev/null');
is($r, 0x1, "exit status == 0x1");

# --version
($r) = $q->run_with_valgrind('--version');
is($r, 0x0, "exit status == 0x0");    # 0x0 = QUVI_OK

# --help
($r) = $q->run_with_valgrind('--help');
is($r, 0x0, "exit status == 0x0");    # 0x0 = QUVI_OK

# --support
($r) = $q->run_with_valgrind('--support');
is($r, 0x0, "exit status == 0x0");    # 0x0 = QUVI_OK

# --format help
($r) = $q->run_with_valgrind('-qf', 'help');
is($r, 0x0, "exit status == 0x0");    # 0x0 = QUVI_OK

# --format list
($r) = $q->run_with_valgrind('-qf', 'list');
is($r, 0x0, "exit status == 0x0");    # 0x0 = QUVI_OK

# --format list arg
($r) = $q->run_with_valgrind('-qf', 'list', 'dailym');
is($r, 0x0, "exit status == 0x0");    # 0x0 = QUVI_OK

# --support arg
($r) =
  $q->run_with_valgrind('http://vimeo.com/1485507', '--support -qr');
is($r, 0x0, "exit status == 0x0");

# fetch, parse, exit
($r) = $q->run_with_valgrind('http://vimeo.com/1485507', '-qr');
is($r, 0x0, "exit status == 0x0");

# (fetch, parse) x 2, exit
($r) =
  $q->run_with_valgrind(
                        'http://vimeo.com/1485507',
                        'http://megavideo.com/?v=HJVPVMTV',
                        '-qr'
                       );
is($r, 0x0, "exit status == 0x0");

# (fetch, parse) x 2, exit
# NOTE: first fails intentionally
($r) =
  $q->run_with_valgrind(
                        'http://ww.vimeo.com/1485507',
                        'http://megavideo.com/?v=HJVPVMTV',
                        '-qr'
                       );
is($r, 0x0, "exit status == 0x0");
