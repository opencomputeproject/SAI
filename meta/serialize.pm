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

sub CreateSerializeMethodsForNonObjectId
{
    my $structName = shift;

    my %struct = ExtractStructInfo($structName, "struct_");

    my $structBase = $1 if $structName =~/^sai_(\w+)_t$/;

    my @keys = GetStructKeysInOrder(\%struct);

    WriteHeader "int sai_serialize_${structBase}(";
    WriteHeader "        _Out_ char *buffer,";
    WriteHeader "        _In_ const $structName *$structBase);";

    WriteSource "int sai_serialize_${structBase}(";
    WriteSource "        _Out_ char *buffer,";
    WriteSource "        _In_ const $structName *$structBase)";
    WriteSource "{";

    my $template = "";

    for my $member (@keys)
    {
        WriteSource "    char $member\[128\];"; # TODO base on memebr determine size
    }

    WriteSource "";
    WriteSource "    int ret = 0;";
    WriteSource "";

    for my $member (@keys)
    {
        my $type = $struct{$member}{type};

        # XXX we always add "quote" in %s, but this may be bad if we serialize other
        # object list list, or other structure, we need to know ad hoc that item we are
        # serializing, also single numbers don't need quotes

        $template .= "\\\"$member\\\":\\\"%s\\\",";

        if ($type =~ m/^sai_(object_id|mac)_t$/)
        {
            WriteSource "    ret |= sai_serialize_$1($member, $structBase->$member);";
            next;
        }

        # we can figure out that item is strict and then use "&"
        if ($type =~ m/^sai_(ip_address|ip_prefix)_t$/)
        {
            WriteSource "    ret |= sai_serialize_$1($member, &$structBase->$member);";
            next;
        }

        if (defined $main::SAI_ENUMS{$type})
        {
            WriteSource "    ret |= sai_serialize_enum($member, &sai_metadata_enum_$type, $structBase->$member);";
            next;
        }

        LogError "not supported '$member' -> $type";
    }

    chop $template;

    WriteSource "";
    WriteSource "    if (ret < 0)";
    WriteSource "    {";
    WriteSource "        SAI_META_LOG_WARN(\"failed to serialize $structName\");";
    WriteSource "        return SAI_SERIALIZE_ERROR;";
    WriteSource "    }";
    WriteSource "";

    my $locals = join(",", @keys);

    WriteSource "    return sprintf(buffer,";
    WriteSource "                   \"{$template}\",";
    WriteSource "                   $locals);";

    WriteSource "}";
}

sub CreateSerializeForNoIdStructs
{
    my @rawnames = GetNonObjectIdStructNames();

    for my $rawname (@rawnames)
    {
        CreateSerializeMethodsForNonObjectId("sai_${rawname}_t");
    }
}

# XXX this serialize is wrong, it should go like normal struct since
# its combination for easier readibility, we need 2 versions of this ?

sub CreateSerializeMetaKey
{
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

sub CreateSerializeMethods
{
    CreateSerializeForEnums();

    CreateSerializeForNoIdStructs();

    CreateSerializeMetaKey();
}

BEGIN
{
    our @ISA    = qw(Exporter);
    our @EXPORT = qw/
    CreateSerializeMethods
    /;
}

1;
