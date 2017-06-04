#!/usr/bin/perl

package test;

use strict;
use warnings;
use diagnostics;
use Getopt::Std;
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

    for my $pointer (sort keys %main::NOTIFICATION_NAMES)
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

    WriteTest "    sai_object_type_t checked[SAI_OBJECT_TYPE_MAX];";
    WriteTest "    memset(checked, 0, SAI_OBJECT_TYPE_MAX * sizeof(sai_object_type_t));";

    WriteTest "    void *dummy = NULL;";

    for my $ot (@objects)
    {
        next if $ot =~ /^SAI_OBJECT_TYPE_(MAX|NULL)$/;

        $ot =~ /^SAI_OBJECT_TYPE_(\w+)$/;

        if (IsSpecialObject($ot))
        {
            # those obejcts are special, just attributes, no APIs
            WriteTest "    checked[(int)$ot] = $ot;";
            next;
        }

        my $short = lc($1);

        my $api = $main::OBJTOAPIMAP{$ot};

        WriteTest "    {";
        WriteTest "        sai_${api}_api_t ${api}_api;";

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
            WriteTest "        checked[(int)$ot] = $ot;";
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
            WriteTest "        checked[(int)$ot] = $ot;";
        }

        WriteTest "    }";
    }

    WriteTest "    int index = SAI_OBJECT_TYPE_NULL;";

    WriteTest "    for (; index < SAI_OBJECT_TYPE_MAX; ++index)";
    WriteTest "    {";
    WriteTest "        printf(\"checking: %s checked (%d) == index (%d)\\n\",";
    WriteTest "             sai_metadata_enum_sai_object_type_t.valuesnames[index],";
    WriteTest "             checked[index],(sai_object_type_t)index);";
    WriteTest "        TEST_ASSERT_TRUE(checked[index] == (sai_object_type_t)index, \"not all obejcts were processed\");";
    WriteTest "    }";

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
        WriteTest "    count = s_$struct.count;";
        WriteTest "    ptr   = s_$struct.list;";
        WriteTest "    printf(\"$struct %p %u\\n\", ptr, count);";
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
    WriteTest "#define PP(x) printf(\"%p\\n\", (x));";
    WriteTest "#define TEST_ASSERT_TRUE(x,msg) if (!(x)){ fprintf(stderr, \"ASSERT TRUE FAILED(%d): %s: %s\\n\", __LINE__, #x, msg); exit(1);}";
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

sub CreateTests
{
    WriteTestHeader();

    CreateNonObjectIdTest();

    CreateSwitchIdTest();

    CreateCustomRangeTest();

    CreatePointersTest();

    CreateEnumSizeCheckTest();

    CreateListCountTest();

    CreateApiNameTest();

    CreateStructListTest();

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

