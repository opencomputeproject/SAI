# Copyright 2021-present Intel Corporation.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

package Utils::Format;

use Term::ANSIColor;
use Env qw(PATH);
use Carp;

use namespace::autoclean;
use Moose;

sub _find_binaries {
    my $class = shift;
    my $name  = shift;
    my $ver   = shift;

    my @binaries;
    push @binaries, grep { /\/$name(-?\d*[.]?\d*)?$/ } glob "$_/*"
      for split /:/, $PATH;

    my @binaries_ver;
    @binaries_ver = grep { /$name-?$ver/ } @binaries if defined $ver;

    return \@binaries, \@binaries_ver;
}

sub _formatter {
    my $class     = shift;
    my $file      = shift;
    my $formatter = shift;
    my $opts      = shift;
    my $ver       = shift;

    # Find clang-format binaries
    my ( $formatters, $formatters_ver ) =
      $class->_find_binaries( $formatter, $ver );

    # Run expected version, or print warning and run another one
    if ( @{$formatters_ver} ) {
        system "$formatters->[0] --version";
        system "$formatters_ver->[0] $opts " . $file;
    }
    else {
        if ($ver) {
            carp colored( "$formatter-$ver not found", 'dark yellow' );
            print "\tUse "
              . colored( '--no-format', 'bold' )
              . " to disable formatting.\n";
        }
        if ( @{$formatters} ) {
            print "\tUsing $formatters->[0] instead: " if $ver;
            system "$formatters->[0] --version";
            system "$formatters->[0] $opts " . $file;
        }
        elsif ( !$ver ) {
            carp colored( "$formatter not found", 'dark yellow' );
            print "\tUse "
              . colored( '--no-format', 'bold' )
              . " to disable formatting.\n";
        }
    }
    return;
}

sub cpp {
    my $class = shift;
    my $file  = shift;
    my $ver   = shift;

    my $formatter = 'clang-format';
    my $opts      = '-i';
    $ver //= '3.6';

    return $class->_formatter( $file, $formatter, $opts, $ver );
}

sub python {
    my $class = shift;
    my $file  = shift;
    my $ver   = shift;

    my $formatter = 'yapf';
    my $opts      = '-i --style pep8';

    $class->_formatter( $file, $formatter, $opts );

    $formatter = 'autopep8';
    $opts      = '-iaa';

    return $class->_formatter( $file, $formatter, $opts );
}

__PACKAGE__->meta->make_immutable;
1;
