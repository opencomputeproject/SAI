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

package SAI::RPC::Type;

use Carp;

use Moose::Role;

requires 'name';
requires 'is_attr';
requires 'is_list';

# Attr list is a special struct that contains attribute list and count.
# This role has is_attr_list attribute and a function used to perform
# a converion.
sub is_attr_list;
has 'is_attr_list' =>
  ( is => 'ro', isa => 'Bool', writer => '_set_is_attr_list', default => 0 );

sub convert_to_attr_list {
    my $self = shift;

    if ( $self->is_attr and $self->is_list ) {
        $self->_set_is_attr_list(1);
    }
    else {
        croak $self->name
          . ' cannot be converted to sai_thrift_attribute_list_t';
    }

    return;
}

1;
