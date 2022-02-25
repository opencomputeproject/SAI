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

package SAI::Enum::Element;

use namespace::autoclean;
use Moose;

has 'name'        => ( is => 'ro', isa => 'Str',  required => 1 );
has 'value'       => ( is => 'rw', isa => 'Str',  required => 0 );
has 'initialized' => ( is => 'ro', isa => 'Bool', required => 0 );

with 'SAI::Utils::XMLLoader';

###########
# Methods #
###########

################
# TT coditions #
################

# Get the simple name of the enum element
sub simple_name {
    my $self = shift;

    return lc "_$2$3" if $self->name =~ /SAI_\w+_(ATTR|STAT)_(\d+)(\w+)$/;
    return lc $2 if $self->name =~ /SAI_\w+_(ATTR|STAT)_(\w+)$/;
    return lc $self->name;
}

################
# Construction #
################

sub parse_xml_typedef {
    my $class       = shift;
    my $xml_typedef = shift;

    my $name = $xml_typedef->{name}[0];
    my $value;

    # If the element has an initializer, then assign the value.
    # Otherwise it needs to be set by setter.
    if ( $xml_typedef->{initializer} ) {

        # Trim and ignore '=' and ',', then take all characters
        $value = $1
          if $xml_typedef->{initializer}[0] =~ /[=]?\s*(.*)\s*[,]?/;

        return ( name => $name, value => $value, initialized => 1 )
          if defined $value;
    }

    return ( name => $name );
}

sub validate_xml_typedef {
    my $class       = shift;
    my $xml_typedef = shift;

    # Invalid elements contains _START and _END suffixes
    return 0 if $xml_typedef->{name}[0] =~ /_(START|END)$/;
    return 1;
}

__PACKAGE__->meta->make_immutable;
1;
