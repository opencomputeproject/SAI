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
#          each argument it allocated exactly once on every path out of it
#

use strict;
use warnings;
use diagnostics;

my $exitcode = 0;

my $file = shift @ARGV or die("usage: $0 sai_rpc_server.cpp\n");

print "checking rpc allocations for $file\n";

open(F, $file) or die("Could not open $file $! file.");

my $function = "";      # function being checked, empty between functions
my $depth = 0;          # brace depth inside the function body
my $returned = 0;       # the function level path returned
my %allocated = ();     # variable => malloc count
my %released = ();      # variable => free count on the function level path
my %freed = ();         # variable => free count in the current branch
my %skipped = ();       # variables not allocated on the current else path
my @opened = ();        # variables allocated in the open if block
my @closed = ();        # variables allocated in the last closed if block

sub error
{
    my $message = shift;

    print STDERR "ERROR: $function: $message\n";

    $exitcode = 1;
}

sub check_branch_exit
{
    my $exit = shift;

    for my $variable (sort keys %allocated)
    {
        next if defined $skipped{$variable};

        my $frees = $freed{$variable} // 0;

        next if $frees == 1;

        error("$exit after freeing $variable $frees time(s)");
    }
}

sub check_function_exit
{
    for my $variable (sort keys %allocated)
    {
        my $frees = $released{$variable} // 0;

        next if $frees == $allocated{$variable};

        error("allocates $variable $allocated{$variable} time(s) but frees it $frees time(s) before returning");
    }
}

sub start_function
{
    $function = shift;
    $depth = 1;
    $returned = 0;
    %allocated = ();
    %released = ();
    %freed = ();
    %skipped = ();
    @opened = ();
    @closed = ();
}

for my $line (<F>)
{
    chomp $line;

    my ($start) = $line =~ /^  \S.*?\b(sai_thrift_\w+)\(.*\) \{$/;

    if ($function eq "")
    {
        start_function($start) if defined $start;
        next;
    }

    if (defined $start)
    {
        error("$start starts before this function ends");
        start_function($start);
        next;
    }

    # branches are tracked line by line, other layouts would be checked wrongly

    error("unsupported branch layout: $line") if $line =~ /\}\s*else|\belse if\b/;

    if ($line =~ /^\s*if \(.*\) \{$/)
    {
        %freed = ();
        %skipped = ();
        @opened = ();
    }
    elsif ($line =~ /^\s*else \{$/)
    {
        %freed = ();
        %skipped = map { $_ => 1 } @closed;
    }

    if ($line =~ /^\s*(sai_\w+) = \(.*\*\)malloc\(/)
    {
        $allocated{$1}++;
        push @opened, $1 if $depth > 1;
    }

    for my $variable ($line =~ /\bfree\((sai_\w+)\);/g)
    {
        if (not defined $allocated{$variable})
        {
            error("frees $variable, which it never allocated");
        }
        elsif ($depth > 1)
        {
            $freed{$variable}++;
        }
        else
        {
            $released{$variable}++;
        }
    }

    check_branch_exit("throws") if $line =~ /^\s*throw e;/;

    if ($line =~ /\breturn\b/)
    {
        if ($depth > 1)
        {
            check_branch_exit("returns");
        }
        else
        {
            check_function_exit();
            $returned = 1;
        }
    }

    my $opens = () = $line =~ /\{/g;
    my $closes = () = $line =~ /\}/g;

    my $previous = $depth;

    $depth += $opens - $closes;

    if ($previous > 1 and $depth == 1)
    {
        @closed = @opened;
        @opened = ();
        %skipped = ();
    }

    next if $depth > 0;

    check_function_exit() if not $returned;

    $function = "";
}

close F;

error("file ends inside the function") if $function ne "";

exit $exitcode;
