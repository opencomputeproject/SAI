#!/usr/bin/perl
#
# Copyright (c) 2026 Microsoft Open Technologies, Inc.
#
#    Licensed under the Apache License, Version 2.0 (the "License"); you may
#    not use this file except in compliance with the License. You may obtain
#    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
#
#    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
#    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
#    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
#    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
#
#    See the Apache Version 2.0 License for specific language governing
#    permissions and limitations under the License.
#
#    Microsoft would like to thank the following companies for their review and
#    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
#    Dell Products, L.P., Facebook, Inc., Marvell International Ltd.
#
# @file    checkrpcleaks.pl
#
# @brief   This module checks that every generated RPC server function frees
#          the arguments it allocated before it throws an exception
#

use strict;
use warnings;
use diagnostics;

my $exitcode = 0;

my $file = shift @ARGV or die("usage: $0 sai_rpc_server.cpp\n");

print "checking rpc allocations for $file\n";

open(F, $file) or die("Could not open $file $! file.");

my $function = "";
my @allocated = ();
my %freed = ();

for my $line (<F>)
{
    chomp $line;

    if ($line =~ /^  \S.*?\b(sai_thrift_\w+)\(.*\) \{$/)
    {
        $function = $1;
        @allocated = ();
        %freed = ();
        next;
    }

    if ($line =~ /^\s*(sai_\w+) = \(.*\*\)malloc\(/)
    {
        push @allocated, $1;
        next;
    }

    # each error branch starts with an if, only frees inside it count

    if ($line =~ /^\s*if \(.*\) \{$/)
    {
        %freed = ();
        next;
    }

    if ($line =~ /^\s*free\((sai_\w+)\);/)
    {
        $freed{$1} = 1;
        next;
    }

    next if not $line =~ /^\s*throw e;/;

    for my $variable (@allocated)
    {
        next if defined $freed{$variable};

        print STDERR "ERROR: $function throws without freeing $variable\n";

        $exitcode = 1;
    }
}

close F;

exit $exitcode;
