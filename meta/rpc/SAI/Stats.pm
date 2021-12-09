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

package SAI::Stats;

use namespace::autoclean;
use MooseX::Aliases;
use Moose;

extends 'SAI::Enum';

alias 'all' => 'all_elements';

# Validate function definition in XML typedef (SAI::Utils::XMLLoader role)
around validate_xml_typedef => sub {
    my $orig        = shift;
    my $class       = shift;
    my $xml_typedef = $_[0];

    return 0 unless $class->$orig(@_);
    return 0 unless $xml_typedef->{name}[0] =~ /stat_t/;

    return 1;
};

__PACKAGE__->meta->make_immutable;
1;
