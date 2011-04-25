
use warnings;
use strict;

use Getopt::Long;
use Test::More;

eval "use Test::Pod 1.00";
plan skip_all => "Test::Pod 1.00 required for testing POD" if $@;

my %config;
GetOptions(\%config, 'pod_dir|pod-dir|poddir|p=s@')
    or exit 1;

my @dirs = @{$config{pod_dir}} if $config{pod_dir};
plan skip_all => "Nothing to test" if scalar @dirs == 0;

all_pod_files_ok(all_pod_files(@dirs));
