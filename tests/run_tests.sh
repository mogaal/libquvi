#!/bin/sh
#
# libquvi
# Copyright (C) 2012  Toni Gundogdu <legatvs@gmail.com>
#
# This file is part of libquvi <http://quvi.sourceforge.net/>.
#
# This program is free software: you can redistribute it and/or
# modify it under the terms of the GNU Affero General Public
# License as published by the Free Software Foundation, either
# version 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General
# Public License along with this program.  If not, see
# <http://www.gnu.org/licenses/>.
#
set -e

# cmds
Z='zenity'
# flags
M= # run mem tests
# arrays 
T= # tests

find_tests()
{
  f="`dirname $0`/find_tests.sh"
  T=`$f | sort -hr` || {
    echo "error: $! ($?)"
    exit 1
  }
  [ -n "$T" ] || {
    echo "error: tests not found"
    exit 1
  }
}

choose_skipped_tests()
{
  a=; for t in $T; do
    a="FALSE $t $a"
  done
  o='--list --width 320 --height 480 --separator=,'
  o="$o --column Skip --column Test"
  T=`$Z $o --checklist $a`
  export TEST_SKIP="$T"
#  echo "TEST_SKIP=$TEST_SKIP"
  return 0
}

choose_test_opts()
{
  c='--column Enable --column Test'
  a='false TEST_INTERNET false TEST_VERBOSE'
  r=`$Z --width 320 --height 240 --list --checklist $c $a`
  expr "$r" : ".*INTERNET.*" >/dev/null && export TEST_INTERNET=1
  expr "$r" : ".*VERBOSE.*" >/dev/null && export TEST_VERBOSE=1
  return 0
}

run_tests()
{
  echo "TEST_INTERNET=$TEST_INTERNET"
  echo "TEST_VERBOSE=$TEST_VERBOSE"
  make test
  return 0
}

run_tests_mem()
{
  echo "TODO: run_tests_mem"
  exit 0
}

help()
{
  echo "Usage: $0 [OPTIONS]

$0 is a convenience script for running the tests.

OPTIONS
  -h  Show this help and exit
  -m  Run target 'test-mem'

NOTE: Run 'configure' script first

  Example:
    cd \$top_srcdir ; mkdir tmp ; cd tmp
    ../configure && make && ../$0

$0 sets the following environment variables before
running the tests:
  TEST_INTERNET
  TEST_VERBOSE
  TEST_SKIP

Refer to the curl(1) manual for a complete list of proxy environment
variables. The testsuite does not set any by default."
  exit 0
}

while [ $# -gt 0 ]
do
  case "$1" in
    -h) help;;
    -m) M=1;;
    *) break;;
  esac
  shift
done

[ -f "config.log" ] || {
  echo "error: config.log not found: have you run configure?"
  exit 1
}

find_tests
choose_skipped_tests
choose_test_opts

if [ -z "$M" ]; then
  run_tests
else
  run_tests_mem
fi

# vim: set ts=2 sw=2 tw=72 expandtab:
