#!/usr/bin/env perl
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
# @file    check_primitive_buffer_size.pl
#
# @brief   Verify PRIMITIVE_BUFFER_SIZE consistency
#
# This script checks that saiserialize.c and saiserializetest.c define the same
# PRIMITIVE_BUFFER_SIZE value. It also scans the SAI header enum-style values in
# the repository root and verifies that the longest name fits in a buffer size
# defined by PRIMITIVE_BUFFER_SIZE.

use strict;
use warnings;
use FindBin qw($Bin);

use constant MACRO_NAME => 'PRIMITIVE_BUFFER_SIZE';

my @source_files = qw(
    saiserialize.c
    saiserializetest.c
);

my @header_globs = qw(
    inc/*h
    experimental/*h
);

sub find_macro_value
{
    my ($path) = @_;

    open my $fh, '<', $path or die "failed to open $path: $!\n";

    while (my $line = <$fh>)
    {
        if ($line =~ /^\s*#\s*define\s+@{[MACRO_NAME]}\s+(\d+)\b/)
        {
            return $1;
        }
    }

    die MACRO_NAME . " is not defined in $path\n";
}

sub find_longest_header_name
{
    my ($repo_dir) = @_;
    my @headers;

    for my $header_glob (@header_globs)
    {
        push @headers, sort glob "$repo_dir/$header_glob";
    }

    die "no header files found to check\n" unless @headers;

    my ($longest_length, $longest_name);

    for my $header (@headers)
    {
        open my $fh, '<', $header or die "failed to open $header: $!\n";

        while (my $line = <$fh>)
        {
            next unless $line =~ /^\s+SAI_/;
            next if $line =~ /=/;

            chomp $line;
            $line =~ s/^\s+//;
            $line =~ s/,\s*$//;

            my $length = length $line;

            if (!defined $longest_length || $length > $longest_length)
            {
                $longest_length = $length;
                $longest_name = $line;
            }
        }
    }

    die "no SAI names found in header files\n" unless defined $longest_length;

    return ($longest_length, $longest_name);
}

sub main
{
    my %values;

    for my $source_file (@source_files)
    {
        my $path = "$Bin/$source_file";
        $values{$source_file} = find_macro_value($path);
        print "$source_file: ", MACRO_NAME, "=$values{$source_file}\n";
    }

    my %unique_values = map { $_ => 1 } values %values;

    die MACRO_NAME . " values do not match\n" if keys(%unique_values) != 1;

    my ($primitive_buffer_size) = keys %unique_values;
    my ($longest_length, $longest_name) = find_longest_header_name("$Bin/..");

    print "longest header name: $longest_length -> $longest_name\n";

    if ($longest_length >= $primitive_buffer_size)
    {
        die "longest header name length $longest_length must be less than " .
            MACRO_NAME . " ($primitive_buffer_size)\n";
    }

    print "ok: definitions match and all header names fit\n";
    return 0;
}

eval { exit main(); };

chomp $@;
print STDERR "error: $@\n";
exit 1;
