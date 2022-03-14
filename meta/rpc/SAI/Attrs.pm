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

package SAI::Attrs;

use namespace::autoclean;
use MooseX::Aliases;
use Moose;

use SAI::Attrs::Attribute;

extends 'SAI::Enum';

##################
# Public methods #
##################

# Return all attributes
alias 'all' => 'all_elements';

# Return other attributes
# ('other' operation is returned by non recognized SAI::Function objects)
alias 'other' => 'all';

# Return all 'get' attributes
alias 'get' => 'all';

# Return all 'set' attributes
sub set {
    my $self = shift;
    return $self->filter_elements( sub { $_->set } );
}

# Return all 'mandatory' attributes
sub mandatory {
    my $self = shift;
    return $self->filter_elements( sub { $_->mandatory } );
}

# Return all 'create' attributes - excluding mandatory
sub create {
    my $self = shift;
    return $self->filter_elements( sub { $_->create and not $_->mandatory } );
}

################
# Construction #
################

# Validate function definition in XML typedef (SAI::Utils::XMLLoader role)
around validate_xml_typedef => sub {
    my $orig        = shift;
    my $class       = shift;
    my $xml_typedef = $_[0];

    return 0 unless $class->$orig(@_);
    return 0 unless $xml_typedef->{name}[0] =~ /attr_t/;

    return 1;
};

# Validate method should be overridden
sub _validate_element {
    my $class     = shift;
    my $enumvalue = shift;
    return SAI::Attrs::Attribute->validate_xml_typedef($enumvalue);
}

# Create method should be overridden, should return a class of Enum::Element
sub _create_element {
    my $class     = shift;
    my $enumvalue = shift;
    return SAI::Attrs::Attribute->new( xml_typedef => $enumvalue );
}

__PACKAGE__->meta->make_immutable;
1;
