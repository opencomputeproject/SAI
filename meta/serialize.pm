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
# @file    serialize.pm
#
# @brief   This module defines SAI Metadata Serialize Parser
#

package serialize;

use strict;
use warnings;
use diagnostics;
use Data::Dumper;
use utils;
use xmlutils;

require Exporter;

sub CreateSerializeForEnums
{
    WriteSectionComment "Enum serialize methods";

    for my $key (sort keys %main::SAI_ENUMS)
    {
        next if $key =~ /_attr_t$/;

        if (not $key =~ /^sai_(\w+)_t$/)
        {
            LogWarning "wrong enum name '$key'";
            next;
        }

        my $suffix = $1;

        WriteHeader "extern int sai_serialize_$suffix(";
        WriteHeader "_Out_ char *buffer,";
        WriteHeader "_In_ $key $suffix);\n";

        WriteSource "int sai_serialize_$suffix(";
        WriteSource "_Out_ char *buffer,";
        WriteSource "_In_ $key $suffix)";
        WriteSource "{";
        WriteSource "return sai_serialize_enum(buffer, &sai_metadata_enum_$key, $suffix);";
        WriteSource "}";
    }
}

#
# we are using sprintf here, but in all cases it's const string passed, so
# compiler will optimize this to strcpy, and no snprintf function will be
# actually called, actual functions called will be those written by user in
# saiserialize.c and optimization should focus on those functions
#
# TODO we need version with like snprintf to write only N characters since
# right how we don't know how long output will be, for long arrays it can be
# even kB
#
# we will treat notification params as struct members and they will be
# serialized as json object all consts printfs could be exchanged to memcpy for
# optimize but we assume number of notifications is small, and this is fast
# enough
#

sub CreateSerializeSingleStruct
{
    my $structName = shift;

    my %structInfoEx = ExtractStructInfoEx($structName, "struct_");

    ProcessMembersForSerialize(\%structInfoEx);
}

sub IsMetadataStruct
{
    #
    # check if structure is declared inside metadata
    #

    my $refStructInfoEx = shift;

    my $key = $refStructInfoEx->{keys}[0];

    return 1 if $refStructInfoEx->{membersHash}{$key}->{file} =~ m!(meta/sai\w+.h|^sai(meta\w+.h))$!;

    return 0;
}

# TODO for lists we need countOnly param, as separate version
# @param[in] only_count Flag specifies whether on *_list_t only
# list count should be serialized, this is handy when serializing
# attributes when API returned #SAI_STATUS_BUFFER_OVERFLOW.

# TODO on s32/s32_list in struct we could declare enum type

sub EmitSerializeFunctionHeader
{
    my $refStructInfoEx = shift;

    my %structInfoEx = %{ $refStructInfoEx };

    my $structName = $structInfoEx{name};
    my $structBase = $structInfoEx{baseName};
    my $membersHash = $structInfoEx{membersHash};

    my @keys = @{ $structInfoEx{keys} };

    WriteHeader "extern int sai_serialize_$structBase(";
    WriteHeader "_Out_ char *buf,";

    WriteSource "int sai_serialize_$structBase(";
    WriteSource "_Out_ char *buf,";

    if (defined $structInfoEx{union} and not defined $structInfoEx{extraparam})
    {
        LogError "union $structName, extraparam required";
        return;
    }

    if (defined $structInfoEx{ismethod})
    {
        #
        # we create serialize method as this funcion was method instead of
        # struct, this will be used to create serialize for notifications
        #

        for my $name (@keys)
        {
            my $type = $membersHash->{$name}{type};

            LogDebug "$structName $structBase $name $type";

            my $last = 1 if $keys[-1] eq $name;

            my $end = (defined $last) ? ")" : ",";
            my $endheader = (defined $last) ? ");" : ",";

            WriteSource "_In_ $type $name$end";
            WriteHeader "_In_ $type $name$endheader";
        }
    }
    else
    {
        if (defined $structInfoEx{extraparam})
        {
            my @params = @{ $structInfoEx{extraparam} };

            for my $param (@params)
            {
                WriteHeader "_In_ $param,";
                WriteSource "_In_ $param,";
            }
        }

        WriteHeader "_In_ const $structName *$structBase);\n";
        WriteSource "_In_ const $structName *$structBase)";
    }
}

sub GetTypeInfoForSerialize
{
    my ($refStructInfoEx, $name) = @_;

    my %structInfoEx = %{ $refStructInfoEx };

    my $structName = $structInfoEx{name};
    my $structBase = $structInfoEx{baseName};

    my $type = $structInfoEx{membersHash}->{$name}{type};

    my %TypeInfo = (
            name => $name,
            type => $type,
            noptrtype => $type,
            needQuote => 0,
            ispointer => 0,
            isattribute => 0,
            amp => "",
            deamp => "",
            objectType =>"UNKNOWN_OBJ_TYPE",
            castName => "");

    $type = $1 if $type =~ /^const\s+(.+)$/;

    if ($type =~ /^(sai_\w+_t)\[(\d+)\]$/)
    {
        $type = $1;
        $TypeInfo{constCount} = $2;
        $TypeInfo{ispointer} = 1;
        $TypeInfo{deamp} = "&";
        $TypeInfo{noptrtype} = $1;
    }

    if ($type =~ /(.*)\s*\*$/)
    {
        $type =~ s!\s*\*$!!;
        $TypeInfo{ispointer} = 1;
        $TypeInfo{noptrtype} = $type;
    }

    $TypeInfo{suffix} = ($type =~ /sai_(\w+)_t/) ? $1 : $type;

    if ($type =~ /^(bool|sai_size_t)$/)
    {
        # ok
        $TypeInfo{deamp} = "&";
    }
    elsif ($type eq "void")
    {
        # treat void* as uint8_t*

        $TypeInfo{suffix} = "uint8";
        $TypeInfo{castName} = "(const uint8_t*)";
    }
    elsif ($type =~ /^sai_ip6_t$/)
    {
        $TypeInfo{needQuote} = 1;
    }
    elsif ($type =~ /^sai_ip4_t$/)
    {
        $TypeInfo{needQuote} = 1;
        $TypeInfo{deamp} = "&";
    }
    elsif ($type =~ /^sai_(vlan_id)_t$/)
    {
        $TypeInfo{suffix} = "uint16";
        $TypeInfo{deamp} = "&";
    }
    elsif ($type =~ /^sai_(label_id)_t$/)
    {
        $TypeInfo{suffix} = "uint32";
        $TypeInfo{deamp} = "&";
    }
    elsif ($type =~ /^sai_(stat_id)_t$/)
    {
        $TypeInfo{suffix} = "uint32";
        $TypeInfo{deamp} = "&";
    }
    elsif ($type =~ /^sai_(cos|queue_index)_t$/)
    {
        $TypeInfo{suffix} = "uint8";
        $TypeInfo{deamp} = "&";
    }
    elsif ($type =~ /^(?:sai_)?(u?int\d+)_t$/)
    {
        # enums!
        $TypeInfo{suffix} = $1;
        $TypeInfo{deamp} = "&";
    }
    elsif ($type =~ m/^sai_(ip_address|ip_prefix)_t$/)
    {
        $TypeInfo{needQuote} = 1;
        $TypeInfo{amp} = "&";
        $TypeInfo{deamp} = "&";
    }

#
# NOTE: check for objects is not that simple here, since object_id or
# object_list can have multiple different object types allowed depending on
# attribute which is being serialized and it may be a hudge task to make this
# check here and also it would require to pass meta param to each serialized
# struct even if meta param is not required
#
#    elsif ($type =~ m/^sai_object_list_t$/)
#    {
#        $TypeInfo{amp} = "&";
#
#        if (not defined $structInfoEx{membersHash}->{$name}{objects})
#        {
#            LogError "param '$name' is '$type' on '$structName' and requires object type specified in \@objects";
#            return undef;
#        }
#    }
#    elsif ($type =~ m/^sai_object_id_t$/)
#    {
#        # TODO object_list check if object is defined
#        $TypeInfo{needQuote} = 1;
#
#        if (not defined $structInfoEx{membersHash}->{$name}{objects})
#        {
#            LogError "param '$name' is '$type' on '$structName' and requires object type specified in \@objects";
#            return undef;
#        }
#    }

    elsif ($type =~ m/^sai_mac_t$/)
    {
        $TypeInfo{needQuote} = 1;
    }
    elsif ($type =~ m/^sai_attr_id_t$/)
    {
        $TypeInfo{needQuote} = 1;
    }
    elsif ($type =~ m/^sai_object_id_t$/)
    {
        $TypeInfo{needQuote} = 1;
        $TypeInfo{deamp} = "&";
    }
    elsif ($type =~ /^sai_pointer_t$/)
    {
        # need quote since "ptr:" is added on serialize

        $TypeInfo{needQuote} = 1;
        $TypeInfo{deamp} = "&";
    }
    elsif ($type =~ /^sai_attribute_t$/)
    {
        $TypeInfo{amp} = "&";
        $TypeInfo{deamp} = "&";
        $TypeInfo{isattribute} = 1;

        if (not defined $structInfoEx{membersHash}->{$name}{objects})
        {
            LogError "param '$name' is '$type' on '$structName' and requires object type specified in \@objects";
            return undef;
        }

        my @ot = @{ $structInfoEx{membersHash}->{$name}{objects} };

        if (scalar@ot != 1)
        {
            LogWarning "expected only 1 obejct type, but given '@ot'";
            return undef;
        }

        $TypeInfo{objectType} = $ot[0];

        if (not defined $main::OBJECT_TYPE_MAP{$TypeInfo{objectType}})
        {
            LogError "unknown object type '$TypeInfo{objectType}' on $structName :: $name";
            return undef;
        }
    }
    elsif (defined $main::ALL_STRUCTS{$type} and $type =~ /^sai_(\w+)_t$/)
    {
        $TypeInfo{amp} = "&";
        $TypeInfo{deamp} = "&";

        # sai_s32_list_t enum !
    }
    elsif (defined $main::SAI_ENUMS{$type} and $type =~ /^sai_(\w+)_t$/)
    {
        $TypeInfo{needQuote} = 1;
        $TypeInfo{deamp} = "&";
    }
    elsif (defined $main::SAI_UNIONS{$type} and $type =~ /^sai_(\w+)_t$/)
    {
        $TypeInfo{union} = 1;
        $TypeInfo{amp} = "&";
        $TypeInfo{deamp} = "&";
    }
    elsif ($type eq "char[32]")
    {
        $TypeInfo{needQuote} = 1;
        $TypeInfo{suffix} = "chardata";
    }
    elsif (defined $main::PRIMITIVE_TYPES{$type} and $main::PRIMITIVE_TYPES{$type}{isarray} == 1)
    {
        $TypeInfo{needQuote} = 1;
    }
    else
    {
        LogError "serialization type not handled $type $name in $structName, FIXME";
        return undef;
    }

    my $memberName = (defined $structInfoEx{ismethod}) ? $name : "$structBase\->$name";

    $memberName = "($TypeInfo{castName}$memberName)" if $TypeInfo{castName} ne "";

    $TypeInfo{memberName} = $memberName;
    $TypeInfo{suffix} = $structInfoEx{membersHash}->{$name}{suffix} if defined $structInfoEx{membersHash}->{$name}{suffix};

    return \%TypeInfo;
}

sub GetCounterNameAndType
{
    my ($refStructInfoEx, $refTypeInfo) = @_;

    my $refMembersHash = $refStructInfoEx->{membersHash};

    my $name = $refTypeInfo->{name};

    my $structName = $refStructInfoEx->{name};
    my $structBase = $refStructInfoEx->{baseName};

    if (defined $refTypeInfo->{constCount})
    {
        my $countMemberName = $refTypeInfo->{constCount};
        my $countType = "uint32_t";

        return ($countMemberName, $countType);
    }

    my $count = $refMembersHash->{$name}{count};

    if (not defined $count)
    {
        LogError "count must be defined for pointer '$name' in $structName";
        return ("undef", "undef");
    }

    if (not defined $refMembersHash->{$count})
    {
        LogWarning "count '$count' not found in '$structName'";
        return ("undef", "undef");
    }

    my $countMemberName = $refMembersHash->{$count}{name};
    my $countType = $refMembersHash->{$count}{type};

    $countMemberName = (defined $refStructInfoEx->{ismethod}) ? $countMemberName: "$structBase\->$countMemberName";

    if (not $countType =~ /^(uint32_t|sai_size_t)$/)
    {
        LogWarning "count '$count' on '$structName' has invalid type '$countType', expected uint32_t";
        return ("undef", "undef");
    }

    if (not defined $refStructInfoEx->{processed}->{$count})
    {
        # check if count was declared before list, since deserialize
        # needs to know how many items is on list to not make realoc and monitor
        # number of elements

        LogError "count member '$count' on $structName is defined after '$name', not allowed";
    }

    return ($countMemberName, $countType);
}

sub EmitSerializeHeader
{
    WriteSource "{";
    WriteSource "char *begin_buf = buf;";
    WriteSource "int ret;\n";
    WriteSource "EMIT(\"{\");\n";
}

sub WriteSkipForMask
{
    # mask is special, since it's combined with field data, but there
    # is not field corresponded to object or object list so we can skip
    # them in serialize and deserialize

    WriteSource "else if (meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_ID)";
    WriteSource "{";
    WriteSource "/* skip */";
    WriteSource "}";
    WriteSource "else if (meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_LIST)";
    WriteSource "{";
    WriteSource "/* skip */";
    WriteSource "}";
}

sub EmitSerializeFooter
{
    my $refStructInfoEx = shift;

    if (defined $refStructInfoEx->{union})
    {
        my $name = $refStructInfoEx->{name};

        WriteSkipForMask() if $name eq "sai_acl_field_data_mask_t";

        # NOTE: if it's union, we must check if we serialized something
        # (not always true for acl mask)

        WriteSource "else";
        WriteSource "{";
        WriteSource "SAI_META_LOG_WARN(\"nothing was serialized for '$name', bad condition?\");";
        # WriteSource "return SAI_SERIALIZE_ERROR;";
        WriteSource "}\n";
    }

    WriteSource "EMIT(\"}\");\n";

    WriteSource "return (int)(buf - begin_buf);";

    WriteSource "}";
}

sub GetEmitMacroName
{
    my $refTypeInfo = shift;

    return "EMIT_QUOTE_CHECK" if $refTypeInfo->{needQuote};

    return "EMIT_CHECK";
}

sub GetPassParamsForSerialize
{
    my ($refStructInfoEx, $refTypeInfo) = @_;

    my $name = $refTypeInfo->{name};

    my $refMembersHash = $refStructInfoEx->{membersHash};

    return "" if not defined $refMembersHash->{$name}{passparam};

    my $structBase = $refStructInfoEx->{baseName};

    my @params = @{ $refMembersHash->{$name}{passparam} };

    my $passParams = "";

    for my $param (@params)
    {
        if (not $param =~ /->/ and defined $refMembersHash->{$param})
        {
            $passParams .= "$structBase\->$param, ";
        }
        else
        {
            $passParams .= "$param, ";
        }
    }

    return $passParams;
}

sub EmitSerializePrimitive
{
    my ($refStructInfoEx, $refTypeInfo) = @_;

    my $suffix = $refTypeInfo->{suffix};

    my $emitMacro = GetEmitMacroName($refTypeInfo);

    my $passParams = GetPassParamsForSerialize($refStructInfoEx, $refTypeInfo);

    my $serializeCall = "sai_serialize_$suffix(buf, $passParams$refTypeInfo->{amp}$refTypeInfo->{memberName})";

    WriteSource "$emitMacro($serializeCall, $suffix);";
}

sub GetEmitKeyMacroName
{
    my ($refStructInfoEx, $name) = @_;

    my $firstKey = $refStructInfoEx->{keys}->[0];

    return "EMIT_KEY" if ($firstKey eq $name) or defined $refStructInfoEx->{union};

    return "EMIT_NEXT_KEY";
}

sub EmitSerializeMemberKey
{
    my ($refStructInfoEx, $refTypeInfo) = @_;

    my $name = $refTypeInfo->{name};

    my $emitKeyMacro = GetEmitKeyMacroName($refStructInfoEx, $name);

    WriteSource "$emitKeyMacro(\"$name\");\n";
}

sub GetConditionForSerialize
{
    my ($refStructInfoEx, $refTypeInfo) = @_;

    my $name = $refTypeInfo->{name};

    my $structName = $refStructInfoEx->{name};
    my $structBase = $refStructInfoEx->{baseName};

    my $refMembersHash = $refStructInfoEx->{membersHash};

    my @conditions = @{ $refMembersHash->{ $refTypeInfo->{name} }->{validonly} };

    if (scalar @conditions != 1)
    {
        LogWarning "only 1 condition is supported for '$name' in '$structName', FIXME";
        return "";
    }

    # if condition value is struct member, we need to check
    # if it was processed previously, since it could be declared
    # after current member, and then deserialize will fail

    my $condition = shift @conditions;

    if (not $condition =~ /^(\w+|\w+->\w+|sai_metadata_\w+\(\w+\)) == (\w+)$/)
    {
        LogWarning "invalid condition '$condition' on '$name' in '$structName'";
        return "";
    }

    if (defined $refStructInfoEx->{baseName})
    {
        $condition = "(sai_int32_t)$condition" if $refStructInfoEx->{baseName} eq "object_key_entry";
    }

    if (defined $refMembersHash->{$1})
    {
        $condition = "$structBase->$1 == $2";

        if (not defined $refStructInfoEx->{processed}->{$1})
        {
            LogError "validonly condition '$1' on $structName is declared after '$name', not allowed";
            return "";
        }
    }

    return $condition;
}

sub EmitSerializeValidOnlyHeader
{
    my ($refStructInfoEx, $refTypeInfo) = @_;

    return if not defined $refStructInfoEx->{membersHash}{$refTypeInfo->{name}}->{validonly};

    my $condition = GetConditionForSerialize($refStructInfoEx, $refTypeInfo);

    my $first = $refStructInfoEx->{keys}->[0] eq $refTypeInfo->{name};

    my $else = (defined $refStructInfoEx->{union} and not $first) ? "else " : "";

    WriteSource "${else}if ($condition)";
    WriteSource "{"
}

sub EmitSerializeValidOnlyFooter
{
    my ($refStructInfoEx, $refTypeInfo) = @_;

    my $refMembersHash = $refStructInfoEx->{membersHash};

    my $footer = "";

    $footer = "}" if defined $refMembersHash->{ $refTypeInfo->{name} }->{validonly};

    WriteSource $footer;
}

sub EmitSerializeArray
{
    my ($refStructInfoEx, $refTypeInfo) = @_;

    my ($countMemberName, $countType) = GetCounterNameAndType($refStructInfoEx, $refTypeInfo);

    WriteSource "if ($refTypeInfo->{memberName} == NULL || $countMemberName == 0)";
    WriteSource "{";
    WriteSource "EMIT(\"null\");";
    WriteSource "}";
    WriteSource "else";
    WriteSource "{";
    WriteSource "EMIT(\"[\");\n";
    WriteSource "$countType idx;\n";
    WriteSource "for (idx = 0; idx < $countMemberName; idx++)";
    WriteSource "{";
    WriteSource "if (idx != 0)";
    WriteSource "{";
    WriteSource "EMIT(\",\");";
    WriteSource "}\n";

    my $passParams = GetPassParamsForSerialize($refStructInfoEx, $refTypeInfo);

    if ($refTypeInfo->{isattribute})
    {
        WriteSource "const sai_attr_metadata_t *meta =";
        WriteSource "    sai_metadata_get_attr_metadata($refTypeInfo->{objectType}, $refTypeInfo->{memberName}\[idx\].id);\n";

        $passParams = "meta, $passParams";
    }

    my $suffix = $refTypeInfo->{suffix};

    my $serializeCall = "sai_serialize_$suffix(buf, $passParams$refTypeInfo->{amp}$refTypeInfo->{memberName}\[idx\])";

    my $emitMacro = GetEmitMacroName($refTypeInfo);

    WriteSource "$emitMacro($serializeCall, $suffix);";

    WriteSource "}\n";
    WriteSource "EMIT(\"]\");";
    WriteSource "}";
}

sub IsTypeInfoValid
{
    my ($refStructInfoEx, $refTypeInfo) = @_;

    my $name = $refTypeInfo->{name};

    my $structName = $refStructInfoEx->{name};

    my $refMembersHash = $refStructInfoEx->{membersHash};

    my $validonly = $refMembersHash->{$name}{validonly};

    if ($refStructInfoEx->{keys}[0] eq $name and defined $validonly and
            not defined $refStructInfoEx->{union})
    {
        # since we don't know if put comma before next key
        LogError "first key '$name' on '$structName' can't be validonly";
        return undef;
    }

    if (defined $refStructInfoEx->{union} and not defined $validonly)
    {
        LogError "member '$name' in $structName require \@validonly tag";
        return undef;
    }

    if (defined $refTypeInfo->{union} and
            not defined $refMembersHash->{$name}{passparam} and
            not defined $refTypeInfo->{nestedunion})
    {
        LogError "member '$name' in $structName is union, \@passparam tag is required";
        return undef;
    }

    # TODO if member is struct with extraparam passparam is required

    if (defined $refTypeInfo->{nestedunion})
    {
        LogError "nested union not supported $structName $name";
        return undef;
    }

    return 1;
}

sub ProcessMembersForSerialize
{
    my $refStructInfoEx = shift;

    my %structInfoEx = %{ $refStructInfoEx };

    my $structName = $structInfoEx{name};
    my $structBase = $structInfoEx{baseName};

    # don't create serialize methods for metadata structs

    # TODO add tag "noserialize"

    return if defined $structInfoEx{ismetadatastruct} and $structName ne "sai_object_meta_key_t";

    LogDebug "Creating serialize for $structName";

    my %membersHash = %{ $structInfoEx{membersHash} };

    my @keys = @{ $structInfoEx{keys} };

    EmitSerializeFunctionHeader($refStructInfoEx);

    EmitSerializeHeader;

    my %processedMembers = ();

    $refStructInfoEx->{processed} = \%processedMembers;

    for my $name (@keys)
    {
        my $refTypeInfo = GetTypeInfoForSerialize($refStructInfoEx, $name);

        next if not defined $refTypeInfo;

        next if not IsTypeInfoValid($refStructInfoEx, $refTypeInfo);

        EmitSerializeValidOnlyHeader($refStructInfoEx, $refTypeInfo);

        EmitSerializeMemberKey($refStructInfoEx, $refTypeInfo);

        if ($refTypeInfo->{ispointer})
        {
            EmitSerializeArray($refStructInfoEx, $refTypeInfo);
        }
        else
        {
            EmitSerializePrimitive($refStructInfoEx, $refTypeInfo);
        }

        EmitSerializeValidOnlyFooter($refStructInfoEx, $refTypeInfo);

        $refStructInfoEx->{processed}{$name} = 1;
    }

    EmitSerializeFooter($refStructInfoEx);
}

sub CreateSerializeStructs
{
    WriteSectionComment "Serialize structs";

    for my $struct (sort keys %main::ALL_STRUCTS)
    {
        # user defined serialization

        next if $struct eq "sai_ip_address_t";
        next if $struct eq "sai_ip_prefix_t";
        next if $struct eq "sai_attribute_t";

        my %structInfoEx = ExtractStructInfoEx($struct, "struct_");

        next if defined $structInfoEx{containsfnpointer};

        ProcessMembersForSerialize(\%structInfoEx);
    }
}

sub CreateSerializeUnions
{
    WriteSectionComment "Serialize unions";

    for my $unionTypeName (sort keys %main::SAI_UNIONS)
    {
        my %unionInfoEx = ExtractStructInfoEx($unionTypeName, "union_");

        ProcessMembersForSerialize(\%unionInfoEx);
    }
}

sub CreateSerializeNotifications
{
    WriteSectionComment "Serialize notifications";

    for my $ntfName (sort keys %main::NOTIFICATIONS)
    {
        ProcessMembersForSerialize($main::NOTIFICATIONS{$ntfName});
    }
}

sub CreateSerializeEmitMacros
{
    WriteSectionComment "Emit macros";

    WriteSource "#define EMIT(x)        buf += sprintf(buf, x)";
    WriteSource "#define EMIT_QUOTE     EMIT(\"\\\"\")";
    WriteSource "#define EMIT_KEY(k)    EMIT(\"\\\"\" k \"\\\":\")";
    WriteSource "#define EMIT_NEXT_KEY(k) { EMIT(\",\"); EMIT_KEY(k); }";
    WriteSource "#define EMIT_CHECK(expr, suffix) {                                 \\";
    WriteSource "    ret = (expr);                                                  \\";
    WriteSource "    if (ret < 0) {                                                 \\";
    WriteSource "        SAI_META_LOG_WARN(\"failed to serialize \" #suffix \"\");      \\";
    WriteSource "        return SAI_SERIALIZE_ERROR; }                              \\";
    WriteSource "    buf += ret; }";
    WriteSource "#define EMIT_QUOTE_CHECK(expr, suffix) {\\";
    WriteSource "    EMIT_QUOTE; EMIT_CHECK(expr, suffix); EMIT_QUOTE; }";
}

#
# DESERIALIZE - Move to separate file, or join with serialize functions
#

sub CreateDeserializeEmitMacros
{
    WriteSectionComment "Expect macros";

    # TODO each expect should free memory current object recursivly on exit
    # since we know that memory was allocated with zero (except that user one
    # provided then we need to release memory, actually only in places where we
    # have count and not const count

    WriteSource "#define EXPECT(x) { \\";
    WriteSource "    if (strncmp(buf, x, sizeof(x) - 1) == 0) { buf += sizeof(x) - 1; } \\";
    WriteSource "    else { \\";
    WriteSource "        SAI_META_LOG_WARN(\"expected '%s' but got '%.*s...'\", x, (int)sizeof(x), buf); \\";
    WriteSource "        return SAI_SERIALIZE_ERROR; } }";
    WriteSource "#define EXPECT_QUOTE     EXPECT(\"\\\"\")";
    WriteSource "#define EXPECT_KEY(k)    EXPECT(\"\\\"\" k \"\\\":\")";
    WriteSource "#define EXPECT_NEXT_KEY(k) { EXPECT(\",\"); EXPECT_KEY(k); }";
    WriteSource "#define EXPECT_CHECK(expr, suffix) {                                 \\";
    WriteSource "    ret = (expr);                                                  \\";
    WriteSource "    if (ret < 0) {                                                 \\";
    WriteSource "        SAI_META_LOG_WARN(\"failed to deserialize \" #suffix \"\");      \\";
    WriteSource "        return SAI_SERIALIZE_ERROR; }                              \\";
    WriteSource "    buf += ret; }";
    WriteSource "#define EXPECT_QUOTE_CHECK(expr, suffix) {\\";
    WriteSource "    EXPECT_QUOTE; EXPECT_CHECK(expr, suffix); EXPECT_QUOTE; }";
}

sub EmitDeserializeFunctionHeader
{
    my $refStructInfoEx = shift;

    my %structInfoEx = %{ $refStructInfoEx };

    my $structName = $structInfoEx{name};
    my $structBase = $structInfoEx{baseName};
    my $membersHash = $structInfoEx{membersHash};

    my @keys = @{ $structInfoEx{keys} };

    WriteHeader "extern int sai_deserialize_$structBase(";
    WriteHeader "_In_ const char *buf,";

    WriteSource "int sai_deserialize_$structBase(";
    WriteSource "_In_ const char *buf,";

    if (defined $structInfoEx{union} and not defined $structInfoEx{extraparam})
    {
        LogError "union $structName, extraparam required";
        return;
    }

    # TODO revisit params

    if (defined $structInfoEx{ismethod})
    {
        #
        # we create serialize method as this funcion was method instead of
        # struct, this will be used to create serialize for notifications
        #

        for my $name (@keys)
        {
            my $type = $membersHash->{$name}{type};

            LogDebug "$structName $structBase $name $type";

            my $last = 1 if $keys[-1] eq $name;

            my $end = (defined $last) ? ")" : ",";
            my $endheader = (defined $last) ? ");" : ",";

            # TODO pointers

            WriteSource "_Out_ $type $name$end";
            WriteHeader "_Out_ $type $name$endheader";
        }
    }
    else
    {
        if (defined $structInfoEx{extraparam})
        {
            my @params = @{ $structInfoEx{extraparam} };

            for my $param (@params)
            {
                WriteHeader "_In_ $param,";
                WriteSource "_In_ $param,";
            }
        }

        WriteHeader "_Out_ $structName *$structBase);\n";
        WriteSource "_Out_ $structName *$structBase)";
    }
}

sub EmitDeserializeHeader
{
    WriteSource "{";
    WriteSource "const char *begin_buf = buf;";
    WriteSource "int ret;\n";
    WriteSource "EXPECT(\"{\");\n";
}

sub EmitDeserializeValidOnlyHeader
{
    my ($refStructInfoEx, $refTypeInfo) = @_;

    return if not defined $refStructInfoEx->{membersHash}{$refTypeInfo->{name}}->{validonly};

    my $condition = GetConditionForSerialize($refStructInfoEx, $refTypeInfo);

    my $first = $refStructInfoEx->{keys}->[0] eq $refTypeInfo->{name};

    my $else = (defined $refStructInfoEx->{union} and not $first) ? "else " : "";

    WriteSource "${else}if ($condition)";
    WriteSource "{"
}

sub EmitDeserializeValidOnlyFooter
{
    my ($refStructInfoEx, $refTypeInfo) = @_;

    my $refMembersHash = $refStructInfoEx->{membersHash};

    my $footer = "";

    $footer = "}" if defined $refMembersHash->{ $refTypeInfo->{name} }->{validonly};

    WriteSource $footer;
}

sub GetExpectKeyMacroName
{
    my ($refStructInfoEx, $name) = @_;

    my $firstKey = $refStructInfoEx->{keys}->[0];

    return "EXPECT_KEY" if ($firstKey eq $name) or defined $refStructInfoEx->{union};

    return "EXPECT_NEXT_KEY";
}

sub EmitDeserializeMemberKey
{
    my ($refStructInfoEx, $refTypeInfo) = @_;

    my $name = $refTypeInfo->{name};

    my $expectKeyMacro = GetExpectKeyMacroName($refStructInfoEx, $name);

    WriteSource "$expectKeyMacro(\"$name\");\n";
}

sub GetExpectMacroName
{
    my $refTypeInfo = shift;

    return "EXPECT_QUOTE_CHECK" if $refTypeInfo->{needQuote};

    return "EXPECT_CHECK";
}

sub GetPassParamsForDeserialize
{
    my ($refStructInfoEx, $refTypeInfo) = @_;

    my $name = $refTypeInfo->{name};

    my $refMembersHash = $refStructInfoEx->{membersHash};

    return "" if not defined $refMembersHash->{$name}{passparam};

    my $structBase = $refStructInfoEx->{baseName};

    my @params = @{ $refMembersHash->{$name}{passparam} };

    my $passParams = "";

    for my $param (@params)
    {
        if (not $param =~ /->/ and defined $refMembersHash->{$param})
        {
            $passParams .= "$structBase\->$param, ";
        }
        else
        {
            $passParams .= "$param, ";
        }
    }

    return $passParams;
}

sub EmitDeserializePrimitive
{
    my ($refStructInfoEx, $refTypeInfo) = @_;

    my $suffix = $refTypeInfo->{suffix};

    my $emitMacro = GetExpectMacroName($refTypeInfo);

    my $passParams = GetPassParamsForDeserialize($refStructInfoEx, $refTypeInfo);

    my $amp = $refTypeInfo->{deamp};

    my $serializeCall = "sai_deserialize_$suffix(buf, $passParams$amp$refTypeInfo->{memberName})";

    WriteSource "$emitMacro($serializeCall, $suffix);";
}

sub EmitDeserializeFooter
{
    my $refStructInfoEx = shift;

    if (defined $refStructInfoEx->{union})
    {
        my $name = $refStructInfoEx->{name};

        WriteSkipForMask() if $name eq "sai_acl_field_data_mask_t";

        # if it's union, we must check if we serialized something
        # (not always true for acl mask)

        WriteSource "else";
        WriteSource "{";
        WriteSource "SAI_META_LOG_WARN(\"nothing was deserialized for '$name', bad condition?\");";
        # WriteSource "return SAI_SERIALIZE_ERROR;";
        WriteSource "}\n";
    }

    WriteSource "EXPECT(\"}\");\n";

    WriteSource "return (int)(buf - begin_buf);";

    WriteSource "}";
}

sub EmitDeserializeArray
{
    my ($refStructInfoEx, $refTypeInfo) = @_;

    my ($countMemberName, $countType) = GetCounterNameAndType($refStructInfoEx, $refTypeInfo);

    if (not $countMemberName =~ /^$NUMBER_REGEX$/)
    {
        WriteSource "if (strncmp(buf, \"null\", 4) == 0)";
        WriteSource "{";
        WriteSource "$refTypeInfo->{memberName} = NULL;\n";
        WriteSource "buf += 4;";

        WriteSource "}";
        WriteSource "else";
    }

    WriteSource "{";

    #
    # if count is const, we dont need to allocate memory since is a buffer like
    # char[32] or something similar
    #

    if (not $countMemberName =~ /^$NUMBER_REGEX$/)
    {
        WriteSource "$refTypeInfo->{memberName} = calloc(($countMemberName), sizeof($refTypeInfo->{noptrtype}));\n";
    }

    WriteSource "EXPECT(\"[\");\n";
    WriteSource "$countType idx;\n";
    WriteSource "for (idx = 0; idx < $countMemberName; idx++)";
    WriteSource "{";
    WriteSource "if (idx != 0)";
    WriteSource "{";
    WriteSource "EXPECT(\",\");";
    WriteSource "}\n";

    my $passParams = GetPassParamsForSerialize($refStructInfoEx, $refTypeInfo);

    if ($refTypeInfo->{isattribute})
    {
        #
        # NOTE: deserialize attribute not require metadata since we have user
        # provided deserialize, and serialized attr id points to exact metadata
        #
        # WriteSource "const sai_attr_metadata_t *meta =";
        # WriteSource "    sai_metadata_get_attr_metadata($refTypeInfo->{objectType}, $refTypeInfo->{memberName}\[idx\].id);\n";
        # $passParams = "meta, $passParams";
    }

    my $amp = $refTypeInfo->{deamp};

    my $suffix = $refTypeInfo->{suffix};

    my $serializeCall = "sai_deserialize_$suffix(buf, $passParams$amp$refTypeInfo->{memberName}\[idx\])";

    my $emitMacro = GetExpectMacroName($refTypeInfo);

    WriteSource "$emitMacro($serializeCall, $suffix);";

    WriteSource "}\n";
    WriteSource "EXPECT(\"]\");";
    WriteSource "}";
}

# TODO in case of failure we need to recursivly free memory that we allocated
# to prevent memory leak

sub ProcessMembersForDeserialize
{
    my $refStructInfoEx = shift;

    my %structInfoEx = %{ $refStructInfoEx };

    my $structName = $structInfoEx{name};
    my $structBase = $structInfoEx{baseName};

    # don't create serialize methods for metadata structs

    # TODO add tag "noserialize"

    return if defined $structInfoEx{ismetadatastruct} and $structName ne "sai_object_meta_key_t";

    LogDebug "Creating deserialize for $structName";

    my %membersHash = %{ $structInfoEx{membersHash} };

    my @keys = @{ $structInfoEx{keys} };

    EmitDeserializeFunctionHeader($refStructInfoEx);

    EmitDeserializeHeader();

    my %processedMembers = ();

    $refStructInfoEx->{processed} = \%processedMembers;

    for my $name (@keys)
    {
        my $refTypeInfo = GetTypeInfoForSerialize($refStructInfoEx, $name);

        next if not defined $refTypeInfo;

        next if not IsTypeInfoValid($refStructInfoEx, $refTypeInfo);

        EmitDeserializeValidOnlyHeader($refStructInfoEx, $refTypeInfo);

        EmitDeserializeMemberKey($refStructInfoEx, $refTypeInfo);

        if ($refTypeInfo->{ispointer})
        {
            EmitDeserializeArray($refStructInfoEx, $refTypeInfo);
        }
        else
        {
            EmitDeserializePrimitive($refStructInfoEx, $refTypeInfo);
        }

        EmitDeserializeValidOnlyFooter($refStructInfoEx, $refTypeInfo);

        $refStructInfoEx->{processed}{$name} = 1;
    }

    EmitDeserializeFooter($refStructInfoEx);
}

sub CreateDeserializeStructs
{
    WriteSectionComment "Deserialize structs";

    for my $struct (sort keys %main::ALL_STRUCTS)
    {
        # user defined deserialization

        next if $struct eq "sai_ip_address_t";
        next if $struct eq "sai_ip_prefix_t";
        next if $struct eq "sai_attribute_t";

        my %structInfoEx = ExtractStructInfoEx($struct, "struct_");

        next if defined $structInfoEx{containsfnpointer};

        ProcessMembersForDeserialize(\%structInfoEx);
    }
}

sub CreateDeserializeForEnums
{
    WriteSectionComment "Enum deserialize methods";

    for my $key (sort keys %main::SAI_ENUMS)
    {
        next if $key =~ /_attr_t$/;

        if (not $key =~ /^sai_(\w+)_t$/)
        {
            LogWarning "wrong enum name '$key'";
            next;
        }

        my $suffix = $1;

        WriteHeader "extern int sai_deserialize_$suffix(";
        WriteHeader "_In_ const char *buffer,";
        WriteHeader "_Out_ $key *$suffix);\n";

        WriteSource "int sai_deserialize_$suffix(";
        WriteSource "_In_ const char *buffer,";
        WriteSource "_Out_ $key *$suffix)";
        WriteSource "{";
        WriteSource "return sai_deserialize_enum(buffer, &sai_metadata_enum_$key, (int*)$suffix);";
        WriteSource "}";
    }
}

sub CreateDeserializeUnions
{
    WriteSectionComment "Deserialize unions";

    for my $unionTypeName (sort keys %main::SAI_UNIONS)
    {
        my %unionInfoEx = ExtractStructInfoEx($unionTypeName, "union_");

        ProcessMembersForDeserialize(\%unionInfoEx);
    }
}

sub CreateSerializeMethods
{
    CreateSerializeForEnums();

    CreateSerializeEmitMacros();

    CreateSerializeStructs();

    CreateSerializeNotifications();

    CreateSerializeUnions();

    CreateDeserializeForEnums();

    CreateDeserializeEmitMacros();

    CreateDeserializeStructs();

    CreateDeserializeUnions();

    # TODO deserialize notifications
}

BEGIN
{
    our @ISA    = qw(Exporter);
    our @EXPORT = qw/
    CreateSerializeMethods
    /;
}

1;

# TODO we could also generate deserialize and call notifation where
# notification struct would be passed and notification would be called and then
# free itself (but on exception there will be memory leak)
#
# TODO generate notifications metadata, if param is object id then objects must
# be specified for validation wheter notification returned valid object, and
# each struct that is using object id should have @objects on object id
# members, then we should generate all struct infos to get all functions for
# oid extraction etc
#
# TODO validate if "validonly" param is not validonly struct/union and in union
# validonly must be param not any union member
#
# TODO support enum_list 32 - on unions and acl capability list force to add
# @type wheter enum or int32 / int32_list for those types
#
# TODO generate deserialize
#
# TODO generate validate - object types and enums, also objecttype in union
# passed from params must be forced to add, in sai_list32_oid - any add special
# case
#
# TODO generate transfer methods
#
# TODO since we need object type for validation on notification params, then we
# need notifications metadata, is object_id, allowed object types, is
# attribute, is pointer, etc
#
# TODO generate serialize and deserialize versions with only count
#
# TODO force unions to serialize something + add exception on serialize for
# mask @flags serialize:allowempty
#
# TODO validate if count is not pointer when serializing counts
#
# TODO we could generate serialize methods for all api functions since we have
# metadata now and final step would be to generate RPC client/server for SAI
