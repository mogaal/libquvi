
use warnings;
use strict;

use Test::More;

eval "use JSON::XS";
plan skip_all => "JSON::XS required for testing" if $@;

eval "use Test::Deep";
plan skip_all => "Test::Deep required for testing" if $@;

use Test::Quvi;

my $q = Test::Quvi->new;

my @files = $q->find_json(
    qw(
      data/format/other
      data/format/default/ignore/length_bytes
      )
);

plan skip_all => "Nothing to test" if scalar @files == 0;
plan tests => scalar @files * 2;

my $j   = $q->get_json_obj;
my $ign = qr|/ignore/(.*?)/|;

foreach (@files)
{
    my $e = $q->read_json($_);

    $q->mark_ignored(\$e, $1) if $_ =~ /$ign/;

    my $f = $e->{format_requested};
    my ($r, $o) = $q->run($e->{page_url}, "-qrf $f");
    is($r, 0, "quvi exit status == 0")
      or diag $e->{page_url};
  SKIP:
    {
        skip 'quvi exit status != 0', 1 if $r != 0;
        cmp_deeply($j->decode($o), $e, "compare with $_")
          or diag $e->{page_url};
    }
}
