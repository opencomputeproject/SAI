#!/usr/bin/perl
#
# Copyright (c) 2014 Microsoft Open Technologies, Inc.
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
# @file    test.pm
#
# @brief   This module defines SAI Metadata Test Parser
#

package test;

use strict;
use warnings;
use diagnostics;
use Data::Dumper;
use utils;
use xmlutils;

require Exporter;

our @TESTNAMES = ();

sub DefineTestName
{
    my $name = shift;

    push @TESTNAMES,$name;

    WriteTest "void $name(void)";
}

sub CreatePointersTest
{
    DefineTestName "check_pointer_names_test";

    WriteTest "{";

    for my $pointer (sort keys %main::NOTIFICATIONS)
    {
        if (not $pointer =~ /^sai_(\w+)_notification_fn/)
        {
            LogWarning "notification function $pointer is not ending on _notification_fn";
            next;
        }

        my $name = $1;

        # make sure taht declared pointer is correct
        # by testing if it will compile in test

        WriteTest "    $pointer var_$pointer = NULL;";
        WriteTest "    PP(var_$pointer);";

        # make sure that switch attribute exists corresponding to pointer name

        my $swattrname = uc "SAI_SWITCH_ATTR_${name}_NOTIFY";

        WriteTest "    printf(\"%d\\n\", $swattrname);";
    }

    WriteTest "}";
}

sub CreateNonObjectIdTest
{
    DefineTestName "non_object_id_test";

    WriteTest "{";

    WriteTest "    sai_object_key_t ok;";
    WriteTest "    volatile void *p;";

    my @rawnames = GetNonObjectIdStructNames();

    # add object id since it should be in the struct also

    push @rawnames, "object_id";

    for my $rawname (@rawnames)
    {
        # we are getting pointers for each member of non object id
        # to make sure its declared in the struct, if its not then
        # it will fail to compile

        WriteTest "    p = &ok.key.$rawname;";
        WriteTest "    printf(\"$rawname: \"); PP(p);";

        WriteTest "    TEST_ASSERT_TRUE(&ok.key == (void*)&ok.key.$rawname, \"member $rawname don't start at union begin! Standard C fail\");";
    }

    WriteTest "}";
}

sub CreateSwitchIdTest
{
    DefineTestName "switch_id_position_test";

    WriteTest "{";

    my @rawnames = GetNonObjectIdStructNames();

    for my $rawname (@rawnames)
    {
        WriteTest "    sai_${rawname}_t $rawname = { 0 };";
        WriteTest "    TEST_ASSERT_TRUE(&$rawname == (void*)&$rawname.switch_id, \"$rawname.switch_id is not at the struct beginning\");";
    }

    WriteTest "}";
}

sub CreateCustomRangeTest
{
    DefineTestName "custom_range_test";

    # purpose of this test is to make sure
    # all objects define custom range start and end markers

    WriteTest "{";

    my @all = @{ $main::SAI_ENUMS{sai_object_type_t}{values} };

    for my $obj (@all)
    {
        next if $obj eq "SAI_OBJECT_TYPE_NULL";
        next if $obj eq "SAI_OBJECT_TYPE_MAX";

        next if not $obj =~ /SAI_OBJECT_TYPE_(\w+)/;

        WriteTest "    TEST_ASSERT_TRUE(SAI_$1_ATTR_CUSTOM_RANGE_START == 0x10000000, \"invalid custom range start for $1\");";
        WriteTest "    TEST_ASSERT_TRUE(SAI_$1_ATTR_CUSTOM_RANGE_END > 0x10000000, \"invalid custom range end for $1\");";
    }

    WriteTest "}";
}

sub CreateCustomRangeAllTest
{
    DefineTestName "custom_range_all_test";

    # purpose of this test is to make sure
    # all enums define custom range start and end markers

    WriteTest "{";

    my @keys = sort keys %main::SAI_ENUMS_CUSTOM_RANGES;

    for my $key (@keys)
    {
        my @all = @{ $main::SAI_ENUMS_CUSTOM_RANGES{$key}{customranges} };

        for my $enum (@all)
        {
            WriteTest "    TEST_ASSERT_TRUE($enum == 0x10000000, \"invalid custom range start for $enum\");" if $enum =~ /_START$/;
            WriteTest "    TEST_ASSERT_TRUE($enum < 0x20000000, \"invalid custom range end for $enum\");" if $enum =~ /_END$/;
        }
    }

    WriteTest "}";
}

sub CreateCustomRangeBaseTest
{
    DefineTestName "custom_range_base_test";

    WriteTest "{";

    for my $key (sort keys %main::SAI_ENUMS)
    {
        next if not defined $main::SAI_ENUMS{$key}{ranges};

        my @ranges = @{ $main::SAI_ENUMS{$key}{ranges} };

        next if scalar @ranges == 0;

        for my $range (@ranges)
        {
            my $prefix = uc $1 if $key =~ /(sai_\w+)_t$/;

            if ($range eq "${prefix}_CUSTOM_RANGE_BASE")
            {
                WriteTest "    TEST_ASSERT_TRUE_EXT($range == 0x10000000, \"invalid custom range base for $range = 0x%x\", $range);" ;
                next;
            }

            if ($range eq "${prefix}_EXTENSIONS_RANGE_BASE")
            {
                WriteTest "    TEST_ASSERT_TRUE_EXT($range == 0x20000000, \"invalid extensions range base for $range: = 0x%x\", $range);" ;
                next;
            }

            LogInfo "Skipping range base $range";

            # currently any other range should be less than custom

            WriteTest "    TEST_ASSERT_TRUE_EXT($range < 0x10000000 , \"invalid extensions range base for $range = 0x%x\", $range);" ;
        }
    }

    WriteTest "}";
}

sub CreateEnumSizeCheckTest
{
    DefineTestName "enum_size_check_test";

    WriteTest "{";

    # purpose of this test is to check if all enums size is int32_t in this compiler
    # since serialize/deserialize enums make assumption that enum base is int32_t

    for my $key (sort keys %main::SAI_ENUMS)
    {
        next if not $key =~ /^(sai_\w+_t)$/;
        next if $key =~ /^(sai_null_attr_t)$/;

        WriteTest "    TEST_ASSERT_TRUE((sizeof($1) == sizeof(int32_t)), \"invalid enum $1 size\");";
    }

    WriteTest "    TEST_ASSERT_TRUE((sizeof(sai_status_t) == sizeof(int32_t)), \"invalid sai_status_t size\");";

    WriteTest "}";
}

sub CreateListCountTest
{
    #
    # purpose of this test is to check if all list structs have count as first
    # item so later on we can cast any structure to extract count
    #

    DefineTestName "list_count_test";

    WriteTest "{";

    my %Union = ExtractStructInfo("sai_attribute_value_t", "union_");

    WriteTest "    size_t size_ref = sizeof(sai_object_list_t);";

    for my $key (sort keys %Union)
    {
        my $type = $Union{$key}->{type};

        next if not $type =~ /^sai_(\w+_list)_t$/;

        my $name = $1;

        WriteTest "    $type $name;";
        WriteTest "    TEST_ASSERT_TRUE(sizeof($type) == size_ref, \"type $type has different sizeof than sai_object_type_t\");";
        WriteTest "    TEST_ASSERT_TRUE(sizeof($name.count) == sizeof(uint32_t), \"$type.count should be uint32_t\");";
        WriteTest "    TEST_ASSERT_TRUE(sizeof($name.list) == sizeof(void*), \"$type.list should be pointer\");";
        WriteTest "    TEST_ASSERT_TRUE(&$name == (void*)&$name.count, \"$type.count should be first member in $type\");";
    }

    WriteTest "}";
}

sub CreateApiNameTest
{
    #
    # Purpose of this check is to find out if all api names correspond to
    # actual object names and follow convention name and the same signature except
    # some special objects.
    #

    DefineTestName "api_name_test";

    WriteTest "{";

    my @objects = @{ $main::SAI_ENUMS{sai_object_type_t}{values} };

    WriteTest "    int visited = 1; /* 1 for NULL object */";

    WriteTest "    void *dummy = NULL;";

    for my $ot (@objects)
    {
        next if $ot =~ /^SAI_OBJECT_TYPE_(MAX|NULL)$/;

        $ot =~ /^SAI_OBJECT_TYPE_(\w+)$/;

        if (IsSpecialObject($ot))
        {
            # those objects are special, just attributes, no APIs
            WriteTest "    visited++;";
            next;
        }

        my $short = lc($1);

        my $api = $main::OBJTOAPIMAP{$ot};

        if (not defined $api)
        {
            LogError "$ot is not defined in OBJTOAPIMAP, missing sai_XXX_api_t declaration?";
            next;
        }

        WriteTest "    {";
        WriteTest "        sai_${api}_api_t ${api}_api = { 0 };";

        if (defined $main::NON_OBJECT_ID_STRUCTS{$ot})
        {
            # object type is non object id, we must generate stuff on the fly

            WriteTest "        typedef sai_status_t (*${short}_create_fn)(\\";
            WriteTest "                _In_ const sai_${short}_t *$short,\\";
            WriteTest "                _In_ uint32_t attr_count,\\";
            WriteTest "                _In_ const sai_attribute_t *attr_list);";

            WriteTest "        typedef sai_status_t (*${short}_remove_fn)(\\";
            WriteTest "                _In_ const sai_${short}_t *$short);";

            WriteTest "        typedef sai_status_t (*${short}_set_fn)(\\";
            WriteTest "                _In_ const sai_${short}_t *$short,\\";
            WriteTest "                _In_ const sai_attribute_t *attr);";

            WriteTest "        typedef sai_status_t (*${short}_get_fn)(\\";
            WriteTest "                _In_ const sai_${short}_t *$short,\\";
            WriteTest "                _In_ uint32_t attr_count,\\";
            WriteTest "                _Inout_ sai_attribute_t *attr_list);";

            WriteTest "        ${short}_create_fn create = ${api}_api.create_$short;";
            WriteTest "        ${short}_remove_fn remove = ${api}_api.remove_$short;";
            WriteTest "        ${short}_set_fn set = ${api}_api.set_${short}_attribute;";
            WriteTest "        ${short}_get_fn get = ${api}_api.get_${short}_attribute;";

            # just to check if function is declared

            WriteTest "        sai_create_${short}_fn cr = NULL;";
            WriteTest "        sai_remove_${short}_fn re = NULL;";
            WriteTest "        sai_set_${short}_attribute_fn se = NULL;";
            WriteTest "        sai_get_${short}_attribute_fn ge = NULL;";

            WriteTest "        dummy = &create;";
            WriteTest "        dummy = &remove;";
            WriteTest "        dummy = &set;";
            WriteTest "        dummy = &get;";
            WriteTest "        dummy = &cr;";
            WriteTest "        dummy = &re;";
            WriteTest "        dummy = &se;";
            WriteTest "        dummy = &ge;";
            WriteTest "        dummy = NULL;";
            WriteTest "        visited++;";
        }
        else
        {
            if ($ot eq "SAI_OBJECT_TYPE_SWITCH")
            {
                WriteTest "        sai_create_switch_fn create = ${api}_api.create_$short;";
            }
            else
            {
                WriteTest "        sai_generic_create_fn create = ${api}_api.create_$short;";
            }

            WriteTest "        sai_generic_remove_fn remove = ${api}_api.remove_$short;";
            WriteTest "        sai_generic_set_fn set = ${api}_api.set_${short}_attribute;";
            WriteTest "        sai_generic_get_fn get = ${api}_api.get_${short}_attribute;";

            # just to check if function is declared

            WriteTest "        sai_create_${short}_fn cr = NULL;";
            WriteTest "        sai_remove_${short}_fn re = NULL;";
            WriteTest "        sai_set_${short}_attribute_fn se = NULL;";
            WriteTest "        sai_get_${short}_attribute_fn ge = NULL;";

            WriteTest "        dummy = &create;";
            WriteTest "        dummy = &remove;";
            WriteTest "        dummy = &set;";
            WriteTest "        dummy = &get;";
            WriteTest "        dummy = &cr;";
            WriteTest "        dummy = &re;";
            WriteTest "        dummy = &se;";
            WriteTest "        dummy = &ge;";
            WriteTest "        dummy = NULL;";
            WriteTest "        visited++;";
        }

        WriteTest "    }";
    }

    WriteTest "        int sum = SAI_OBJECT_TYPE_MAX + (SAI_OBJECT_TYPE_EXTENSIONS_RANGE_END - SAI_OBJECT_TYPE_EXTENSIONS_RANGE_START);";
    WriteTest "        TEST_ASSERT_TRUE_EXT(sum == visited, \"not all objects were processed, expexted: %d, but got: %d\", sum, visited);";

    WriteTest "    PP(dummy);";

    WriteTest "}";
}

sub CreateStructListTest
{
    #
    # make sure that all structs _list_t contains 2 items
    # and purpose is to be list, so .count and .list
    #

    my %StructLists = GetStructLists();

    DefineTestName "struct_list_test";

    WriteTest "{";

    WriteTest "    uint32_t count;";
    WriteTest "    void *ptr;";

    for my $struct (sort keys %StructLists)
    {
        WriteTest "    TEST_ASSERT_TRUE(sizeof($struct) == sizeof(sai_object_list_t), \"struct $struct sizeof is differenat than sai_object_list_t\");";
        WriteTest "    $struct s_$struct;";
        WriteTest "    memset(&s_$struct,0, sizeof($struct));";
        WriteTest "    count = s_$struct.count;";
        WriteTest "    ptr   = s_$struct.list;";
        WriteTest "    printf(\"$struct %p %u\\n\", ptr, count);";
    }

    WriteTest "}";
}

sub CreatePassParamsForSerializeTest
{
    my $refStructInfoEx = shift;

    my $structName = $refStructInfoEx->{name};

    return "" if not defined $refStructInfoEx->{extraparam};

    my @params = @{ $refStructInfoEx->{extraparam} };

    my $passParams = "";

    for my $param (@params)
    {
        if (not $param =~ m!^(const\s+)?(\w+)(\s+|\s*(\*)\s*)?(\w+)$!)
        {
            LogWarning "failed to parse extra param '$param' on $structName";
            next;
        }

        my $paramType = $2;
        my $pointer = 1 if defined $4;
        my $paramName = $5;

        $passParams .= "$paramName, ";

        WriteTest "    $param;";

        if (not defined $pointer)
        {
            WriteTest "    memset(&$paramName, 0, sizeof($paramType));";

            if ($paramType eq "sai_object_type_t")
            {
                WriteTest "    $paramName = SAI_OBJECT_TYPE_PORT;";
            }
        }

        if (defined $pointer and $paramType eq "sai_attr_metadata_t")
        {
            if ($structName =~ /acl_field_data/)
            {
                WriteTest "    $paramName = sai_metadata_get_attr_metadata(SAI_OBJECT_TYPE_ACL_ENTRY, SAI_ACL_ENTRY_ATTR_FIELD_SRC_IP);";
            }
            elsif ($structName =~ /acl_action/)
            {
                WriteTest "    $paramName = sai_metadata_get_attr_metadata(SAI_OBJECT_TYPE_ACL_ENTRY, SAI_ACL_ENTRY_ATTR_ACTION_COUNTER);";
            }
            else
            {
                WriteTest "    $paramName = sai_metadata_get_attr_metadata(SAI_OBJECT_TYPE_SWITCH, SAI_SWITCH_ATTR_PORT_NUMBER);";
            }
        }
    }

    return $passParams;
}

sub CreateSerializeSingleStructTest
{
    my $refStructInfoEx = shift;

    my $structName = $refStructInfoEx->{name};
    my $structBase = $refStructInfoEx->{baseName};

    WriteTest "  {";
    WriteTest "    printf(\"serializing $structName ... \");";
    WriteTest "    fflush(stdout);";
    WriteTest "    $structName $structBase;";
    WriteTest "    memset(&$structBase, 0, sizeof($structName));";

    if ($structName eq "sai_object_meta_key_t")
    {
        WriteTest "    $structBase.objecttype = SAI_OBJECT_TYPE_PORT;";
    }

    my $passParams = CreatePassParamsForSerializeTest($refStructInfoEx);

    WriteTest "    ret = sai_serialize_$structBase(buf, $passParams&$structBase);";
    WriteTest "    TEST_ASSERT_TRUE(ret > 0, \"failed to serialize $structName\");";
    WriteTest "    printf(\"serialized $structName: %s\\n\", buf);";
    WriteTest "  }";
}

sub CreateSerializeStructsTest
{
    # make sure that all structs can be serialized fine

    DefineTestName "serialize_structs";

    WriteTest "{";

    WriteTest "    char buf[0x4000];";
    WriteTest "    int ret;";

    for my $structname (sort keys %main::ALL_STRUCTS)
    {
        next if $structname  =~ /_api_t$/;
        next if $structname  eq "sai_service_method_table_t";


        my %structInfoEx = ExtractStructInfoEx($structname, "struct_");

        # TODO add tag "noserialize"

        next if defined $structInfoEx{ismetadatastruct} and $structname ne "sai_object_meta_key_t";

        # TODO for all structs with attributes, we should iterate via all attributes
        # defined in metadata

        CreateSerializeSingleStructTest(\%structInfoEx);
    }

    WriteTest "}";
}

sub CreateSerializeUnionsTest
{
    # make sure that all unions can be serialized fine

    DefineTestName "serialize_unions";

    WriteTest "{";
    WriteTest "    char buf[0x4000];";
    WriteTest "    int ret;";

    for my $unionTypeName (sort keys %main::SAI_UNIONS)
    {
        next if not $unionTypeName =~ /^sai_(\w+)_t$/;

        my %structInfoEx = ExtractStructInfoEx($unionTypeName, "union_");

        CreateSerializeSingleStructTest(\%structInfoEx);
    }

    WriteTest "}";
}

sub CreateStatEnumTest
{
    # make sure that all objects with stats are populated

    DefineTestName "statenum_defined";

    WriteTest "{";

    for my $key (sort keys %main::SAI_ENUMS)
    {
        next if not $key =~ /sai_(\w+)_stat_t/;

        my $ot = uc("SAI_OBJECT_TYPE_$1");

        WriteTest "    TEST_ASSERT_TRUE(sai_metadata_object_type_info_${ot}".".statenum != NULL, \"statenum field for object $ot must be populated\");";
    }

    WriteTest "}";
}

sub CreateApiImplementedTest
{
    # make sure that all apis are implemented in vendor library

    DefineTestName "api_implemented";

    WriteTest "{";

    WriteTest "#ifdef API_IMPLEMENTED_TEST";

    for my $name (sort keys %main::GLOBAL_APIS)
    {
        my $type = $main::GLOBAL_APIS{$name}{type};

        # link will fail if one of below apis is not implemented and exported in vendor sai library

        WriteTest "    ${name}_fn var_$name = &$name;";
        WriteTest "    PP(var_$name);";
    }

    WriteTest "#endif";

    WriteTest "}";
}

sub CreateStructUnionSizeCheckTest
{
    my @headers = GetHeaderFiles(); # we ignore experimental headers

    my %STRUCTS = ();

    DefineTestName "struct_union_size";

    WriteTest "{";

    for my $header (@headers)
    {
        my $data = ReadHeaderFile($header);

        my @lines = split/\n/,$data;

        for my $line (@lines)
        {
            next if not $line =~ /typedef\s+(?:struct|union)\s+_(sai_\w+_t)/;

            my $name = $1;

            $STRUCTS{$name} = $name;

            next if $name =~ /^sai_\w+_api_t$/; # skip api structs
            next if $name eq "sai_switch_health_data_t";

            my $upname = uc($name);

            WriteTest "#ifdef ${upname}_SIZE";
            WriteTest "    TEST_ASSERT_TRUE_EXT(sizeof($name) == (${upname}_SIZE), \"wrong size of $name, expected %d, got %zu\", ${upname}_SIZE, sizeof($name));";
            WriteTest "    TEST_ASSERT_TRUE_EXT(sizeof($name [3]) == (3*${upname}_SIZE), \"wrong size of $name [3], expected %d, got %zu\", 3*${upname}_SIZE, 3*sizeof($name));";
            WriteTest "#else";
            WriteTest "    fprintf(stderr, \"struct/union $name not defined in base branch, skipping size check\\n\");";
            WriteTest "#endif";
        }
    }

    WriteTest "}";
}

sub WriteTestHeader
{
    #
    # Purpose is to write saimedatatest.c header
    #

    WriteTest "#include <stdio.h>";
    WriteTest "#include <stdlib.h>";
    WriteTest "#include <string.h>";
    WriteTest "#include \"saimetadata.h\"";
    WriteTest "#include \"saimetadatasize.h\"";
    WriteTest "#define PP(x) printf(\"%p\\n\", (x));";
    WriteTest "#define TEST_ASSERT_TRUE(x,msg) if (!(x)){ fprintf(stderr, \"ASSERT TRUE FAILED(%d): %s: %s\\n\", __LINE__, #x, msg); exit(1);}";
    WriteTest "#define TEST_ASSERT_TRUE_EXT(x,format,...) if (!(x)){ fprintf(stderr, \"ASSERT TRUE FAILED(%d): %s: \" format \"\\n\", __LINE__, #x, ##__VA_ARGS__); exit(1);}";
}

sub WriteTestMain
{
    #
    # Purpose is to write saimedatatest.c main funcion
    # and all test names
    #

    WriteTest "int main()";
    WriteTest "{";

    my $count = @TESTNAMES;

    my $n = 0;

    for my $name (@TESTNAMES)
    {
        $n++;

        WriteTest "    printf(\"Executing Test [$n/$count]: $name\\n\");";
        WriteTest "    $name();";
    }

    WriteTest "    return 0;";
    WriteTest "}";
}

sub CreatePragmaPush
{
    #
    # because we are merging extension attributes into existing
    # enums, new versions of gcc can warn when 2 different enums
    # are mixed, so lets ignore this warning using pragmas
    #

    WriteTest "#pragma GCC diagnostic push";
    WriteTest "#pragma GCC diagnostic ignored \"-Wpragmas\"";
    WriteTest "#pragma GCC diagnostic ignored \"-Wenum-conversion\"";
    WriteTest "#pragma GCC diagnostic ignored \"-Wsuggest-attribute=noreturn\"";
}

sub CreatePragmaPop
{
    WriteTest "#pragma GCC diagnostic pop";
}

sub CreateExtensionRangeTest
{
    DefineTestName "extension_range_test";

    # purpose of this test is to make sure
    # all extensions range bases are from

    WriteTest "{";

    for my $key (sort keys %main::SAI_ENUMS)
    {
        next if not defined $main::SAI_ENUMS{$key}{ranges};

        my @ranges = @{ $main::SAI_ENUMS{$key}{ranges} };

        for my $range (@ranges)
        {
            next if not $range =~ /EXTENSIONS_RANGE_BASE/;

            WriteTest "    TEST_ASSERT_TRUE($range == 0x20000000, \"invalid extension range base for $range\");";
        }
    }

    WriteTest "}";
}

sub CreateTests
{
    WriteTestHeader();

    CreatePragmaPush();

    CreateNonObjectIdTest();

    CreateSwitchIdTest();

    CreateCustomRangeTest();

    CreatePointersTest();

    CreateEnumSizeCheckTest();

    CreateListCountTest();

    CreateApiNameTest();

    CreateStructListTest();

    # TODO tests for notifications

    CreateSerializeStructsTest();

    CreateSerializeUnionsTest();

    CreateStatEnumTest();

    CreateApiImplementedTest();

    CreateStructUnionSizeCheckTest();

    CreateCustomRangeAllTest();

    CreateCustomRangeBaseTest();

    CreateExtensionRangeTest();

    CreatePragmaPop();

    WriteTestMain();
}

BEGIN
{
    our @ISA    = qw(Exporter);
    our @EXPORT = qw/
    CreateTests
    /;
}

1;

