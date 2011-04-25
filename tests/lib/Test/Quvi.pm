# quvi
# Copyright (C) 2011  Toni Gundogdu <legatvs@gmail.com>
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301  USA
#

package Test::Quvi;

use warnings;
use strict;

use version 0.77 (); our $VERSION = version->declare("0.0.1");

require Exporter;
use vars qw(@ISA @EXPORT @EXPORT_OK);

our @ISA    = qw(Exporter);
our @EXPORT = ();

use Getopt::Long qw(:config bundling);
use Carp qw(croak);
use Test::More;
use Test::Deep;
use File::Spec;
use Cwd qw(cwd);

=for comment
Parse command line options, create object(s), etc. Make a note of
default values (e.g. assume quvi(1) to be found in the $PATH if
--quvi-path is not used.
=cut

sub new
{
    my ($class, %args) = @_;
    my $self = bless {}, $class;

    my %config;
    GetOptions(
               \%config,
               'url|u=s',
               'quvi_path|quvi-path|quvipath|q=s',
               'quvi_basedir|quvi-basedir|quvibasedir|b=s',
               'quvi_opts|quvi-opts|quviopts|o=s',
               'json_file|json-file|jsonfile|j=s',
               'dump_json|dump-json|dumpjson|J',
               'data_root|data-root|dataroot|d=s',
               'ignore|i=s',
               'valgrind_path|valgrind-path|valgrindpath|v=s',
               'todo',
               'nlfy',
               'nsfw',
              );
    $config{quvi_path} ||= 'quvi';   # Presume it is found in the $PATH.
    $config{data_root} ||= cwd;
    $self->{config} = \%config;
    $self->{jobj}   = JSON::XS->new;
    $ENV{QUVI_BASEDIR} = $config{quvi_basedir} if $config{quvi_basedir};

    $self;
}

=for comment
Reuse the JSON object instead of re-creating one for each test.
=cut

sub get_json_obj
{
    my ($self) = @_;
    $self->{jobj};
}

=for comment
A short-hand to access the parsed command line options.
=cut

sub get_config
{
    my ($self) = @_;
    $self->{config};
}

=for comment
Find all occurences of '*.json' from the specified paths.
=cut

sub find_json
{
    my ($self, @paths) = @_;
    my @files;
    my $d = $self->{config}{data_root};
    foreach (@paths)
    {
        my $p = File::Spec->catfile($d, $_, '*.json');
        @files = (@files, glob($p));
    }
    @files;
}

=for comment
Read the specified JSON file. Prepend $config{data_root} to the file
path if requested, this is needed typically if read_json is called
without a preceeding call to find_json (e.g. t/redirect.t and
t/shortened.t skip find_json).
=cut

sub read_json
{
    my ($self, $fpath, $prepend_data_root) = @_;

    if ($prepend_data_root)
    {
        my $d = $self->{config}{data_root};
        $fpath = File::Spec->catfile($d, $fpath);
    }

    note "read $fpath";
    open my $fh, "<", "$fpath" or croak "$fpath: $!";
    my $e = $self->{jobj}->decode(join '', <$fh>);
    close $fh;

    # Ignore these by default.
    my @ignore = qw(url thumbnail_url);

    # Any aditional JSON keys to be ignored.
    if ($self->{config}{ignore})
    {
        @ignore = (@ignore, split /,/, $self->{config}{ignore});
    }

    mark_ignored($self, \$e, @ignore);
    $e;
}

=for comment
Mark those JSON elements that are to be ignored in deep comparison.
Note that 'link' is a special case. We have to also assume that there
could be more than one 'link'.
=cut

sub mark_ignored
{
    my ($self, $json, @a) = @_;
    for my $i (@a)
    {
        while (my ($k, $v) = each(%{$$json}))
        {
            if ($k eq "link")
            {
                my $n = 0;
                for my $l (@{$v})
                {
                    while (my ($kl, $vl) = each(%{$l}))
                    {
                        $$json->{$k}[$n]->{$kl} = ignore()
                          if $kl eq $i;
                    }
                    ++$n;
                }
            }
            else
            {
                $$json->{$k} = ignore() if $k eq $i;
            }
        }
    }
}

=for comment
Construct the command to run quvi.
=cut

sub _build_cmd
{
    my ($self, $url, @extra_args) = @_;
    my $q = $self->{config}{quvi_path};
    my $c = qq/$q "$url" /;

    if ($self->{config}{quvi_opts})
    {
        $c .= ' ' . $self->{config}{quvi_opts};
    }
    else
    {
        $c .= join ' ', @extra_args if @extra_args;
    }
    $c;
}

=for comment
Run the quvi command.
=cut

sub _run_cmd
{
    my ($self, $cmd) = @_;

    note "run: $cmd";

    my $o = join '', qx/$cmd/;
    my $r = $? >> 8;

    print STDERR "\n$o"
        if $r == 0 and $self->{config}{dump_json};

    ($r, $o);
}

=for comment
Run quvi(1) with the specified options. Return quvi exit status and
the output (printed to stdout).
=cut

sub run
{
    my $self = shift;
    _run_cmd($self, _build_cmd($self, @_));
}

=for comment
Same as above but run quvi through valgrind.
=cut

sub run_with_valgrind
{
    my $self = shift;
    my $c = _build_cmd($self, @_);
    if ($self->{config}{valgrind_path})
    {
        my $v = $self->{config}{valgrind_path};
        $c = "libtool --mode=execute $v -q --leak-check=full "
          . "--track-origins=yes --error-exitcode=1 $c";
    }
    _run_cmd($self, $c);
}

1;
