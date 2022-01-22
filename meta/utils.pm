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
# @file    utils.pm
#
# @brief   This module defines SAI Metadata Utils Parser
#

package utils;

use strict;
use warnings;
use diagnostics;
use Term::ANSIColor;

require Exporter;

our $NUMBER_REGEX = '(?:-?\d+|0x[A-F0-9]+)';

our $errors = 0;
our $warnings = 0;

our $HEADER_CONTENT = "";
our $SOURCE_CONTENT = "";
our $TEST_CONTENT = "";
our $SWIG_CONTENT = "";

my $identLevel = 0;

sub GetIdent
{
    my $content = shift;

    return ""                       if $content =~ /\\$/;
    return "    "                   if $content =~ /^\s*_(In|Out)/;
    return "    " x --$identLevel   if $content =~ /^\s*%?}/;
    return "    " x $identLevel++   if $content =~ /{$/;
    return "    " x $identLevel;
}

sub WriteHeader
{
    my $content = shift;

    my $ident = GetIdent($content);

    my $line = $ident . $content . "\n";

    $line = "\n" if $content eq "";

    $HEADER_CONTENT .= $line;
}

sub WriteSource
{
    my $content = shift;

    my $ident = GetIdent($content);

    my $line = $ident . $content . "\n";

    $line = "\n" if $content eq "";

    $SOURCE_CONTENT .= $line;
}

sub WriteTest
{
    my $content = shift;

    my $ident = ""; # TODO tests should have it's own ident, since it's different file GetIdent($content);

    $TEST_CONTENT .= $ident . $content . "\n";
}

sub WriteSwig
{
    my $content = shift;

    my $ident = GetIdent($content);

    $SWIG_CONTENT .= $ident . $content . "\n";
}

sub WriteSourceSectionComment
{
    my $content = shift;

    WriteSource "\n/* $content */\n";
}

sub WriteSectionComment
{
    my $content = shift;

    WriteHeader "\n/* $content */\n";
    WriteSource "\n/* $content */\n";
}

sub GetCallerInfo
{
    return "" if not defined $main::optionShowLogCaller;

    my ($package, $filename, $line, $sub) = caller(1);

    my $logLine = $line;

    ($package, $filename, $line, $sub) = caller(2);

    return "$sub($logLine): ";
}

sub LogDebug
{
    my $sub = GetCallerInfo();

    print color('bright_blue') . "$sub@_" . color('reset') . "\n" if $main::optionPrintDebug;
}

sub LogInfo
{
    my $sub = GetCallerInfo();

    print color('bright_green') . "$sub@_" . color('reset') . "\n";
}

sub LogWarning
{
    my $sub = GetCallerInfo();

    $warnings++;
    print color('bright_yellow') . "WARNING: $sub@_" . color('reset') . "\n";
}

sub LogError
{
    my $sub = GetCallerInfo();

    $errors++;
    print color('bright_red') . "ERROR: $sub@_" . color('reset') . "\n";
}

sub WriteFile
{
    my ($file, $content) = @_;

    open (F, ">", $file) or die "$0: open $file $!";

    print F $content;

    close F;
}

sub GetHeaderFiles
{
    my $dir = shift;

    $dir = $main::INCLUDE_DIR if not defined $dir;

    opendir(my $dh, $dir) or die "Can't opendir $dir: $!";

    my @headers = grep { /^sai\w*\.h$/ and -f "$dir/$_" } readdir($dh);

    closedir $dh;

    return sort @headers;
}

sub GetMetaSourceFiles
{
    my $dir = shift;

    $dir = "." if not defined $dir;

    opendir(my $dh, $dir) or die "Can't opendir $dir: $!";

    my @sources = grep { /^sai\w*\.(c|cpp)$/ and -f "$dir/$_" } readdir($dh);

    closedir $dh;

    return sort @sources;
}

sub GetMetaHeaderFiles
{
    return GetHeaderFiles(".");
}

sub GetExperimentalHeaderFiles
{
    return GetHeaderFiles($main::EXPERIMENTAL_DIR);
}

sub GetFilesByRegex
{
    my ($dir,$regex) = @_;

    $dir = $main::INCLUDE_DIR if not defined $dir;

    opendir(my $dh, $dir) or die "Can't opendir $dir: $!";

    my @files = grep { /$regex/ and -f "$dir/$_" } readdir($dh);

    closedir $dh;

    return sort @files;
}

sub GetMetadataSourceFiles
{
    my $dir = ".";

    my @sources;

    push @sources, GetFilesByRegex($dir, '^\w+\.(pm|pl|h|cpp|c|sh)$');
    push @sources, GetFilesByRegex($dir, '^Makefile$');

    return @sources;
}

sub ReadHeaderFile
{
    my $file = shift;

    local $/ = undef;

    # first search file in meta directory

    my $filename = $file;

    $filename = "$main::INCLUDE_DIR/$file" if not -e $filename;
    $filename = "$main::EXPERIMENTAL_DIR/$file" if not -e $filename;

    open FILE, $filename or die "Couldn't open file $filename: $!";

    binmode FILE;

    my $string = <FILE>;

    close FILE;

    return $string;
}

sub GetNonObjectIdStructNames
{
    my %structs;

    my @headers = (GetHeaderFiles(), GetExperimentalHeaderFiles());

    # TODO must support experimental extensions

    for my $header (@headers)
    {
        my $data = ReadHeaderFile($header);

        # TODO there should be better way to extract those

        while ($data =~ /sai_(?:create|set)_\w+.+?\n.+const\s+(sai_(\w+)_t)/gim)
        {
            my $name = $1;
            my $rawname = $2;

            $structs{$name} = $rawname;

            if (not $name =~ /_entry_t$/)
            {
                LogError "non object id struct name '$name'; should end on _entry_t";
                next;
            }
        }
    }

    return sort values %structs;
}

sub GetStructLists
{
    my $data = ReadHeaderFile("$main::INCLUDE_DIR/saitypes.h");

    my %StructLists = ();

    my @lines = split/\n/,$data;

    for my $line (@lines)
    {
        next if not $line =~ /typedef\s+struct\s+_(sai_\w+_list_t)/;

        $StructLists{$1} = $1;
    }

    return %StructLists;
}

sub IsSpecialObject
{
    my $objectType = shift;

    return ($objectType eq "SAI_OBJECT_TYPE_FDB_FLUSH" or $objectType eq "SAI_OBJECT_TYPE_HOSTIF_PACKET");
}

sub SanityCheckContent
{
    # since we generate so much metadata now
    # lets put some primitive sanity check
    # if everything we generated is fine

    my $testCount = @test::TESTNAMES;

    if ($testCount < 5)
    {
        LogError "there should be at least 5 test defined, got $testCount";
    }

    my $metaHeaderSize =  127588 * 0.99;
    my $metaSourceSize = 5190419 * 0.99;
    my $metaTestSize   =  195323 * 0.99;

    if (length($HEADER_CONTENT) < $metaHeaderSize)
    {
        LogError "generated saimetadata.h size is too small";
    }

    if (length($SOURCE_CONTENT) < $metaSourceSize)
    {
        LogError "generated saimetadata.c size is too small";
    }

    if (length($TEST_CONTENT) < $metaTestSize)
    {
        LogError "generated saimetadatatest.c size is too small";
    }
}

sub WriteMetaDataFiles
{
    SanityCheckContent();

    exit 1 if ($warnings > 0 or $errors > 0);

    WriteFile("saimetadata.h", $HEADER_CONTENT);
    WriteFile("saimetadata.c", $SOURCE_CONTENT);
    WriteFile("saimetadatatest.c", $TEST_CONTENT);
    WriteFile("saiswig.i", $SWIG_CONTENT);
}

sub GetStructKeysInOrder
{
    my $structRef = shift;

    my @values = ();

    for my $key (keys %$structRef)
    {
        $values[$structRef->{$key}->{idx}] = $key;
    }

    return @values;
}

sub Trim
{
    my $value = shift;

    $value =~ s/\s+/ /g;
    $value =~ s/^\s*//;
    $value =~ s/\s*$//;

    return $value;
}

sub ExitOnErrors
{
    return if $errors == 0;

    LogError "please corret all $errors error(s) before continue";

    exit 1;
}

sub ExitOnErrorsOrWarnings
{
    return if $errors == 0 and $warnings == 0;

    LogError "please corret all $errors error(s) and all $warnings warnings before continue";

    exit 1;
}

sub ProcessEnumInitializers
{
    #
    # This function attempts to figure out enum integers values during paring
    # time in similar way as C compiler would do. Because SAI community agreed
    # that enum grouping is more beneficial then ordering enums, then enum
    # values could be not sorted any more. But if we figure out integers
    # values, we could perform stable sort at this parser level, and generate
    # enums metadata where enum values are sorted.
    #

    my ($arr_ref, $ini_ref, $enumTypeName, $SAI_DEFINES_REF) = @_;

    return if $enumTypeName =~ /_extensions_t$/; # ignore initializers on extensions

    if (scalar(@$arr_ref) != scalar(@$ini_ref))
    {
        LogError "attr array not matching initializers array on $enumTypeName";
        return;
    }

    #return if grep (/<</, @$ini_ref); # skip shifted flags enum

    my $previousEnumValue = -1;

    my $idx = 0;

    # using reference here, will cause update $ini inside initializer table
    # reference and that's what we want

    for my $ini (@$ini_ref)
    {
        if ($ini eq "")
        {
            $previousEnumValue += 1;

            $ini = sprintf("0x%08x", $previousEnumValue);
        }
        elsif ($ini =~ /^= (0x[0-9a-f]{8})$/)
        {
            $previousEnumValue = hex($1);

            $ini = sprintf("0x%08x", $previousEnumValue);
        }
        elsif ($ini =~ /^=\s+(\d+)$/)
        {
            $previousEnumValue = hex($1);

            $ini = sprintf("0x%08x", $previousEnumValue);
        }
        elsif ($ini =~ /= (SAI_\w+)$/)
        {
            for my $i (0..$idx)
            {
                if ($$arr_ref[$i] eq $1)
                {
                    $ini = @$ini_ref[$i];

                    $previousEnumValue = hex($ini);
                    last;
                }
            }

            LogError "initializer $ini not found on $enumTypeName before $$arr_ref[$idx]" if not $ini =~ /^0x/;
        }
        elsif ($ini =~ /^= (SAI_\w+) \+ (SAI_\w+)$/) # special case SAI_ACL_USER_DEFINED_FIELD_ATTR_ID_RANGE
        {
            # this case is in form: = (sai enum value) + (sai define)

            my $first = $1;

            my $val = $SAI_DEFINES_REF->{$2};

            if (not defined $val)
            {
                LogError "Value $2 not defined using #define directive";
            }
            elsif (not $val =~ /^0x[0-9a-f]+$/i)
            {
                LogError "$val not in hex format 0xYY";
            }
            else
            {
                for my $i (0..$idx)
                {
                    if ($$arr_ref[$i] eq $first)
                    {
                        $ini = sprintf("0x%08x", hex(@$ini_ref[$i]) + hex($val));

                        $previousEnumValue = hex($ini);
                        last;
                    }
                }

                LogError "initializer $ini not found on $enumTypeName before $$arr_ref[$idx]" if not $ini =~ /^0x/;
            }
        }
        elsif ($ini =~/^= (SAI_\w+) \+ (\d+)$/)
        {
            my $first = $1;
            my $val = $2;

            for my $i (0..$idx)
            {
                if ($$arr_ref[$i] eq $first)
                {
                    $ini = sprintf("0x%08x", hex(@$ini_ref[$i]) + $val);

                    $previousEnumValue = hex($ini);
                    last;
                }
            }

            LogError "initializer $ini not found on $enumTypeName before $$arr_ref[$idx]" if not $ini =~ /^0x/;
        }
        elsif ($ini =~/^= (SAI_\w+) \+ (0x[0-9a-f]{1,8})$/)
        {
            my $first = $1;
            my $val = $2;

            for my $i (0..$idx)
            {
                if ($$arr_ref[$i] eq $first)
                {
                    $ini = sprintf("0x%08x", hex(@$ini_ref[$i]) + hex($val));

                    $previousEnumValue = hex($ini);
                    last;
                }
            }

            LogError "initializer $ini not found on $enumTypeName before $$arr_ref[$idx]" if not $ini =~ /^0x/;
        }
        elsif ($ini =~ /^= \(?(\d+) << (\d+)\)?$/)
        {
            $previousEnumValue = $1 << $2;

            $ini = sprintf("0x%08x", $previousEnumValue);
        }
        else
        {
            LogError "not supported initializer '$ini' on $$arr_ref[$idx], FIXME";
        }

        $idx++;
    }

    # in final form all initializers must be hex numbers 8 digits long, since
    # they will be used in stable sort

    if (scalar(grep (/^0x[0-9a-f]{8}$/, @$ini_ref)) != scalar(@$ini_ref))
    {
        LogError "wrong initializers on $enumTypeName: @$ini_ref";
        return;
    }

    my $before = "@$arr_ref";

    my @joined = ();

    for my $idx (0..$#$arr_ref)
    {
        push @joined, "$$ini_ref[$idx]$$arr_ref[$idx]"; # format is: 0x00000000SAI_
    }

    my @sorted = sort { substr($a, 0, 10) cmp substr($b, 0, 10) } @joined;

    s/^0x[0-9a-f]{8}SAI/SAI/i for @sorted;

    my $after = "@sorted";

    return if $after eq $before;

    LogDebug "Need sort initalizers for $enumTypeName";

    @$arr_ref = ();

    push @$arr_ref, @sorted;
}


BEGIN
{
    our @ISA    = qw(Exporter);
    our @EXPORT = qw/
    LogDebug LogInfo LogWarning LogError
    WriteFile GetHeaderFiles GetMetaHeaderFiles GetExperimentalHeaderFiles GetMetadataSourceFiles ReadHeaderFile GetMetaSourceFiles
    GetNonObjectIdStructNames IsSpecialObject GetStructLists GetStructKeysInOrder
    Trim ExitOnErrors ExitOnErrorsOrWarnings ProcessEnumInitializers
    WriteHeader WriteSource WriteTest WriteSwig WriteMetaDataFiles WriteSectionComment WriteSourceSectionComment
    $errors $warnings $NUMBER_REGEX
    $HEADER_CONTENT $SOURCE_CONTENT $TEST_CONTENT
    /;
}

1;
