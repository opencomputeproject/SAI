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

package SAI::Struct::Member;

use namespace::autoclean;
use Moose;

extends 'SAI::Variable';

# Typedef should look like this:
# struct <struct_name> {
#   <type1> <name1>,
#   <type2> <name2>,
# }

with 'SAI::Utils::XMLLoader';

###########
# Methods #
###########

################
# TT coditions #
################

################
# Construction #
################

sub BUILD {
    my $self = shift;

    # Tread all memeber pointers as lists
    $self->type->convert_to_list if $self->type->ptr;

    return;
}

sub parse_xml_typedef {
    my $class       = shift;
    my $xml_typedef = shift;

    my $name = $xml_typedef->{name};
    my $type = $xml_typedef->{type};
    my $id   = $xml_typedef->{idx} + 1;

    return ( name => $name, type => $type, id => $id );
}

sub validate_xml_typedef {
    my $class       = shift;
    my $xml_typedef = shift;

    return 1;
}

__PACKAGE__->meta->make_immutable;
1;
