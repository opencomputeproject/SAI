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

package SAI::Function::Argument;

use Term::ANSIColor;
use Carp;

use namespace::autoclean;
use Moose;

extends 'SAI::Variable';

# Argument is a variable with some additional info.
# Argument list should look like this:
# ret_type function(<type1> <name1>, <type2> <name2>)

##########
# Fields #
##########
has 'pos'    => ( is => 'ro', isa => 'Int', writer => '_set_pos' );
has 'in'     => ( is => 'ro' );
has 'out'    => ( is => 'ro' );
has 'parent' => ( is => 'ro', isa => 'SAI::Function', writer => '_set_parent' );

has 'is_retval' => ( is => 'rw', default => 0 );

has 'count'    => ( is => 'rw' );
has 'is_count' => ( is => 'rw' );

with 'SAI::RPC::Function::Argument';

###########
# Methods #
###########

# We need to rename some variables to avoid conflicts
around name => sub {
    my $orig = shift;
    my $self = shift;

    # Call the original function
    my $name = $self->$orig(@_);

    # _id variables may conflicts with attribute names on python side
    $name =~ s/_id$/_oid/g if $name !~ /switch_o?id/;

    return $name;
};

sub has_attributes {
    my $self = shift;

    return 1
      if $self->type->is_list
      and $self->type->subtype
      and $self->type->subtype->name =~ /attr/;

    return 0;
}

sub is_return {
    my $self = shift;

    my $is_return = $self->compare( $self->parent->return );

    carp colored(
        'Return value mismatch ('
          . $self->parent->name . '(): '
          . $self->name . ")\n",
        'yellow'
    ) unless !$is_return == !$self->is_retval;

    return $is_return;
}

# Compare the position of argument. Both must exist (and be the same)
# or both of them must be undefined. Otherwise, the arg is not the same as
# another one.
sub compare {
    my $self = shift;
    my $obj  = shift;
    return (
        $self->parent->name eq $obj->parent->name
          and (
            (
                defined $self->pos and defined $obj->pos
                and $self->pos == $self->parent->rpc_return->pos
            )
            or ( not defined $self->pos and not defined $obj->pos )
          )
    );
}

################
# Construction #
################

# Build argument from string
around BUILDARGS => sub {
    my $orig  = shift;
    my $class = shift;

    my $argstr;
    $argstr = $_[0] if @_ == 1;
    return $class->$orig(@_)
      if not $argstr
      or ref $argstr
      or not $argstr =~ /^_(\w+)_(\s\S+)* (\S+) (\S+)$/;

    my ( $direction, $arg_type, $arg_name ) = ( $1, $3, $4 );

    $direction = lc $direction;
    my $in  = ( $direction =~ /in/ );
    my $out = ( $direction =~ /out/ );

    # Move pointer information into the type
    my $ptr = ( $arg_name =~ s/[*]//g );
    $arg_type .= q{*} x $ptr if $ptr;

    return $class->$orig(
        name => $arg_name,
        type => $arg_type,
        in   => $in,
        out  => $out,
        ptr  => $ptr,
    );
};

__PACKAGE__->meta->make_immutable;
1;
