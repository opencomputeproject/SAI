#!/usr/bin/perl

package serialize;

use strict;
use warnings;
use diagnostics;
use Getopt::Std;
use Data::Dumper;
use utils;
use xmlutils;

require Exporter;

sub CreateSerializeForEnums
{
    WriteSectionComment "Enum serialize methods";

    for my $key (sort keys %main::SAI_ENUMS)
    {
        next if $key =~/_attr_t$/;

        if (not $key =~ /^sai_(\w+)_t$/)
        {
            LogWarning "wrong enum name '$key'";
            next;
        }

        my $inner = $1;

        WriteHeader "extern int sai_serialize_$inner(";
        WriteHeader "        _Out_ char *buffer,";
        WriteHeader "        _In_ $key $inner);";

        WriteSource "int sai_serialize_$inner(";
        WriteSource "        _Out_ char *buffer,";
        WriteSource "        _In_ $key $inner)";
        WriteSource "{";
        WriteSource "    return sai_serialize_enum(buffer, &sai_metadata_enum_$key, $inner);";
        WriteSource "}";
    }
}

# XXX this serialize is wrong, it should go like normal struct since
# its combination for easier readibility, we need 2 versions of this ?

sub CreateSerializeMetaKey
{
    WriteSectionComment "Serialize meta key";

    WriteHeader "extern int sai_serialize_object_meta_key(";
    WriteHeader "        _Out_ char *buffer,";
    WriteHeader "        _In_ const sai_object_meta_key_t *meta_key);";

    WriteSource "int sai_serialize_object_meta_key(";
    WriteSource "        _Out_ char *buffer,";
    WriteSource "        _In_ const sai_object_meta_key_t *meta_key)";
    WriteSource "{";

    WriteSource "    if (!sai_metadata_is_object_type_valid(meta_key->objecttype))";
    WriteSource "    {";
    WriteSource "        SAI_META_LOG_WARN(\"invalid object type (%d) in meta key\", meta_key->objecttype);";
    WriteSource "        return SAI_SERIALIZE_ERROR;";
    WriteSource "    }";

    WriteSource "    buffer += sai_serialize_object_type(buffer, meta_key->objecttype);";

    WriteSource "    *buffer++ = ':';";

    WriteSource "    switch (meta_key->objecttype)";
    WriteSource "    {";

    my @rawnames = GetNonObjectIdStructNames();

    for my $rawname (@rawnames)
    {
        my $OT = uc ("SAI_OBJECT_TYPE_$rawname");

        WriteSource "        case $OT:";
        WriteSource "            return sai_serialize_$rawname(buffer, &meta_key->objectkey.key.$rawname);";
    }

    WriteSource "        default:";
    WriteSource "            return sai_serialize_object_id(buffer, meta_key->objectkey.key.object_id);";

    WriteSource "    }";

    WriteSource "}";
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

sub CreateSerializeSingleNotification
{
    my $refNtf = shift;

    my %ntfInfo = %{ $refNtf };

    my $ntfName  = $ntfInfo{name};
    my $baseName = $ntfInfo{baseName};

    LogInfo "Creating serialzie for $ntfName";

    my $buf = "buf"; # can be changed if there will be name conflict

    WriteSource "int sai_serialize_$baseName(";
    WriteSource "        _Inout_ char *$buf,";

    WriteHeader "extern int sai_serialize_$baseName(";
    WriteHeader "        _Inout_ char *$buf,";

    my @params = @{ $ntfInfo{members} };

    for my $param (@params)
    {
        my $next = 1 if defined $params[$param->{idx} + 1];

        my $end = ($next) ? "," : ")";
        my $endheader = ($next) ? "," : ");";

        my $type = $param->{type};
        my $name = $param->{name};

        WriteSource "        _In_ $type $name$end";
        WriteHeader "        _In_ $type $name$endheader";
    }

    WriteSource "{";
    WriteSource "    char *start_$buf = $buf;";
    WriteSource "    int ret;";
    WriteSource "    $buf += sprintf($buf, \"{\");";

    my $quot = "$buf += sprintf($buf, \"\\\"\")";

    my %membersHash = %{ $ntfInfo{membersHash} };

    my @keys = @{ $ntfInfo{keys} };

    my $idx = 0;

    for my $name (@keys)
    {
        my $type = $membersHash{$name}{type};

        my $comma = ($idx != 0) ? "," : "";

        WriteSource "    $buf += sprintf($buf, \"$comma\\\"$name\\\":\");";

        my $suffix = $type;

        my $needQuote = 0;
        my $ispointer = 0;
        my $isattribute = 0;
        my $amp = "";
        my $objectType = "UNKNOWN_OBJ_TYPE";
        my $castName = $name;

        if ($type =~ /\s*\*$/)
        {
            $type =~ s!\s*\*$!!;
            $ispointer = 1;
        }

        $type = $1 if $type =~ /^const\s+(.+)$/;

        # TODO all this quote/amp, suffix could be defined on respected members
        # as metadata and we could automatically get that, and keep track in
        # deserialize and free methods by free instead of listing all of them here

        if ($type eq "bool")
        {
        }
        elsif ($type eq "sai_size_t")
        {
            $suffix = "size";
        }
        elsif ($type eq "void")
        {
            # treat void* as uint8_t*

            $suffix = "u8";
            $castName = "((const uint8_t*)$name)";
        }
        elsif ($type =~ /^sai_ip6_t$/)
        {
            $needQuote = 1;
            $suffix = "ipv6";
        }
        elsif ($type =~ /^sai_(vlan_id)_t$/)
        {
            $suffix = "u16";
        }
        elsif ($type =~ /^sai_(cos|queue_index)_t$/)
        {
            $suffix = "u8";
        }
        elsif ($type =~ /^(?:sai_)?(u?int\d+)_t$/)
        {
            $suffix = $1;
            $suffix =~ s/uint/u/;
            $suffix =~ s/int/s/;

            # enums!
        }
        elsif ($type =~ m/^sai_(ip_address|ip_prefix)_t$/)
        {
            $suffix = $1;
            $needQuote = 1;
            $amp = "&";
        }
        elsif ($type =~ m/^sai_(object_id|mac)_t$/)
        {
            $suffix = $1;
            $needQuote = 1;
        }
        elsif ($type =~ /^sai_(attribute)_t$/)
        {
            $suffix = $1;
            $amp = "&";
            $isattribute = 1;

            if (not defined $membersHash{$name}{objecttype})
            {
                LogError "param '$name' is '$type' on '$ntfName' and requires object type specified in \@objects";
                next;
            }

            $objectType = $membersHash{$name}{objecttype};

            if (not defined $main::OBJECT_TYPE_MAP{$objectType})
            {
                LogError "unknown object type '$objectType' on $ntfName :: $name";
                next;
            }
        }
        elsif (defined $main::ALL_STRUCTS{$type} and $type =~ /^sai_(\w+)_t$/)
        {
            $suffix = $1;
            $amp = "&";

            # sai_s32_list_t enum !
        }
        elsif (defined $main::SAI_ENUMS{$type} and $type =~ /^sai_(\w+)_t$/)
        {
            $suffix = $1;
            $needQuote = 1;
        }
        else
        {
            LogError "not handled '$type' '$name' in $ntfName, FIXME";
            next;
        }

        $idx++;

        if (not $ispointer)
        {
            # XXX we don't need to check for many types which won't fail like int/uint, object id, enums

            WriteSource "    $quot;" if $needQuote;
            WriteSource "    ret = sai_serialize_$suffix($buf, $amp$name);";
            WriteSource "    if (ret < 0)";
            WriteSource "    {";
            WriteSource "        SAI_META_LOG_WARN(\"failed to serialize '$name'\");";
            WriteSource "        return SAI_SERIALIZE_ERROR;";
            WriteSource "    }";
            WriteSource "    $buf += ret;";
            WriteSource "    $quot;" if $needQuote;
            next;
        }

        my $countName = "count";
        my $countType = "uint32_t";

        # for _list_t we can deduce type, since we force lists to have only 2 elements
        if (not $ntfName =~ /^sai_(\w+)_list_t$/)
        {
            my $count = $membersHash{$name}{count};

            if (not defined $count)
            {
                LogError "count must be defined for pointer '$name' in $ntfName";
                next;
            }

            if (not defined $membersHash{$count})
            {
                LogWarning "count '$count' not found in '$ntfName'";
                next;
            }

            # this is pointer

            $countName = $membersHash{$count}{name};
            $countType = $membersHash{$count}{type};

            if (not $countType =~ /^(uint32_t|sai_size_t)$/)
            {
                LogWarning "count '$count' on '$ntfName' has invalid type '$countType', expected uint32_t";
                next;
            }
        }

        WriteSource "    if ($name == NULL)";
        WriteSource "    {";
        WriteSource "        $buf += sprintf($buf, \"null\");";
        WriteSource "    }";
        WriteSource "    else";
        WriteSource "    {";
        WriteSource "        $buf += sprintf($buf, \"[\");"; # begin of array
        WriteSource "        $countType idx;";
        WriteSource "        for (idx = 0; idx < $countName; idx++)";
        WriteSource "        {";
        WriteSource "            $quot;" if $needQuote;

        if ($isattribute)
        {
            WriteSource "            const sai_attr_metadata_t *meta =";
            WriteSource "                       sai_metadata_get_attr_metadata($objectType, $name\[idx\].id);";
            WriteSource "            ret = sai_serialize_$suffix($buf, meta, $amp$name\[idx\]);";
        }
        else
        {
            WriteSource "            ret = sai_serialize_$suffix($buf, $amp$castName\[idx\]);";
        }

        WriteSource "            if (ret < 0)";
        WriteSource "            {";
        WriteSource "                SAI_META_LOG_WARN(\"failed to serialize '$name' at index %u\", (uint32_t)idx);";
        WriteSource "                return SAI_SERIALIZE_ERROR;";
        WriteSource "            }";
        WriteSource "            $buf += ret;";
        WriteSource "            $quot;" if $needQuote;
        WriteSource "            if (idx != $countName - 1)";
        WriteSource "               $buf += sprintf($buf, \",\");";
        WriteSource "        }";
        WriteSource "        $buf += sprintf($buf, \"]\");"; # end of array
        WriteSource "    }";
    }

    WriteSource "    $buf += sprintf($buf, \"}\");"; # end of struct
    WriteSource "    return (int)($buf - start_$buf);";
    WriteSource "}";
}

sub CreateSerializeNotifications
{
    WriteSectionComment "Serialize notifications";

    for my $ntfName (sort keys %main::NOTIFICATIONS)
    {
        CreateSerializeSingleNotification($main::NOTIFICATIONS{$ntfName});
    }
}

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

    return 1 if $refStructInfoEx->{membersHash}{$key}->{file} =~ m!meta/sai\w+.h$!;

    return 0;
}

sub ProcessMembersForSerialize
{
    my $refHashStructInfoEx = shift;

    my %structInfoEx = %{ $refHashStructInfoEx };

    my $structName = $structInfoEx{name};

    my $structBase = $structInfoEx{baseName};

    # don't create serialize methods for metadata structs

    return if IsMetadataStruct(\%structInfoEx);

    LogInfo "Creating serialzie for $structName";

    my $buf = "buf"; # can be changed if there will be name conflict

    # TODO also at this place we need to figure out whether we need
    # to pass more params or not also if any member serialize
    # method requires params

    WriteHeader "int sai_serialize_${structBase}(";
    WriteHeader "        _Out_ char *$buf,";
    WriteHeader "        _In_ const $structName *$structBase);";

    WriteSource "int sai_serialize_${structBase}(";
    WriteSource "        _Out_ char *$buf,";
    WriteSource "        _In_ const $structName *$structBase)";

    WriteSource "{";
    WriteSource "    char *start_$buf = $buf;";
    WriteSource "    int ret;";
    WriteSource "    $buf += sprintf($buf, \"{\");";

    my $quot = "$buf += sprintf($buf, \"\\\"\")";

    # TODO for lists we need countOnly param, as separate version?
    # * @param[in] only_count Flag specifies whether on *_list_t only
    # * list count should be serialized, this is handy when serializing
    # * attributes when API returned #SAI_STATUS_BUFFER_OVERFLOW.

    # TODO on s32/s32_list in struct we could declare enum type

    my %membersHash = %{ $structInfoEx{membersHash} };

    my @keys = @{ $structInfoEx{keys} };

    my $idx = 0;

    for my $name (@keys)
    {
        my $type = $membersHash{$name}{type};

        my $comma = ($idx != 0) ? "," : "";

        WriteSource "    $buf += sprintf($buf, \"$comma\\\"$name\\\":\");";

        my $suffix = $type;

        my $needQuote = 0;
        my $ispointer = 0;
        my $isattribute = 0;
        my $amp = "";
        my $objectType = "UNKNOWN_OBJ_TYPE";

        if ($type =~ /\s*\*$/)
        {
            $type =~ s!\s*\*$!!;
            $ispointer = 1;
        }

        # TODO all this quote/amp, suffix could be defined on respected members
        # as metadata and we could automatically get that, and keep track in
        # deserialize and free methods by free instead of listing all of them here

        if ($type eq "bool")
        {
        }
        elsif ($type =~ /^sai_ip6_t$/)
        {
            $needQuote = 1;
            $suffix = "ipv6";
        }
        elsif ($type =~ /^sai_(vlan_id)_t$/)
        {
            $suffix = "u16";
        }
        elsif ($type =~ /^sai_(cos|queue_index)_t$/)
        {
            $suffix = "u8";
        }
        elsif ($type =~ /^(?:sai_)?(u?int\d+)_t$/)
        {
            $suffix = $1;
            $suffix =~ s/uint/u/;
            $suffix =~ s/int/s/;

            # enums!
        }
        elsif ($type =~ m/^sai_(ip_address|ip_prefix)_t$/)
        {
            $suffix = $1;
            $needQuote = 1;
            $amp = "&";
        }
        elsif ($type =~ m/^sai_(object_id|mac)_t$/)
        {
            $suffix = $1;
            $needQuote = 1;
        }
        elsif ($type =~ /^sai_(attribute)_t$/)
        {
            $suffix = $1;
            $amp = "&";
            $isattribute = 1;

            if (not defined $membersHash{$name}{objects})
            {
                LogError "param '$name' is '$type' on '$structName' and requires object type specified in \@objects";
                next;
            }

            my @ot = @{ $membersHash{$name}{objects} };

            if (scalar@ot != 1)
            {
                LogWarning "expected only 1 obejct type, but given '@ot'";
                next;
            }

            $objectType = $ot[0];

            if (not defined $main::OBJECT_TYPE_MAP{$objectType})
            {
                LogError "unknown object type '$objectType' on $structName :: $name";
                next;
            }
        }
        elsif (defined $main::ALL_STRUCTS{$type} and $type =~ /^sai_(\w+)_t$/)
        {
            $suffix = $1;
            $amp = "&";

            # sai_s32_list_t enum !
        }
        elsif (defined $main::SAI_ENUMS{$type} and $type =~ /^sai_(\w+)_t$/)
        {
            $suffix = $1;
            $needQuote = 1;
        }
        else
        {
            LogError "not handled $type $name in $structName, FIXME";
            next;
        }

        $idx++;

        if (not $ispointer)
        {
            # XXX we don't need to check for many types which won't fail like int/uint, object id, enums

            WriteSource "    $quot;" if $needQuote;
            WriteSource "    ret = sai_serialize_$suffix($buf, $amp$structBase->$name);";
            WriteSource "    if (ret < 0)";
            WriteSource "    {";
            WriteSource "        SAI_META_LOG_WARN(\"failed to serialize '$name'\");";
            WriteSource "        return SAI_SERIALIZE_ERROR;";
            WriteSource "    }";
            WriteSource "    $buf += ret;";
            WriteSource "    $quot;" if $needQuote;
            next;
        }

        my $countName = "count";
        my $countType = "uint32_t";

        # for _list_t we can deduce type, since we force lists to have only 2 elements
        if (not $structName =~ /^sai_(\w+)_list_t$/)
        {
            my $count = $membersHash{$name}{count};

            if (not defined $count)
            {
                LogError "count must be defined for pointer '$name' in $structName";
                next;
            }

            if (not defined $membersHash{$count})
            {
                LogWarning "count '$count' not found in '$structName'";
                next;
            }

            # this is pointer

            $countName = $membersHash{$count}{name};
            $countType = $membersHash{$count}{type};

            if (not $countType =~ /^uint32_t$/)
            {
                LogWarning "count '$count' on '$structName' has invalid type '$countType', expected uint32_t";
                next;
            }
        }

        WriteSource "    if ($structBase->$name == NULL)";
        WriteSource "    {";
        WriteSource "        $buf += sprintf($buf, \"null\");";
        WriteSource "    }";
        WriteSource "    else";
        WriteSource "    {";
        WriteSource "        $buf += sprintf($buf, \"[\");"; # begin of array
        WriteSource "        $countType idx;";
        WriteSource "        for (idx = 0; idx < $structBase->$countName; idx++)";
        WriteSource "        {";
        WriteSource "            $quot;" if $needQuote;

        if ($isattribute)
        {
            WriteSource "            const sai_attr_metadata_t *meta =";
            WriteSource "                       sai_metadata_get_attr_metadata($objectType, $structBase->$name\[idx\].id);";
            WriteSource "            ret = sai_serialize_$suffix($buf, meta, $amp$structBase->$name\[idx\]);";
        }
        else
        {
            WriteSource "            ret = sai_serialize_$suffix($buf, $amp$structBase->$name\[idx\]);";
        }

        WriteSource "            if (ret < 0)";
        WriteSource "            {";
        WriteSource "                SAI_META_LOG_WARN(\"failed to serialize '$name' at index %u\", (uint32_t)idx);";
        WriteSource "                return SAI_SERIALIZE_ERROR;";
        WriteSource "            }";
        WriteSource "            $buf += ret;";
        WriteSource "            $quot;" if $needQuote;
        WriteSource "            if (idx != $structBase->$countName - 1)";
        WriteSource "               $buf += sprintf($buf, \",\");";
        WriteSource "        }";
        WriteSource "        $buf += sprintf($buf, \"]\");"; # end of array
        WriteSource "    }";
    }

    WriteSource "    $buf += sprintf($buf, \"}\");"; # end of struct
    WriteSource "    return (int)($buf - start_$buf);";
    WriteSource "}";
}

sub CreateSerializeStructs
{
    WriteSectionComment "Serialize structs";

    #
    # this method will auto generate serialization methods for all structs with
    # some exceptions like when struct contains unions and function pointers
    #

    for my $struct (sort keys %main::ALL_STRUCTS)
    {
        # TODO we could auto skip structs with unions and function pointers

        # user defined serialization
        next if $struct eq "sai_ip_address_t";
        next if $struct eq "sai_ip_prefix_t";
        next if $struct eq "sai_acl_action_data_t";
        next if $struct eq "sai_acl_field_data_t";
        next if $struct eq "sai_attribute_t";
        next if $struct eq "sai_tlv_t";
        next if $struct eq "sai_hmac_t";

        # TODO sai_acl_capability_t has enum list and it's only valid when
        # similar for sai_tam_threshold_breach_event_t

        # non serializable
        next if $struct eq "sai_service_method_table_t";
        next if $struct eq "sai_object_key_t";
        next if $struct =~ /^sai_\w+_api_t$/;

        CreateSerializeSingleStruct($struct);
    }
}

sub CreateSerializeMethods
{
    CreateSerializeForEnums();

    CreateSerializeMetaKey();

    CreateSerializeStructs();

    CreateSerializeNotifications();
}

BEGIN
{
    our @ISA    = qw(Exporter);
    our @EXPORT = qw/
    CreateSerializeMethods
    /;
}

1;

# we could also generate deserialize and call notifation where notification
# struct would be passed and notification would be called and then free itself
# (but on exception there will be memory leak)
#
# TODO generate notifications metadata, if param is object id then objects must
# be specified for validation wheter notification returned valid object, and
# each struct that is using object id should have @objects on object id
# members, then we should generate all struct infos to get all functions for
# oid extraction etc
