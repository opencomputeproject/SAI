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

# TODO we need similar processing like tag processing on attr

sub ExtractCountFromDesc
{
    my ($type, $desc) = @_;

    my %Count = ();

    if (not defined $desc)
    {
        LogWarning "desc is not defined in '$type'";

        return %Count;
    }

    while ($desc =~ /\@\@count\s+(\w+)\[(\w+|\d+)\]/g)
    {
        $Count{$1} = $2;

        if ($1 eq $2)
        {
            LogError "count '$1' can't point to itself in \@count on $type";
            return;
        }
    }

    return %Count;
}

sub ExtractObjectsFromDesc
{
    my ($type, $member, $desc) = @_;

    my @objectTypes = ();

    if (not defined $desc)
    {
        LogWarning "desc is not defined in '${type}::$member'";

        return \@objectTypes;
    }

    if (not $desc =~ /\@\@objects\s+(\w+(,\s*\w+)*)/g)
    {
        return \@objectTypes;
    }

    $desc = $1;
    $desc =~ s/\s*//g;

    @objectTypes = split/[,]/,$desc;

    for my $ot (@objectTypes)
    {
        if (not $ot =~ /^SAI_OBJECT_TYPE_\w+$/)
        {
            LogError "invalid objecttype '$ot' on ${type}::$member";
            return undef;
        }
    }

    return \@objectTypes;
}

sub ExtractStructInfo
{
    my $struct = shift;
    my $prefix = shift;

    my %S = ();

    my $filename = "${prefix}${struct}.xml";

    $filename =~ s/_/__/g;

    $filename = $prefix if -e "$main::XMLDIR/$prefix" and $prefix =~ /\.xml$/;

    my $file = "$main::XMLDIR/$filename"; # example: xml/struct__sai__fdb__entry__t.xml

    # read xml, we need to get each struct field and it's type and description

    my $ref = ReadXml $file;

    my @sections = @{ $ref->{compounddef}[0]->{sectiondef} };

    if (scalar @sections != 1)
    {
        LogError "expected only 1 section in $file for $struct";
        return %S;
    }

    my @members = @{ $sections[0]->{memberdef} };

    if (scalar@members < 2)
    {
        LogError "there must be at least 2 members in struct $struct";
        return %S;
    }

    my $desc = ExtractDescription($struct, $struct, $ref->{compounddef}[0]->{detaileddescription}[0]);

    my %Count = ExtractCountFromDesc($struct, $desc);

    my $idx = 0;

    for my $member (@members)
    {
        my $name = $member->{name}[0];
        my $type = $member->{definition}[0];
        my $args = $member->{argsstring}[0];
        my $file = $member->{location}[0]->{file};

        # if argstring is empty in xml, then it returns empty hash, skip this
        # args contain extra arguments like [32] for "char foo[32]" or
        # function parameters

        $args = "" if ref $args eq "HASH";

        $type = $1 if $type =~ /^(.+) _sai_\w+_t::(?:\w+|::)+(.*)$/;

        if (defined $2 and $2 ne "")
        {
            my $suffix= $2;

            if ($suffix =~/^\[\d+\]$/)
            {
                $type .= $suffix;
            }
            else
            {
                LogError "not supported type '$member->{definition}[0]'\n";
            }
        }

        my $desc = ExtractDescription($struct, $struct, $member->{detaileddescription}[0]);

        $S{$name}{count} = $Count{$name} if defined $Count{$name};
        $S{$name}{type} = $type;
        $S{$name}{desc} = $desc;
        $S{$name}{args} = $args;
        $S{$name}{file} = $file;
        $S{$name}{name} = $name;
        $S{$name}{objecttype} = ExtractObjectsFromDesc($struct, $name, $desc);
        $S{$name}{idx}  = $idx++;
    }

    return %S;
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
    ExtractDescription ExtractStructInfo
    /;
}

1;
