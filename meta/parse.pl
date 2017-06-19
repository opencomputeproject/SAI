#!/usr/bin/perl

use strict;
use warnings;
use diagnostics;

use XML::Simple qw(:strict);
use Getopt::Std;
use Data::Dumper;
use utils;
use xmlutils;
use style;
use test;
use serialize;

our $XMLDIR = "xml";
our $INCLUDE_DIR = "../inc/";

our %SAI_ENUMS = ();
our %METADATA = ();
our %NON_OBJECT_ID_STRUCTS = ();
our %NOTIFICATIONS = ();
our %OBJTOAPIMAP = ();
our %APITOOBJMAP = ();
our %ALL_STRUCTS = ();
our %OBJECT_TYPE_MAP = ();

my $FLAGS = "MANDATORY_ON_CREATE|CREATE_ONLY|CREATE_AND_SET|READ_ONLY|KEY|DYNAMIC|SPECIAL";

# TAGS HANDLERS

my %TAGS = (
        "type"      , \&ProcessTagType,
        "flags"     , \&ProcessTagFlags,
        "objects"   , \&ProcessTagObjects,
        "allownull" , \&ProcessTagAllowNull,
        "condition" , \&ProcessTagCondition,
        "validonly" , \&ProcessTagCondition, # since validonly uses same format as condition
        "default"   , \&ProcessTagDefault,
        "ignore"    , \&ProcessTagIgnore,
        "isvlan"    , \&ProcessTagIsVlan,
        "getsave"   , \&ProcessTagGetSave,
        );

my %options = ();
getopts("dsAS", \%options);

our $optionPrintDebug        = 1 if defined $options{d};
our $optionDisableAspell     = 1 if defined $options{A};
our $optionUseXmlSimple      = 1 if defined $options{s};
our $optionDisableStyleCheck = 1 if defined $options{S};

# LOGGING FUNCTIONS HELPERS

$SIG{__DIE__} = sub
{
    LogError "FATAL ERROR === MUST FIX === : @_";
    exit 1;
};

my %ACL_FIELD_TYPES = ();
my %ACL_FIELD_TYPES_TO_VT = ();
my %ACL_ACTION_TYPES = ();
my %ACL_ACTION_TYPES_TO_VT = ();

my %VALUE_TYPES = ();
my %VALUE_TYPES_TO_VT = ();

sub ProcessTagType
{
    my ($type, $value, $val) = @_;

    if ($val =~/^sai_s32_list_t sai_\w+_t$/)
    {
        return $val;
    }

    if ($val =~/^sai_acl_field_data_t (sai_\w+_t|bool)$/)
    {
        return $val;
    }

    if ($val =~/^sai_acl_action_data_t (sai_\w+_t|bool)$/)
    {
        return $val;
    }

    if ($val =~ /^(bool|char)$/)
    {
        return $val;
    }

    if ($val =~/^sai_\w+_t$/ and not $val =~ /_attr_t/)
    {
        return $val;
    }

    if ($val =~/^sai_pointer_t (sai_\w+_fn)$/)
    {
        my $pointerfn = $1;

        if (not $pointerfn =~ /^sai_\w+_(callback|notification)_fn$/)
        {
            LogWarning "function name $pointerfn should be in format sai_\\w+_(callback|notification)_fn";
        }

        return $val;
    }

    LogError "invalid type tag value '$val' expected sai type or enum";

    return undef;
}

sub ProcessTagFlags
{
    my ($type, $value, $val) = @_;

    $val =~ s/\s*//g;

    my @flags = split/\|/,$val;

    for my $flag (@flags)
    {
        if (not $flag =~ /^($FLAGS)$/)
        {
            LogError "invalid flags tag value '$val' ($flag) on $value, expected in '$FLAGS'";
            return undef;
        }
    }

    return \@flags;
}

sub ProcessTagObjects
{
    my ($type, $value, $val) = @_;

    $val =~ s/\s*//g;

    my @ots = split/[,]/,$val;

    for my $ot (@ots)
    {
        if (not $ot =~ /^SAI_OBJECT_TYPE_\w+$/)
        {
            LogError "invalid objecttype tag value '$val' ($ot) in $type $value";
            return undef;
        }
    }

    return \@ots;
}

sub ProcessTagAllowNull
{
    my ($type, $value, $val) = @_;

    if (not $val =~/^(true|false)$/i)
    {
        LogError "allownull tag value '$val', expected true/false";
        return undef;
    }

    return $val;
}

sub ProcessTagCondition
{
    my ($type, $value, $val) = @_;

    my @conditions = split/\s+(?:or|and)\s+/,$val;

    if ($val =~/or.+and|and.+or/)
    {
        LogError "mixed conditions and/or is not supported: $val";
        return undef;
    }

    for my $cond (@conditions)
    {
        if (not $cond =~/^(SAI_\w+) == (true|false|SAI_\w+|$NUMBER_REGEX)$/)
        {
            LogError "invalid condition tag value '$val' ($cond), expected SAI_ENUM == true|false|SAI_ENUM|number";
            return undef;
        }
    }

    # if there is only one condition, then type does not matter

    $type = "SAI_ATTR_CONDITION_TYPE_" . (($val =~ /and/) ? "AND" : "OR");

    unshift @conditions, $type;

    return \@conditions;
}

sub ProcessTagDefault
{
    my ($type, $value, $val) = @_;

    if ($val =~/^(empty|internal|vendor|const)/)
    {
        return $val;
    }

    if ($val =~/^(attrvalue) SAI_\w+_ATTR_\w+$/)
    {
        return $val;
    }

    if ($val =~/^(true|false|NULL|SAI_\w+|$NUMBER_REGEX)$/ and not $val =~ /_ATTR_|OBJECT_TYPE/)
    {
        return $val;
    }

    if ($val =~/^0\.0\.0\.0$/)
    {
        # currently we only support default ip address
        return $val;
    }

    if ($val eq "disabled")
    {
        # for aclfield and aclaction
        return $val;
    }

    LogError "invalid default tag value '$val' on $type $value";
    return undef;
}

sub ProcessTagIgnore
{
    my ($type, $value, $val) = @_;

    return "true";
}

sub ProcessTagIsVlan
{
    my ($type, $value, $val) = @_;

    return $val if $val =~/^(true|false)$/i;

    LogError "isvlan tag value '$val', expected true/false";
    return undef;
}

sub ProcessDescription
{
    my ($type, $value, $desc, $brief) = @_;

    my @order = ();

    $desc =~ s/@@/\n@@/g;

    while ($desc =~ /@@(\w+)(.*)/g)
    {
        my $tag = $1;
        my $val = $2;

        push @order,$tag;

        $val =~ s/\s+/ /g;
        $val =~ s/^\s*//;
        $val =~ s/\s*$//;

        if (not defined $TAGS{$tag})
        {
            LogError "unrecognized tag '$tag' on $type $value";
            next;
        }

        $val = $TAGS{$tag}->($type, $value, $val);

        $METADATA{$type}{$value}{$tag}          = $val;
        $METADATA{$type}{$value}{objecttype}    = $type;
        $METADATA{$type}{$value}{attrid}        = $value;
    }

    $brief =~ s/^\s*//;
    $brief =~ s/\s*$//;

    $METADATA{$type}{$value}{brief} = $brief if $brief ne "";

    my $count = @order;

    return if $count == 0;

    my $order = join(":",@order);

    return if $order =~/^type:flags(:objects)?(:allownull)?(:isvlan)?(:default)?(:condition|:validonly)?$/;
    return if $order =~/^ignore$/;

    LogWarning "metadata tags are not in right order: $order on $value";
}

sub ProcessEnumSection
{
    my $section = shift;

    for my $memberdef (@{ $section->{memberdef} })
    {
        next if not $memberdef->{kind} eq "enum";

        my $id = $memberdef->{id};

        my $enumtypename = $memberdef->{name}[0];

        $enumtypename =~ s/^_//;

        if (not $enumtypename =~ /^sai_/)
        {
            LogWarning "enum $enumtypename is not prefixed sai_";
            next;
        }

        if (defined $SAI_ENUMS{$enumtypename})
        {
            LogError "duplicated enum $enumtypename";
            next;
        }

        my $ed = ExtractDescription($enumtypename, $enumtypename, $memberdef->{detaileddescription}[0]);

        $SAI_ENUMS{$enumtypename}{flagsenum} = ($ed =~ /\@\@flags/s) ? "true" : "false";

        my @arr = ();

        $SAI_ENUMS{$enumtypename}{values} = \@arr;

        my $enumprefix = uc($1) if $enumtypename =~ /^(sai_\w+)t$/;

        for my $ev (@{ $memberdef->{enumvalue} })
        {
            my $enumvaluename = $ev->{name}[0];

            my $eitemd = ExtractDescription($enumtypename, $enumvaluename, $ev->{detaileddescription}[0]);

            if ($eitemd =~/\@ignore/)
            {
                LogInfo "Ignoring $enumvaluename";
                next;
            }

            LogDebug "$id $enumtypename $enumvaluename";

            push@arr,$enumvaluename;

            LogWarning "Value $enumvaluename of $enumtypename is not prefixed as $enumprefix" if not $enumvaluename =~ /^$enumprefix/;

            if (not $enumvaluename =~/^[A-Z0-9_]+$/)
            {
                LogError "enum $enumvaluename uses characters outside [A-Z0-9_]+";
            }
        }

        # remove unnecessary attributes
        my @values = @{ $SAI_ENUMS{$enumtypename}{values} };
        @values = grep(!/^SAI_\w+_(START|END)$/, @values);
        @values = grep(!/^SAI_\w+(CUSTOM_RANGE_BASE)$/, @values);

        if ($enumtypename =~ /^(sai_\w+)_t$/)
        {
            my $valuescount = @values;

            if ($valuescount == 0)
            {
                LogError "enum $enumtypename is empty, after removing suffixed entries _START/_END/_CUSTOM_RANGE_BASE";
                LogError "  those suffixes are reserved for range markers and are removed by metadata parser, don't use them";
                LogError "  as actual part of valid enum name, take a look at sai_udf_group_type_t for valid usage";
                next;
            }

            my $last = $values[$#values];

            if ($last eq uc("$1_MAX"))
            {
                $last =  pop @values;
                LogInfo "Removing last element $last";
            }
        }

        $SAI_ENUMS{$enumtypename}{values} = \@values;

        next if not $enumtypename =~ /^(sai_(\w+)_attr_)t$/;

        my $prefix = uc$1;

        # remove unnecessary attributes
        @values = @{ $SAI_ENUMS{$enumtypename}{values} };
        @values = grep(!/^${prefix}(CUSTOM_RANGE_|FIELD_|ACTION_)?(START|END)$/, @values);
        $SAI_ENUMS{$enumtypename}{values} = \@values;

        # this is attribute

        for my $ev (@{ $memberdef->{enumvalue} })
        {
            my $enumvaluename = $ev->{name}[0];

            my $desc = ExtractDescription($enumtypename, $enumvaluename, $ev->{detaileddescription}[0]);
            my $brief = ExtractDescription($enumtypename, $enumvaluename, $ev->{briefdescription}[0]);

            ProcessDescription($enumtypename, $enumvaluename, $desc, $brief);

            # remove ignored attributes from enums from list
            # since enum must match attribute list size

            next if not defined $METADATA{$enumtypename}{$enumvaluename}{ignore};

            @values = grep(!/^$enumvaluename$/, @values);
            $SAI_ENUMS{$enumtypename}{values} = \@values;
        }
    }
}

sub ProcessTypedefSection
{
    my $section = shift;

    for my $memberdef (@{ $section->{memberdef} })
    {
        next if not $memberdef->{kind} eq "typedef";

        my $id = $memberdef->{id};

        my $typedefname = $memberdef->{name}[0];

        my $typedeftype;

        $typedeftype = $memberdef->{type}[0] if ref $memberdef->{type}[0] eq "";

        $typedeftype = $memberdef->{type}[0]->{content} if ref $memberdef->{type}[0] eq "HASH";

        if ($typedeftype =~ /^struct/)
        {
            # record structs for later serialization
            # this will also include structs from metadata
            $ALL_STRUCTS{$typedefname} = 1;
            next;
        }

        if ($typedefname =~ /^sai_\w+_notification_fn$/)
        {
            if (not $typedeftype =~ /void\(\*/)
            {
                LogWarning "notification $typedefname should return void, but got '$typedeftype'";
                next;
            }

            ProcessNotifications($memberdef, $typedefname);
            next;
        }

        next if not $typedeftype =~ /^enum/;

        if (not defined $SAI_ENUMS{$typedefname})
        {
            LogError "enum $typedefname has no typedef enum $typedefname";
            next;
        }

        next if not $typedefname =~ /^sai_(\w+)_attr_t$/;

        # this enum is attribute definition for object

        $SAI_ENUMS{$typedefname}{objecttype} = "SAI_OBJECT_TYPE_" . uc($1);
    }
}

sub ProcessNotificationCount
{
    my ($ntfName, $tagValue, $previousTagValue) = @_;

    my %count = ();

    %count = %{ $previousTagValue } if defined $previousTagValue;

    if (not $tagValue =~ /^(\w+)\[(\w+|\d+)\]$/g)
    {
        LogError "unable to parse count '$tagValue' on $ntfName";
        return undef;
    }

    my $pointerParam = $1;
    my $countParam = $2;

    $count{$pointerParam} = $countParam;

    LogDebug "adding count $pointerParam\[$countParam\] on $ntfName";

    if ($pointerParam eq $countParam)
    {
        LogError "count '$pointerParam' can't point to itself in \@count on $ntfName";
        undef;
    }

    return \%count;
}

sub ProcessNotificationObjects
{
    #
    # object type for attribute params are described in
    # notification descripnon, it would be easier
    # if they would be described on params itself
    #

    my ($ntfName, $tagValue, $previousTagValue) = @_;

    my %objectTypes = ();

    %objectTypes = %{ $previousTagValue } if defined $previousTagValue;

    if (not $tagValue =~ /^(\w+)\s+(SAI_OBJECT_TYPE_\w+)$/g)
    {
        LogError "invalid object type tag value '$tagValue' in $ntfName";
        return undef;
    }

    $objectTypes{$1} = $2;

    LogDebug "adding object type $2 on param $1 in $ntfName";

    return \%objectTypes;
}

my %NOTIFICATION_TAGS = (
        "count"       , \&ProcessNotificationCount,
        "objects"     , \&ProcessNotificationObjects,
        );

sub ProcessNotificationDescription
{
    my ($refNtf, $desc) = @_;

    $refNtf->{desc} = $desc;

    my $ntfName = $refNtf->{name};

    $desc =~ s/@@/\n@@/g;

    while ($desc =~ /@@(\w+)(.*)/g)
    {
        my $tag = $1;
        my $value = Trim($2);

        if (not defined $NOTIFICATION_TAGS{$tag})
        {
            LogError "unrecognized tag '$tag' on $ntfName: $value";
            next;
        }

        LogDebug "processing tag '$tag' on $ntfName";

        $refNtf->{$tag} = $NOTIFICATION_TAGS{$tag}->($ntfName, $value, $refNtf->{$tag});
    }
}

sub ProcessNotifications
{
    my ($member, $typedefname) = @_;

    my $args = $member->{argsstring}[0];

    $args =~ s/[()]//g;

    my @params = split/,/,$args;

    my %N = (name => $typedefname);

    my $desc = ExtractDescription($typedefname, $typedefname, $member->{detaileddescription}[0]);

    ProcessNotificationDescription(\%N, $desc);

    my @Members = ();

    my $idx = 0;

    my $ParamRegex = '^\s*_In_ ((const )?\w+\s*?\*?)\s*(\w+)$';

    my @keys = ();

    for my $param (@params)
    {
        # NOTE multple pointers or consts are not supported

        if (not $param =~ /$ParamRegex/)
        {
            LogWarning "can't match param '$param' on $typedefname";
            return
        }

        my %M = ();

        my $type = $1;
        my $name = $3;

        push @keys, $name;

        if (defined $N{objects} and defined $N{objects}{$name})
        {
            my @objects = ();

            push @objects, $N{objects}{$name};

            $M{objects} = \@objects;
        }

        $M{param}       = $param;
        $M{type}        = $type;
        $M{name}        = $name;
        $M{idx}         = $idx;
        $M{file}        = $member->{location}[0]->{file};

        push @Members, \%M;

        $N{membersHash}{ $name } = \%M;

        $idx++;
    }

    # second pass is needed if count param is defined after data pointer

    for my $param (@params)
    {
        next if (not $param =~ /$ParamRegex/);

        my $type = $1;
        my $name = $3;

        next if not $type =~ /\*/;

        if (not defined $N{count} and not defined $N{count}->{$name})
        {
            LogWarning "type '$type' is pointer, \@count is required, but missing on $typedefname";
            next;
        }

        my $countParamName = $N{count}->{$name};

        $N{membersHash}->{$name}{count} = $countParamName;

        if ($countParamName =~ /^(\d+)$/)
        {
            # count is given explicit
            next;
        }

        if (not defined $N{membersHash}->{$countParamName})
        {
            LogWarning "count param name '$countParamName' on $typedefname is not defined";
            next;
        }

        my $countType = $N{membersHash}->{$countParamName}{type};

        if (not $countType =~ /^(uint32_t|sai_size_t)$/)
        {
            LogWarning "param '$countParamName' used as count for param '$name' ($typedefname)";
            LogWarning " is wrong type '$countType' allowed: (uint32_t|sai_size_t)";
        }
    }

    $N{params} = \@params;
    $N{keys} = \@keys;
    $N{members} = \@Members;
    $N{ismethod} = 1;

    $N{baseName} = $1 if $typedefname =~ /^sai_(\w+_notification)_fn$/;

    $NOTIFICATIONS{$typedefname} = \%N;
}

sub ProcessXmlFile
{
    my $file = shift;

    my $ref = ReadXml $file;

    my @sections = @{ $ref->{compounddef}[0]->{sectiondef} };

    for my $section (@sections)
    {
        ProcessEnumSection($section) if ($section->{kind} eq "enum");

        ProcessTypedefSection($section) if ($section->{kind} eq "typedef");
    }
}

sub ProcessSingleEnum
{
    my ($key, $typedef, $prefix) = @_;

    my $enum = $SAI_ENUMS{$key};

    my @values = @{$enum->{values}};

    my $flags = (defined $enum->{flagsenum}) ? $enum->{flagsenum} : "false";

    WriteSource "const $typedef sai_metadata_${typedef}_enum_values[] = {";

    for my $value (@values)
    {
        LogWarning "Value $value of $typedef must use only capital letters" if $value =~ /[a-z]/;

        LogWarning "Value $value of $typedef is not prefixed as $prefix" if not $value =~ /^$prefix/;

        WriteSource "    $value,";
    }

    WriteSource "    -1"; # guard

    WriteSource "};";

    WriteSource "const char* const sai_metadata_${typedef}_enum_values_names[] = {";

    for my $value (@values)
    {
        WriteSource "    \"$value\",";
    }

    WriteSource "    NULL";
    WriteSource "};";

    WriteSource "const char* const sai_metadata_${typedef}_enum_values_short_names[] = {";

    for my $value (@values)
    {
        $value =~ s/^${prefix}//;

        WriteSource "    \"$value\",";
    }

    WriteSource "    NULL";
    WriteSource "};";

    my $count = @values;

    WriteHeader "extern const sai_enum_metadata_t sai_metadata_enum_$typedef;";

    WriteSource "const sai_enum_metadata_t sai_metadata_enum_$typedef = {";
    WriteSource "    .name              = \"${typedef}\",";
    WriteSource "    .valuescount       = $count,";
    WriteSource "    .values            = (const int*)sai_metadata_${typedef}_enum_values,";
    WriteSource "    .valuesnames       = sai_metadata_${typedef}_enum_values_names,";
    WriteSource "    .valuesshortnames  = sai_metadata_${typedef}_enum_values_short_names,";
    WriteSource "    .containsflags     = $flags,";
    WriteSource "};";

    return $count;
}

sub CreateMetadataHeaderAndSource
{
    # since sai_status is not enum and it will declare extra statuses
    ProcessSaiStatus();

    WriteSource "#include <stdio.h>";
    WriteSource "#include \"saimetadata.h\"";

    WriteSectionComment "Enums metadata";

    for my $key (sort keys %SAI_ENUMS)
    {
        if (not $key =~ /^((sai_\w+_)t)$/)
        {
            LogWarning "Enum typedef $key is not matching SAI format";
            next;
        }

        ProcessSingleEnum($key, $1, uc $2);
    }

    # all enums

    WriteHeader "extern const sai_enum_metadata_t* const sai_metadata_all_enums[];";
    WriteSource "const sai_enum_metadata_t* const sai_metadata_all_enums[] = {";

    for my $key (sort keys %SAI_ENUMS)
    {
        if (not $key =~ /^((sai_\w+_)t)$/)
        {
            LogWarning "Enum typedef $key is not matching SAI format";
            next;
        }

        my $typedef = $1;

        WriteSource "    &sai_metadata_enum_$typedef,";
    }

    WriteSource "    NULL";
    WriteSource "};";

    my $count = keys %SAI_ENUMS;

    WriteHeader "extern const size_t sai_metadata_all_enums_count;";
    WriteSource "const size_t sai_metadata_all_enums_count = $count;";

    WriteHeader "extern const sai_enum_metadata_t* const sai_metadata_attr_enums[];";
    WriteSource "const sai_enum_metadata_t* const sai_metadata_attr_enums[] = {";

    $count = 0;

    for my $key (sort keys %SAI_ENUMS)
    {
        if (not $key =~ /^(sai_\w+_attr_t)$/)
        {
            next;
        }

        my $typedef = $1;

        WriteSource "    &sai_metadata_enum_$typedef,";

        $count++;
    }

    WriteSource "    NULL";
    WriteSource "};";

    WriteHeader "extern const size_t sai_metadata_attr_enums_count;";
    WriteSource "const size_t sai_metadata_attr_enums_count = $count;";

    # attr enums as object types for sanity check

    WriteSource "const sai_object_type_t sai_metadata_object_types[] = {";

    for my $key (sort keys %SAI_ENUMS)
    {
        if (not $key =~ /^(sai_(\w+)_attr_t)$/)
        {
            next;
        }

        my $typedef = $1;
        my $objtype = $2;

        WriteSource "    SAI_OBJECT_TYPE_" . uc($objtype). ",";
    }

    WriteSource "};";
}

sub ProcessType
{
    my ($attr, $type) = @_;

    if (not defined $type)
    {
        LogError "type is not defined for $attr";
        return "";
    }

    if ($type =~ /^sai_acl_field_data_t (bool|sai_\w+_t)$/)
    {
        my $prefix = "SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA";

        return "${prefix}_BOOL" if $1 eq "bool";

        return "${prefix}_$ACL_FIELD_TYPES_TO_VT{$1}" if defined $ACL_FIELD_TYPES_TO_VT{$1};

        if (not defined $SAI_ENUMS{$1})
        {
            LogError "invalid enum specified acl field '$type' on $attr";
            return "";
        }

        return "${prefix}_INT32";
    }

    if ($type =~ /^sai_acl_action_data_t (sai_\w+_t)$/)
    {
        my $prefix = "SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA";

        return "${prefix}_$ACL_ACTION_TYPES_TO_VT{$1}" if defined $ACL_ACTION_TYPES_TO_VT{$1};

        if (not defined $SAI_ENUMS{$1})
        {
            LogError "invalid enum specified acl action '$type' on $attr";
            return "";
        }

        return "${prefix}_INT32";
    }

    if ($type =~ /^sai_s32_list_t (sai_\w+_t)$/)
    {
        if (not defined $SAI_ENUMS{$1})
        {
            LogError "invalid enum list specified '$type' on $attr";
            return "";
        }

        return "SAI_ATTR_VALUE_TYPE_INT32_LIST";
    }

    if ($type =~ /^(sai_\w+_t)$/)
    {
        my $prefix = "SAI_ATTR_VALUE_TYPE";

        return "${prefix}_$VALUE_TYPES_TO_VT{$1}" if defined $VALUE_TYPES_TO_VT{$1};

        if (not defined $SAI_ENUMS{$1})
        {
            LogError "invalid enum specified '$type' on $attr";
            return "";
        }

        return "${prefix}_INT32";
    }

    if ($type eq "bool")
    {
        return "SAI_ATTR_VALUE_TYPE_BOOL";
    }

    if ($type eq "char")
    {
        return "SAI_ATTR_VALUE_TYPE_CHARDATA";
    }

    if ($type =~ /^sai_pointer_t sai_\w+_fn$/)
    {
        return "SAI_ATTR_VALUE_TYPE_POINTER";
    }

    LogError "invalid type '$type' for $attr";
    return "";
}

sub ProcessFlags
{
    my ($value,$flags) = @_;

    if (not defined $flags)
    {
        LogError "flags are not defined for $value";
        return "";
    }

    my @flags = @{ $flags };

    @flags = map {s/^/SAI_ATTR_FLAGS_/; $_; } @flags;

    return "(sai_attr_flags_t)(" . join("|",@flags) . ")";
}

sub ProcessAllowNull
{
    my ($value,$allownull) = @_;

    return $allownull if defined $allownull;

    return "false";
}

sub ProcessObjects
{
    my ($attr, $objects) = @_;

    return "NULL" if not defined $objects;

    WriteSource "const sai_object_type_t sai_metadata_${attr}_allowed_objects[] = {";

    my @all = @{ $SAI_ENUMS{sai_object_type_t}{values} };

    for my $obj (@{ $objects })
    {
        if (not defined $OBJECT_TYPE_MAP{$obj})
        {
            LogError "unknown object type '$obj' on $attr";
            return "";
        }

        WriteSource "    $obj,";
    }

    WriteSource "};";

    return "sai_metadata_${attr}_allowed_objects";
}

sub ProcessObjectsLen
{
    my ($value, $objects) = @_;

    return "0" if not defined $objects;

    my $count = @{ $objects };

    return $count;
}

sub ProcessDefaultValueType
{
    my ($attr, $default) = @_;

    return "SAI_DEFAULT_VALUE_TYPE_NONE" if not defined $default;

    return "SAI_DEFAULT_VALUE_TYPE_CONST" if $default =~ /^SAI_NULL_OBJECT_ID$/;

    return "SAI_DEFAULT_VALUE_TYPE_SWITCH_INTERNAL" if $default =~ /^internal$/;

    return "SAI_DEFAULT_VALUE_TYPE_CONST" if $default =~ /^(true|false|const|NULL|$NUMBER_REGEX|SAI_\w+)$/ and not $default =~ /_ATTR_|SAI_OBJECT_TYPE_/;

    return "SAI_DEFAULT_VALUE_TYPE_EMPTY_LIST" if $default =~ /^empty$/;

    return "SAI_DEFAULT_VALUE_TYPE_VENDOR_SPECIFIC" if $default =~ /^vendor$/;

    return "SAI_DEFAULT_VALUE_TYPE_ATTR_VALUE" if $default =~ /^attrvalue SAI_\w+$/ and $default =~ /_ATTR_/;

    return "SAI_DEFAULT_VALUE_TYPE_ATTR_RANGE" if $default =~ /^attrrange SAI_\w+$/ and $default =~ /_ATTR_/;

    return "SAI_DEFAULT_VALUE_TYPE_CONST" if $default =~ /^0\.0\.0\.0$/;

    return "SAI_DEFAULT_VALUE_TYPE_CONST" if $default eq "disabled";

    LogError "invalid default value type '$default' on $attr";

    return "";
}

sub ProcessDefaultValue
{
    my ($attr, $default, $type) = @_;

    return "NULL" if not defined $default;

    my $val = "const sai_attribute_value_t sai_metadata_${attr}_default_value";

    if ($default =~ /^(true|false)$/ and $type eq "bool")
    {
        WriteSource "$val = { .booldata = $default };";
    }
    elsif ($default =~ /^SAI_NULL_OBJECT_ID$/ and $type =~ /^sai_object_id_t$/)
    {
        WriteSource "$val = { .oid = $default };";
    }
    elsif ($default =~ /^SAI_\w+$/ and $type =~ /^sai_\w+_t$/ and not defined $VALUE_TYPES{$type})
    {
        WriteSource "$val = { .s32 = $default };";
    }
    elsif ($default =~ /^0$/ and $type =~ /^sai_acl_field_data_t (sai_u?int\d+_t)/)
    {
        WriteSource "$val = { 0 };";
    }
    elsif ($default =~ /^0$/ and $type =~ /^sai_acl_action_data_t (sai_u?int\d+_t)/)
    {
        WriteSource "$val = { 0 };";
    }
    elsif ($default =~ /^$NUMBER_REGEX$/ and $type =~ /^sai_u?int\d+_t/)
    {
        WriteSource "$val = { .$VALUE_TYPES{$type} = $default };";
    }
    elsif ($default =~ /^NULL$/ and $type =~ /^(sai_pointer_t) (sai_\w+_fn)$/)
    {
        WriteSource "$val = { .$VALUE_TYPES{$1} = $default };";
    }
    elsif ($default =~ /^(attrvalue|attrrange|vendor|empty|const|internal)/)
    {
        return "NULL";
    }
    elsif ($default =~ /^NULL$/ and $type =~ /^sai_pointer_t/)
    {
        LogError "missing typedef function in format 'sai_\\w+_fn' on $attr ($type)";
    }
    elsif ($default =~ /^0\.0\.0\.0$/ and $type =~ /^(sai_ip_address_t)/)
    {
        # ipv4 address needs to be converted to uint32 number so we support now only 0.0.0.0

        WriteSource "$val = { .$VALUE_TYPES{$1} = { .addr_family = SAI_IP_ADDR_FAMILY_IPV4, .addr = { .ip4 = 0 } } };";
    }
    elsif ($default =~ /^disabled$/ and $type =~ /^(sai_acl_action_data_t|sai_acl_field_data_t) /)
    {
        WriteSource "$val = { .$VALUE_TYPES{$1} = { .enable = false } };";
    }
    else
    {
        LogError "invalid default value '$default' on $attr ($type)";
    }

    return "&sai_metadata_${attr}_default_value";
}

sub ProcessDefaultValueObjectType
{
    my ($attr, $value, $type) = @_;

    $value = "" if not defined $value;

    return "SAI_OBJECT_TYPE_$2" if $value =~ /^(attrvalue|attrrange) SAI_(\w+)_ATTR_\w+$/;

    return "SAI_OBJECT_TYPE_NULL";
}

sub ProcessDefaultValueAttrId
{
    my ($attr, $value, $type) = @_;

    $value = "" if not defined $value;

    return $2 if $value =~ /^(attrvalue|attrrange) ((SAI_\w+)_ATTR_\w+)$/;

    return "SAI_INVALID_ATTRIBUTE_ID";
}

sub ProcessStoreDefaultValue
{
    my ($attr, $value, $flags) = @_;

    # at this point we can't determine whether object is non object id

    if (defined $value and $value =~/vendor|attrvalue/)
    {
        return "true";
    }

    my @flags = @{ $flags };

    $flags = "@flags";

    if ($flags =~/MANDATORY/ and $flags =~ /CREATE_AND_SET/)
    {
        return "true";
    }

    return "false";
}

sub ProcessIsEnum
{
    my ($value, $type) = @_;

    return "false" if not defined $type;

    return "true" if $type =~ /^sai_\w+_t$/ and not defined $VALUE_TYPES{$type};
    return "true" if $type =~ /^sai_acl_field_data_t (sai_\w+_t)$/ and not defined $ACL_FIELD_TYPES{$1};
    return "true" if $type =~ /^sai_acl_action_data_t (sai_\w+_t)$/ and not defined $ACL_ACTION_TYPES{$1};

    return "false";
}

sub ProcessIsEnumList
{
    my ($attr, $type) = @_;

    return "false" if not defined $type;

    return "true" if $type =~ /^sai_s32_list_t sai_\w+_t$/;

    return "false";
}

sub ProcessEnumMetadata
{
    my ($attr, $type) = @_;

    return "NULL" if not defined $type;

    return "&sai_metadata_enum_$1" if $type =~ /^(sai_\w+_t)$/ and not defined $VALUE_TYPES{$type};
    return "&sai_metadata_enum_$1" if $type =~ /^sai_acl_field_data_t (sai_\w+_t)$/ and not defined $ACL_FIELD_TYPES{$1};
    return "&sai_metadata_enum_$1" if $type =~ /^sai_acl_action_data_t (sai_\w+_t)$/ and not defined $ACL_ACTION_TYPES{$1};
    return "&sai_metadata_enum_$1" if $type =~ /^sai_s32_list_t (sai_\w+_t)$/;

    return "NULL";
}

sub ProcessIsVlan
{
    my ($attr, $value, $type) = @_;

    if (not defined $value and $type =~ /uint16/)
    {
        LogWarning "$attr is $type, must define TAG isvlan";
    }

    return "false" if not defined $value;

    return $value;
}

sub ProcessGetSave
{
    my ($attr, $value) = @_;

    return "false" if not defined $value;

    return $value;
}

sub ProcessConditionType
{
    my ($attr, $value) = @_;

    return "SAI_ATTR_CONDITION_TYPE_NONE" if not defined $value;

    return @{$value}[0];
}

sub ProcessConditionsGeneric
{
    my ($attr, $conditions, $enumtype, $name) = @_;

    return "NULL" if not defined $conditions;

    my @conditions = @{ $conditions };

    shift @conditions;

    my $count = 0;

    my @values = ();

    for my $cond (@conditions)
    {
        if (not $cond =~ /^(SAI_\w+) == (true|false|SAI_\w+|$NUMBER_REGEX)$/)
        {
            LogError "invalid condition '$cond' on $attr";
            return "";
        }

        my $attrid = $1;
        my $val = $2;

        my $main_attr = $1 if $attr =~ /^SAI_(\w+?)_ATTR_/;
        my $cond_attr = $1 if $attrid =~ /^SAI_(\w+?)_ATTR_/;

        if ($main_attr ne $cond_attr)
        {
            LogError "$name attribute $attr has condition from different object $attrid";
            return "";
        }

        WriteSource "const sai_attr_condition_t sai_metadata_${name}_${attr}_$count = {";

        if ($val eq "true" or $val eq "false")
        {
            WriteSource "    .attrid = $attrid,";
            WriteSource "    .condition = { .booldata = $val }";
        }
        elsif ($val =~ /^SAI_/)
        {
            WriteSource "    .attrid = $attrid,";
            WriteSource "    .condition = { .s32 = $val }";
        }
        elsif ($val =~ /^$NUMBER_REGEX$/ and $enumtype =~ /^sai_u?int(\d+)_t$/)
        {
            my $n = $1;
            my $item = ($enumtype =~ /uint/) ? "u$n" : "s$n";

            WriteSource "    .attrid = $attrid,";
            WriteSource "    .condition = { .$item = $val }";
        }
        else
        {
            LogError "unknown $name value: $val on $attr $enumtype";
            return "";
        }

        WriteSource "};";

        $count++;
    }

    WriteSource "const sai_attr_condition_t* const sai_metadata_${name}s_${attr}\[\] = {";

    $count = 0;

    for my $cond (@conditions)
    {
        WriteSource "    &sai_metadata_${name}_${attr}_$count,";

        $count++;
    }

    WriteSource "    NULL";

    WriteSource "};";

    return "sai_metadata_${name}s_${attr}";
}

sub ProcessConditions
{
    ProcessConditionsGeneric(@_, "condition");
}

sub ProcessConditionsLen
{
    my ($attr, $value) = @_;

    return "0" if not defined $value;

    my @conditions = @{ $value };

    # NOTE: number returned is -1 since first item is condition type

    return $#conditions;
}

sub ProcessValidOnlyType
{
    my ($attr, $value) = @_;

    return "SAI_ATTR_CONDITION_TYPE_NONE" if not defined $value;

    return @{$value}[0];
}

sub ProcessValidOnly
{
    ProcessConditionsGeneric(@_, "validonly");
}

sub ProcessValidOnlyLen
{
    my ($attr, $value) = @_;

    return "0" if not defined $value;

    my @conditions = @{ $value };

    # NOTE: number returned is -1 since first item is condition type

    return $#conditions;
}

sub ProcessAllowRepeat
{
    my ($attr, $value) = @_;

    return "false" if not defined $value;

    return $value;
}

sub ProcessAllowMixed
{
    my ($attr, $value) = @_;

    return "false" if not defined $value;

    return $value;
}

sub ProcessAllowEmpty
{
    my ($attr, $value) = @_;

    return "false" if not defined $value;

    return $value;
}

sub ProcessAttrName
{
    my ($attr, $type) = @_;

    return "\"$attr\"";
}

sub ProcessIsAclField
{
    my $attr = shift;

    return "true" if $attr =~ /^SAI_ACL_ENTRY_ATTR_FIELD_\w+$/;

    return "false";
}

sub ProcessIsAclAction
{
    my $attr = shift;

    return "true" if $attr =~ /^SAI_ACL_ENTRY_ATTR_ACTION_\w+$/;

    return "false";
}

sub ProcessBrief
{
    my ($attr, $brief) = @_;

    if (not defined $brief or $brief eq "")
    {
        LogWarning "missing brief description for $attr";
        return "";
    }

    if ($brief =~ m!([^\x20-\x7e]|\\|")!is)
    {
        LogWarning "Not allowed char '$1' in brief description on $attr: $brief";
    }

    if (length $brief > 200)
    {
        LogWarning "Long brief > 200 on $attr:\n - $brief";
    }

    return "\"$brief\"";
}

sub ProcessIsPrimitive
{
    my ($attr, $type) = @_;

    return "false" if $type =~ /(_list_t|acl_capability_t)/;

    return "true";
}

sub ProcessSingleObjectType
{
    my ($typedef, $objecttype) = @_;

    my $enum = $SAI_ENUMS{$typedef};

    my @values = @{ $enum->{values} };

    for my $attr (@values)
    {
        if (not defined $METADATA{$typedef} or not defined $METADATA{$typedef}{$attr})
        {
            LogError "metadata is missing for $attr";
            next;
        }

        my %meta = %{ $METADATA{$typedef}{$attr} };

        next if defined $meta{ignore};

        my $type            = ProcessType($attr, $meta{type});
        my $attrname        = ProcessAttrName($attr, $meta{type});
        my $flags           = ProcessFlags($attr, $meta{flags});
        my $allownull       = ProcessAllowNull($attr, $meta{allownull});
        my $objects         = ProcessObjects($attr, $meta{objects});
        my $objectslen      = ProcessObjectsLen($attr, $meta{objects});
        my $allowrepeat     = ProcessAllowRepeat($attr, $meta{allowrepeat});
        my $allowmixed      = ProcessAllowMixed($attr, $meta{allowmixed});
        my $allowempty      = ProcessAllowEmpty($attr, $meta{allowempty});
        my $defvaltype      = ProcessDefaultValueType($attr, $meta{default});
        my $defval          = ProcessDefaultValue($attr, $meta{default}, $meta{type});
        my $defvalot        = ProcessDefaultValueObjectType($attr, $meta{default}, $meta{type});
        my $defvalattrid    = ProcessDefaultValueAttrId($attr, $meta{default}, $meta{type});
        my $storedefaultval = ProcessStoreDefaultValue($attr, $meta{default}, $meta{flags});
        my $isenum          = ProcessIsEnum($attr, $meta{type});
        my $isenumlist      = ProcessIsEnumList($attr, $meta{type});
        my $enummetadata    = ProcessEnumMetadata($attr, $meta{type});
        my $conditiontype   = ProcessConditionType($attr, $meta{condition});
        my $conditions      = ProcessConditions($attr, $meta{condition}, $meta{type});
        my $conditionslen   = ProcessConditionsLen($attr, $meta{condition});
        my $validonlytype   = ProcessValidOnlyType($attr, $meta{validonly});
        my $validonly       = ProcessValidOnly($attr, $meta{validonly}, $meta{type});
        my $validonlylen    = ProcessValidOnlyLen($attr, $meta{validonly});
        my $isvlan          = ProcessIsVlan($attr, $meta{isvlan}, $meta{type});
        my $getsave         = ProcessGetSave($attr, $meta{getsave});
        my $isaclfield      = ProcessIsAclField($attr);
        my $isaclaction     = ProcessIsAclAction($attr);
        my $brief           = ProcessBrief($attr, $meta{brief});
        my $isprimitive     = ProcessIsPrimitive($attr, $meta{type});

        my $ismandatoryoncreate = ($flags =~ /MANDATORY/)       ? "true" : "false";
        my $iscreateonly        = ($flags =~ /CREATE_ONLY/)     ? "true" : "false";
        my $iscreateandset      = ($flags =~ /CREATE_AND_SET/)  ? "true" : "false";
        my $isreadonly          = ($flags =~ /READ_ONLY/)       ? "true" : "false";
        my $iskey               = ($flags =~ /KEY/)             ? "true" : "false";

        WriteSource "const sai_attr_metadata_t sai_metadata_attr_$attr = {";

        WriteSource "    .objecttype                    = $objecttype,";
        WriteSource "    .attrid                        = $attr,";
        WriteSource "    .attridname                    = $attrname,";
        WriteSource "    .brief                         = $brief,";
        WriteSource "    .attrvaluetype                 = $type,";
        WriteSource "    .flags                         = $flags,";
        WriteSource "    .allowedobjecttypes            = $objects,";
        WriteSource "    .allowedobjecttypeslength      = $objectslen,";
        WriteSource "    .allowrepetitiononlist         = $allowrepeat,";
        WriteSource "    .allowmixedobjecttypes         = $allowmixed,";
        WriteSource "    .allowemptylist                = $allowempty,";
        WriteSource "    .allownullobjectid             = $allownull,";
        WriteSource "    .isoidattribute                = ($objectslen > 0),";
        WriteSource "    .defaultvaluetype              = $defvaltype,";
        WriteSource "    .defaultvalue                  = $defval,";
        WriteSource "    .defaultvalueobjecttype        = $defvalot,";
        WriteSource "    .defaultvalueattrid            = $defvalattrid,";
        WriteSource "    .storedefaultvalue             = $storedefaultval,";
        WriteSource "    .isenum                        = $isenum,";
        WriteSource "    .isenumlist                    = $isenumlist,";
        WriteSource "    .enummetadata                  = $enummetadata,";
        WriteSource "    .conditiontype                 = $conditiontype,";
        WriteSource "    .conditions                    = $conditions,";
        WriteSource "    .conditionslength              = $conditionslen,";
        WriteSource "    .isconditional                 = ($conditionslen != 0),";
        WriteSource "    .validonlytype                 = $validonlytype,";
        WriteSource "    .validonly                     = $validonly,";
        WriteSource "    .validonlylength               = $validonlylen,";
        WriteSource "    .isvalidonly                   = ($validonlylen != 0),";
        WriteSource "    .getsave                       = $getsave,";
        WriteSource "    .isvlan                        = $isvlan,";
        WriteSource "    .isaclfield                    = $isaclfield,";
        WriteSource "    .isaclaction                   = $isaclaction,";
        WriteSource "    .ismandatoryoncreate           = $ismandatoryoncreate,";
        WriteSource "    .iscreateonly                  = $iscreateonly,";
        WriteSource "    .iscreateandset                = $iscreateandset,";
        WriteSource "    .isreadonly                    = $isreadonly,";
        WriteSource "    .iskey                         = $iskey,";
        WriteSource "    .isprimitive                   = $isprimitive,";

        WriteSource "};";

        # check enum attributes if their names are ending on enum name

        CheckEnumNaming($attr, $meta{type}) if $isenum eq "true" or $isenumlist eq "true";
    }
}

sub CheckEnumNaming
{
    my ($attr, $type) = @_;

    LogError "can't match sai type on '$type'" if not $type =~/.*sai_(\w+)_t/;

    my $enumTypeName = uc($1);

    return if $attr =~ /_${enumTypeName}_LIST$/;
    return if $attr =~ /_$enumTypeName$/;

    $attr =~/SAI_(\w+?)_ATTR(_\w+)/;

    my $attrObjectType = $1;
    my $attrSuffix = $2;

    if ($enumTypeName =~/^${attrObjectType}_(\w+)$/)
    {
        my $enumTypeNameSuffix = $1;

        return if $attrSuffix =~/_$enumTypeNameSuffix$/;

        LogError "enum starts by object type $attrObjectType but not ending on $enumTypeNameSuffix in $enumTypeName";
    }

    LogError "$type == $attr not ending on enum name $enumTypeName";
}

sub CreateMetadata
{
    for my $key (sort keys %SAI_ENUMS)
    {
        next if not $key =~ /^(sai_(\w+)_attr_t)$/;

        my $typedef = $1;
        my $objtype = "SAI_OBJECT_TYPE_" . uc($2);

        ProcessSingleObjectType($typedef, $objtype);
    }
}

sub ProcessSaiStatus
{
    my $filename = "../inc/saistatus.h";

    open(my $fh, '<', $filename) or die "Could not open file '$filename' $!";

    my @values = ();

    WriteSectionComment "Extra SAI statuses";

    while (my $line = <$fh>)
    {
        next if not $line =~ /define\s+(SAI_STATUS_\w+).+(0x00\w+)/;

        my $status = $1;
        my $base = $2;

        push@values,$status;

        next if not ($status =~ /(SAI_\w+)_0$/);

        for my $idx (1..10)
        {
            $status = "$1_$idx";

            WriteHeader "#define $status  SAI_STATUS_CODE(($base + ${idx}))";

            push@values,$status;
        }
    }

    close $fh;

    $SAI_ENUMS{"sai_status_t"}{values} = \@values;
    $SAI_ENUMS{"sai_status_t"}{flagsenum} = "true";
}

sub CreateMetadataForAttributes
{
    my @objects = @{ $SAI_ENUMS{sai_object_type_t}{values} };

    for my $ot (@objects)
    {

        if (not $ot =~ /^SAI_OBJECT_TYPE_(\w+)$/)
        {
            LogError "invalid obejct type '$ot'";
            next;
        }

        my $type = "sai_" . lc($1) . "_attr_t";

        if (not defined $SAI_ENUMS{$type})
        {
            my @empty = ();

            $SAI_ENUMS{$type}{values} = \@empty;
        }

        WriteSource "const sai_attr_metadata_t* const sai_metadata_object_type_$type\[\] = {";

        my @values = @{ $SAI_ENUMS{$type}{values} };

        for my $value (@values)
        {
            next if defined $METADATA{$type}{$value}{ignore};

            WriteSource "    &sai_metadata_attr_$value,";
        }

        WriteSource "    NULL";
        WriteSource "};";
    }

    WriteHeader "extern const sai_attr_metadata_t* const* const sai_metadata_attr_by_object_type[];";
    WriteSource "const sai_attr_metadata_t* const* const sai_metadata_attr_by_object_type[] = {";

    for my $ot (@objects)
    {
        if (not $ot =~ /^SAI_OBJECT_TYPE_(\w+)$/)
        {
            LogError "invalid obejct type '$ot'";
            next;
        }

        my $type = "sai_" . lc($1) . "_attr_t";

        WriteSource "    sai_metadata_object_type_$type,";
    }

    WriteSource "    NULL";
    WriteSource "};";

    my $count = @objects;

    WriteHeader "extern const size_t sai_metadata_attr_by_object_type_count;";
    WriteSource "const size_t sai_metadata_attr_by_object_type_count = $count;";
}

sub CreateEnumHelperMethod
{
    my $key = shift;

    return if not $key =~ /^sai_(\w+)_t/;

    WriteSource "const char* sai_metadata_get_$1_name(";
    WriteSource "        _In_ $key value)";
    WriteSource "{";
    WriteSource "    return sai_metadata_get_enum_value_name(&sai_metadata_enum_$key, value);";
    WriteSource "}";

    WriteHeader "extern const char* sai_metadata_get_$1_name(";
    WriteHeader "        _In_ $key value);";
}

sub CreateEnumHelperMethods
{
    WriteSectionComment "Get enum name helper methods";

    for my $key (sort keys %SAI_ENUMS)
    {
        next if $key =~/_attr_t$/;

        CreateEnumHelperMethod($key);
    }
}

sub ProcessIsNonObjectId
{
    my $struct = shift;

    return "false" if not defined $struct;

    return "true";
}

sub ProcessStructValueType
{
    my $type = shift;

    return "SAI_ATTR_VALUE_TYPE_OBJECT_ID"      if $type eq "sai_object_id_t";
    return "SAI_ATTR_VALUE_TYPE_MAC"            if $type eq "sai_mac_t";
    return "SAI_ATTR_VALUE_TYPE_IP_ADDRESS"     if $type eq "sai_ip_address_t";
    return "SAI_ATTR_VALUE_TYPE_IP_PREFIX"      if $type eq "sai_ip_prefix_t";
    return "SAI_ATTR_VALUE_TYPE_UINT16"         if $type eq "sai_vlan_id_t";
    return "SAI_ATTR_VALUE_TYPE_INT32"          if $type =~ /^sai_\w+_type_t$/; # enum

    LogError "invalid struct member value type $type";

    return -1;
}

sub ProcessStructIsVlan
{
    my $type = shift;

    return "true" if $type eq "sai_vlan_id_t";

    return "false";
}

sub ProcessStructObjects
{
    my ($rawname, $key, $struct) = @_;

    my $type = $struct->{type};

    return "NULL" if not $type eq "sai_object_id_t";

    WriteSource "const sai_object_type_t sai_metadata_struct_member_sai_${rawname}_t_${key}_allowed_objects[] = {";

    my $objects = $struct->{objects};

    for my $obj (@{ $objects })
    {
        WriteSource "    $obj,";
    }

    WriteSource "};";

    return "sai_metadata_struct_member_sai_${rawname}_t_${key}_allowed_objects";
}

sub ProcessStructObjectLen
{
    my ($rawname, $key, $struct) = @_;

    my $type = $struct->{type};

    return 0 if not $type eq "sai_object_id_t";

    my @objects = @{ $struct->{objects} };

    my $count = @objects;

    return $count;
}

sub ProcessStructEnumData
{
    my $type = shift;

    return "&sai_metadata_enum_$type" if $type =~ /^sai_\w+_type_t$/; # enum

    return "NULL";
}

sub ProcessStructIsEnum
{
    my $type = shift;

    return "true" if $type =~ /^sai_\w+_type_t$/; # enum

    return "false";
}

sub ProcessStructGetOid
{
    my ($type, $key, $rawname) = @_;

    return "NULL" if $type ne "sai_object_id_t";

    my $fname = "sai_metadata_struct_member_get_sai_${rawname}_t_${key}";

    WriteSource "sai_object_id_t $fname(";
    WriteSource "        _In_ const sai_object_meta_key_t *object_meta_key)";
    WriteSource "{";
    WriteSource "    return object_meta_key->objectkey.key.${rawname}.${key};";
    WriteSource "}";

    return $fname;
}

sub ProcessStructSetOid
{
    my ($type, $key, $rawname) = @_;

    return "NULL" if $type ne "sai_object_id_t";

    my $fname = "sai_metadata_struct_member_set_sai_${rawname}_t_${key}";

    WriteSource "void $fname(";
    WriteSource "        _Inout_ sai_object_meta_key_t *object_meta_key,";
    WriteSource "        _In_ sai_object_id_t oid)";
    WriteSource "{";
    WriteSource "    object_meta_key->objectkey.key.${rawname}.${key} = oid;";
    WriteSource "}";

    return $fname;
}

sub ProcessStructMembers
{
    my ($struct, $ot, $rawname) = @_;

    return "NULL" if not defined $struct;

    my @keys = GetStructKeysInOrder($struct);

    if ($keys[0] ne "switch_id")
    {
        LogError "switch_id is not first item in $rawname";
    }

    for my $key (@keys)
    {
        my $valuetype   = ProcessStructValueType($struct->{$key}{type});
        my $isvlan      = ProcessStructIsVlan($struct->{$key}{type});
        my $objects     = ProcessStructObjects($rawname, $key, $struct->{$key});
        my $objectlen   = ProcessStructObjectLen($rawname, $key, $struct->{$key});
        my $isenum      = ProcessStructIsEnum($struct->{$key}{type});
        my $enumdata    = ProcessStructEnumData($struct->{$key}{type});
        my $getoid      = ProcessStructGetOid($struct->{$key}{type}, $key, $rawname);
        my $setoid      = ProcessStructSetOid($struct->{$key}{type}, $key, $rawname);

        WriteSource "const sai_struct_member_info_t sai_metadata_struct_member_sai_${rawname}_t_$key = {";

        WriteSource "    .membervaluetype           = $valuetype,";
        WriteSource "    .membername                = \"$key\",";
        WriteSource "    .isvlan                    = $isvlan,";
        WriteSource "    .allowedobjecttypes        = $objects,";
        WriteSource "    .allowedobjecttypeslength  = $objectlen,";
        WriteSource "    .isenum                    = $isenum,";
        WriteSource "    .enummetadata              = $enumdata,";
        WriteSource "    .getoid                    = $getoid,";
        WriteSource "    .setoid                    = $setoid,";

        # TODO allow null

        WriteSource "};";

        if ($objectlen > 0 and not $key =~ /_id$/)
        {
            LogWarning "struct member key '$key' should end at _id in sai_${rawname}_t";
        }
    }

    WriteSource "const sai_struct_member_info_t* const sai_metadata_struct_members_sai_${rawname}_t[] = {";

    for my $key (@keys)
    {
        WriteSource "    &sai_metadata_struct_member_sai_${rawname}_t_$key,";
    }

    WriteSource "    NULL";
    WriteSource "};";

    return "sai_metadata_struct_members_sai_${rawname}_t";
}

sub ProcessStructMembersCount
{
    my $struct = shift;

    return "0" if not defined $struct;

    my $count = keys %$struct;

    return $count;
}

sub ProcessRevGraph
{
    #
    # Purpose of this method is to generate metadata where current object type
    # is used since currentrly if we have attribute metadata we can easly scan
    # attributes with oids values and extract information of object being used
    # on that attribute, scanning all attributes of that object type we have
    # dependency graph
    #
    # but what we create here is reverse dependency graph it will tell us on
    # which object and which attrubute current object type is used
    #
    # we can of course create both graphs right at the same time
    #

    my %REVGRAPH = GetReverseDependencyGraph();

    my $objectType = shift;

    if (not defined $REVGRAPH{$objectType})
    {
        # some objects are not used, so they will be not defined
        return "NULL";
    }

    my @dep = @{ $REVGRAPH{$objectType} };

    @dep = sort @dep;

    my $index = 0;

    my @membernames = ();;

    for my $dep (@dep)
    {
        my ($depObjectType, $attrId) = split/,/,$dep;

        my $membername = "sai_metadata_${objectType}_rev_graph_member_$index";

        push@membernames,$membername;

        WriteSource "const sai_rev_graph_member_t $membername = {";

        WriteSource "    .objecttype          = $objectType,";
        WriteSource "    .depobjecttype       = $depObjectType,";

        if ($attrId =~ /^SAI_\w+_ATTR_\w+/)
        {
            # this is attribute

            WriteSource "    .attrmetadata        = &sai_metadata_attr_$attrId,";
            WriteSource "    .structmember        = NULL,";
        }
        else
        {
            # this is struct member inside non object id struct

            my $DEPOT = lc ($1) if $depObjectType =~ /SAI_OBJECT_TYPE_(\w+)/;

            WriteSource "    .attrmetadata        = NULL,";
            WriteSource "    .structmember        = &sai_metadata_struct_member_sai_${DEPOT}_t_$attrId,";
        }

        WriteSource "};";

        $index++;
    }

    WriteSource "const sai_rev_graph_member_t* const sai_metadata_${objectType}_rev_graph_members[] = {";

    for my $mn (@membernames)
    {
        WriteSource "    &$mn,";
    }

    WriteSource "    NULL,";

    WriteSource "};";

    return "sai_metadata_${objectType}_rev_graph_members";
}

sub ProcessRevGraphCount
{
    my %REVGRAPH = GetReverseDependencyGraph();

    my $objectType = shift;

    if (not defined $REVGRAPH{$objectType})
    {
        return 0;
    }

    my $count = @{ $REVGRAPH{$objectType} };

    return $count;
}

sub CreateStructNonObjectId
{
    my @objects = @{ $SAI_ENUMS{sai_object_type_t}{values} };

    for my $ot (@objects)
    {
        if (not $ot =~ /^SAI_OBJECT_TYPE_(\w+)$/)
        {
            LogError "invalid obejct type '$ot'";
            next;
        }

        next if $1 eq "NULL" or $1 eq "MAX";

        my $type = "sai_" . lc($1) . "_attr_t";

        my $enum  = "&sai_metadata_enum_${type}";

        my $struct = $NON_OBJECT_ID_STRUCTS{$ot};

        my $structmembers = ProcessStructMembers($struct, $ot ,lc($1));
    }
}

sub ProcessStructMembersName
{
    my ($struct, $ot, $rawname) = @_;

    return "NULL" if not defined $struct;

    return "sai_metadata_struct_members_sai_${rawname}_t";
}

sub ProcessCreate
{
    my $struct = shift;
    my $ot = shift;

    my $small = lc($1) if $ot =~ /SAI_OBJECT_TYPE_(\w+)/;

    my $api = $OBJTOAPIMAP{$ot};

    WriteSource "sai_status_t sai_metadata_generic_create_$ot(";
    WriteSource "        _Inout_ sai_object_meta_key_t *meta_key,";
    WriteSource "        _In_ sai_object_id_t switch_id,";
    WriteSource "        _In_ uint32_t attr_count,";
    WriteSource "        _In_ const sai_attribute_t *attr_list)";
    WriteSource "{";

    if (IsSpecialObject($ot))
    {
        WriteSource "    return SAI_STATUS_NOT_IMPLEMENTED;";
    }
    elsif (not defined $struct)
    {
        if ($small eq "switch")
        {
            WriteSource "    return sai_metadata_sai_${api}_api->create_$small(&meta_key->objectkey.key.object_id, attr_count, attr_list);";
        }
        else
        {
            WriteSource "    return sai_metadata_sai_${api}_api->create_$small(&meta_key->objectkey.key.object_id, switch_id, attr_count, attr_list);";
        }
    }
    else
    {
        WriteSource "    return sai_metadata_sai_${api}_api->create_$small(&meta_key->objectkey.key.$small, attr_count, attr_list);";
    }

    WriteSource "}";

    return "sai_metadata_generic_create_$ot";
}

sub ProcessRemove
{
    my $struct = shift;
    my $ot = shift;

    my $small = lc($1) if $ot =~ /SAI_OBJECT_TYPE_(\w+)/;

    my $api = $OBJTOAPIMAP{$ot};

    WriteSource "sai_status_t sai_metadata_generic_remove_$ot(";
    WriteSource "        _In_ const sai_object_meta_key_t *meta_key)";
    WriteSource "{";

    if (IsSpecialObject($ot))
    {
        WriteSource "    return SAI_STATUS_NOT_IMPLEMENTED;";
    }
    elsif (not defined $struct)
    {
        WriteSource "    return sai_metadata_sai_${api}_api->remove_$small(meta_key->objectkey.key.object_id);";
    }
    else
    {
        WriteSource "    return sai_metadata_sai_${api}_api->remove_$small(&meta_key->objectkey.key.$small);";
    }

    WriteSource "}";

    return "sai_metadata_generic_remove_$ot";
}

sub ProcessSet
{
    my $struct = shift;
    my $ot = shift;

    my $small = lc($1) if $ot =~ /SAI_OBJECT_TYPE_(\w+)/;

    my $api = $OBJTOAPIMAP{$ot};

    WriteSource "sai_status_t sai_metadata_generic_set_$ot(";
    WriteSource "        _In_ const sai_object_meta_key_t *meta_key,";
    WriteSource "        _In_ const sai_attribute_t *attr)";
    WriteSource "{";

    if (IsSpecialObject($ot))
    {
        WriteSource "    return SAI_STATUS_NOT_IMPLEMENTED;";
    }
    elsif (not defined $struct)
    {
        WriteSource "    return sai_metadata_sai_${api}_api->set_${small}_attribute(meta_key->objectkey.key.object_id, attr);";
    }
    else
    {
        WriteSource "    return sai_metadata_sai_${api}_api->set_${small}_attribute(&meta_key->objectkey.key.$small, attr);";
    }

    WriteSource "}";

    return "sai_metadata_generic_set_$ot";
}

sub ProcessGet
{
    my $struct = shift;
    my $ot = shift;

    my $small = lc($1) if $ot =~ /SAI_OBJECT_TYPE_(\w+)/;

    my $api = $OBJTOAPIMAP{$ot};

    WriteSource "sai_status_t sai_metadata_generic_get_$ot(";
    WriteSource "        _In_ const sai_object_meta_key_t *meta_key,";
    WriteSource "        _In_ uint32_t attr_count,";
    WriteSource "        _Inout_ sai_attribute_t *attr_list)";
    WriteSource "{";

    if (IsSpecialObject($ot))
    {
        WriteSource "    return SAI_STATUS_NOT_IMPLEMENTED;";
    }
    elsif (not defined $struct)
    {
        WriteSource "    return sai_metadata_sai_${api}_api->get_${small}_attribute(meta_key->objectkey.key.object_id, attr_count, attr_list);";
    }
    else
    {
        WriteSource "    return sai_metadata_sai_${api}_api->get_${small}_attribute(&meta_key->objectkey.key.$small, attr_count, attr_list);";
    }

    WriteSource "}";

    return "sai_metadata_generic_get_$ot";
}

sub CreateApis
{
    WriteSectionComment "Global SAI API declarations";

    for my $key (sort keys %APITOOBJMAP)
    {
        WriteSource "sai_${key}_api_t *sai_metadata_sai_${key}_api = NULL;";
        WriteHeader "extern sai_${key}_api_t *sai_metadata_sai_${key}_api;";
    }
}

sub CreateApisStruct
{
    my @apis = @{ $SAI_ENUMS{sai_api_t}{values} };

    WriteSectionComment "All APIs struct";

    WriteHeader "typedef struct _sai_apis_t {";

    for my $api (@apis)
    {
        $api =~ /^SAI_API_(\w+)/;

        $api = lc($1);

        next if $api =~/unspecified/;

        WriteHeader "    sai_${api}_api_t* ${api}_api;";
    }

    WriteHeader "} sai_apis_t;";
}

sub CreateApisQuery
{
    WriteSectionComment "SAI API query";

    WriteHeader "typedef sai_status_t (*sai_api_query_fn)(";
    WriteHeader "        _In_ sai_api_t sai_api_id,";
    WriteHeader "        _Out_ void** api_method_table);";

    # for switch we need to generate wrapper, for others we can use pointers
    # so we don't need to use meta key then

    WriteSource "int sai_metadata_apis_query(";
    WriteSource "        _In_ const sai_api_query_fn api_query,";
    WriteSource "        _Inout_ sai_apis_t *apis)";
    WriteSource "{";
    WriteSource "    sai_status_t status = SAI_STATUS_SUCCESS;";
    WriteSource "    int count = 0;";

    WriteSource "    if (api_query == NULL)";
    WriteSource "    {";

    for my $key (sort keys %APITOOBJMAP)
    {
        WriteSource "        sai_metadata_sai_${key}_api = NULL;";
        WriteSource "        apis->${key}_api = NULL;";
    }

    WriteSource "        return count;";
    WriteSource "    }";

    for my $key (sort keys %APITOOBJMAP)
    {
        my $api = uc("SAI_API_${key}");

        WriteSource "    status = api_query($api, (void**)&sai_metadata_sai_${key}_api);";
        WriteSource "    apis->${key}_api = sai_metadata_sai_${key}_api;";
        WriteSource "    if (status != SAI_STATUS_SUCCESS)";
        WriteSource "    {";
        WriteSource "        count++;";
        WriteSource "        const char *name = sai_metadata_get_enum_value_name(&sai_metadata_enum_sai_status_t, status);";
        WriteSource "        SAI_META_LOG_WARN(\"failed to query api $api: %s (%d)\", name, status);";
        WriteSource "    }";
    }

    WriteSource "    return count; /* number of unsuccesfull apis */";

    WriteSource "}";

    WriteHeader "extern int sai_metadata_apis_query(";
    WriteHeader "        _In_ const sai_api_query_fn api_query,";
    WriteHeader "        _Inout_ sai_apis_t *apis);";
}

sub CreateObjectInfo
{
    WriteSectionComment "Object info metadata";

    my @objects = @{ $SAI_ENUMS{sai_object_type_t}{values} };

    for my $ot (@objects)
    {
        if (not $ot =~ /^SAI_OBJECT_TYPE_(\w+)$/)
        {
            LogError "invalid obejct type '$ot'";
            next;
        }

        next if $1 eq "NULL" or $1 eq "MAX";

        my $type = "sai_" . lc($1) . "_attr_t";

        my $start = "SAI_" . uc($1) . "_ATTR_START";
        my $end   = "SAI_" . uc($1) . "_ATTR_END";

        my $enum  = "&sai_metadata_enum_${type}";

        my $struct = $NON_OBJECT_ID_STRUCTS{$ot};

        #
        # here we need to only generate struct member names
        # since we use those members in rev graph entries
        # so struct members must be generated previously
        #

        my $isnonobjectid       = ProcessIsNonObjectId($struct, $ot);
        my $structmembers       = ProcessStructMembersName($struct, $ot ,lc($1));
        my $structmemberscount  = ProcessStructMembersCount($struct, $ot);
        my $revgraph            = ProcessRevGraph($ot);
        my $revgraphcount       = ProcessRevGraphCount($ot);
        my $attrmetalength      = @{ $SAI_ENUMS{$type}{values} };

        my $create = ProcessCreate($struct, $ot);
        my $remove = ProcessRemove($struct, $ot);
        my $set = ProcessSet($struct, $ot);
        my $get = ProcessGet($struct, $ot);

        WriteHeader "extern const sai_object_type_info_t sai_metadata_object_type_info_$ot;";

        WriteSource "const sai_object_type_info_t sai_metadata_object_type_info_$ot = {";
        WriteSource "    .objecttype           = $ot,";
        WriteSource "    .objecttypename       = \"$ot\",";
        WriteSource "    .attridstart          = $start,";
        WriteSource "    .attridend            = $end,";
        WriteSource "    .enummetadata         = $enum,";
        WriteSource "    .attrmetadata         = sai_metadata_object_type_$type,";
        WriteSource "    .attrmetadatalength   = $attrmetalength,";
        WriteSource "    .isnonobjectid        = $isnonobjectid,";
        WriteSource "    .isobjectid           = !$isnonobjectid,";
        WriteSource "    .structmembers        = $structmembers,";
        WriteSource "    .structmemberscount   = $structmemberscount,";
        WriteSource "    .revgraphmembers      = $revgraph,";
        WriteSource "    .revgraphmemberscount = $revgraphcount,";
        WriteSource "    .create               = $create,";
        WriteSource "    .remove               = $remove,";
        WriteSource "    .set                  = $set,";
        WriteSource "    .get                  = $get,";
        WriteSource "};";
    }

    WriteSectionComment "Object infos table";

    WriteHeader "extern const sai_object_type_info_t* const sai_metadata_all_object_type_infos[];";

    WriteSource "const sai_object_type_info_t* const sai_metadata_all_object_type_infos[] = {";

    for my $ot (@objects)
    {
        if (not $ot =~ /^SAI_OBJECT_TYPE_(\w+)$/)
        {
            LogError "invalid obejct type '$ot'";
            next;
        }

        if ($1 eq "NULL" or $1 eq "MAX")
        {
            WriteSource "    NULL,";
            next;
        }

        WriteSource "    &sai_metadata_object_type_info_$ot,";
    }

    WriteSource "    NULL";
    WriteSource "};";
}

sub ExtractObjectsFromDesc
{
    my ($struct, $member, $desc) = @_;

    $desc =~ s/@@/\n@@/g;

    while ($desc =~ /@@(\w+)(.+)/g)
    {
        my $tag = $1;
        my $val = $2;

        $val =~ s/\s+/ /g;
        $val =~ s/^\s*//;
        $val =~ s/\s*$//;

        next if not $tag eq "objects";

        return ProcessTagObjects($struct, $member, $val);
    }

    return undef;
}

sub ProcessSingleNonObjectId
{
    my $rawname = shift;

    my @types = @{ $SAI_ENUMS{sai_object_type_t}{values} };

    my $structname = "sai_${rawname}_t";

    my $ot = "SAI_OBJECT_TYPE_" .uc(${rawname});

    if (not grep(/$ot/,@types))
    {
        LogError "struct $structname does not correspont to known object type";
        return undef;
    }

    # NOTE: since this is a HASH then order of the members is not preserved as
    # they appear in struct definition

    my %struct = ExtractStructInfo($structname, "struct_");

    for my $member (GetStructKeysInOrder(\%struct))
    {
        my $type = $struct{$member}{type};
        my $desc = $struct{$member}{desc};

        # allowed entries on object structs

        if (not $type =~ /^sai_(mac|object_id|vlan_id|ip_address|ip_prefix|\w+_type)_t$/)
        {
            LogError "struct member $member type '$type' is not allowed on struct $structname";
            next;
        }

        next if not $type eq "sai_object_id_t";

        my $objects = ExtractObjectsFromDesc($structname, $member, $desc);

        if (not defined $objects)
        {
            LogError "no object type defined on $structname $member";
            next;
        }

        $struct{$member}{objects} = $objects;
    }

    return %struct;
}

sub ProcessNonObjectIdObjects
{
    my @rawnames = GetNonObjectIdStructNames();

    for my $rawname (@rawnames)
    {
        my %struct = ProcessSingleNonObjectId($rawname);

        my $objecttype = "SAI_OBJECT_TYPE_" . uc($rawname);

        $NON_OBJECT_ID_STRUCTS{$objecttype} = \%struct;
    }
}

sub CreateListOfAllAttributes
{
    # list will be used to find attribute metadata
    # based on attribute string name

    WriteSectionComment "List of all attributes";

    my %ATTRIBUTES = ();

    for my $key (sort keys %SAI_ENUMS)
    {
        next if not $key =~ /^(sai_(\w+)_attr_t)$/;

        my $typedef = $1;

        my $enum = $SAI_ENUMS{$typedef};

        my @values = @{ $enum->{values} };

        for my $attr (@values)
        {
            if (not defined $METADATA{$typedef} or not defined $METADATA{$typedef}{$attr})
            {
                LogError "metadata is missing for $attr";
                next;
            }

            next if defined $METADATA{$typedef}{$attr}{ignore};

            $ATTRIBUTES{$attr} = 1;
        }
    }

    WriteSource "const sai_attr_metadata_t* const sai_metadata_attr_sorted_by_id_name[] = {";
    WriteHeader "extern const sai_attr_metadata_t* const sai_metadata_attr_sorted_by_id_name[];";

    my @keys = sort keys %ATTRIBUTES;

    for my $attr (@keys)
    {
        WriteSource "    &sai_metadata_attr_$attr,";
    }

    my $count = @keys;

    WriteSource "    NULL";
    WriteSource "};";

    WriteSource "const size_t sai_metadata_attr_sorted_by_id_name_count = $count;";
    WriteHeader "extern const size_t sai_metadata_attr_sorted_by_id_name_count;";
}

sub CheckApiStructNames
{
    #
    # purpose of this check is to find out
    # whether sai_api_t enums match actual
    # struct of api declarations
    #

    my @values = @{ $SAI_ENUMS{"sai_api_t"}{values} };

    for my $value (@values)
    {
        next if $value eq "SAI_API_UNSPECIFIED";

        if (not $value =~ /^SAI_API_(\w+)$/)
        {
            LogError "invalie api name $value";
            next;
        }

        my $api = lc($1);

        my $structName = "sai_${api}_api_t";

        my $structFile = "struct_$structName.xml";

        # doxygen doubles underscores

        $structFile =~ s/_/__/g;

        my $file = "$XMLDIR/$structFile";

        if (not -e $file)
        {
            LogError "there is no struct $structName corresponding to api name $value";
        }
    }
}

sub CheckApiDefines
{
    #
    # purpose of this check is to check whether
    # all enum entries defined in sai_api_t
    # have corresponding structs defined for each
    # defined object like sai_fdb_api_t
    #

    my @apis = @{ $SAI_ENUMS{sai_api_t}{values} };

    for my $api (@apis)
    {
        my $short = lc($1) if $api =~/SAI_API_(\w+)/;

        next if $short eq "unspecified";

        if (not defined $APITOOBJMAP{$short})
        {
            LogError "$api is defined in sai.h but no corresponding struct for objects found";
        }
    }
}

sub ExtractApiToObjectMap
{
    #
    # Purpose is to get which object type
    # maps to which API, since multiple object types like acl
    # can map to one api structure
    #

    my @headers = GetHeaderFiles();

    for my $header (@headers)
    {
        my $data = ReadHeaderFile($header);

        my @lines = split/\n/,$data;

        my $empty = 0;
        my $emptydoxy = 0;

        my @objects = ();
        my $api = undef;

        for my $line (@lines)
        {
            if ($line =~ /typedef\s+enum\s+_sai_(\w+)_attr_t/)
            {
                push@objects,uc("SAI_OBJECT_TYPE_$1");
            }

            if ($line =~ /typedef\s+struct\s+_sai_(\w+)_api_t/)
            {
                $api = $1;
                last;
            }
        }

        if (not defined $api)
        {
            my $len = @objects;

            if ($len > 0)
            {
                LogError "api struct was not found in file $header, but objects are defined @objects";
                next;
            }

            next;
        }

        my $shortapi = $api;

        $shortapi =~ s/_//g;

        my $correct = "sai$shortapi.h";

        if ($header ne $correct)
        {
            LogWarning "File $header should be named $correct";
        }

        for my $obj(@objects)
        {
            $OBJTOAPIMAP{$obj} = $api;
        }

        $APITOOBJMAP{$api} = \@objects;
    }
}

sub GetReverseDependencyGraph
{
    #
    # Purpose of this method is to generate reverse
    # dependency graph of where object ID are used
    #

    my %REVGRAPH = ();

    my @objects = @{ $SAI_ENUMS{sai_object_type_t}{values} };

    for my $ot (@objects)
    {
        if (not $ot =~ /^SAI_OBJECT_TYPE_(\w+)$/)
        {
            LogError "invalid obejct type '$ot'";
            next;
        }

        my $otname = $1;

        my $typedef = lc("sai_${otname}_attr_t");

        next if $ot =~ /^SAI_OBJECT_TYPE_(MAX|NULL)$/;

        # for each objec types we need to scann all objects
        # also non object id structs

        my $enum = $SAI_ENUMS{$typedef};

        my @values = @{ $enum->{values} };

        for my $attr (@values)
        {
            # metadata of single attribute of this object type

            my $meta = $METADATA{$typedef}{$attr};

            next if not defined $meta->{objects};

            # we will also include RO attributes

            my @objects = @{ $meta->{objects} };

            my $attrid = $meta->{attrid};

            for my $usedot (@objects)
            {
                if (not defined $REVGRAPH{$usedot})
                {
                    my @arr = ();
                    $REVGRAPH{$usedot} = \@arr;
                }

                my $ref = $REVGRAPH{$usedot};
                push@$ref,"$ot,$attrid";
            }
        }

        next if not defined $NON_OBJECT_ID_STRUCTS{$ot};

        # handle non object id types

        my %struct = %{ $NON_OBJECT_ID_STRUCTS{$ot} };

        for my $key (sort keys %struct)
        {
            next if not defined $struct{$key}{objects};

            my @objs = @{ $struct{$key}{objects} };

            for my $usedot (@objs)
            {
                if (not defined $REVGRAPH{$usedot})
                {
                    my @arr = ();
                    $REVGRAPH{$usedot} = \@arr;
                }

                my $ref = $REVGRAPH{$usedot};
                push@$ref,"$ot,$key";
            }
        }
    }

    return %REVGRAPH;
}

sub WriteLoggerVariables
{
    #
    # logger requires 2 variables
    # - log level
    # - log function
    #
    # we can extract this to another source file saimetadatalogger.c
    # but now seems to be unnecessary
    #

    WriteSource "volatile sai_log_level_t sai_metadata_log_level = SAI_LOG_LEVEL_NOTICE;";
    WriteSource "volatile sai_metadata_log_fn sai_metadata_log = NULL;";
}

my %ProcessedItems = ();

sub ProcessStructItem
{
    my ($type, $struct, $allowPointers) = @_;

    $type = $1 if $struct =~ /^sai_(\w+)_list_t$/ and $type =~/^(\w+)\*$/;

    return if defined $ProcessedItems{$type};

    return if defined $SAI_ENUMS{$type}; # struct entry is enum

    return if $type eq "bool";

    return if $type =~/^sai_(u?int\d+|ip[46]|mac|cos|vlan_id|queue_index)_t/; # primitives, we could get that from defines
    return if $type =~/^u?int\d+_t/;
    return if $type =~/^sai_[su]\d+_list_t/;

    if ($type eq "sai_object_id_t" or $type eq "sai_object_list_t")
    {
        # NOTE: don't change that, we can't have object id's inside complicated structures

        LogError "type $type in $struct can't be used, please convert struct to new object type and this item to an attribute";
        return;
    }

    my %S = ();

    if ($type =~ /^union (\w+)::(\w+)\s*$/)
    {
        # union is special, but now since all unions are named
        # then members are not flattened anyway, and we need to examine
        # entries from union xml
        # XXX may require revisit if union names will be complicated

        my $unionStructName = $1;
        my $unionName = $2;

        $unionStructName =~ s/_/__/g;
        $unionName =~ s/_/__/g;

        my $filename = "union${unionStructName}_1_1$unionName.xml";

        %S = ExtractStructInfo($unionStructName, $filename);
    }
    else
    {
        %S = ExtractStructInfo($type, "struct_");
    }

    for my $key (sort keys %S)
    {
        my $item = $S{$key}{type};

        ProcessStructItem($item, $type);

        $ProcessedItems{$item} = 1;
    }
}

sub CheckAttributeValueUnion
{
    #
    # purpose of this test is to find out if attribute
    # union contains complex structures members that also contain
    # object id, all object ids should be simple object id member oid
    # or object list objlist, other complext structures containing
    # objects are NOT supported since it will be VERY HARD to track
    # object dependencies via metadata and comparison logic
    #

    my %Union = ExtractStructInfo("sai_attribute_value_t", "union_");

    my @primitives = qw/sai_acl_action_data_t sai_acl_field_data_t sai_pointer_t sai_object_id_t sai_object_list_t char/;

    for my $key (sort keys %Union)
    {
        my $type = $Union{$key}{type};

        next if $type eq "char[32]";
        next if $type =~/sai_u?int\d+_t/;
        next if $type =~/sai_[su]\d+_list_t/;

        next if grep(/^$type$/, @primitives);

        ProcessStructItem($type, "sai_attribute_value_t", 1);
    }
}

sub CreateNotificationStruct
{
    #
    # create notification struct for easier notification
    # manipulation in code
    #

    WriteSectionComment "SAI notifications struct";

    WriteHeader "typedef struct _sai_switch_notifications_t {";

    for my $name (sort keys %NOTIFICATIONS)
    {
        if (not $name =~ /^sai_(\w+)_notification_fn/)
        {
            LogWarning "notification function $name is not ending on _notification_fn";
            next;
        }

        WriteHeader "    $name on_$1;";
    }

    WriteHeader "} sai_switch_notifications_t;";
}

sub CreateNotificationEnum
{
    #
    # create notification enum for easier notification
    # manipulation in code
    #

    WriteSectionComment "SAI notifications enum";

    my $typename = "sai_switch_notification_type_t";

    WriteHeader "typedef enum _$typename {";

    my $prefix = uc $typename;

    chop $prefix;

    my @values = ();

    for my $name (sort keys %NOTIFICATIONS)
    {
        if (not $name =~ /^sai_(\w+)_notification_fn/)
        {
            LogWarning "notification function '$name' is not ending on _notification_fn";
            next;
        }

        $name = uc $1;

        WriteHeader "    ${prefix}$name,";

        push @values, "${prefix}$name";
    }

    WriteHeader "} $typename;";

    $SAI_ENUMS{$typename}{values} = \@values;

    WriteSectionComment "sai_switch_notification_type_t metadata";

    ProcessSingleEnum($typename, $typename, $prefix);

    WriteSectionComment "Get sai_switch_notification_type_t helper method";

    CreateEnumHelperMethod("sai_switch_notification_type_t");
}

sub WriteHeaderHeader
{
    WriteSectionComment "AUTOGENERATED FILE! DO NOT EDIT";

    WriteHeader "#ifndef __SAI_METADATA_H__";
    WriteHeader "#define __SAI_METADATA_H__";

    WriteHeader "#include <sai.h>";
    WriteHeader "#include \"saimetadatatypes.h\"";
    WriteHeader "#include \"saimetadatautils.h\"";
    WriteHeader "#include \"saimetadatalogger.h\"";
    WriteHeader "#include \"saiserialize.h\"";
}

sub WriteHeaderFotter
{
    WriteHeader "#endif /* __SAI_METADATA_H__ */";
}

sub ProcessXmlFiles
{
    for my $file (GetXmlFiles($XMLDIR))
    {
        LogInfo "Processing $file";

        ProcessXmlFile("$XMLDIR/$file");
    }

    #print Dumper %SAI_ENUMS;
}

sub ProcessValues
{
    my ($refUnion, $refValueTypes, $refValueTypesToVt) = @_;

    for my $key (keys %$refUnion)
    {
        my $type = $refUnion->{$key}->{type};

        if (not $type =~ /^sai_(\w+)_t$/)
        {
            next if $type eq "char[32]" or $type eq "bool";

            LogWarning "skipping type $type, FIXME";
            next;
        }

        my $innername = $1;

        $innername =~ s/^s(\d+)/INT$1/;
        $innername =~ s/^u(\d+)/UINT$1/;
        $innername =~ s/^ip(\d+)/IPV$1/;

        $refValueTypes->{$type} = $key;
        $refValueTypesToVt->{$type} = uc($innername);
    }
}

sub PopulateValueTypes
{
    my %Union = ExtractStructInfo("sai_attribute_value_t", "union_");

    ProcessValues(\%Union, \%VALUE_TYPES, \%VALUE_TYPES_TO_VT);

    %Union = ExtractStructInfo("sai_acl_action_data_t", "union__sai__acl__action__data__t_1_1__parameter.xml");

    ProcessValues(\%Union, \%ACL_ACTION_TYPES, \%ACL_ACTION_TYPES_TO_VT);

    %Union = ExtractStructInfo("sai_acl_field_data_t", "union__sai__acl__field__data__t_1_1__data.xml");

    ProcessValues(\%Union, \%ACL_FIELD_TYPES, \%ACL_FIELD_TYPES_TO_VT);
}

sub CreateObjectTypeMap
{
    map { $OBJECT_TYPE_MAP{$_} = $_ } @{ $SAI_ENUMS{sai_object_type_t}{values} };
}

#
# MAIN
#

CheckHeadersStyle() if not defined $optionDisableStyleCheck;

ExtractApiToObjectMap();

GetStructLists();

PopulateValueTypes();

ProcessXmlFiles();

CreateObjectTypeMap();

WriteHeaderHeader();

CreateMetadataHeaderAndSource();

CreateMetadata();

CreateMetadataForAttributes();

CreateEnumHelperMethods();

ProcessNonObjectIdObjects();

CreateStructNonObjectId();

CreateApis();

CreateApisStruct();

CreateApisQuery();

CreateObjectInfo();

CreateListOfAllAttributes();

CheckApiStructNames();

CheckApiDefines();

CheckAttributeValueUnion();

CreateNotificationStruct();

CreateNotificationEnum();

CreateSerializeMethods();

WriteHeaderFotter();

# Test Section

CreateTests();

WriteLoggerVariables();

WriteMetaDataFiles();
