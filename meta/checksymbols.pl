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
# @file    checksymbols.pl
#
# @brief   This module defines SAI Check Symbols Parser
#

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

    next if $name =~ /^(sai_(metadata|(de)?serialize)_\w+|__func__)/ and $type =~ /[rRBTtD]/;

    # metadata log level is exception since it can be changed

    next if $1 eq "sai_metadata_log_level";

    print STDERR "ERROR: symbol '$line' is not prefixed 'sai_metadata_' or not in read-only section\n";

    $exitcode = 1;
}

exit $exitcode;
