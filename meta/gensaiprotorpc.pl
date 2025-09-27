#!/usr/bin/env perl
#
# Copyright (c) 2021 Microsoft Open Technologies, Inc.
#
#    Licensed under the Apache License, Version 2.0 (the "License"); you may
#    not use this file except in compliance with the License. You may obtain
#    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
#
#    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
#    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
#    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
#    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
#
#    See the Apache Version 2.0 License for specific language governing
#    permissions and limitations under the License.
#
#    Microsoft would like to thank the following companies for their review and
#    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
#    Dell Products, L.P., Facebook, Inc., Marvell International Ltd.
#
# @file    gensaiprotorpc.pl
#
# @brief   This module generates RPC interface of SAI for PTF
#

use strict;
use warnings;
use diagnostics;

use 5.020;

use File::Spec::Functions qw(catdir catfile rel2abs);
use File::Basename qw(dirname);
use English qw(-no_match_vars);
use Getopt::Long::Descriptive;
use File::Path qw(rmtree);
use Term::ANSIColor;
use Cwd qw(getcwd);
use Data::Dumper;
use Const::Fast;
use File::Copy;
use Template;
use Carp;
use Tie::File;

use lib catdir( dirname( rel2abs(__FILE__) ), 'rpc' );

use Utils::Format;
use Utils;

use SAI::Function::Argument;
use SAI::Struct::Member;
use SAI::Function;
use SAI::Typedef;
use SAI::Struct;
use SAI::Attrs;
use SAI::Stats;
use SAI::Type;

# Avoid warnings related to given/when
no if $] >= 5.018, warnings => 'experimental::smartmatch';

# Consts
# TODO: move some of them to common package (e.g. PREFIX or RETVAL) so that they can be shared
const my $PREFIX         => 'sai_protobuf_';
const my $GEN_DIR        => 'generated-protobuf';
const my $COUNTER        => -1;
const my $RETVAL         => 0;

# Keep the dbg data sorted, to make it comparable
$Data::Dumper::Sortkeys = 1;

my $run_dir     = getcwd;
my $script_dir  = dirname( rel2abs($PROGRAM_NAME) );
my $script_path = rel2abs(__FILE__);
my $gen_dir     = catdir( $script_dir, $GEN_DIR );
my $templates_dir =
  catdir( $ENV{PAR_TEMP} ? "$ENV{PAR_TEMP}/inc" : $script_dir, 'templates' );

# Parameters
my $sai_dir = catdir( dirname($script_dir) );

#<<<
( my $args, my $usage ) = describe_options(
    "$PROGRAM_NAME %o",
    [ 'dbg',             'Print debug information in generated files, implies --dump',                                            { default => 0 }        ],
    [ 'experimental|e',  'Generate also experimental files',                                                                      { default => 0 }        ],
    [ 'dist',            'Create the standalone \'gensairpc\' script (experimental)',                                             { default => 0 }        ],
    [ 'dump|d',          'Dump all data to the file',                                                                             { default => 0 }        ],
    [ 'clean-meta|c',    'Perform clean meta before generation',                                                                  { default => 0 }        ],
    [ 'verbose|v',       'Print more details',                                                                                    { default => 0 }        ],
    [ 'mandatory-attrs', 'Make mandatory attributes obligatory in sai_adapter.py',                                                { default => 0 }        ],
    [ 'dev-utils:s',     'Generate additional development utils within the generated code. Additional options: [=log,zero]',      { default => 0 }        ],
    [ 'adapter_logger',  'Enable the logger in sai_adapter, it will log all the method invocation.',                              { default => 0}         ],
    [ 'attr-header',     'Generate additional header of attributes definitions (including object types)',                         { default => 0 }        ],
    [ 'help|h',          'Print this help',                                                                                       { shortcircuit => 1 }   ],
);
#>>>

if ( $args->help ) {
    print $usage->text;
    exit;
}

if ( $args->dist ) {
    Utils->dist($script_path);
    exit;
}

my $dbg     = $args->dbg;
my $dump    = $args->dump;
my $verbose = $args->verbose;
$dump    = 1 if $dbg;
$verbose = 1 if $dump;
my $experimental  = $args->experimental;
my $clean         = $args->clean_meta;
my $mandatory_attrs = $args->mandatory_attrs;
my $dev_utils       = ( $args->dev_utils ne q{} ? $args->dev_utils : 1 );
my $adapter_logger   = ( $args->adapter_logger ne q{} ? $args->adapter_logger : 1 );
my $attr_header     = $args->attr_header;

# Configure SAI meta
my $sai_meta_dir = catdir( $sai_dir, 'meta' );
my $sai_parse_path = catfile( $sai_meta_dir, 'parse.pl' );

-d $sai_dir or die "\"$sai_dir\" directory is invalid";

# Declare SAI meta global variables, its libraries need them
our $XMLDIR           = catdir( $sai_meta_dir, 'xml' );
our $INCLUDE_DIR      = catdir( $sai_dir,      'inc' );
our $EXPERIMENTAL_DIR = catdir( $sai_dir,      'experimental' );

# Include SAI meta libs. The intention is that SAI meta path
# can be changed by the parameter, so we cannot use 'use'
our @INC;
unshift @INC, $sai_meta_dir;

require xmlutils;
xmlutils->import;

require utils;
utils->import;

if ($clean) {
    say colored( "Removing \"$GEN_DIR\" directory...", 'bold blue' );
    rmtree $gen_dir;
    say colored( 'Cleaning SAI meta...', 'bold blue' );
    system 'make clean -C ' . $sai_meta_dir;
}

# Generate Doxygen xml files
say colored( 'Building SAI meta XML...', 'bold blue' );
system 'make xml -C ' . $sai_meta_dir;

say colored( 'Parsing...', 'bold blue' );
my $data = get_definitions();
my $vars = {
    apis            => $data->{apis},
    functions       => $data->{functions},
    methods         => $data->{methods},
    structs         => $data->{structs},
    dbg             => $dbg,
    mandatory_attrs => $mandatory_attrs,
    dev_utils       => $dev_utils,
    adapter_logger  => $adapter_logger,
    templates_dir   => $templates_dir
};

mkdir $gen_dir;

if ($dump) {
    say colored( 'Generating sai_dbg.dump...', 'bold blue' );
    open my $dump_file, '>', catfile( $gen_dir, 'sai_dbg.dump' )
      or die 'Could not open the dump file';
    print {$dump_file} Dumper $data;
    close $dump_file or die;
}

my $template = Template->new( ABSOLUTE => 1 );

my $sai_proto_path = catfile($run_dir, 'sai.proto');

say colored( 'Generating sai.proto...', 'bold blue' );
$template->process( catfile( $templates_dir, 'sai.protobuf.tt' ),
    $vars, $sai_proto_path )
  or die $template->error();

# Tie the file to an array
tie my @lines, 'Tie::File', $sai_proto_path or die "Couldn't tie $sai_proto_path: $!";

# Replace 'sai_thrift_' with 'sai_protobuf_' in each line
foreach my $line (@lines) {
    $line =~ s/sai_thrift_/sai_protobuf_/g;
}

# Untie the file
untie @lines;

say colored( 'Generating ProtoBuf files...', 'bold blue' );
chdir $gen_dir;
system('protoc --cpp_out=. --proto_path=.. sai.proto') == 0 or die colored("Command failed: $!", "red");
chdir $run_dir;


# Find the api name within the header related to the currenct XML file.
sub get_api_name
{
    # it may happen that metadata is already generated, and SAI directory is
    # mounted in docker, then the location will be invalid inside docker, so
    # let's use only filename and inc directory

    my $location = shift;

    $location =~ s!.+/!!; # leave only filename

    # The hash should be static - don't open and traverse the header
    # if we already found the API name inside.
    state %api_names = (
            "saimetadatalogger.h" => "common",
            "saimetadatatypes.h" => "common",
            "sai.h" => "common",
            "saiobject.h" => "object", # for sai_object_key_entry_t
            "saitypes.h" => "common" );

    return $api_names{$location} if exists $api_names{$location};

    my $file = "$sai_dir/inc/$location";

    $file = "$sai_dir/experimental/$location" if $location =~ /experimental|extension/;

    open(H, '<', $file) or die "Failed to open: $file: $!";

    my @lines = <H>;

    close H or croak;

    for my $line (@lines)
    {
        if ($line =~ /^typedef struct _sai_(\w+)_api_t/)
        {
            $api_names{$location} = $1;

            return $1;
        }
    }

    if ($location =~ /^sai\w*extensions.h$/)
    {
        $api_names{$location} = "common";

        return "common";
    }

    die "File $file doesn't contain api struct!";
}

# The main function that parses all XML files and creates all
# types definitions.
sub get_definitions {
    my %methods_table;
    my %all_functions;
    my %all_structs;
    my %all_attrs;
    my @all_enums;
    my %apis;
    my $i = 0;

    # Iterate over files
    for ( GetSaiXmlFiles($XMLDIR) ) {
        my $xml = ReadXml($_);

        # Iterate over definitions
        for ( @{ $xml->{compounddef}[0]->{sectiondef} } ) {
            if ( $_->{kind} eq 'typedef' or $_->{kind} eq 'enum' ) {
                for ( @{ $_->{memberdef} } ) {
                    my $function;
                    my $typedef;
                    my $object;
                    my $struct;
                    my $attrs;
                    my $stats;
                    my $api;

                    # Get API name
                    my $location = $_->{location}[0]->{file};
                    unless ($experimental) {
                        next if $location =~ /(experimental|extension)/;
                    }
                    $api = get_api_name($location);
                    $apis{$api} = {} unless ref $apis{$api} eq 'HASH';

                    # Populate attribute list per object
                    next if get_object_types( $api, $apis{$api}, $_ );
                    next if get_attributes( $apis{$api}, $_ );
                    next if get_stats( $apis{$api}, $_ );

                    # Populate type definitions and function declarations

                    next if get_typedef( $apis{$api}, $_, $api eq 'common' );

                    next
                      if get_struct( $apis{$api}, \%all_structs,
                        \%methods_table, $_ );

                    next
                      if $api ne 'common'
                      and
                      get_function( $apis{$api}, \%all_functions, $_, $api );

                    # Add enum name to the list
                    if ( $_->{kind} eq 'enum' ) {
                        my $enum_name = $_->{name}[0];
                        $enum_name =~ s/^_//;
                        push @all_enums, $enum_name;
                    }

                }
            }
        }
    }

    my $api_list = assign_attr_types( \%apis, \@all_enums );

    return {
        apis      => $api_list,
        attrs     => \%all_attrs,
        structs   => \%all_structs,
        functions => \%all_functions,
        methods   => \%methods_table
    };
}

# To set or get attribute, the proper value struct field need to be used.
# Obtain the name of this field and assign it to the attribute.
# If the attribute is of enum type, then it is s32, otherwise the correct type
# should be found within sai_attribute_value_t fields.
sub get_attr_type {
    my $attr       = shift;
    my $attr_types = shift;
    my $all_enums  = shift;

    my $type = $attr->type->protobuf_name;

    # First, check if we have enum
    return 'int32' if ( $attr->type->name ~~ $all_enums );

    # Try to compare types of attribute and attr value otherwise
    for ( @{ $attr_types->members } ) {
        return $_->protobuf_name if ( $type eq $_->type->protobuf_name );
    }

    carp colored( "Unknown type $type of attribute " . $attr->name, 'red' );
    return;
}

# To set or get attribute, the proper value struct field need to be used.
# Obtain the name of this field and assign it to all attributes.
sub assign_attr_types {
    my $apis      = shift;
    my $all_enums = shift;

    my $attr_types;
    for ( @{ $apis->{common}->{structs} } ) {
        $attr_types = $_ if $_->name eq 'sai_attribute_value_t';
    }

    croak unless $attr_types;

    for my $api ( values %{$apis} ) {
        for my $object ( values %{ $api->{objects} } ) {
            for my $attr ( $object->{attrs}->all ) {
                $attr->typename(
                    get_attr_type( $attr, $attr_types, $all_enums ) );
            }
        }
    }

    return $apis;
}

# Create and store the object type Enum
sub get_object_types {
    my $api_name = shift;
    my $api      = shift;
    my $enum     = shift;

    return 0
      unless SAI::Enum->validate_xml_typedef($enum)
      and $enum->{name}[0] =~ /object_type_t/;
    my $object_types = SAI::Enum->new( xml_typedef => $enum );

    if ($object_types) {
        say "\"object_types\" added to \"$api_name\" hash" if $verbose;
        $api->{object_types} = $object_types;
    }

    return 1;
}

# Create and store the Attrs object
sub get_attributes {
    my $api  = shift;
    my $enum = shift;

    return 0 unless SAI::Attrs->validate_xml_typedef($enum);
    my $attrs = SAI::Attrs->new( xml_typedef => $enum );

    if ( $attrs and $attrs->object ) {
        $api->{objects}->{ $attrs->object }->{attrs} = $attrs;
    }

    return 1;
}

# Create and store the Stats object
sub get_stats {
    my $api  = shift;
    my $enum = shift;

    return 0 unless SAI::Stats->validate_xml_typedef($enum);
    my $stats = SAI::Stats->new( xml_typedef => $enum );

    if ( $stats and $stats->object ) {
        $api->{objects}->{ $stats->object }->{stats} = $stats;
    }

    return 1;
}

# Create and store the Typedef object
sub get_typedef {
    my $api         = shift;
    my $xml_typedef = shift;
    my $raw         = shift;

    # If $raw, the we can use raw thrift types only
    $raw //= 0;

    return 0 unless SAI::Typedef->validate_xml_typedef($xml_typedef);
    my $typedef = SAI::Typedef->new(
        raw         => $raw,
        xml_typedef => $xml_typedef,
    );

    push @{ $api->{typedefs} }, $typedef;

    return 1;
}

# In SAI RPC server we don't call SAI functions directly - pointers
# to them are stored in some structures, so we need to know their
# names.
sub get_method_names {
    my $struct = shift;

    my $function;
    my %methods;

    for my $method ( GetStructKeysInOrder($struct) ) {
        my $type =
          { SAI::Struct::Member->parse_xml_typedef( $struct->{$method} ) }
          ->{type};
        if ( $type =~ /^(\w+)_fn$/ ) { $function = $1 }
        else                         { next }

        $methods{$function} = $method;
    }

    return \%methods;
}

# Create and store the Struct object.
# The struct of API function pointers is an exception - just the its name.
sub get_struct {
    my $api           = shift;
    my $all_structs   = shift;
    my $methods_table = shift;
    my $xml_typedef   = shift;

    my @members;
    my $name;

    # Make sure we have a struct
    return 0 unless SAI::Struct->validate_xml_typedef($xml_typedef);

    $name = { SAI::Struct->parse_xml_typedef($xml_typedef) }->{name};
    my %struct_def = ExtractStructInfo( $name, 'struct_' );

    # If we have method table, then we don't create a structure -
    # just take the method names
    if ( $name =~ /_api_t$/ ) {
        my $method_names = get_method_names( \%struct_def );
        %{$methods_table} = ( %{$methods_table}, %{$method_names} );
        return 1;
    }

    push @members, SAI::Struct::Member->new( xml_typedef => $struct_def{$_} )
      for ( GetStructKeysInOrder( \%struct_def ) );

    my $struct = SAI::Struct->new(
        members     => \@members,
        xml_typedef => $xml_typedef,
    );

    push @{ $api->{structs} }, $struct;
    $all_structs->{ $struct->protobuf_name } = $struct;

    return 1;
}

# Create and store the Function object.
sub get_function {
    my $api           = shift;
    my $all_functions = shift;
    my $definition    = shift;
    my $api_name      = shift;

    return 0 unless SAI::Function->validate_xml_typedef($definition);
    my $function = SAI::Function->new( xml_typedef => $definition );

    $function->api($api_name);

    push @{ $api->{functions} }, $function;
    $all_functions->{ $function->name } = $function;

    return 1;
}

__END__
