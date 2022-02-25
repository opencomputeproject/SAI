#!/usr/bin/perl
#
# Copyright (c) 2021 Microsoft Open Technologies, Inc.
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
# @file    ancestry.pl
#
# @brief   This module defines enum ancestry check for SAI headers
#

BEGIN { push @INC,'.'; }

use strict;
use warnings;
use diagnostics;
use sort 'stable'; # for enum initializers sort

use Getopt::Std;
use Data::Dumper;
use utils;

my %options = ();

getopts("dsASl", \%options);

our $optionPrintDebug        = 1 if defined $options{d};
our $optionDisableAspell     = 1 if defined $options{A};
our $optionUseXmlSimple      = 1 if defined $options{s};
our $optionDisableStyleCheck = 1 if defined $options{S};
our $optionShowLogCaller     = 1 if defined $options{l};

$SIG{__DIE__} = sub
{
    LogError "FATAL ERROR === MUST FIX === : @_";
    exit 1;
};

our $INCLUDE_DIR = "temp";
our %SAI_ENUMS = ();
our %SAI_DEFINES = ();
our %HISTORY = ();
our %IGNORED = ();

sub ProcessSingleHeader
{
    my $header = shift;

    my $data = ReadHeaderFile $header;

    my @lines = split/\n/,$data;

    my $currentEnum = "undefined";
    my $currentEnumPrefix = "undefined";

    my $ignore = "";

    for my $line (@lines)
    {
        if ($line =~ /#define\s+(SAI_\w+)\s+(\(?".*"|$NUMBER_REGEX\)?)$/)
        {
            LogDebug "Defined $1 = $2";

            $SAI_DEFINES{$1} = $2;
            next;
        }

        if ($line =~ /^\s*typedef\s+enum\s+_((sai_\w+_)t)/)
        {
            $currentEnum = $1;
            $currentEnumPrefix = uc($2);

            my @values = ();
            my @inits = ();

            $SAI_ENUMS{$currentEnum}->{values} = \@values;
            $SAI_ENUMS{$currentEnum}->{inits} = \@inits;

            LogDebug "enum found $currentEnum";
            next;
        }

        $ignore = "ignore" if $line =~ /\@ignore/;

        if ($line =~ /^\s*(${currentEnumPrefix}\w+)(.*)$/)
        {
            my $enumName = $1;
            my $init = (defined $2) ? $2 : "";

            $init =~ s!\s*/\*.*\*/!!;    # remove potential comment
            $init =~ s/^\s*=\s*/= /;      # remove assigner
            $init =~ s/\s*,\s*$//;      # remove comma

            push @{ $SAI_ENUMS{$currentEnum}->{values} }, $enumName;
            push @{ $SAI_ENUMS{$currentEnum}->{inits} }, $init;

            $IGNORED{$enumName} = $init if $ignore ne "";

            $ignore = "";
        }
    }
}

sub ProcessHeaders
{
    my $commit = shift;

    my @headers = GetHeaderFiles("temp/commit-$commit/inc");

    for my $header (@headers)
    {
        LogDebug "Processing $header";

        ProcessSingleHeader "temp/commit-$commit/inc/$header";
    }
}

sub ProcessAllEnumInitializers
{
    for my $enumTypeName (sort keys %SAI_ENUMS)
    {
        LogDebug $enumTypeName;

        my $arr_ref = $SAI_ENUMS{$enumTypeName}->{values};
        my $ini_ref = $SAI_ENUMS{$enumTypeName}->{inits};

        ProcessEnumInitializers($arr_ref, $ini_ref, $enumTypeName, \%SAI_DEFINES);
    }
}

sub BuildCommitHistory
{
    my $commit = shift;

    for my $enumTypeName (sort keys %SAI_ENUMS)
    {
        LogDebug $enumTypeName;

        my $arr_ref = $SAI_ENUMS{$enumTypeName}->{values};
        my $ini_ref = $SAI_ENUMS{$enumTypeName}->{inits};

        my $count = scalar @$arr_ref;

        for (my $idx = 0; $idx < $count; $idx++)
        {
            my $enumName = $arr_ref->[$idx];
            my $enumValue = $ini_ref->[$idx];

            # CheckAllEnumsEndings make sure _START match _END

            next if $enumName =~ /_START$/;
            next if $enumName =~ /_END$/;
            next if $enumName =~ /_RANGE_BASE$/;

            next if $enumName eq "SAI_API_MAX";
            next if $enumName eq "SAI_OBJECT_TYPE_MAX";
            next if $enumName eq "SAI_PORT_INTERFACE_TYPE_MAX";

            LogError "wrong initializer on $enumName $enumValue" if not $enumValue =~ /^0x[0-9a-f]{8}$/;

            if (defined $HISTORY{$enumTypeName}{$enumName} and $HISTORY{$enumTypeName}{$enumName}{value} eq $enumValue)
            {
                # ok, value is the same
            }
            elsif (not defined $HISTORY{$enumTypeName} or not defined $HISTORY{$enumTypeName}{$enumName})
            {
                $HISTORY{$enumTypeName}{$enumName}{name} = $enumName;
                $HISTORY{$enumTypeName}{$enumName}{value} = $enumValue;
                $HISTORY{$enumTypeName}{$enumName}{commit} = $commit;

                if (not defined $HISTORY{$enumTypeName}{$enumValue})
                {
                    $HISTORY{$enumTypeName}{$enumValue} = $enumName;
                }
                elsif ($HISTORY{$enumTypeName}{$enumValue} eq $enumName)
                {
                    # ok this is the same enum in history
                }
                elsif (defined $IGNORED{$enumName})
                {
                    # ok, values are the sam, but enum is ignored (left for backward compatibility)
                    # but we don't check if ignored value changed, it potentially switch to different ignore
                }
                else # 2 enums have same integer value
                {
                    #print "elsif (defined $enumName $IGNORED{$enumName} and $IGNORED{$enumName} eq $HISTORY{$enumTypeName}{$enumName}{name})";

                    LogWarning "Both enums have the same value $enumName and $HISTORY{$enumTypeName}{$enumValue} = $enumValue";
                }
            }
            else
            {
                LogError "check ! $enumName value is $enumValue, but on was $HISTORY{$enumTypeName}{$enumName}{value} on commit $HISTORY{$enumTypeName}{$enumName}{commit}";

                $HISTORY{$enumTypeName}{$enumName}{value} = $enumValue;
                $HISTORY{$enumTypeName}{$enumName}{commit} = $commit;
            }
        }
    }
}

sub CleanData
{
    %SAI_ENUMS = ();
    %SAI_DEFINES = ();
    %IGNORED = ();
}

#
# MAIN
#

for my $commit (@ARGV)
{
    # reset

    LogInfo "processing commit $commit";

    CleanData();

    ProcessHeaders $commit;

    ProcessAllEnumInitializers();

    # print Dumper \%SAI_ENUMS;

    BuildCommitHistory $commit;
}

ExitOnErrorsOrWarnings();
