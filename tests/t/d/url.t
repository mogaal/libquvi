
use warnings;
use strict;

use Test::More;

eval "use JSON::XS";
plan skip_all => "JSON::XS required for testing" if $@;

eval "use Test::Deep";
plan skip_all => "Test::Deep required for testing" if $@;

use Test::Quvi;

my $q = Test::Quvi->new;
my $c = $q->get_config;

plan skip_all =>
  "URL required for testing, use ':: --url URL' or ('--json-file FILE')"
  unless $c->{url}
      or $c->{json_file};    # --url or --json-file (read URL from json)

my $e;
my $u = $c->{url};
if (!$u)
{
    $e = $q->read_json($c->{json_file});
    plan skip_all =>
      "URL required, JSON did not contain 'page_url', use --url URL"
      unless $e->{page_url};
    $u = $e->{page_url};
}

my $t = 1;
$t += 1 if $c->{json_file};
plan tests => $t;

my ($r, $o) = $q->run($u);
is($r, 0, "quvi exit status == 0")
  or diag $c->{url};
if ($c->{json_file})
{
  SKIP:
    {
        skip 'quvi exit status != 0', 1 if $r != 0;
        my $f = $c->{json_file};
        my $j = $q->get_json_obj;
        cmp_deeply($j->decode($o), $e, "compare with $f")
          or diag $c->{url};
    }
}
