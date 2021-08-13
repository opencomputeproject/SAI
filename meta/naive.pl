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
# @file    naive.pl
#
# @brief   This module defines SAI Metadata Parser
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

# TODO `false` or die "command failed $!";

# TODO we need to also track some defines and it's values since they can be
# used to calculate current enum field

our $INCLUDE_DIR = "../inc/";
our %SAI_ENUMS = ();
our %SAI_DEFINES = ();
our %HISTORY = ();

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
            my @ignore= ();

            $SAI_ENUMS{$currentEnum}->{values} = \@values;
            $SAI_ENUMS{$currentEnum}->{inits} = \@inits;
            $SAI_ENUMS{$currentEnum}->{ignore} = \@ignore;

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

            $enumName = "SAI_HOSTIF_TABLE_ENTRY_ATTR_START" if $currentEnum eq "sai_hostif_table_entry_attr_t" and $enumName eq "SAI_HOSTIF_ATTR_START";
            $init = "= SAI_HOSTIF_TABLE_ENTRY_ATTR_START" if $currentEnum eq "sai_hostif_table_entry_attr_t" and $init eq "= SAI_HOSTIF_ATTR_START";

            # print "$enumName, $currentEnum, $init\n";
            push @{ $SAI_ENUMS{$currentEnum}->{values} }, $enumName;
            push @{ $SAI_ENUMS{$currentEnum}->{inits} }, $init;
            push @{ $SAI_ENUMS{$currentEnum}->{ignore} }, $ignore;


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

    #print Dumper \%SAI_ENUMS;
}

sub ProcessAllEnumInitializers
{
    for my $enumTypeName (sort keys %SAI_ENUMS)
    {
        LogDebug $enumTypeName;

        my $arr_ref = $SAI_ENUMS{$enumTypeName}->{values};
        my $ini_ref = $SAI_ENUMS{$enumTypeName}->{inits};

        # TODO IGNORE WE ALSO NEED TO SORT !!!! LOL FAIL

        ProcessEnumInitializers($arr_ref, $ini_ref, $enumTypeName, \%SAI_DEFINES);
    }
}

sub BuildCommitHistory
{
    my $commit = shift;

    #print Dumper \%SAI_ENUMS;

    for my $enumTypeName (sort keys %SAI_ENUMS)
    {
        LogDebug $enumTypeName;

        my $arr_ref = $SAI_ENUMS{$enumTypeName}->{values};
        my $ini_ref = $SAI_ENUMS{$enumTypeName}->{inits};
        my $ign_ref = $SAI_ENUMS{$enumTypeName}->{ignore};

        next if grep (/<</, @$ini_ref); # TODO skip for now shifted flags enum

        my $count = scalar @$arr_ref;

        for (my $idx = 0; $idx < $count; $idx++)
        {
            my $enumName = $arr_ref->[$idx];
            my $enumInit = $ini_ref->[$idx];
            my $enumIgn  = $ign_ref->[$idx];

            LogError "wrong initializer on $enumName $enumInit" if not $enumInit =~ /^0x[0-9a-f]{8}$/;

            # TODO check ignored,
            # TODO write commit id

            if (not defined $HISTORY{$enumTypeName} or not defined $HISTORY{$enumTypeName}{$enumName})
            {
                $HISTORY{$enumTypeName}{$enumName}{name} = $enumName;
                $HISTORY{$enumTypeName}{$enumName}{value} = $enumInit;
                $HISTORY{$enumTypeName}{$enumName}{ignore} = $enumIgn;
                $HISTORY{$enumTypeName}{$enumName}{commit} = $commit; # TODO - ranges do not save ranges !

                $HISTORY{$enumTypeName}{$enumInit} = $enumName;
            }
            elsif (defined $HISTORY{$enumTypeName}{$enumName} and $HISTORY{$enumTypeName}{$enumName}{value} eq $enumInit)
            {
                # ok, value is the same
            }
            elsif ($enumName =~ /_ATTR_END$/)
            {
                # ok, end of attributes can change
            }
            else
            {
                next if $enumTypeName =~ /stat_t$/; # ignore stats errors

                LogError "check ! $enumName value is $enumInit, but on was $HISTORY{$enumTypeName}{$enumName}{value} on commit $HISTORY{$enumTypeName}{$enumName}{commit}";
            }

            #print "$enumName: $enumInit\n";
        }

        # TODO skip extensions

        #print Dumper $SAI_ENUMS{$enumTypeName}

        # TODO save in global history with enum + check if exists
        # TODO some values can shift SAI_ACL_ENTRY_ATTR_ACTION_END etc
        # next if $key =~ /^SAI_\w+_ATTR_END$/;
        # next if $key eq "SAI_ACL_TABLE_ATTR_FIELD_END";
        # next if $key eq "SAI_ACL_ENTRY_ATTR_FIELD_END";
        # next if $key eq "SAI_ACL_ENTRY_ATTR_ACTION_END";
    }
}

# TODO we need by enum - history by value as well - and monitor which one is
# ignore and ignore idgnored, and put in history only the ones that are current
# this will help us to detect potential issue if a enum was renamed but previous
# value was not put in the ignore state

#
# MAIN
#

for my $commit (@ARGV)
{
    # reset

    %SAI_ENUMS = ();
    %SAI_DEFINES = ();

    LogInfo "processing commit $commit";

    ProcessHeaders $commit;

    ProcessAllEnumInitializers();

    #print Dumper \%SAI_ENUMS;

    BuildCommitHistory $commit;
}

ExitOnErrorsOrWarnings();
