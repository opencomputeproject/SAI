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
# @file    structs.pl
#
# @brief   This module defines structs backward compatibility check for SAI headers
#

BEGIN { push @INC,'.'; }

use strict;
use warnings;
use diagnostics;
use sort 'stable'; # for sort

use Getopt::Std;
use Data::Dumper;
use utils;

my %options = ();

getopts("dsASlDH:", \%options);

our $optionPrintDebug        = 1 if defined $options{d};
our $optionDisableAspell     = 1 if defined $options{A};
our $optionUseXmlSimple      = 1 if defined $options{s};
our $optionDisableStyleCheck = 1 if defined $options{S};
our $optionShowLogCaller     = 1 if defined $options{l};
our $optionDumpHistoryFile   = 1 if defined $options{D};
our $optionHistoryFile       = $options{H} if defined $options{H};

$SIG{__DIE__} = sub
{
    LogError "FATAL ERROR === MUST FIX === : @_";
    exit 1;
};

our $INCLUDE_DIR = "temp";
our %SAI_STRUCTS = ();
our %HISTORY = ();

# TODO: union should be treated as hash map, since order don't matter

sub ProcessSingleHeader
{
    my $header = shift;

    my $data = ReadHeaderFile $header;

    my @lines = split/\n/,$data;

    my $currentStruct = "undefined";

    for my $line (@lines)
    {
        if ($line =~ /^\s*typedef\s+(struct|union)\s+_((sai_\w+_)t)/)
        {
            $currentStruct = $2;

            my @fields = ();

            $SAI_STRUCTS{$currentStruct}->{fields} = \@fields;
            $SAI_STRUCTS{$currentStruct}->{type} = $1;

            LogDebug "found $1 $currentStruct";
            next;
        }

        $currentStruct = "undefined" if $line =~ /}\s*$currentStruct/;

        next if $currentStruct eq "undefined";

        next if $line =~ /^{?$/;        # skip struct open bracket and empty lines
        next if $line =~ m!^\s*/?\*!;   # skip comment

        if ($line =~ /^\s*((\w+)\s+\*?(\w+)(\[\d+\])?);/)
        {
            my $field = $1;

            $field =~ s/\s+/ /g;

            # NOTE: field type is as string here, is of actual type is complex,
            # like for example function pointer and parameters of that function
            # will change, then this check will not catch that, we will need
            # then a much more complex type checking, it could be added

            push @{ $SAI_STRUCTS{$currentStruct}->{fields} }, $field;
        }
        else
        {
            LogError "unknown struct/union line '$line', FIXME";
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

sub BuildCommitHistory
{
    my $commit = shift;

    for my $structTypeName (sort keys %SAI_STRUCTS)
    {
        LogDebug $structTypeName;

        if ($structTypeName eq "sai_object_key_entry_t")
        {
            # skip this union, since it contain experimental entries
            # and it can be modified time to time
            next;
        }

        my $arr_ref = $SAI_STRUCTS{$structTypeName}->{fields};
        my $type = $SAI_STRUCTS{$structTypeName}->{type};

        if (not defined $HISTORY{$structTypeName})
        {
            $HISTORY{$structTypeName}{values} = $arr_ref;
            next;
        }

        LogDebug "compare $structTypeName on $commit";

        my $hist_arr_ref = $HISTORY{$structTypeName}{values};

        my $currCount = scalar @$arr_ref;
        my $histCount = scalar @$hist_arr_ref;

        LogDebug "histCount $histCount vs currCount $currCount on $structTypeName";

        # NOTE: we allow api structs to change member count since we can add new api
        # at the end of the struct.
        #
        # NOTE: we also allow to change number of members in union, since size
        # of union may not increase by adding members, and actual union size
        # check is performed by sai sanity check

        if ($currCount != $histCount and not $structTypeName =~ /^sai_\w+_api_t$/
                and $structTypeName ne "sai_switch_health_data_t"
                and $structTypeName ne "sai_port_oper_status_notification_t")
        {
            LogError "FATAL: struct $structTypeName member count differs, was $histCount but is $currCount on commit $commit" if $type eq "struct";
        }

        if ($histCount > $currCount)
        {
            LogError "FATAL: $structTypeName members were removed on commit $commit, NOT ALLOWED!";
            exit 1;
        }

        my $minCount = ($histCount > $currCount) ? $currCount : $histCount;

        for (my $idx = 0; $idx < $minCount; $idx++)
        {
            my $hist_field = $hist_arr_ref->[$idx];
            my $field = $arr_ref->[$idx];

            next if $hist_field eq $field;

            LogError "FATAL: field on index $idx do not match, was '$hist_field' and now is '$field' on $commit";
        }

        if ($histCount != $currCount)
        {
            LogInfo "updating $structTypeName since member count changed from $histCount to $currCount";

            $HISTORY{$structTypeName}{values} = $arr_ref;
        }

        # NOTE: we could allow some other structs than *_api_t to also be
        # extended in th future (added fields at the end), for example simple
        # structures that are used as attribute value, and not lists
    }
}

sub CleanData
{
    %SAI_STRUCTS = ();
}

#
# MAIN
#

if (defined $optionHistoryFile)
{
    my $history = ReadHeaderFile($optionHistoryFile);

    eval($history) or die "failed to eval history file: $optionHistoryFile";

    die "history file $optionHistoryFile not complete, missing too many keys" if scalar keys %HISTORY < 133;

    LogInfo "loaded history from $optionHistoryFile";
}

for my $commit (@ARGV)
{
    # reset

    LogInfo "processing commit $commit";

    CleanData();

    ProcessHeaders $commit;

    #print Dumper \%SAI_STRUCTS;

    BuildCommitHistory $commit;
}

ExitOnErrorsOrWarnings();

if (defined $optionDumpHistoryFile and (scalar @ARGV > 0))
{
    $Data::Dumper::Indent = 0;

    my $history = Data::Dumper->Dump([\%HISTORY],[qw/*HISTORY/]);

    my $lastCommit = $ARGV[-1];

    WriteFile("structs.$lastCommit.history", $history);

    LogInfo "ancestry history file saved to: structs.$lastCommit.history";
}
