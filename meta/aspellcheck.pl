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
# @file    aspellcheck.pl
#
# @brief   This module run aspell on meta source and headers
#

BEGIN { push @INC,'.'; }

use strict;
use warnings;
use diagnostics;

use Term::ANSIColor;

use utils;
use style;

our $errors = 0;
our $warnings = 0;

sub GetSourceFilesAndHeaders
{
    my @files = `find . -name "*.cpp" -o -name "*.[ch]"`;

    return @files;
}

sub ReadFile
{
    my $filename = shift;

    local $/ = undef;

    # first search file in meta directory

    open FILE, $filename or die "Couldn't open file $filename: $!";

    binmode FILE;

    my $string = <FILE>;

    close FILE;

    return $string;
}

sub ExtractComments
{
    my $input = shift;

    my $comments = "";

    # good enough comments extractor C/C++ source

    while ($input =~ m!(".*?")|//.*?[\r\n]|/\*.*?\*/!s)
    {
        $input = $';

        $comments .= $& if not $1;
    }

    return $comments;
}

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

        chomp $res;
        next if $res =~ /^\*?$/;

        print "$res\n";
        next if not $res =~ /^\s*&\s*(\S+)/;

        my $word = $1;

        next if $word =~ /^wred$/i;

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

        LogWarning "Word '$word' is misspelled $where";
    }
}

my @acronyms = GetAcronyms();

my %spellAcronyms = ();

$spellAcronyms{$_} = 1 for @acronyms;

my @exceptions = qw/ IPv4 IPv6 0xFF IPv SAIMETADATALOGGER auth objecttype saimetadatalogger sak /;

my %spellExceptions = map { $_ => $_ } @exceptions;

my @files = GetSourceFilesAndHeaders();

my %wordsToCheck = ();

for my $file (@files)
{
    chomp $file;
    next if $file =~ m!saiswig.cpp!;
    next if $file =~ m!temp!;
    next if $file =~ m!xml!;
    next if $file =~ m!saimetadata.[ch]!;

    my $data = ReadFile $file;

    my $comments = ExtractComments $data;

    $comments =~ s!github.com\S+! !g;
    $comments =~ s!l2mc! !g;
    $comments =~ s!\s+\d+(th|nd) ! !g;
    $comments =~ s!(/\*|\*/)! !g;
    $comments =~ s!//! !g;
    $comments =~ s!\s+\*\s+! !g;
    $comments =~ s![^a-zA-Z0-9_]! !g;

    my @words = split/\s+/,$comments;

    for my $word (@words)
    {
        next if defined $spellAcronyms{$word};
        next if defined $spellExceptions{$word};

        next if $word =~ /_/;
        next if $word =~ /xYYY+/;
        next if $word =~ /fe\d+/;
        next if $word =~ /ebe\d+/;
        next if $word =~ /Werror/;
        next if $word =~ /^[A-Za-z][a-z]+([A-Z][a-z]+)+$/; # fooBar FooBar

        $wordsToCheck{$word} = $file;
    }
}

RunAspell(\%wordsToCheck);

exit 1 if ($warnings > 0 or $errors > 0);
