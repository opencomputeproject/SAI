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
# @file    xmlutils.pm
#
# @brief   This module defines SAI Metadata Xml Utils Parser
#

package xmlutils;

use strict;
use warnings;
use diagnostics;
use Data::Dumper;
use utils;

require Exporter;

my $ident = 0;
my $debug = 0;

sub PrintDebug
{
    my $line = shift;
    my $spaces = "  " x $ident;

    print "$spaces$line\n" if $debug == 1;
}

sub PrintError
{
    my $line = shift;

    print STDERR "ERROR: $line\n";
    exit 1;
}

sub UnescapeXml
{
    my $line = shift;

    $line =~ s!&quot;!"!g;
    $line =~ s!&gt;!>!g;
    $line =~ s!&lt;!<!g;
    $line =~ s!&apos;!'!g;
    $line =~ s!&amp;!&!g;

    return $line;
}

sub ProcessTag
{
    $ident++;

    my ($Lines, $node, $tag, $tagparams) = @_;

    #PrintDebug "> entering $tag";

    if (not defined $node->{$tag})
    {
        #PrintDebug ": adding key '$tag' to node";

        my @arr = ();

        $node->{$tag} = \@arr;
    }

    my $tagarr = $node->{$tag};

    my %current = ();

    push @{$tagarr}, \%current;

    $current{content} = "";

    if (defined $tagparams)
    {
        while ($tagparams =~ m!(\w+)="([^"]*)"!g)
        {
            $current{$1} = $2;
        }
    }

    while (1)
    {
        my $line = shift @{$Lines};

        last if not defined $line;

        chomp $line;

        if ($line eq "</$tag>")
        {
            #PrintDebug "< exiting $tag";

            $current{content} =~ s/\s*$//;
            $current{content} =~ s/^\s*//;

            # cleanup node
            if (scalar(keys%current) == 1)
            {
                pop@{$tagarr};
                push@{$tagarr}, $current{content};
            }

            $ident--;
            return;
        }

        next if $line eq "";
        next if $line eq "<linebreak/>";

        if ($line =~ m!^<(\w+)(\s+[^<>]+[^/])?>$!)
        {
            ProcessTag($Lines, \%current, $1, $2);
            next;
        }

        if ($line =~ m!^[^<>]+$!)
        {
            $current{content} .= UnescapeXml "$line ";
            #PrintDebug ": content: $line";
            next;
        }

        PrintError "not supported line: '$line'";
    }

    PrintError "EOF reached when parsing tag '$tag'";
}

sub ReadXml
{
    my $filename = shift;

    $filename = "$main::XMLDIR/$filename" if not -f $filename;

    if (defined $main::optionUseXmlSimple)
    {
        my $xs = XML::Simple->new();

        return $xs->XMLin($filename, KeyAttr => { }, ForceArray => 1);
    }

    my ($package, $file, $line, $sub) = caller(3);

    open(FH, '<', $filename) or die "Could not open file '$filename' $!\n called from ${file}::${sub}:$line";

    my @values = ();
    my %ROOT = ();

    my $header = <FH>;
    my $doxyline = <FH>;

    if (not $doxyline =~ /^<(doxygen\w*)/)
    {
        PrintError "ERR $filename: first line is not doxygen: $doxyline";
    }

    my $doxygenTag = $1;

    my @lines = ();

    while (my $line = <FH>)
    {
        $line =~ s!\s*</?(emphasis)(\s+[^>]+)?>\s*!"!g;
        $line =~ s!<ndash/>!-!g;

        # split single tags to 2 to easier parsing
        $line =~ s!<(\w+)(\s+[^<>]+)/>!<$1$2></$1>!g;

        # split each line by any xml tag
        push @lines, split/\s*(<[^>]+>)\s*/,$line;
    }

    close FH;

    $ident = 0;
    $debug = 1 if defined $main::optionPrintDebug and $main::optionPrintDebug > 1;

    ProcessTag(\@lines, \%ROOT, $doxygenTag);

    # print Dumper (%ROOT);
    return $ROOT{$doxygenTag}[0];
}

sub GetXmlFiles
{
    my $dir = shift;

    opendir(my $dh, $dir) or die "Can't open $dir $!";

    my @files = ();

    while (readdir $dh)
    {
        next if not /^\w+\.xml$/i or not -f "$dir/$_";

        push @files,$_;
    }

    closedir $dh;

    return sort @files;
}

sub GetSaiXmlFiles
{
    my $dir = shift;

    my @files = GetXmlFiles($dir);

    return grep { /^sai\w*_8h\.xml$/ } @files;
}

sub GetXmlUnionFiles
{
    my $dir = shift;

    my @files = GetXmlFiles($dir);

    return grep { /^union_\w*\.xml$/ } @files;
}

sub ProcessStructCount
{
    my ($structName, $tagValue, $previousTagValue) = @_;

    my %count = ();

    %count = %{ $previousTagValue } if defined $previousTagValue;

    if (not $tagValue =~ /^(\w+)\[(\w+|\d+)\]$/g)
    {
        LogError "unable to parse count '$tagValue' on $structName";
        return undef;
    }

    my $pointerParam = $1;
    my $countParam = $2;

    $count{$pointerParam} = $countParam;

    LogDebug "adding count $pointerParam\[$countParam\] on $structName";

    if ($pointerParam eq $countParam)
    {
        LogError "count '$pointerParam' can't point to itself in \@count on $structName";
        undef;
    }

    return \%count;
}

sub ProcessStructObjects
{
    my ($structName, $tagValue) = @_;

    $tagValue =~ s/\s*//g;

    my @objectTypes = split/[,]/,$tagValue;

    for my $ot (@objectTypes)
    {
        if (not $ot =~ /^SAI_OBJECT_TYPE_\w+$/)
        {
            LogError "invalid object type tag value ($ot) in $structName $tagValue";
            return undef;
        }
    }

    LogDebug "adding objects @objectTypes on $structName";

    return \@objectTypes;
}

sub ProcessStructValidOnly
{
    my ($structName, $tagValue) = @_;

    my @conditions = split/\s+(?:or|and)\s+/,$tagValue;

    $tagValue =~ s/\s+/ /g;

    if ($tagValue =~ /\bor\b.*\band\b|\band\b.*\bor\b/)
    {
        LogError "mixed conditions and/or is not supported: $tagValue";
        return undef;
    }

    for my $cond (@conditions)
    {
        # it can be single value (struct member or param) or param pointer

        if (not $cond =~ /^(\w+|\w+->\w+|sai_metadata_\w+\(\w+\)) == (true|false|SAI_\w+|$NUMBER_REGEX)$/)
        {
            LogError "invalid condition tag value '$tagValue' ($cond), expected (\\w+|\\w+->\\w+) == true|false|SAI_ENUM|number";
            return undef;
        }
    }

    LogDebug "adding conditions @conditions on $structName";

    return \@conditions;
}

sub ProcessStructExtraParam
{
    my ($structName, $tagValue, $previousTagValue) = @_;

    my @params = ();

    @params = @{ $previousTagValue } if defined $previousTagValue;

    if (not $tagValue =~ /^((const\s+)?\w+(\s+|\s*\*\s*)\w+)$/)
    {
        LogError "unable to parse extraparam '$tagValue' on $structName";
        return undef;
    }

    push @params, $1;

    LogDebug "adding extraparam '$1' on $structName";

    return \@params;
}

sub ProcessStructPassParam
{
    my ($structName, $tagValue, $previousTagValue) = @_;

    my @params = ();

    @params = @{ $previousTagValue } if defined $previousTagValue;

    if (not $tagValue =~ /^(&?\w+|\w+->\w+)$/)
    {
        LogError "unable to parse passparam '$tagValue' on $structName";
        return undef;
    }

    push @params, $1;

    LogDebug "adding passparam '$1' on $structName";

    return \@params;
}

sub ProcessStructSuffix
{
    my ($structName, $tagValue) = @_;

    if (not $tagValue =~ /^(\w+)/)
    {
        LogError "unable to parse suffix '$tagValue' on $structName";
        return undef;
    }

    LogDebug "adding suffix '$1' on $structName";

    return $1;
}

sub ProcessStructFlags
{
    my ($structName, $tagValue) = @_;

    if (not $tagValue =~ /^(sai_\w+_t)$/)
    {
        # TODO field type must be enum type or uintX_t

        LogError "unable to parse suffix '$tagValue' on $structName";
        return undef;
    }

    LogDebug "adding flags '$1' on $structName";

    return $1;
}

my %STRUCT_TAGS = (
        "count"       , \&ProcessStructCount,
        "objects"     , \&ProcessStructObjects,
        "validonly"   , \&ProcessStructValidOnly,
        "passparam"   , \&ProcessStructPassParam,
        "extraparam"  , \&ProcessStructExtraParam,
        "suffix"      , \&ProcessStructSuffix,
        "flags"       , \&ProcessStructFlags,
        );

sub ProcessStructDescription
{
    my ($struct, $desc) = @_;

    $struct->{desc} = $desc;

    my $structName = $struct->{name};

    $desc =~ s/@@/\n@@/g;

    while ($desc =~ /@@(\w+)(.*)/g)
    {
        my $tag = $1;
        my $value = Trim($2);

        if (not defined $STRUCT_TAGS{$tag})
        {
            LogError "unrecognized tag '$tag' on $structName: $value";
            next;
        }

        LogDebug "processing tag '$tag' on $structName";

        $struct->{$tag} = $STRUCT_TAGS{$tag}->($structName, $value, $struct->{$tag});
    }
}

sub ExtractStructInfo
{
    my ($structName, $filePrefix) = @_;

    my %struct = ExtractStructInfoEx($structName, $filePrefix);

    return %{ $struct{membersHash} };
}

sub ExtractStructInfoEx
{
    my ($structName, $prefix) = @_;

    LogDebug "processing struct/union $structName: prefix: $prefix";

    my %Struct = (name => $structName);

    my $filename = "${prefix}${structName}.xml";

    $filename =~ s/_/__/g;

    $filename = $prefix if -e "$main::XMLDIR/$prefix" and $prefix =~ /\.xml$/;

    my $file = "$main::XMLDIR/$filename"; # example: xml/struct__sai__fdb__entry__t.xml

    if (not -e $file)
    {
        $file =~ s/struct_/union_/;
    }

    # read xml, we need to get each struct field and it's type and description

    my $ref = ReadXml $file;

    my @sections = @{ $ref->{compounddef}[0]->{sectiondef} };

    if (scalar @sections != 1)
    {
        LogError "expected only 1 section in $file for $structName";
        return %Struct;
    }

    my @members = @{ $sections[0]->{memberdef} };

    if (scalar @members < 1)
    {
        LogError "there must be at least 1 member in struct $structName";
        return %Struct;
    }

    my $desc = ExtractDescription($structName, $structName, $ref->{compounddef}[0]->{detaileddescription}[0]);

    ProcessStructDescription(\%Struct, $desc);

    $Struct{desc} = $desc;

    my $idx = 0;

    my @StructMembers = ();
    my @keys = ();

    $Struct{count}->{list} = "count" if $structName =~ /^sai_(\w+)_list_t$/;

    for my $member (@members)
    {
        my $name = $member->{name}[0];
        my $type = $member->{definition}[0];
        my $args = $member->{argsstring}[0];
        my $file = $member->{location}[0]->{file};

        LogDebug "processing member '$name' on $structName";

        # if argstring is empty in xml, then it returns empty hash, skip this
        # args contain extra arguments like [32] for "char foo[32]" or
        # function parameters

        $args = "" if ref $args eq "HASH";

        $type = $1 if $type =~ /^(.+) _sai_\w+_t::(?:\w+|::)+(.*)$/;

        my $typeSuffix = $2;

        if ($typeSuffix ne "")
        {
            if ($typeSuffix =~ /^\[\d+\]$/)
            {
                $type .= $typeSuffix;
            }
            else
            {
                LogError "not supported type '$member->{definition}[0]'\n";
            }
        }

        my $desc = ExtractDescription($structName, $name, $member->{detaileddescription}[0]);

        my %M = ();

        $M{count} = $Struct{count}->{$name} if defined $Struct{count}->{$name};
        $M{type} = $type;
        $M{desc} = $desc;
        $M{args} = $args;
        $M{file} = $file;
        $M{name} = $name;
        $M{idx}  = $idx++;
        $M{union} = $member->{type}[0]->{ref}[0]->{refid} if $member->{definition}[0] =~ /union /;

        ProcessStructDescription(\%M, $desc);

        $Struct{membersHash}{$name} = \%M;

        push @StructMembers, \%M;
        push @keys, $name;

        $Struct{ismetadatastruct} = 1 if $file =~ m!meta/sai\w+.h$|saimeta\w+!;
        $Struct{containsfnpointer} = 1 if $type =~ /^sai_\w+_fn$/;
    }

    $Struct{members} = \@StructMembers;
    $Struct{keys} = \@keys;
    $Struct{baseName} = ($structName =~ /^sai_(\w+)_t$/) ? $1 : $structName;
    $Struct{baseName} =~ s/^_//;
    $Struct{union} = 1 if $ref->{compounddef}[0]->{kind} eq "union";

    return %Struct;
}

sub ExtractDescription
{
    my ($type, $value, $item) = @_;

    return $item if ref $item eq "";

    if (not ref $item eq "HASH")
    {
        LogError "invalid description provided in $type $value";
        return undef;
    }

    my $content = "";

    if (defined $item->{simplesect})
    {
        my @sim = @{ $item->{simplesect} };

        for my $s (@sim)
        {
            $content .= ExtractDescription($type, $value, $s);
        }

        return $content;
    }

    if (defined $item->{para})
    {
        my @para = @{ $item->{para} };

        for my $p (@para)
        {
            $content .= " " . ExtractDescription($type, $value, $p);
        }

        return $content;
    }

    if (defined $item->{content} and defined $item->{ref})
    {
        my $n = 0;

        if (ref ($item->{content}) eq "")
        {
            # content is just string

            return $item->{content} . $item->{ref}[0]->{content};
        }

        for my $c ( @{ $item->{content} })
        {
            my $ref = $item->{ref}[$n++]->{content};

            # ref array can be 1 item shorter than content

            $content .= $c;
            $content .= $ref if defined $ref;
        }
    }

    return $content;
}

BEGIN
{
    our @ISA    = qw(Exporter);
    our @EXPORT = qw/
    ReadXml UnescapeXml GetSaiXmlFiles GetXmlUnionFiles
    ExtractDescription ExtractStructInfo ExtractStructInfoEx
    /;
}

1;
