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

package SAI::Utils::XMLLoader;

use Carp;

use Moose::Role;

# This role can be composed into classes, that should
# to be initialized with data provided from XML

requires 'validate_xml_typedef';
requires 'parse_xml_typedef';

################
# Construction #
################

# Prepare constructor arguments before creation
around BUILDARGS => sub {
    my $orig  = shift;
    my $class = shift;

    my %args = @_;

    # If we have XML typedef, then obtain the type definition
    if ( $args{xml_typedef} ) {
        my $xml_typedef = delete $args{xml_typedef};
        $class->validate_xml_typedef($xml_typedef)
          or croak "Invalid XML definition for $class object";
        %args = ( %args, $class->parse_xml_typedef($xml_typedef) );
        return $class->$orig(%args);
    }

    return $class->$orig(@_);
};

1;
