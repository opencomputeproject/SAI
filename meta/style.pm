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
# @file    style.pm
#
# @brief   This module defines SAI Metadata Style Parser
#

package style;

use strict;
use warnings;
use diagnostics;
use Data::Dumper;
use utils;

require Exporter;

sub RunAspell
{
    my $hash = shift;

    my %wordsToCheck = %{ $hash };

    if (not -e "/usr/bin/aspell")
    {
        LogError "ASPELL IS NOT PRESENT, please install aspell";
        return;
    }

    LogInfo "Running Aspell";

    my @keys = sort keys %wordsToCheck;

    my $count = @keys;

    my $all = "@keys";

    LogInfo "Words to check: $count";

    my @result = `echo "$all" | /usr/bin/aspell -l en -a -p ./aspell.en.pws 2>&1`;

    for my $res (@result)
    {
        if ($res =~ /error/i)
        {
            LogError "aspell error: $res";
            last;
        }

        next if not $res =~ /^\s*&\s*(\S+)/;

        my $word = $1;

        chomp $res;

        my $where = "??";

        if (not defined $wordsToCheck{$word})
        {
            for my $k (@keys)
            {
                if ($k =~ /(^$word|$word$)/)
                {
                    $where = $wordsToCheck{$k};
                    last;
                }

                $where = $wordsToCheck{$k} if ($k =~ /$word/);
            }
        }
        else
        {
            $where = $wordsToCheck{$word};
        }

        if ($word =~ /^[A-Z0-9]{2,}$/)
        {
            LogWarning "Word '$word' is misspelled or is acronym, add to acronyms.txt? $where";
        }
        else
        {
            LogWarning "Word '$word' is misspelled $where";
        }
    }
}

sub CheckDoxygenStyle
{
    my ($header, $line, $n) = @_;

    return if (not $line =~ /\@(\w+)/);

    my $mark = $1;

    if ($mark eq "file" and not $line =~ /\@file\s+($header)/)
    {
        LogWarning "\@file should match format: sai\\w+.h: $header $n:$line";
        return;
    }

    if ($mark eq "brief" and not $line =~ /\@brief\s+[A-Z]/)
    {
        LogWarning "\@brief should start with capital letter: $header $n:$line";
        return;
    }

    if ($mark eq "return" and not $line =~ /\@return\s+(#SAI_|[A-Z][a-z])/)
    {
        LogWarning "\@return should start with #: $header $n:$line";
        return;
    }

    if ($mark eq "param" and not $line =~ /\@param\[(in|out|inout)\] (\.\.\.|[a-z]\w+)\s+([A-Z]\w+)/)
    {
        LogWarning "\@param should be in format \@param[in|out|inout] [a-z]\\w+ [A-Z]\\w+: $header $n:$line";
        return;
    }

    if ($mark eq "defgroup" and not $line =~ /\@defgroup SAI\w* SAI - \w+/)
    {
        LogWarning "\@defgroup should be in format \@defgroup SAI\\w* SAI - \\w+: $header $n:$line";
        return;
    }
}

sub ExtractComments
{
    my $input = shift;

    my @comments = ();

    # good enough comments extractor C/C++ source

    while ($input =~ m!(".*?")|//.*?[\r\n]|/\*.*?\*/!s)
    {
        $input = $';

        push @comments,$& if not $1;
    }

    return @comments;
}

sub CheckHeaderLicense
{
    my ($data, $file) = @_;

    my $header = <<_END_
/**
 * Copyright (c) 20XX Microsoft Open Technologies, Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License"); you may
 *    not use this file except in compliance with the License. You may obtain
 *    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
 *    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 *    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 *    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 *    Microsoft would like to thank the following companies for their review and
 *    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
 *    Dell Products, L.P., Facebook, Inc., Marvell International Ltd.
_END_
;

    $header =~ s/^( \*|\/\*\*)/#/gm if $data =~ /^#/;

    # remove first line (shell definition)
    $data =~ s/^#.+\n// if $data =~ /^#/;

    my $is = substr($data, 0, length($header));

    $is =~ s/ 20\d\d / 20XX /;

    return if $is eq $header;

    LogWarning "Wrong header in $file, is:\n$is\n should be:\n\n$header";
}

sub CheckStatsFunction
{
    my ($fname,$fn,$fnparams) = @_;

    return if $fname eq "sai_clear_port_all_stats_fn"; # exception
    return if $fname eq "sai_get_tam_snapshot_stats_fn"; # exception
    return if $fname eq "sai_bulk_object_get_stats_fn"; # exception
    return if $fname eq "sai_bulk_object_clear_stats_fn"; # exception

    if (not $fname =~ /^sai_((get|clear)_(\w+)_stats|get_\w+_stats_ext)_fn$/)
    {
        LogWarning "wrong stat function name: $fname, expected: sai_(get|clear)_\\w+_stats(_ext)?_fn";
    }

    if (not $fnparams =~ /^\w+_id number_of_counters counter_ids( (mode )?counters)?$/)
    {
        LogWarning "invalid stat function $fname params names: $fnparams";
    }

    my @paramtypes = $fn =~ /_(?:In|Out|Inout)_\s*(.+?)\s*(?:\w+?)\s*[,\)]/gis;
    my $ptypes = "@paramtypes";

    if (not $ptypes =~ /^sai_object_id_t uint32_t const sai_stat_id_t \*( (sai_stats_mode_t )?uint64_t \*)?$/)
    {
        LogWarning "invalid stat function $fname param types: $ptypes";
    }
}

sub CheckFunctionsParams
{
    #
    # make sure that doxygen params match function params names
    #

    my ($data, $file) = @_;

    my $doxygenCommentPattern = '/\*\*((?:(?!\*/).)*?)\*/';
    my $fnTypeDefinition = 'typedef\s*\w+[^;]+?(\w+_fn)\s*\)([^;]+?);';
    my $globalFunction = 'sai_\w+\s*(sai_\w+)[^;]*?\(([^;]+?);';

    while ($data =~ m/$doxygenCommentPattern\s*(?:$fnTypeDefinition|$globalFunction)/gis)
    {
        my $comment = $1;
        my $fname = $2;
        my $fn = $3;

        $fname = $4 if defined $4;
        $fn = $5 if defined $5;

        my @params = $comment =~ /\@param\[\w+]\s+(\.\.\.|\w+)/gis;
        my @fnparams = $fn =~ /_(?:In|Out|Inout)_.+?(\.\.\.|\w+)\s*[,\)]/gis;

        for my $p (@params)
        {
            LogWarning "param $p in $fname should not be prefixed sai_" if $p =~ /sai_/;
        }

        my $params = "@params";
        my $fnparams = "@fnparams";

        if ($params ne $fnparams)
        {
            LogWarning "not matching params in function $fname: $file";
            LogWarning " doxygen '$params' vs code '$fnparams'";
        }

        if ("@params" =~ /[A-Z]/)
        {
            LogWarning "params should use small letters only '@params' in $fname: $file";
        }

        # exceptions
        next if $fname eq "sai_remove_all_neighbor_entries_fn";
        next if $fname eq "sai_switch_register_write_fn";
        next if $fname eq "sai_switch_register_read_fn";
        next if $fname eq "sai_switch_mdio_write_fn";
        next if $fname eq "sai_switch_mdio_read_fn";

        my @paramsFlags = lc($comment) =~ /\@param\[(\w+)]/gis;
        my @fnparamsFlags = lc($fn) =~ /_(\w+)_.+?(?:\.\.\.|\w+)\s*[,\)]/gis;

        if (not "@paramsFlags" eq "@fnparamsFlags")
        {
            LogWarning "params flags not match ('@paramsFlags' vs '@fnparamsFlags') in $fname: $file";
        }

        next if not $fname =~ /_fn$/; # below don't apply for global functions

        if (not $fnparams =~ /^(\w+)(| attr| attr_count attr_list| switch_id attr_count attr_list)$/ and
            not $fname =~ /_(stats|stats_ext|notification)_fn$|^sai_(send|allocate|free|recv|bulk)_|^sai_meta/)
        {
            LogWarning "wrong param names: $fnparams: $fname";
            LogWarning " expected: $params[0](| attr| attr_count attr_list| switch_id attr_count attr_list)";
        }

        if ($fname =~ /^sai_(get|set|create|remove)_(\w+?)(_attribute)?(_stats|_stats_ext)?_fn/)
        {
            my $pattern = $2;
            my $first = $params[0];

            if ($pattern =~ /_entry$/)
            {
                $pattern = "${pattern}_id|${pattern}";
            }
            else
            {
                $pattern = "${pattern}_id";
            }

            if (not $first =~ /^$pattern$/)
            {
                LogWarning "first param should be called ${pattern} but is $first in $fname: $file";
            }
        }

        if ($fname =~ /^sai_\w+_stats_/)
        {
            CheckStatsFunction($fname,$fn,$fnparams);
        }
    }
}

sub CheckNonDoxygenComments
{
    my ($data, $file) = @_;

    while ($data =~ m%( */\*[^\*](?:(?!\*/).)*?\*/)(\n *(\w+))?%gis)
    {
        my $comment = $1;
        my $stick = $3;

        if (($comment =~ /\W\@\w+/is) or defined $stick)
        {
            LogWarning "candidate for doxygen comment in $file:\n$comment";
            LogWarning "comment sticked to $stick" if defined $stick;
        }
    }
}

sub CheckDoxygenCommentFormating
{
    my ($data, $file) = @_;

    while ($data =~ m%/\*\*(?:(?!\*/).)*?(\*/\n[\n]+(\s*[a-z][^\n]*))%gis)
    {
        LogWarning "empty line between doxygen comment and definition: $file: $2";
    }

    while ($data =~ m%( *)(/\*\*(?:(?!\*/).)*?\*/)%gis)
    {
        my $spaces = $1 . " ";
        my $comment = $2;

        next if $comment =~ m!^/\*\*.*\*/$!; # single line comment

        my @lines = split/\n/,$comment;

        my $first = shift @lines;
        my $last = pop @lines;

        if (not $first =~ m!^\s*/..$!)
        {
            LogWarning "first line doxygen comment should be with '/**': $file: $first";
            next;
        }

        if (not $last =~ m!^\s*\*/$!)
        {
            LogWarning "last line doxygen comment should be '*/': $file: $last";
            next;
        }

        if (not $lines[0] =~ m!\* (\@|Copyright )!)
        {
            LogWarning "first doxygen line should contain \@ tag $file: $lines[0]";
        }

        if ($lines[$#lines] =~ m!^\s*\*\s*$!)
        {
            LogWarning "last doxygen line should not be empty $file: $lines[$#lines]";
        }

        for my $line (@lines)
        {
            if (not $line =~ m!^\s*\*( |$)!)
            {
                LogWarning "multiline doxygen comments should start with '* ': $file: $line";
            }

            if (not $line =~ /^$spaces\*/)
            {
                LogWarning "doxygen comment has invalid ident: $file: $line";
            }
        }

        next; # disabled for now since it generates too much changes

        $comment =~ s!^ *(/\*\*|\*/|\* *)!!gm;

        if ($comment =~ m!\@brief\s+(.+?)\n\n!s)
        {
            my $brief = $1;

            if (not $brief =~ /\.$/)
            {
                LogWarning "brief should end with dot $file: $brief";
            }
        }

        my @n = split/^\@\S+ /m,$comment;
    }

    while($data =~ m!(([^\n ])+\n */\*\*.{1,30}.+?\n)!isg)
    {
        next if $2 eq "{";

        LogWarning "doxygen comment must be preceded with blank line: $file:\n$1";
    }
}

sub IsObjectName
{
    my $ot = shift;

    return 1 if defined $main::OBJTOAPIMAP{$ot} or defined $main::OBJTOAPIMAP{uc("SAI_OBJECT_TYPE_".$ot)};

    return 0;
}

sub CheckFunctionNaming
{
    my ($header, $n, $line) = @_;

    return if not $line =~ /^\s*sai_(\w+)_fn\s+(\w+)\s*;/;

    my $typename = $1;
    my $name = $2;

    my @listex = qw(
    allocate_hostif_packet
    flush_fdb_entries
    free_hostif_packet
    profile_get_next_value
    profile_get_value
    recv_hostif_packet
    remove_all_neighbor_entries
    send_hostif_packet
    switch_mdio_read
    switch_mdio_write
    switch_register_read
    switch_register_write);

    my $REG = "(" . (join"|",@listex) . ")";

    if ($name =~ /^$REG$/)
    {
        # ok
    }
    elsif ($name =~ /^(get|clear)_(\w+?)_(all_)?stats(_ext)?$/)
    {
        LogWarning "not object name $2 in $name" if not IsObjectName($2);
    }
    elsif ($name =~ /^(create|remove|get|set)_(\w+?)(_attribute)?$/)
    {
        my $n = $2;

        $n =~ s/_entries$/_entry/ if $typename =~ /^bulk/;
        $n =~ s/s$// if $typename =~ /^bulk/;

        LogWarning "not object name $n in $name" if not IsObjectName($n);
    }
    else
    {
        LogWarning "Line not matching any name pattern: $line";
    }

    if ($typename ne $name and not $typename =~ /^bulk_/)
    {
        LogWarning "function not matching $typename vs $name in $header:$n:$line";
    }

    if (not $name =~ /^(create|remove|get|set)_\w+?(_attribute)?$|^clear_\w+_stats$/)
    {
        # exceptions
        return if $name =~ /^$REG$/;

        LogWarning "function not follow convention in $header:$n:$line";
    }
}

sub CheckQuadApi
{
    my ($data, $file) = @_;

    return if not $data =~ m!(sai_\w+_api_t)(.+?)\1;!igs;

    my $apis = $2;

    my @fns = $apis =~ /sai_(\w+)_fn/g;

    my $fn = join" ",@fns;

    my @quad = split/\bcreate_/,$fn;

    for my $q (@quad)
    {
        next if $q eq "";

        if (not $q =~ /(\w+) remove_\1 set_\1_attribute get_\1_attribute( |$)/)
        {
            LogWarning "quad api must be in order: create remove set get: $file: $q";
        }
    }
}

sub CheckSwitchKeys
{
    my ($data, $file) = @_;

    my $keycount = $1 if $data =~ /#define\s+SAI_SWITCH_ATTR_MAX_KEY_COUNT\s+(\d+)/s;

    my $count = 0;

    while ($data =~ m!#define\s+SAI_KEY_(\w+)\s+"SAI_(\w+)"!gs)
    {
        if ($1 ne $2)
        {
            LogWarning "SAI_(KEY_)$1 should match SAI_$2";
        }

        $count++;
    }

    if ($count != $keycount)
    {
        LogWarning "SAI_SWITCH_ATTR_MAX_KEY_COUNT is $keycount, but found only $count keys";
    }
}

sub CheckStructAlignment
{
    my ($data, $file) = @_;

    # return if $file eq "saitypes.h";

    while ($data =~ m!typedef\s+(?:struct|union)\s+_(sai_\w+)(.+?)}\s*(\w+);!igs)
    {
        my $struct = $1;
        my $inner = $2;
        my $enddef = $3;

        if ($struct ne $enddef)
        {
            LogError "expected same names for _$struct $enddef";
        }

        # we use space in regex since \s will capture \n

        $inner =~ m/^( *)(\w.+\s+)(\w+)\s*;$/im;

        my $spaces = $1;
        my $inside = $2;
        my $name = $3;

        while ($inner =~ m/^( *)(\w.+\s+)(\w+)\s*;$/gim)
        {
            my $itemname = $2;

            if ($1 ne $spaces or (length($2) != length($inside) and $struct =~ /_api_t/))
            {
                LogError "$struct items has invalid column ident: $file: $itemname";
            }
        }
    }
}

sub GetAcronyms
{
    # load acronyms list from file

    my $filename = "acronyms.txt";

    open FILE, $filename or die "Couldn't open file $filename: $!";

    my @acronyms;

    while (<FILE>)
    {
        chomp;
        my $line = $_;

        next if $line =~ /(^#|^$)/;

        if (not $line =~ /^([A-Z0-9]{2,})(\s+-\s+(.+))?$/)
        {
            LogWarning "invalid line in $filename: $line";
            next;
        }

        my $acronym = $1;
        my $definition = $3;

        push @acronyms,$acronym;
    }

    close FILE;

    my $prev = "";

    for my $acr (@acronyms)
    {
        LogWarning "Acronyms are not sorted: $prev, $acr" if ($prev cmp $acr) > 0;

        $prev = $acr;
    }

    return @acronyms;
}

sub CheckMetadataSourceFiles
{
    my @files = GetMetadataSourceFiles();

    for my $file (@files)
    {
        # skip auto generated headers

        next if $file eq "saimetadata.h";
        next if $file eq "saimetadata.c";
        next if $file eq "saimetadatatest.c";

        next if $file =~ /swig|wrap/;

        my $data = ReadHeaderFile($file);

        CheckHeaderLicense($data, $file);

        LogError "missing declaration '\@file    $file'" if (not $data =~ /[*#]\s+\@file\s+\Q$file\E$/m);

        my @lines = split/\n/,$data;

        my $n = 0;

        for my $line(@lines)
        {
            $n++;

            LogWarning "found trailing spaces in $file:$n: $line" if $line =~ /\s+$/;

            $line =~ s/\t+/    /g if $file =~ /Makefile/;

            if ($line =~ /[^\x20-\x7e]/)
            {
                LogWarning "line contains non ascii characters $file:$n: $line";
            }
        }
    }
}

sub CheckInOutParams
{
    my ($header, $n, $line) = @_;

    return if not $line =~ /  _(In|Out|Inout)_ /;
    return if $header eq "saiserialize.h";
    return if $header eq "saimetadatalogger.h";

    if (not $line =~ /  _(In|Out|Inout)_ (const )?(\w+)( \*| \*\*| )(\w+)[\),]/)
    {
        LogError "not matching param line: $header:$n: $line";
        return;
    }

    my $inout = $1;
    my $const = (not defined $2) ? "" : "const";
    my $type = $3;
    my $ptr = $4;
    my $param = $5;

    if ($type eq "sai_object_id_t" and not $line =~ /_In_ sai_object_id_t \w+|_In_ const sai_object_id_t \*\w+|_Out_ sai_object_id_t \*\w+/)
    {
        LogError "wrong in/out param mix: $header:$n:$line";
    }

    if ($type eq "sai_attribute_t" and not $line =~ /_In_ const sai_attribute_t \*\*?\w+|_Inout_ sai_attribute_t \*\*?\w+/)
    {
        return if $header eq "saihostif.h";

        LogError "wrong in/out param mix: $header:$n:$line";
    }

    # TODO we should know if type is simple or struct/union

    return if $line =~ /_In_ \w+ \w+/ and $const eq "";      # non const types without pointer should be In
    return if $line =~ /_Inout_ \w+ \*\w+/ and $const eq ""; # non const types with pointer should be Inout
    return if $line =~ /_Out_ \w+ \*\w+/ and $const eq "";   # non const types with pointer should be Out
    return if $line =~ /_In_ const \w+ \*\*?\w+/;            # const types with pointer should be In

    return if $line =~ /_Out_ const char \*\*\w+/;
    return if $line =~ /_Out_ void \*\*\w+/;
    return if $line =~ /_Inout_ sai_attribute_t \*\*\w+/;

    LogWarning "Not supported param prefixes, FIXME: $header:$n $line";
}

sub CheckComment
{
    my ($data, $header) = @_;

    my @lines = split/\n/,$data;

    return if $data =~ /\s*\s*\@(file|defgroup|}|def |extraparam|passparam)/;

    my $c = "";

    for my $line (@lines)
    {
        next if $line =~ m!^/\*\*|\*/!;

        $line =~ s/\@note|\@par //g;

        if ($line =~ /^\s*\*\s+\@warning/)      { $c .= "W"; next }
        if ($line =~ /^\s*\*\s+\@param/)        { $c .= "P"; next }
        if ($line =~ /^\s*\*$/)                 { $c .= " "; next }
        if ($line =~ /^\s*\*\s+\@brief/)        { $c .= "B"; next }
        if ($line =~ /^\s*\*\s+\@return/)       { $c .= "R"; next }
        if ($line =~ /^\s*\*\s+\@/)             { $c .= "@"; next }

        $c .= "x";
    }

    return if $c eq "";

    $c =~ s/x+/x/g;
    $c =~ s/Px/P/g;
    $c =~ s/P( P)+/P/g;
    $c =~ s/P+/P/g;
    $c =~ s/Bx/B/g;
    $c =~ s/Rx/R/g;
    $c =~ s/x( x)+/x/g;
    $c =~ s/\@+/@/g;

    return if $c =~ /^B( W)?( x)?( \@)?( P)?( R)?$/;

    LogWarning "empty line required between each below elements:";
    LogWarning "desired elemen order: \@brief \@warning? description? \@attributes? \@params? \@return?";
    LogWarning "invalid spacing ($c) on $header:\n$data\n";
}

sub CheckDoxygenSpacing
{
    my ($data, $header) = @_;

    my @comments = ExtractComments($data, $header);

    for my $com (@comments)
    {
        next if $com =~ m!^//!;
        next if not $com =~ m!^/\*\*!;

        CheckComment($com, $header);
    }
}

sub GetWordsFromSources
{
    my $wordsToCheck = shift;

    my @sources = GetMetaSourceFiles();

    my @acronyms = GetAcronyms();

    my @spellExceptions = qw/ IPv4 IPv6 /;

    my %exceptions = map { $_ => $_ } @spellExceptions;

    my %ac = ();

    $ac{$_} = 1 for @acronyms;

    for my $src (sort @sources)
    {
        next if $src =~ /saimetadata.c/;
        next if $src =~ /saimetadatatest.c/;
        next if $src =~ /saiswig/;

        my $data = ReadHeaderFile($src);

        my @comments = ExtractComments($data);

        for my $comment(@comments)
        {
            my @lines = split/\n/,$comment;

            for my $line (@lines)
            {
                while ($line =~ /\b([a-z0-9]+)\b/ig)
                {
                    my $pre = $`;
                    my $post = $';
                    my $word = $1;

                    next if $word =~ /xFF/;
                    next if defined $ac{$word};
                    next if defined $wordsToCheck->{$word};
                    next if defined $exceptions{$word};

                    $wordsToCheck->{$word} = $src;
                }
            }
        }
    }
}

sub CheckHeadersStyle
{
    #
    # Purpose of this check is to find out
    # whether header files are correctly formated
    #
    # Wrong formating includes:
    # - multiple empty lines
    # - double spaces
    # - wrong spacing idient
    #

    # we could put that to local dictionary file

    my @acronyms = GetAcronyms();

    my @spellExceptions = qw/ CRC32 IPv4 IPv6 BGPv6 6th 0xFF /;

    my %exceptions = map { $_ => $_ } @spellExceptions;

    my %wordsToCheck = ();
    my %wordsChecked = ();

    CheckMetadataSourceFiles();

    my @headers = GetHeaderFiles();
    my @metaheaders = GetMetaHeaderFiles();
    my @exheaders = GetExperimentalHeaderFiles();

    @headers = (@headers, @metaheaders, @exheaders);

    for my $header (@headers)
    {
        next if $header eq "saimetadata.h"; # skip auto generated header

        my $data = ReadHeaderFile($header);

        my $oncedef = uc ("__${header}_");

        $oncedef =~ s/\./_/g;

        my $oncedefCount = 0;

        CheckHeaderLicense($data, $header);
        CheckFunctionsParams($data, $header);
        CheckDoxygenCommentFormating($data, $header);
        CheckQuadApi($data, $header);
        CheckStructAlignment($data, $header);
        CheckNonDoxygenComments($data, $header);
        CheckSwitchKeys($data, $header) if $header eq "saiswitch.h";
        CheckDoxygenSpacing($data, $header);

        my @lines = split/\n/,$data;

        my $n = 0;

        my $empty = 0;
        my $emptydoxy = 0;

        for my $line (@lines)
        {
            $n++;

            CheckFunctionNaming($header, $n, $line);
            CheckInOutParams($header, $n, $line);

            $oncedefCount++ if $line =~ /\b$oncedef\b/;

            # detect multiple empty lines

            if ($line =~ /^$/)
            {
                $empty++;

                if ($empty > 1)
                {
                    LogWarning "header contains two empty lines one after another $header $n";
                }
            }
            else { $empty = 0 }

            # detect multiple empty lines in doxygen comments

            if ($line =~ /^\s+\*\s*$/)
            {
                $emptydoxy++;

                if ($emptydoxy > 1)
                {
                    LogWarning "header contains two empty lines in doxygen $header $n";
                }
            }
            else { $emptydoxy = 0 }

            if ($line =~ /^\s+\* / and not $line =~ /\*( {4}| {8}| )[^ ]/)
            {
                LogWarning "not expected number of spaces after * (1,4,8) $header $n:$line";
            }

            if ($line =~ /\*\s+[^ ].*  / and not $line =~ /\* \@(brief|file|note)/)
            {
                if (not $line =~ /const.+const\s+\w+;/ and not $line =~ m!\\$!)
                {
                    LogWarning "too many spaces after *\\s+ $header $n:$line";
                }
            }

            if ($line =~ /(typedef|{|}|_In\w+|_Out\w+)( [^ ].*  |  )/ and not $line =~ /typedef\s+u?int/i and not $line =~ m!\\$!)
            {
                LogWarning "too many spaces $header $n:$line";
            }

            if ($line =~ m!/\*\*! and not $line =~ m!/\*\*\s*$! and not $line =~ m!/\*\*.+\*/!)
            {
                LogWarning "multiline doxygen comment should start '/**' $header $n:$line";
            }

            if ($line =~ m![^ ]\*/!)
            {
                LogWarning "coment is ending without space $header $n:$line";
            }

            if ($line =~ /^\s*sai_(\w+)_fn\s+(\w+);/)
            {
                # make struct function members to follow convention

                LogWarning "$2 should be equal to $1" if (($1 ne $2) and not($1 =~ /^bulk/))
            }

            if ($line =~ /_(?:In|Out)\w+\s+(?:sai_)?uint32_t\s*\*?(\w+)/)
            {
                my $param = $1;

                my $pattern = '^(attr_count|object_count|number_of_counters|count|u32|device_addr|start_reg_addr|number_of_registers|reg_val)$';

                if (not $param =~ /$pattern/)
                {
                    LogWarning "param $param should match $pattern $header:$n:$line";
                }
            }

            if ($line =~ /typedef.+_fn\s*\)/ and not $line =~ /typedef( \S+)+ \(\*\w+_fn\)\(/)
            {
                LogWarning "wrong style typedef function definition $header:$n:$line";
            }

            if ($line =~ / ([.,:;)])/ and not $line =~ /\.(1D|1Q|\.\.)/)
            {
                LogWarning "space before '$1' : $header:$n:$line";
            }

            if ($line =~ / \* / and not $line =~ /^\s*\* / and not $line =~ /^#define/)
            {
                LogWarning "floating star $header:$n:$line";
            }

            if ($line =~ /}[^ ]/)
            {
                LogWarning "no space after '}' $header:$n:$line" if (not $line =~ /^\s*\* /);
            }

            if ($line =~ /_[IO].+\w+\*\*? /)
            {
                LogWarning "star should be next to param name $header:$n:$line";
            }

            if ($line =~ /_In_ .+\*/ and not $line =~ /_In_ const/)
            {
                LogWarning "pointer input parameters should be const $header:$n:$line";
            }

            if ($line =~ /[^ ]\s*_(In|Out|Inout)_/ and not $line =~ /^#define/)
            {
                LogWarning "each param should be in separate line $header:$n:$line";
            }

            if ($line =~ m!/\*\*\s+[a-z]!)
            {
                LogWarning "doxygen comment should start with capital letter: $header:$n:$line";
            }

            if ($line =~ /sai_\w+_statistics_fn/)
            {
                LogWarning "statistics should use 'stats' to follow convention $header:$n:$line";
            }

            if ($line =~ /^\s*(SAI_\w+)\s*=\s*(.*)$/)
            {
                my $init = $2;

                if ($init =~ m!^(0x\w+|SAI_\w+|SAI_\w+ \+ \d+|SAI_\w+ \+ 0x[0-9a-f]{1,8}|SAI_\w+ \+ SAI_\w+|\d+|\(?\d+ << \d+\)?),?\s*(/\*\*.*\*/)?$!)
                {
                    # supported initializers for enum:
                    # - 0x00000000 (hexadecimal number)
                    # - 0 (decimal number)
                    # - SAI_... (other SAI enum)
                    # - n << m (flags shifted)
                    # - SAI_.. + SAI_.. (sum of SAI enums)
                    # - SAI_.. + 0x00 (sum of SAI and hexadecimal number)
                    # - SAI_.. + 0 (sum of SAI and decimal number)
                }
                else
                {
                    LogWarning "unsupported initializer on enum: $line";
                }
            }

            if ($line =~ /^\s*SAI_\w+\s*=\s*+0x(\w+)(,|$)/ and length($1) != 8)
            {
                LogWarning "enum number '0x$1' should have 8 digits $header:$n:$line";
            }

            if ($line =~ /^\s*SAI_\w+(\s*)=(\s*)/ and ($1 eq "" or $2 eq ""))
            {
                LogWarning "space is required before and after '=' $header:$n:$line";
            }

            if ($line =~ /#define\s*(\w+)/ and $header ne "saitypes.h")
            {
                my $defname = $1;

                if (not $defname =~ /^(SAI_|__SAI)/)
                {
                    LogWarning "define should start with SAI_ or __SAI: $header:$n:$line";
                }
            }

            if ($line =~ /\s+$/)
            {
                LogWarning "line ends in whitespace $header $n: $line";
            }

            if ($line =~ /[^\x20-\x7e]/)
            {
                LogWarning "line contains non ascii characters $header $n: $line";
            }

            if ($line =~ /typedef .+?\(\s*\*\s*(\w+)\s*\)/)
            {
                my $fname = $1;

                if (not $fname =~ /^sai_\w+_fn$/)
                {
                    LogWarning "all function declarations should be in format sai_\\w+_fn $header $n: $line";
                }
            }

            my $prev = $lines[$n-2];

            if ($line =~ /\*\s*\@\w+/ and $prev =~ /\@brief/)
            {
                LogWarning "missing empty line before $header $n: $line";
            }

            if ($line =~ /==|SAI_\w+/ and $prev =~ /\@(validonly|condition)/)
            {
                LogWarning "merge with previous line: $header $n: $line";
            }

            if ($line =~ /\@type/ and $prev =~ /^\s*\*./)
            {
                LogWarning "missing empty line before: $header $n: $line";
            }

            if ($line =~ /_(In|Out|Inout)_.+(\* | \* )/)
            {
                LogWarning "move * to the right of parameter: $header $n: $line";
            }

            if ($line =~ /\*.*SAI_.+(==|!=)/ and not $line =~ /\@(condition|validonly)/)
            {
                if (not $line =~ /(condition|validonly|valid when|only when)\s+SAI_/i)
                {
                    LogWarning "condition should be preceded by 'valid when' or 'only when': $header $n: $line";
                }
            }

            if ($line =~ /SAI_\w+ \s+=\s+(0x|S)/)
            {
                LogWarning "too many spaces before '=' $header:$n: $line"
            }

            if ($line =~ /__/ and not $line =~ /^#.+__SAI\w*_H_|VA_ARGS|BOOL_DEFINED/)
            {
                LogWarning "double underscore detected: $header $n: $line";
            }

            if ($line eq "" and $prev =~ /{/)
            {
                LogWarning "empty line after '$prev': $header:$n: $line";
            }

            my $pattern = join"|", @acronyms;

            while ($line =~ /\b($pattern)\b/igp)
            {
                my $pre = $`;
                my $post = $';

                # force special word to be capital

                my $word = $1;

                next if $word =~ /^($pattern)$/;
                next if $word =~ /^(an|An)$/; # exception, can be word and acronym
                next if $line =~ /$word.h/;
                next if not $line =~ /\*/; # must contain star, so will be comment
                next if "$pre$word" =~ m!http://$word$!;
                next if ($line =~ /\@param\[\w+\]\s+$word /); # skip word if word is param name

                LogWarning "Word '$word' should use capital letters $header $n:$line";
            }

            # perform aspell checking (move to separate method)

            if ($line =~ m!^\s*(\*|/\*\*)!)
            {
                while ($line =~ /\b([a-z0-9']+)\b/ig)
                {
                    my $pre = $`;
                    my $post = $';
                    my $word = $1;

                    next if $word =~ /^($pattern)$/; # capital words

                    next if ($line =~ /\@validonly\s+\w+->\w+/); # skip valionly code
                    next if ($line =~ /\@passparam\s+\w+/);      # skip passparam
                    next if ($line =~ /\@extraparam\s+\w+/);     # skip extraparam
                    next if ($line =~ /\@param\[\w+\]\s+$word /); # skip word if word is param name

                    # look into good and bad words hash to speed things up

                    next if defined $exceptions{$word};
                    next if $word =~ /^sai\w+/i;
                    next if $word =~ /0x\S+/;
                    next if "$pre$word" =~ /802.\d+\w+/;

                    next if defined $wordsChecked{$word};

                    $wordsChecked{$word} = 1;

                    $wordsToCheck{$word} = "$header $n:$line";
                }
            }

            if ($line =~ /typedef\s*(enum|struct|union).*{/)
            {
                LogWarning "move '{' to new line in typedef $header $n:$line";
            }

            if ($line =~ /^[^*]*union/ and not $line =~ /union _\w+\b/)
            {
                LogError "all unions must be named $header $n:$line";
            }

            CheckDoxygenStyle($header, $line, $n);

            next if $line =~ /^ \*($|[ \/])/;       # doxygen comment
            next if $line =~ /^$/;                  # empty line
            next if $line =~ /^typedef /;           # type definition
            next if $line =~ /^sai_status_t sai_\w+\(/;     # return codes
            next if $line =~ /^sai_object\w+_t sai_\w+\(/;  # return codes
            next if $line =~ /^int sai_\w+\($/;             # methods returning int
            next if $line =~ /^extern /;            # extern in metadata
            next if $line =~ /^[{}#\/]/;            # start end of struct, define, start of comment
            next if $line =~ /^ {8}(_In|_Out|\.\.\.)/;      # function arguments
            next if $line =~ /^ {4}(sai_)/i;        # sai struct entry or SAI enum
            next if $line =~ /^ {4}\/\*/;           # doxygen start
            next if $line =~ /^ {8}\/\*/;           # doxygen start
            next if $line =~ /^ {5}\*/;             # doxygen comment continue
            next if $line =~ /^ {8}sai_/;           # union entry
            next if $line =~ /^ {4}union/;          # union
            next if $line =~ /^ {4}[{}]/;           # start or end of union
            next if $line =~ /^ {4}(u?int)/;        # union entries
            next if $line =~ /^ {4}(char|bool)/;    # union entries
            next if $line =~ /^ {8}bool booldata/;  # union bool
            next if $line =~ /^ {4}(true|false)/;   # bool definition
            next if $line =~ /^ {4}(const|size_t|else)/; # const in meta headers
            next if $line =~ /^(void|bool) /;       # function return
            next if $line =~ m![^\\]\\$!;           # macro multiline
            next if $line =~ /^ {4}(\w+);$/;        # union entries
            next if $line =~ /^union _sai_\w+ \{/;  # union entries

            LogWarning "C++ comment in ANSI C header: $header $n:$line" if $line =~ /\/\//;

            LogWarning "Header doesn't meet style requirements (most likely ident is not 4 or 8 spaces) $header $n:$line";
        }

        if ($oncedefCount != 3)
        {
            LogWarning "$oncedef should be used 3 times in header, but used $oncedefCount";
        }
    }

    GetWordsFromSources(\%wordsToCheck);

    RunAspell(\%wordsToCheck) if not defined $main::optionDisableAspell;
}

BEGIN
{
    our @ISA    = qw(Exporter);
    our @EXPORT = qw/
    CheckHeadersStyle
    /;
}

1;
