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

package SAI::RPC::Function::Argument;

use Moose::Role;

requires 'name';
requires 'type';
requires 'pos';
requires 'in';
requires 'out';
requires 'parent';
requires 'count';
requires 'is_count';
requires 'has_attributes';
requires 'compare';

# Determine if the argument is a return value defined by the RPC interface
sub is_rpc_return {
    my $self = shift;

    return 1 if $self->compare( $self->parent->rpc_return );

    return 0;
}

# Most of types returned by C++ server are the same as in Thrift file,
# but some of them are being changed by Thrift to void, and moved into first parameter.
sub is_cpp_return {
    my $self = shift;

    if ( $self->is_rpc_return ) {
        return 1
          unless $self->is_list
          or $self->type->thrift_name eq 'string'
          or $self->type->thrift_name eq 'void';
    }

    return 0;
}

sub is_adapter_arg {
    my $self = shift;

    # Expose all arguments, with some exceptions:

    # We cannot expose arguments which are not exposed via RPC
    return 0 if not $self->is_rpc_arg;

    # Don't expose attributes - we handle them using arguments
    return 0 if ( $self->is_attr or $self->type->is_attr_list );

    # Don't expose stats lists - we add them all to the list by default
    return 0
      if ( $self->is_list and $self->type->subtype->name =~ 'stat_id_t' );

    # Python can take count from its lists natively
    return 0 if ( $self->is_count );

    return 1;
}

sub is_rpc_arg {
    my $self = shift;

    # Expose all arguments, with some exceptions:
    # Remove switch ID
    return 0
      if $self->name =~ /switch_o?id/ and $self->typename =~ /object_id_t/;

    # Count is taken from vector (unless it is a list of counts)
    return 0 if $self->is_count and not $self->is_list;

    # If the argument is returned by RPC funtion (and is not 'in'),
    # then remove it from RPC argument list
    return 0 if $self->compare( $self->parent->rpc_return ) and not $self->in;

    return 1;
}

# Obsolete
# TODO: Remove this method
sub internal {
    my $self = shift;
    return not $self->is_rpc_arg;
}

# Check if the argument requires additional vector allocated
sub requires_vector {
    my $self = shift;

    # We declare additional vector for attributes. If the argument is a list
    # then we already have a vector (but for sai_attr_list we need it anyway
    # to extract the vector from sai_attr_list structure)
    return 1
      if $self->is_attr and ( not $self->is_list or $self->is_attr_list );

    return 0;
}

# Check if the argument requires malloc - for attribute
# or counter parsing
sub requires_malloc {
    my $self = shift;

    return 1 if $self->is_list or $self->is_attr_list;

    return 0;
}

# Check if the argument requires counter type parsing
sub requires_counter_parsing {
    my $self = shift;

    return 1
      if $self->parent->operation =~ '(stats|clear)' and $self->is_list;

    return 0;
}

# Check if the argument requires struct or another complex
# type parsing
sub requires_parsing {
    my $self = shift;

    return 1
      if ( $self->type->ptr and $self->is_rpc_arg )
      or $self->requires_counter_parsing;

    return 0;
}

sub requires_preprocessing {
    my $self = shift;

    return ( $self->requires_parsing or $self->requires_malloc );
}

sub requires_casting {
    my $self = shift;

    # All RPC arguments should be casted into SAI types. If the argument
    # is preprocessed, then we already have a variable of the proper type.
    return ( $self->is_rpc_arg and not $self->requires_preprocessing );
}

sub requires_address {
    my $self = shift;

    # RPC args are not passed by pointers, so if SAI requires an address
    # then we need to use '&' operator, unless we already declared a pointer.
    return ( $self->type->ptr and not $self->requires_malloc );
}

sub requires_declaration {
    my $self = shift;

    # If argument is a count then don't declare it. We can do it later
    # if needed.
    return 0 if $self->is_count;

    # If argument requires malloc, then we declare it right before malloc
    return 0 if $self->requires_malloc;

    return 1 if $self->requires_parsing;

    # If this is an internal argument only, then we have to declare it,
    # because it is not part of Thrift argument list
    return 1 if not $self->is_rpc_arg;

    # in/out arguments: in Thrift list is of C++ type, so
    # we need a local C copy.
    return 1 if $self->in and $self->out;

    return 0;
}

sub requires_adapter_preprocessing {
    my $self = shift;

    # We preprocess only those attributes which are
    # 1) Related to stats (lists) or attributes
    # 2) Are required by RPC server, but not provided
    #    by python caller.
    #    Note, that expanded attribute lists are not
    #    considered as function arguments (they are not
    #    part of SAI interface).
    #    The attribute argument itself is hidden (not
    #    exposed by adapter).
    return (  not $self->is_adapter_arg
          and $self->is_rpc_arg
          and ( $self->is_list or $self->is_attr ) );
}

1;
