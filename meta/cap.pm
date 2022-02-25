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
# @file    cap.pm
#
# @brief   This module defines SAI Metadata Capability Parser
#

package cap;

use strict;
use warnings;
use diagnostics;
use Term::ANSIColor;

use Data::Dumper;
use utils;

require Exporter;

sub GetCapabilityFiles
{
    my $dir = shift;

    opendir(my $dh, $dir) or die "Can't opendir $dir: $!";

    my @cap = grep { /^\w*\.cap$/ and -f "$dir/$_" } readdir($dh);

    closedir $dh;

    return sort @cap;
}

sub GetVendorCapabilityFiles
{
    # should capability files be in metadata dir or i cap dir ?
    return GetCapabilityFiles(".");
}

sub ReadCapFile
{
    my $filename = shift;

    local $/ = undef;

    open FILE, $filename or die "Couldn't open file $filename: $!";

    binmode FILE;

    my $string = <FILE>;

    close FILE;

    return $string;
}

sub ProcessCapAttr
{
    my $val = shift;

    return $val if $val =~ /^SAI_[0-9A-Z_]+_ATTR_[0-9A-Z_]+$/;

    LogError "wrong attr name: $val";

    return undef;
}

sub ProcessCapVendorId
{
    my $val = shift;

    # TODO numbers 0xA and 10 and 0xa should be considered the same vendor id

    return $val if $val =~ /^$NUMBER_REGEX$/;

    LogError "wrong vendor id $val";

    return undef;
}

sub ProcessCapCapability
{
    my $val = shift;

    $val =~ s/\s*//g;

    my @cap = split/[,]/,$val;

    for my $cap (@cap)
    {
        if (not $cap =~ /^(CREATE|GET|SET)$/)
        {
            LogError "invalid capability tag value '$val' ($cap)";
            return undef;
        }
    }

    return \@cap;
}

sub ProcessCapEnumCapability
{
    my $val = shift;

    $val =~ s/\s*//g;

    my @cap = split/[,]/,$val;

    for my $cap (@cap)
    {
        if (not $cap =~ /^SAI_[0-9A-Z_]+$/)
        {
            LogError "invalid enum capability tag value '$val' ($cap)";
            return undef;
        }
    }

    return \@cap;
}

my %CAP_TAGS = (
    "attr",             \&ProcessCapAttr,
    "vendorid",         \&ProcessCapVendorId,
    "capability",       \&ProcessCapCapability,
    "enumcapability",   \&ProcessCapEnumCapability,
);

sub ShallowCopyAttrEnum
{
    my $refHash = shift;

    my %hash = %{ $refHash };

    my %attr = map { $_, $hash{$_} } keys %hash;

    return \%attr;
}

sub ProcessCapFile
{
    my $file = shift;

    my $data = ReadCapFile($file);

    my %attr = ();

    my @lines = split/\n/,$data;

    my %capability = ();

    for my $line (@lines)
    {
        next if $line =~ /^$/;
        next if $line =~ /^\s*#/;

        if ($line =~ /\@skip/)
        {
            LogInfo "Skipping cap file $file";
            return undef;
        }

        LogError "non ASCII char in $file: $line" if not $line =~ /^[\x20-\x7e]+$/;

        if (not $line =~ /^@(\w+)(.*)/)
        {
            LogError "unrecognized line $line";
            next;
        }

        LogDebug "processing $line";

        my $tag = $1;
        my $val = $2;

        $val =~ s/\s+/ /g;
        $val =~ s/^\s*//;
        $val =~ s/\s*$//;

        if (not defined $CAP_TAGS{$tag})
        {
            LogError "unrecognized tag '$tag' on $line";
            next;
        }

        $val = $CAP_TAGS{$tag}->($val);

        if ($tag eq "attr")
        {
            if (defined $attr{attr} and defined $attr{vendorid})
            {
                LogDebug "Adding $attr{vendorid} -- $attr{attr}";
                $capability{ $attr{attr} }{ $attr{vendorid} } =  ShallowCopyAttrEnum(\%attr);
            }
            elsif (scalar(keys%attr) == 0)
            {
                # ok, first
            }
            else
            {
                LogError "previous attribute (before $line) does not define attr name or vendor id";
            }

            %attr = ();
        }

        $attr{$tag} = $val;
    }

    if (defined $attr{attr} and defined $attr{vendorid})
    {
        LogDebug "Adding $attr{vendorid} -- $attr{attr}";

        $capability{ $attr{attr} }{ $attr{vendorid} } = ShallowCopyAttrEnum(\%attr);
    }
    elsif (scalar(keys%attr) == 0)
    {
        # ok, first
    }
    else
    {
        LogError "previous attribute does not define attr name or vendor id";
    }

    return \%capability;
}

sub GetCapabilities
{
    my %capabilities = ();

    my @files = GetVendorCapabilityFiles();

    for my $cap (@files)
    {
        LogInfo "Processing capability file $cap";

        my $caphash = ProcessCapFile($cap);

        for my $attr (keys %{$caphash})
        {
            for my $vid (keys %{$caphash->{$attr}})
            {
                $capabilities{$attr}{$vid} = $caphash->{$attr}{$vid};
            }
        }

    }

    #print Dumper(%capabilities);
    return \%capabilities;
}

BEGIN
{
    our @ISA    = qw(Exporter);
    our @EXPORT = qw/
    GetCapabilities
    /;
}

1;
