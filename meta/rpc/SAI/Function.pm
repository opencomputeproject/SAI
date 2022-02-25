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

package SAI::Function;

use Moose::Util::TypeConstraints;
use Term::ANSIColor;
use Carp;

use namespace::autoclean;
use Moose;

################
# Helper types #
################
subtype 'SAI::Types::ReturnValue' => as 'SAI::Function::Argument' =>
  where { $_->name eq 'retval' };

# Accept also Str (TypeName) as ReturnValue
coerce 'SAI::Types::ReturnValue' => from 'SAI::Types::TypeName' => via {
    SAI::Function::Argument->new(
        name      => 'retval',
        type      => $_,
        is_retval => 1
      )
};

##########
# Fields #
##########
has 'name' => ( is => 'ro', isa => 'Str', required => 1 );
has 'return' => (
    is      => 'rw',
    isa     => 'SAI::Types::ReturnValue',
    coerce  => 1,
    trigger => \&_return_set,
);
has 'args' => (
    traits => ['Array'],
    is     => 'rw',
    isa    => 'ArrayRef[SAI::Function::Argument]',
    handles =>
      { filter_args => 'grep', get_arg => 'get', all_args => 'elements' },
    trigger => \&_args_set,
);
has 'api'      => ( is => 'rw', isa => 'Str' );
has 'dbg_info' => ( is => 'ro', isa => 'Str' );

with 'SAI::RPC::ThriftName', 'SAI::Utils::XMLLoader', 'SAI::RPC::Function';

##################
# Public methods #
##################

# Get the operation name, e.g. 'set', 'get', 'create', etc.
sub operation {
    my $self = shift;
    return ( $self->_get_info_from_name )[0];
}

# Get the object name, the function operates on
sub object {
    my $self = shift;

    my $object = ( $self->_get_info_from_name )[1];

    # For attr arguments, the object information is required
    if ( not defined $object and $self->filter_args( sub { $_->is_attr } ) ) {
        carp colored( 'Could not obtain object name (' . $self->name . "())\n",
            'red' );
    }

    return $object;
}

###################
# Private methods #
###################

# return field trigger
sub _return_set {
    my $self   = shift;
    my $return = shift;

    # just set the parent object (this 'Function' object)
    $return->_set_parent($self);

    return;
}

# args field trigger
sub _args_set {
    my $self = shift;
    my $args = shift;

    # set the parent object (this 'Function' object) for all args
    $_->_set_parent($self) for ( @{$args} );

    return;
}

# Get (from function name) an object name and operation performed on it
sub _get_info_from_name {
    my $self = shift;

    if (
        $self->name =~
m{(create|remove|set|get|clear)_         # we expect attribute only for those functions
                    (\w+)(?<!_attribute)(?<!_stats)(?<!_ext) # attribute should not be matched
                    (_attribute|_stats)? # attribute is optional (for set or get only)
                    (_ext)?              # ext is optional
                    $}x
      )
    {
        my $operation = $1;
        $operation = 'stats'
          if $operation eq 'get'
          and defined $3
          and $3 eq '_stats';
        return $operation, $2;
    }

    # Special cases
    return ( 'other', 'fdb_flush' )
      if ( $self->name =~ 'flush_fdb_entries' );
    return ( q{}, 'hostif_packet' )
      if ( $self->name =~ 'hostif_packet$' );

    return ( q{}, q{} );
}

# Find the count argument for the specified potential list argument
sub _find_count_arg {
    my $self = shift;
    my $arg  = shift;

    my $direct_predecessor = 1;
    my $arg_pos            = $arg->pos;
    my $prev_arg;

    # Check previous arguments
    while ( $prev_arg = $self->get_arg( --$arg_pos ) ) {

        # Previous variable has its uint32 (is a list)
        if ( $prev_arg->count ) {

            # Get count argument from the prevous one, e.g.:
            # get_stats(uint32_t number_of_elements,
            #           sai_stat_id_t *counter_ids,
            #           uint64_t *counters)
            return $prev_arg->count;
        }
        elsif (
            # Previous variable is uint32
            $prev_arg->type->thrift_name =~ /uint32_t/

            #  or list of uint32 (and we have pointer to pointer)
            or (    $arg->type->ptr == 2
                and $prev_arg->is_list
                and $prev_arg->type->subtype->thrift_name =~ /uint32_t/ )
          )
        {
            # Note, that counter must be directly previous argument.
            return $prev_arg if $direct_predecessor;
        }
        else { $direct_predecessor = 0 }
    }

    return;
}

################
# TT coditions #
################

################
# Construction #
################

# Function arguments play another roles, depending on their neighbours
# e.g. uint32 argument before pointer argument, means that pointer
# argument is a list, and uint32 one is its counter
sub resolve_arg_dependencies {
    my $self = shift;

    # If we have pointer args, then mark some of them as arrays
    for my $arg ( $self->filter_args( sub { $_->type->ptr } ) ) {

        my $count_arg = $self->_find_count_arg($arg);

        if ($count_arg) {

            # Arg is a list:
            $arg->count($count_arg);
            $count_arg->is_count(1);
            $arg->convert_to_list;
        }

    }

    return;
}

# Moose builder, called right after construction
sub BUILD {
    my $self = shift;

    # Assign argument positions
    my $pos = 0;
    $_->_set_pos( $pos++ ) for ( $self->all_args );

    # Finish function creation
    $self->resolve_arg_dependencies;
    return;
}

# Get function definition from XML typedef (SAI::Utils::XMLLoader role)
sub parse_xml_typedef {
    my $class       = shift;
    my $xml_typedef = shift;

    my $ret_type;
    my @args;
    my $fn;

    # Get function name
    $fn = $1 if $xml_typedef->{name}[0] =~ /^(\w+)_fn$/;

    # Get return value
    $ret_type = $1 if $xml_typedef->{type}[0] =~ /^(\w+_t)/;

    # Get parameter list
    my $args_string = $xml_typedef->{argsstring}[0];
    $args_string =~ s/[()]//g;
    my @sai_args = split /, /, $args_string;
    push @args, SAI::Function::Argument->new($_) for (@sai_args);

    return (
        name     => $fn,
        return   => $ret_type,
        args     => \@args,
        dbg_info => $xml_typedef->{type}[0]
          . $xml_typedef->{name}[0]
          . $xml_typedef->{argsstring}[0],
    );
}

# Validate function definition in XML typedef (SAI::Utils::XMLLoader role)
sub validate_xml_typedef {
    my $class       = shift;
    my $xml_typedef = shift;

    return 0
      unless $xml_typedef->{name}[0] =~ /^\w+_fn$/;
    return 0
      unless $xml_typedef->{type}[0] =~ /^\w+_t/;

    return 1;
}

__PACKAGE__->meta->make_immutable;
1;
