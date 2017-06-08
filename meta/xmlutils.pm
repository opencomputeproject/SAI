#!/usr/bin/perl

package xmlutils;

use strict;
use warnings;
use diagnostics;
use Getopt::Std;
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

    if (defined $main::optionUseXmlSimple)
    {
        my $xs = XML::Simple->new();

        return $xs->XMLin($filename, KeyAttr => { }, ForceArray => 1);
    }

    open(FH, '<', $filename) or die "Could not open file '$filename' $!";

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
        next if not /^sai\w*_8h\.xml$/i;

        push @files,$_;
    }

    closedir $dh;

    return sort @files;
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

my %STRUCT_TAGS = (
        "count"       , \&ProcessStructCount,
        "objects"     , \&ProcessStructObjects,
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

    my %Struct = (name => $structName);

    my $filename = "${prefix}${structName}.xml";

    $filename =~ s/_/__/g;

    $filename = $prefix if -e "$main::XMLDIR/$prefix" and $prefix =~ /\.xml$/;

    my $file = "$main::XMLDIR/$filename"; # example: xml/struct__sai__fdb__entry__t.xml

    # read xml, we need to get each struct field and it's type and description

    my $ref = ReadXml $file;

    my @sections = @{ $ref->{compounddef}[0]->{sectiondef} };

    if (scalar @sections != 1)
    {
        LogError "expected only 1 section in $file for $structName";
        return %Struct;
    }

    my @members = @{ $sections[0]->{memberdef} };

    if (scalar @members < 2)
    {
        LogError "there must be at least 2 members in struct $structName";
        return %Struct;
    }

    my $desc = ExtractDescription($structName, $structName, $ref->{compounddef}[0]->{detaileddescription}[0]);

    ProcessStructDescription(\%Struct, $desc);

    $Struct{$desc} = $desc;

    my $idx = 0;

    # we must chage this method to return hash with members $st

    my @StructMembers = ();
    my @keys = ();

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

        my $typeSuffix= $2;

        if ($typeSuffix ne "")
        {
            if ($typeSuffix =~/^\[\d+\]$/)
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

        ProcessStructDescription(\%M, $desc);

        $Struct{membersHash}{$name} = \%M;

        push @StructMembers, \%M;
        push @keys, $name;
    }

    $Struct{members} = \@StructMembers;
    $Struct{keys} = \@keys;
    $Struct{baseName} = $1 if $structName =~/^sai_(\w+)_t$/;

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
    ReadXml UnescapeXml GetXmlFiles
    ExtractDescription ExtractStructInfo ExtractStructInfoEx
    /;
}

1;
