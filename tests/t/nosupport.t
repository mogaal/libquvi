
use warnings;
use strict;

use Test::More;

eval "use JSON::XS";
plan skip_all => "JSON::XS required for testing" if $@;

eval "use Test::Deep";
plan skip_all => "Test::Deep required for testing" if $@;

plan tests => 2;

use Test::Quvi;

my $q = Test::Quvi->new;
my ($r) = $q->run("http://nosupport.url", "--support -qs");
is($r, 0x41, "quvi exit status == QUVI_NOSUPPORT");

($r) = $q->run("http://youtu.be/9dgSa4wmMzk", "--support -qs");
is($r, 0x00, "quvi exit status == QUVI_OK");
