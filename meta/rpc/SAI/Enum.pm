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

package SAI::Enum;

use SAI::Enum::Element;

use namespace::autoclean;
use Moose;

# The simplified Enum implementation - only names of elements
has 'name' => ( is => 'ro', isa => 'Str', required => 1 );
has 'elements' => (
    traits  => ['Array'],
    is      => 'rw',
    isa     => 'ArrayRef[SAI::Enum::Element]',
    handles => {
        filter_elements => 'grep',
        get_element     => 'get',
        all_elements    => 'elements',
        map_elements    => 'map'
    },
    required => 1,
);

with 'SAI::Utils::XMLLoader';

##################
# Public methods #
##################

# Return the SAI object the enum is related to
sub object {
    my $self = shift;

    return lc $1 if $self->name =~ /^[_]?sai_(\w+)_[^W_]+_t$/;
    return;
}

###################
# Private methods #
###################

################
# Construction #
################

# Validate method should be overridden
sub _validate_element {
    my $class     = shift;
    my $enumvalue = shift;
    return SAI::Enum::Element->validate_xml_typedef($enumvalue);
}

# Create method should be overridden, should return a class of Enum::Element
sub _create_element {
    my $class     = shift;
    my $enumvalue = shift;
    return SAI::Enum::Element->new( xml_typedef => $enumvalue );
}

# Get Enum definition from XML typedef (SAI::Utils::XMLLoader role)
sub parse_xml_typedef {
    my $class       = shift;
    my $xml_typedef = shift;

    my @elements;

    # Get function name
    my $name = $xml_typedef->{name}[0];

    # Get elements
    my $current_value = 0;
    my $idx           = 0;
    for ( @{ $xml_typedef->{enumvalue} } ) {
        next unless defined and $class->_validate_element($_);
        my $element = $class->_create_element($_);
        if ( not defined $element->value ) {
            ++$idx;
            $element->value("$current_value + $idx");
        }
        else { $idx = 0; $current_value = $element->value }

        push @elements, $element;
    }

    return (
        name     => $name,
        elements => \@elements,
    );
}

# Validate function definition in XML typedef (SAI::Utils::XMLLoader role)
sub validate_xml_typedef {
    my $class       = shift;
    my $xml_typedef = shift;

    return 0 unless $xml_typedef->{kind} eq 'enum';
    return 0 unless $xml_typedef->{name}[0] =~ /^[_]?sai_(\w+)_[^W_]+_t$/;

    return 1;
}
__PACKAGE__->meta->make_immutable;
1;
