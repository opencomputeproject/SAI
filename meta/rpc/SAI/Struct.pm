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

package SAI::Struct;

use namespace::autoclean;
use Moose::Util::TypeConstraints;

use Moose;

has 'name' => ( is => 'ro', required => 1 );
has 'members' => ( is => 'ro' );

with 'SAI::RPC::ThriftName', 'SAI::Utils::XMLLoader';

###########
# Methods #
###########

sub canonical_name {
    my $self = shift;

    my $name = $self->name;
    $name = $1 if $name =~ /^sai_(\w+)$/;

    return $name;
}

sub short_name {
    my $self = shift;

    my $name = $self->canonical_name;
    $name = $1 if $name =~ /^(\w+)_t$/;

    return $name;
}

################
# TT coditions #
################

################
# Construction #
################

sub parse_xml_typedef {
    my $class       = shift;
    my $xml_typedef = shift;

    my $name = $xml_typedef->{name}[0];

    return ( name => $name );
}

sub validate_xml_typedef {
    my $class       = shift;
    my $xml_typedef = shift;

    my $name = { $class->parse_xml_typedef($xml_typedef) }->{name};
    return 0 unless defined $name;

    return unless $name =~ /^sai_(\w+)$/;

    # Make sure we have struct or union
    my $definition = $xml_typedef->{type}[0];
    $definition = $definition->{content} if ref $definition eq 'HASH';
    return 0 unless $definition eq 'struct' or $definition eq 'union';

    return 1;
}

__PACKAGE__->meta->make_immutable;
1;
