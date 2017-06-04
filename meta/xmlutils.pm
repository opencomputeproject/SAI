#!/usr/bin/perl

package xmlutils;

use strict;
use warnings;
use diagnostics;
use Getopt::Std;
use Data::Dumper;

require Exporter;

my $ident = 0;

sub PrintDebug
{
    my $line = shift;
    my $spaces = "  " x $ident;

    print "$spaces$line\n" if defined $main::optionPrintDebug and $main::optionPrintDebug > 1;
}

sub PrintError
{
    my $line = shift;

    print "ERROR: $line\n";
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

    PrintDebug "> entering $tag";

    if (not defined $node->{$tag})
    {
        PrintDebug ": adding key '$tag' to node";

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
        my $line = shift $Lines;

        last if not defined $line;
        chomp $line;

        if ($line =~ m!^</$tag>$!)
        {
            PrintDebug "< exiting $tag";

            $current{content} =~ s/\s*$//;
            $current{content} =~ s/^\s*//;

            # cleanup node
            if (scalar(keys%current) == 1)
            {
                pop$tagarr;
                push$tagarr, $current{content};
            }

            $ident--;
            return;
        }

        if ($line =~ m!^<(\w+)(\s+[^<>]+[^/])?>$!)
        {
            ProcessTag($Lines, \%current, $1, $2);
            next;
        }

        if ($line =~ m!^[^<>]+$!)
        {
            $current{content} .= UnescapeXml "$line ";
            PrintDebug ": content: $line";
            next;
        }

        PrintError "not supported line: '$line'";
    }

    PrintError "EOF reached when parsing tag '$tag'";
}

sub ReadXml
{
    my $filename = shift;

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

        # split each line by any xml tag and remove white space lines
        push @lines, grep { not $_ =~ /^\s*$/ } split/\s*(<[^>]+>)\s*/,$line;
    }

    close FH;

    $ident = 0;
    ProcessTag(\@lines, \%ROOT, $doxygenTag);

    # print Dumper (%ROOT);
    return $ROOT{$doxygenTag}[0];
}

BEGIN
{
    our @ISA    = qw(Exporter);
    our @EXPORT = qw/ReadXml UnescapeXml/;
}

1;
