#!/usr/bin/perl
#
# Copyright (c) 2014 Microsoft Open Technologies, Inc.
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
# @file    checkheaders.pl
#
# @brief   This module defines integration check for 2 header directories
#

BEGIN { push @INC,'.'; }

use strict;
use warnings;
use diagnostics;

use File::Temp qw/ tempfile /;
use Data::Dumper;
use Getopt::Std;

my %options = ();
getopts("s", \%options);

my $optionSkipSingleDefined = 1 if defined $options{s};

use utils;

sub CheckArguments
{
    if (scalar @ARGV != 2)
    {
        print "expected 2 arguments which are SAI/inc directories: dirA dirB\n";
        exit 1;
    }
}

sub CheckDirExists
{
    my $dir = shift;

    if (not -d $dir)
    {
        print "not directory $dir\n";
        exit 1;
    }
}

CheckArguments;

sub FilterEnums
{
    my ($data, $path) = @_;

    my @lines = split/[\r\n]+/,$data;

    my %enums = ();

    my $nr = 0;

    for my $line (@lines)
    {
        $nr++;

        next if not $line =~ /^\s*(SAI_\w+)/;

        my $key = $1;

        $enums{$key}{path} = $path;
        $enums{$key}{line} = $line;
        $enums{$key}{nr} = $nr;
        $enums{$key}{value} = "undefined";
    }

    return %enums;
}

my $dirA = shift @ARGV;
my $dirB = shift @ARGV;

sub GetEnums
{
    my $dir = shift;

    CheckDirExists $dir;

    my @files = GetHeaderFiles $dir;

    my %enums = ();

    for my $file (@files)
    {
        my $data = ReadHeaderFile("$dir/$file");

        my %en = FilterEnums($data, "$dir/$file");

        %enums = (%enums, %en);
    }

    return %enums;
}

sub ConstructSource
{
    my ($dir, $ref) = @_;

    my %enums = %{ $ref };

    my $source = "#include <stdio.h>\n";

    $source .= "#include \"$dir/sai.h\"\n";
    $source .="int main() { ";

    for my $en (sort keys %enums)
    {
        $source .= "printf(\"%d\\n\", $en);\n";
    }

    $source .= "return 0; }";

    return $source;
}

sub GetValues
{
    my $dir = shift;

    my %enums = GetEnums $dir;

    my $source = ConstructSource($dir, \%enums);

    my ($fhs, $src) = tempfile( SUFFIX => '.c', UNLINK => 1 );

    WriteFile($src, $source);

    my ($fhb, $bin) = tempfile( SUFFIX => '.bin', UNLINK => 1  );

    system("gcc $src -I. -I '$dir' -o $bin") == 0 or die "gcc failed! $!";

    close $fhs;
    close $fhb;

    my %hash = ();

    my @lines = `$bin`;

    for my $key (sort keys %enums)
    {
        my $line = shift @lines;

        $enums{$key}{value} = $1 if $line =~ /(\d+)/;
    }

    return %enums;
}

my %valuesA = GetValues $dirA;
my %valuesB = GetValues $dirB;

File::Temp::cleanup();

sub CheckHash
{
    my ($refA, $refB) = @_;

    my %A = %{$refA};
    my %B = %{$refB};

    for my $key (sort keys %A)
    {
        if (defined $optionSkipSingleDefined)
        {
            # ignore attributes end, since those will shift
            next if $key =~ /^SAI_\w+_ATTR_END$/;

            next if $key eq "SAI_IN_DROP_REASON_END";
            next if $key eq "SAI_ACL_TABLE_ATTR_FIELD_END";
            next if $key eq "SAI_ACL_ENTRY_ATTR_FIELD_END";
            next if $key eq "SAI_ACL_ENTRY_ATTR_ACTION_END";
            next if $key eq "SAI_OBJECT_TYPE_MAX";
            next if $key eq "SAI_API_MAX";
            next if $key eq "SAI_PORT_INTERFACE_TYPE_MAX";

            # NOTE: some other attributes/enum with END range could be added
        }

        if (not defined $B{$key})
        {
            if (not defined $optionSkipSingleDefined)
            {
                LogError "enum $key only defined in $A{$key}{path}:$A{$key}{nr}"
            }
            else
            {
                LogInfo "enum $key only defined in $A{$key}{path}:$A{$key}{nr}"
            }
            next;
        }

        my $valA = $A{$key}{value};
        my $valB = $B{$key}{value};

        if ($valA ne $valB)
        {
            my $locA = "$A{$key}{path}:$A{$key}{nr}";
            my $locB = "$B{$key}{path}:$B{$key}{nr}";

            LogError "value of $key differ: $locA vs $locB => ($valA != $valB)";
        }
    }
}

CheckHash(\%valuesA, \%valuesB);
CheckHash(\%valuesB, \%valuesA);

ExitOnErrors;

LogInfo "headers $dirA and $dirB MATCH!";
