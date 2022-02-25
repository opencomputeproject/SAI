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

package SAI::Attrs::Attribute;

use namespace::autoclean;
use Moose;

has 'type' => ( is => 'rw', isa => 'SAI::Type', required => 1, coerce => 1 );
has 'flags' => ( is => 'rw', isa => 'Str', required => 1 );

has 'get' => ( is => 'rw', isa => 'Bool', default => 1 );
has 'set' => (
    is      => 'rw',
    isa     => 'Bool',
    lazy    => 1,
    default => sub { shift->flags =~ /SET/ },
);
has 'create' => (
    is      => 'rw',
    isa     => 'Bool',
    lazy    => 1,
    default => sub { shift->flags =~ /CREATE/ },
);
has 'mandatory' => (
    is      => 'rw',
    isa     => 'Bool',
    lazy    => 1,
    default => sub { shift->flags =~ /MANDATORY/ },
);

# The name of field in sai_attribute_value_t
has 'typename' => ( is => 'rw', isa => 'Str' );

extends 'SAI::Enum::Element';

###########
# Methods #
###########

################
# TT coditions #
################

################
# Construction #
################

sub _get_flags_and_type {
    my $class       = shift;
    my $xml_typedef = $_[0];

    my $flags;
    my $type;

    # Iterate over the hash to find flags and type
    for ( @{ $xml_typedef->{simplesect} } ) {
        next unless defined $_->{title};
        $flags = $_->{para}[0]
          if ( $_->{title}[0] eq 'Flags:' );

        if ( $_->{title}[0] eq 'Value Type:' ) {
            $type = $1 if $_->{para}[0] =~ /\@type\s*(\w+)/;
        }
        last if defined $type and defined $flags;
    }

    $type =~ s/\@type\s*(\w+)//g if defined $type;

    return ( $flags, $type );
}

around parse_xml_typedef => sub {
    my $orig        = shift;
    my $class       = shift;
    my $xml_typedef = $_[0];

    my $flags;
    my $type;

    for ( @{ $xml_typedef->{detaileddescription}[0]->{para} } ) {
        next unless $class->_validate_xml_enum_description($_);
        ( $flags, $type ) = $class->_get_flags_and_type($_);
        last if defined $type and defined $flags;
    }

    return ( type => $type, flags => $flags, $class->$orig(@_) );
};

# Validate enum descrtiption from XML
sub _validate_xml_enum_description {
    my $class = shift;
    my $desc  = shift;

    return 1 if ref $desc eq 'HASH';
    return 0;
}

around validate_xml_typedef => sub {
    my $orig        = shift;
    my $class       = shift;
    my $xml_typedef = $_[0];

    my $flags;
    my $type;

    return 0 unless $class->$orig(@_);

    for ( @{ $xml_typedef->{detaileddescription}[0]->{para} } ) {
        next unless $class->_validate_xml_enum_description($_);
        ( $flags, $type ) = $class->_get_flags_and_type($_);
        last if defined $type and defined $flags;
    }

    return 0 unless defined $type and defined $flags;

    return 1;
};

__PACKAGE__->meta->make_immutable;
1;
