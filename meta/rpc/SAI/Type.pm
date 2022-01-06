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

package SAI::Type;

use namespace::autoclean;

use Moose::Util::TypeConstraints;
use Term::ANSIColor;
use Carp;

use Moose;

subtype 'SAI::Types::TypeName' => as 'Str' => where { /\S\w+/ };

# Accept also Str (TypeName) as Type
coerce 'SAI::Type' => from 'SAI::Types::TypeName' =>
  via { SAI::Type->new( name => $_ ) };

# The type has a name, and additional type information.
has 'name' => (
    is       => 'ro',
    isa      => 'SAI::Types::TypeName',
    required => 1,
    writer   => '_set_name',
);
has 'ptr'   => ( is => 'rw', isa => 'Int', default => 0 );
has 'array' => ( is => 'ro', isa => 'Int', default => 0 );
has 'subtype' => (
    is      => 'rw',
    isa     => 'SAI::Type',
    coerce  => 1,
    trigger => \&_subtype_set,
);

with 'SAI::RPC::Type', 'SAI::RPC::ThriftName::Type', 'SAI::Utils::XMLLoader';

###########
# Methods #
###########

# The simplified name of a type
sub short_name {
    my $self = shift;

    my $short_name;

    if ( $self->is_list ) {
        $short_name = $self->subtype->name;
    }
    else {
        $short_name = $self->name;
    }

    $short_name =~ s/sai_//g;
    $short_name = $1 if $short_name =~ /^(\w+)_t$/;

    $short_name = 'buffer' if $short_name =~ /void/ and $self->ptr;

    $short_name .= '_list' if $self->is_list;

    return $short_name;
}

sub is_list {
    my $self = shift;
    return 1 if ( $self->array or $self->ptr ) and $self->subtype;
    return 0;
}

sub is_attr {
    my $self = shift;

    return 1 if $self->name =~ /attribute/;

    return 0;
}

sub _subtype_set {
    my $self    = shift;
    my $subtype = shift;

    if ( $self->ptr ) {
        $self->_set_name( 'PTR=>' . $subtype->name );
    }
    elsif ( $self->array ) {
        $self->_set_name( 'ARRAY=>' . $subtype->name );
    }
    else {
        croak 'Cannot set subtype for '
          . $self->name
          . ' - it is neither pointer nor array';
    }

    return;
}

sub convert_to_list {
    my $self = shift;

    if ( $self->ptr ) {
        $self->subtype( $self->name );
    }
    else {
        croak $self->name . ': only pointer can be converted to the list';
    }

    return;
}

################
# TT coditions #
################

################
# Construction #
################

around BUILDARGS => sub {
    my $orig  = shift;
    my $class = shift;

    my %args  = @_;
    my $ptr   = 0;
    my $array = 0;

    # Check if we have a pointer or array
    if ( $args{name} ) {
        $args{name} =~ s/(const )|( const)//g;
        $ptr = $args{name} =~ s/[*]//g unless $args{ptr};
        $array = $1 if $args{name} =~ s/\[(\d+)\]//g and not $args{array};
        if ($ptr) {
            $args{ptr} = $ptr;
        }
        if ($array) {
            $args{subtype} = $args{name};
            $args{array}   = $array;
        }
        return $class->$orig(%args);
    }

    return $class->$orig(@_);
};

sub parse_xml_typedef {
    my $class       = shift;
    my $xml_typedef = shift;

    my $name = $xml_typedef->{name}[0];

    return ( name => $name );
}

sub validate_xml_typedef {
    my $class       = shift;
    my $xml_typedef = shift;

    return 1;
}

__PACKAGE__->meta->make_immutable;
1;
