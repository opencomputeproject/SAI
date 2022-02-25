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

package SAI::RPC::Function;

use Moose::Role;

requires 'return';
requires 'args';
requires 'filter_args';
requires 'resolve_arg_dependencies';

after 'resolve_arg_dependencies' => sub {
    my $self = shift;

    # Attribute lists return as attribute_list_t
    $self->rpc_return->convert_to_attr_list
      if $self->rpc_return->type->name !~ /status_t/
      and $self->rpc_return->is_attr
      and $self->rpc_return->is_list;
};

# Get return variable for RPC interface
sub rpc_return {
    my $self = shift;

    return $self->return if $self->return->type->name !~ /status_t/;

    # "Inout/Out" argument should be returned by the function
    for my $arg ( $self->filter_args( sub { $_->type->ptr } ) ) {
        return $arg if $arg->out;
    }

    # Do not return a status, if not OK, the exception should be raised
    return SAI::Function::Argument->new(
        name      => $self->return->name,
        type      => 'void',
        is_retval => 0,
        parent    => $self
    );
}

# Get parameter list for RPC interface
sub rpc_args {
    my $self = shift;

    return $self->filter_args( sub { $_->is_rpc_arg } );
}

# Get parameter list for python adapter
sub adapter_args {
    my $self = shift;

    return $self->filter_args( sub { $_->is_adapter_arg } );
}

# Get parameter list that requires preprocessing
sub preprocessed_args {
    my $self = shift;

    return $self->filter_args( sub { $_->requires_preprocessing } );
}

# Get parameter list that requires declaration
sub declared_args {
    my $self = shift;

    return $self->filter_args( sub { $_->requires_declaration } );
}

# Get parameter list that requires preprocessing
sub adapter_preprocessed_args {
    my $self = shift;

    return $self->filter_args( sub { $_->requires_adapter_preprocessing } );
}

1;
