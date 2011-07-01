
use warnings;
use strict;

use Test::More;

eval "use JSON::XS";
plan skip_all => "JSON::XS required for testing" if $@;

eval "use Test::Deep";
plan skip_all => "Test::Deep required for testing" if $@;

use Test::Quvi;

my @paths = qw(
  data/format/default/non-http/mms
  data/format/default/non-http/rtmp
  );

my $q     = Test::Quvi->new;
my @files = $q->find_json(@paths);

plan skip_all => "Nothing to test" if scalar @files == 0;
plan tests => scalar @files * 2;

my $j = $q->get_json_obj;

foreach (@files)
{
    my $e = $q->read_json($_);
    my ($r, $o) = $q->run($e->{page_url}, '-qra');
    is($r, 0, "quvi exit status == 0")
      or diag $e->{page_url};
  SKIP:
    {
        skip 'quvi exit status != 0', 1 if $r != 0;
        cmp_deeply($j->decode($o), $e, "compare with $_")
          or diag $e->{page_url};
    }
}
