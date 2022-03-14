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

package SAI::Typedef;

use namespace::autoclean;

use Moose::Util::TypeConstraints;
use Term::ANSIColor;
use Carp;

use Moose;

extends 'SAI::Type';

# Typedef should look like this:
# typedef <type> <name>;
#
# The type is SAI::Type object,
# the name comes from the parent class
has 'raw' => ( is => 'rw', isa => 'Bool', default => 0 );
has 'type' => (
    is       => 'rw',
    isa      => 'SAI::Type',
    required => 1,
    coerce   => 1,
    handles  => { def => 'name' },
);

###########
# Methods #
###########

# Get Typedef definition part
sub thrift_def {
    my $self = shift;

    my $name = $self->type->thrift_name( $self->raw );

    # The special case - replace definition (like i64)
    # with string - only for ip/mac addresses
    $name = 'string' if $self->name =~ /(ip\d+|mac)_t$/;

    croak colored(
        'Circular type dependency '
          . $name . ' -> '
          . $self->type->thrift_name . "\n",
        'bold red'
    ) if $self->thrift_name eq $name;

    return $name;
}

################
# TT coditions #
################

################
# Construction #
################

sub BUILD {
    my $self = shift;

    # Tread all typedefs pointers as lists
    $self->convert_to_list if $self->ptr;

    return;
}

# Get type definition from XML typedef
around parse_xml_typedef => sub {
    my $orig        = shift;
    my $class       = shift;
    my $xml_typedef = $_[0];

    my $definition = _get_definition_from_xml_typedef($xml_typedef);

    return ( type => $definition, $class->$orig(@_) );
};

around validate_xml_typedef => sub {
    my $orig        = shift;
    my $class       = shift;
    my $xml_typedef = $_[0];

    return 0 unless $xml_typedef->{name}[0] =~ m{^sai_(\w+)
                                             (?<!_fn) # _fn suffix is not allowed
                                             $}x;

    my $definition = _get_definition_from_xml_typedef($xml_typedef);

    # Make sure it is not struct or union
    return 0 if $definition eq 'struct' or $definition eq 'union';

    return $class->$orig(@_);
};

sub _get_definition_from_xml_typedef {
    my $xml_typedef = shift;

    # Get type definition
    my $definition = $xml_typedef->{type}[0];
    $definition = $definition->{content} if ref $definition eq 'HASH';

    return $definition;
}

__PACKAGE__->meta->make_immutable;
1;
