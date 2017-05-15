#!/usr/bin/perl

use strict;
use warnings;
use diagnostics;

my $exitcode = 0;

for my $line (<STDIN>)
{
    chomp $line;

    next if not $line =~ /\w+ (\w+) (\w+)/;

    my $type = $1;
    my $name = $2;

    next if $name =~ /^(sai_metadata_\w+|__func__)/ and $type =~ /[rRBT]/;

    # metadata log level is exception since it can be changed

    next if $1 eq "sai_metadata_log_level";

    print STDERR "ERROR: symbol '$line' is not prefixed 'sai_metadata_' or not in read-only section\n";

    $exitcode = 1;
}

exit $exitcode;
