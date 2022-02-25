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

package SAI::Variable;

use namespace::autoclean;
use Moose;

use SAI::Type;

# Variable should look like this:
# <type> <name>;
has 'name' => ( is => 'rw', isa => 'Str', required => 1 );
has 'type' => (
    is       => 'rw',
    isa      => 'SAI::Type',
    required => 1,
    coerce   => 1,
    handles  => {
        def                  => 'name',
        is_attr              => 'is_attr',
        is_list              => 'is_list',
        is_attr_list         => 'is_attr_list',
        typename             => 'name',
        thrift_typename      => 'thrift_name',
        convert_to_list      => 'convert_to_list',
        convert_to_attr_list => 'convert_to_attr_list'
    },
);

with 'SAI::RPC::ThriftName::Variable';

###########
# Methods #
###########

################
# Construction #
################

__PACKAGE__->meta->make_immutable;
1;
