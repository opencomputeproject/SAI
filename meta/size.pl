#!/usr/bin/perl
#
# Copyright (c) 2023 Microsoft Open Technologies, Inc.
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
# @file    size.pl
#
# @brief   This module defines SAI struct/union size generator
#

BEGIN { push @INC,'.'; }

use strict;
use warnings;
use diagnostics;
use sort 'stable';

use utils;
use File::Temp qw/ tempfile /;

our $SIZE_CONTENT = "";

sub WriteSize
{
    my $content = shift;

    $SIZE_CONTENT .= $content . "\n";
}

our %STRUCTS=();
our $TEMP_DIR = "..";

sub CheckArguments
{
    if (scalar @ARGV != 1)
    {
        print "expected 1 argument which is temporary SAI directory\n";
        exit 1;
    }

    $TEMP_DIR = shift @ARGV;
}

sub ExtractStructsAndUnions
{
    my @headers = GetHeaderFiles("$TEMP_DIR/inc/"); # we ignore experimental headers

    for my $header (@headers)
    {
        my $data = ReadHeaderFile("$TEMP_DIR/inc/".$header);

        my @lines = split/\n/,$data;

        for my $line (@lines)
        {
            next if not $line =~ /typedef\s+(?:struct|union)\s+_(sai_\w+_t)/;

            $STRUCTS{$1}=$1;
        }
    }
}

sub ConstructSource
{
    my $source = "#include <stdio.h>\n";

    $source .= "#include <sai.h>\n";
    $source .= "#include <saiextensions.h>\n";
    $source .= "int main() {\n";

    for my $struct (sort keys %STRUCTS)
    {
        my $upname = uc($struct);

        $source .= "printf(\"#define ${upname}_SIZE (%zu)\\n\", sizeof($struct));\n";
    }

    $source .= "return 0; }";

    return $source;
}

sub GetValues
{
    my $dir = $TEMP_DIR;

    my $source = ConstructSource();

    my ($fhs, $src) = tempfile( SUFFIX => '.c', UNLINK => 1 );

    WriteFile($src, $source);

    my ($fhb, $bin) = tempfile( SUFFIX => '.bin', UNLINK => 1  );

    LogDebug("gcc $src -I$dir/inc/ -I$dir/experimental/ -o $bin");

    system("gcc $src -I$dir/inc/ -I$dir/experimental/ -o $bin") == 0 or die "gcc failed! $!";

    close $fhs;
    close $fhb;

    my %hash = ();

    my @lines = `$bin`;

    for my $line(@lines)
    {
        chomp $line;

        WriteSize($line);
    }
}

CheckArguments();

WriteSize("#ifndef __SAI_METADATASIZE_H__");
WriteSize("#define __SAI_METADATASIZE_H__");

ExtractStructsAndUnions();

GetValues();

WriteSize("#endif /* __SAI_METADATASIZE_H__ */");

WriteFile("saimetadatasize.h", $SIZE_CONTENT);
