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

package SAI::RPC::ProtoBufName::Type;

use Moose::Role;

requires 'is_attr_list';

with 'SAI::RPC::ProtoBufName';

###########
# Methods #
###########

# Get the 'gpb' name equivalent
around protobuf_name => sub {
    my $orig     = shift;
    my $self     = shift;
    my $raw_name = shift;
    my $name     = shift;

    # Surround the type with list<>
    if ( $self->is_attr_list ) {
        return 'sai_protobuf_attribute_list_t';
    }
    else {
        return 'repeated ' . $self->subtype->protobuf_name
          if $self->is_list
          and $self->subtype->name !~ /(?:void|char)/;
    }
    $name //= $self->name;

    # If we have a char array, then replace it with a string
    $name = 'string'
      if ( $name =~ /void/ and ( $self->is_list or $self->ptr ) )
      or $name =~ /char/;

    # Handle special types
    $name = 'int32_t' if $name =~ /^enum(\s|$)/;

    # If $raw then convert the type to protobuf type
    $name = "int32" if $raw_name and $name =~ /(u)?int32_t/;
    $name = "int32" if $raw_name and $name =~ /(u)?int8_t/;
    $name = "int32" if $raw_name and $name =~ /(u)?int16_t/;
    $name = "int64" if $raw_name and $name =~ /(u)?int64_t/;

    # Replace any pointer to funtion with the single pointer type
    $name = 'pointer_t' if ( $name =~ /_fn$/ );
    $name = 'int64' if $raw_name and ( $name =~ /size_t$/ or $self->ptr );

    $raw_name = 1 if $name =~ /(?:string|bool|void)/;

    # Call the original function (add prefix etc)
    $name = $self->$orig( @_, $raw_name, $name );

    return $name;
};

1;
