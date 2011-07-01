
use warnings;
use strict;

use Test::More;

eval "use JSON::XS";
plan skip_all => "JSON::XS required for testing" if $@;

eval "use Test::Deep";
plan skip_all => "Test::Deep required for testing" if $@;

plan tests => 6;

use Test::Quvi;

my $q = Test::Quvi->new;
my $j = $q->get_json_obj;

test_redirect_url(    # Test self.redirect_url in academicearth.lua
       "http://www.academicearth.org/lectures/intro-roman-architecture",
       "data/resolve/redirect_url_academicearth.json"
                 );

test_redirect_url(    # Test self.redirect_url in collegehumor.lua
    "http://www.dorkly.com/embed/17349/ridiculous-way-to-get-ammo-to-teammates-in-battlefield-bad-company-2",
    "data/resolve/redirect_url_dorkly.json"
                 );

test_url_shortener(    # Test URL shortener support
        "http://is.gd/EbVFoa",             # -> http://vimeo.com/1485507
        "data/format/default/vimeo.json"
                  );

sub test_redirect_url
{
    my ($url, $json) = @_;
    my ($r, $o) = $q->run($url, "-q");
    is($r, 0, "quvi exit status == 0") or diag $url;
  SKIP:
    {
        skip 'quvi exit status != 0', 1 if $r != 0;
        my $e = $q->read_json($json, 1)
          ;    # 1=prepend --data-root (if specified in cmdline)
        cmp_deeply($j->decode($o), $e, "compare with $json")
          or diag $url;
    }
}

sub test_url_shortener
{
    my ($url, $json) = @_;
    my ($r, $o) = $q->run($url, "-q");
    is($r, 0, "quvi exit status == 0") or diag $url;
  SKIP:
    {
        skip 'quvi exit status != 0', 1 if $r != 0;
        my $e = $q->read_json($json, 1)
          ;    # 1=prepend --data-root (if specified in cmdline)
        cmp_deeply($j->decode($o), $e, "compare with $json")
          or diag $url;
    }
}
