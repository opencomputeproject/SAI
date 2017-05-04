#!/usr/bin/perl

use strict;
use warnings;
use diagnostics;

# disable for experimental::autoderef push pop
no warnings "experimental";

use XML::Simple qw(:strict);
use Getopt::Std;
use Data::Dumper;
use Term::ANSIColor;

my $errors = 0;
my $warnings = 0;
my $XMLDIR = "xml";
my $INCLUDEDIR = "../inc/";
my %SAI_ENUMS = ();
my %METADATA = ();
my %STRUCTS = ();
my %options = ();

# pointers used in switch object for notifications
my @pointers = ();

my @TESTNAMES= ();

my %OBJTOAPIMAP = ();
my %APITOOBJMAP = ();

my $HEADER_CONTENT = "";
my $SOURCE_CONTENT = "";
my $TEST_CONTENT = "";

my $FLAGS = "MANDATORY_ON_CREATE|CREATE_ONLY|CREATE_AND_SET|READ_ONLY|KEY|DYNAMIC|SPECIAL";

# TAGS HANDLERS

my %TAGS = (
        "type"      , \&ProcessTagType,
        "flags"     , \&ProcessTagFlags,
        "objects"   , \&ProcessTagObjects,
        "allownull" , \&ProcessTagAllowNull,
        "condition" , \&ProcessTagCondition,
        "validonly" , \&ProcessTagValidOnly,
        "default"   , \&ProcessTagDefault,
        "ignore"    , \&ProcessTagIgnore,
        "isvlan"    , \&ProcessTagIsVlan,
        "getsave"   , \&ProcessTagGetSave,

        );

getopts("d",\%options);

my $optionPrintDebug = 1 if defined $options{d};

# LOGGING FUNCTIONS HELPERS

sub LogInfo
{
    print color('bright_green') . "@_" . color('reset') . "\n";
}

sub LogWarning
{
    $warnings++;
    print color('bright_yellow') . "@_" . color('reset') . "\n";
}
sub LogError
{
    $errors++;
    print color('bright_red') . "@_" . color('reset') . "\n";
}

sub LogDebug
{
    print color('bright_blue') . "@_" . color('reset') . "\n" if $optionPrintDebug;
}

$SIG{__DIE__} = sub
{
    LogError "FATAL ERROR === MUST FIX === : @_";
    exit 1;
};

sub GetXmlFiles
{
    my $dir = shift;

    opendir(my $dh, $dir) || die "Can't open $dir $!";

    my @files = ();

    while (readdir $dh)
    {
        next if not /^sai\w*_8h\.xml$/i;

        push @files,$_;
    }

    closedir $dh;

    @files = sort @files;

    return @files;
}

sub ExtractDescription
{
    my ($type, $value, $item) = @_;

    return $item if ref $item eq "";

    if (not ref $item eq "HASH")
    {
        LogError "invalid description provided in $type $value";
        return undef;
    }

    my $content = "";

    if (defined $item->{simplesect})
    {
        my @sim = @{ $item->{simplesect} };

        for my $s (@sim)
        {
            $content .= ExtractDescription($type, $value, $s);
        }

        return $content;
    }

    if (defined $item->{para})
    {
        my @para = @{ $item->{para} };

        for my $p (@para)
        {
            $content .= " " . ExtractDescription($type, $value, $p);
        }

        return $content;
    }

    return $content;
}

my %ACL_FIELD_TYPES = qw/
bool                booldata
sai_uint8_t         u8
sai_int8_t          s8
sai_uint16_t        u16
sai_int16_t         s16
sai_uint32_t        u32
sai_int32_t         s32
sai_mac_t           mac
sai_ip4_t           ip4
sai_ip6_t           ip6
sai_object_id_t     oid
sai_object_list_t   objlist
sai_uint8_list_t    u8list/;

my %ACL_FIELD_TYPES_TO_VT = qw/
bool                BOOL
sai_uint8_t         UINT8
sai_int8_t          INT8
sai_uint16_t        UINT16
sai_int16_t         INT16
sai_uint32_t        UINT32
sai_int32_t         INT32
sai_mac_t           MAC
sai_ip4_t           IPV4
sai_ip6_t           IPV6
sai_object_id_t     OBJECT_ID
sai_object_list_t   OBJECT_LIST
sai_uint8_list_t    UINT8_LIST/;

my %ACL_ACTION_TYPES = qw/
sai_uint8_t           u8
sai_int8_t            s8
sai_uint16_t          u16
sai_int16_t           s16
sai_uint32_t          u32
sai_int32_t           s32
sai_mac_t             mac
sai_ip4_t             ip4
sai_ip6_t             ip6
sai_object_id_t       oid
sai_object_list_t     objectlist/;

my %ACL_ACTION_TYPES_TO_VT = qw/
sai_uint8_t           UINT8
sai_int8_t            INT8
sai_uint16_t          UINT16
sai_int16_t           INT16
sai_uint32_t          UINT32
sai_int32_t           INT32
sai_mac_t             MAC
sai_ip4_t             IPV4
sai_ip6_t             IPV6
sai_object_id_t       OBJECT_ID
sai_object_list_t     OBJECT_LIST/;

my %VALUE_TYPES = qw/
sai_uint8_t             u8
sai_int8_t              s8
sai_uint16_t            u16
sai_int16_t             s16
sai_uint32_t            u32
sai_int32_t             s32
sai_uint64_t            u64
sai_int64_t             s64
sai_pointer_t           ptr
sai_mac_t               mac
sai_ip4_t               ip4
sai_ip6_t               ip6
sai_ip_address_t        ipaddr
sai_ip_prefix_t         ipprefix
sai_object_id_t         oid
sai_object_list_t       objlist
sai_u8_list_t           u8list
sai_s8_list_t           s8list
sai_u16_list_t          u16list
sai_s16_list_t          s16list
sai_u32_list_t          u32list
sai_s32_list_t          s32list
sai_u32_range_t         u32range
sai_s32_range_t         s32range
sai_vlan_list_t         vlanlist
sai_acl_field_data_t    aclfield
sai_acl_action_data_t   aclaction
sai_qos_map_list_t      qosmap
sai_tunnel_map_list_t   tunnelmap
sai_acl_capability_t    aclcapability
/;

my %VALUE_TYPES_TO_VT = qw/
sai_uint8_t             UINT8
sai_int8_t              INT8
sai_uint16_t            UINT16
sai_int16_t             INT16
sai_uint32_t            UINT32
sai_int32_t             INT32
sai_uint64_t            UINT64
sai_int64_t             INT64
sai_pointer_t           POINTER
sai_mac_t               MAC
sai_ip4_t               IPV4
sai_ip6_t               IPV6
sai_ip_address_t        IP_ADDRESS
sai_ip_prefix_t         IP_PREFIX
sai_object_id_t         OBJECT_ID
sai_object_list_t       OBJECT_LIST
sai_u8_list_t           UINT8_LIST
sai_s8_list_t           INT8_LIST
sai_u16_list_t          UINT16_LIST
sai_s16_list_t          INT16_LIST
sai_u32_list_t          UINT32_LIST
sai_s32_list_t          INT32_LIST
sai_u32_range_t         UINT32_RANGE
sai_s32_range_t         INT32_RANGE
sai_vlan_list_t         VLAN_LIST
sai_qos_map_list_t      QOS_MAP_LIST
sai_tunnel_map_list_t   TUNNEL_MAP_LIST
sai_acl_capability_t    ACL_CAPABILITY
/;

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

sub ProcessTagValidOnly
{
    my ($type, $value, $val) = @_;

    my @conditions = split/\s+or\s+/,$val;

    if ($val =~ /and/)
    {
        LogError "and condition is not supported yet on $value";
        return undef;
    }

    for my $cond (@conditions)
    {
        if (not $cond =~/^(SAI_\w+) == (true|false|SAI_\w+)$/)
        {
            LogError "invalid validonly tag value '$val' ($cond), expected SAI_ENUM == true|false|SAI_ENUM";
            return undef;
        }
    }

    return \@conditions;
}

sub ProcessTagCondition
{
    my ($type, $value, $val) = @_;

    my @conditions = split/\s+or\s+/,$val;

    if ($val =~ /and/)
    {
        LogError "and condition is not supported yet on $value";
        return undef;
    }

    for my $cond (@conditions)
    {
        if (not $cond =~/^(SAI_\w+) == (true|false|SAI_\w+)$/)
        {
            LogError "invalid condition tag value '$val' ($cond), expected SAI_ENUM == true|false|SAI_ENUM";
            return undef;
        }
    }

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

    if ($val =~/^(true|false|NULL|SAI_\w+|-?\d+|0x[0-9A-F]+)$/ and not $val =~ /_ATTR_|OBJECT_TYPE/)
    {
        return $val;
    }

    if ($val =~/^0\.0\.0\.0$/)
    {
        # currently we only support default ip address
        return $val;
    }

    LogError "invalid default tag value '$val', on $type $value";
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
    my ($type, $value, $desc) = @_;

    my @order = ();

    $desc =~ s/@@/\n@@/g;

    while ($desc =~ /@@(\w+)(.+)/g)
    {
        my $tag = $1;
        my $val = $2;

        push @order,$tag;

        $val =~ s/\s+/ /g;
        $val =~ s/^\s*//;
        $val =~ s/\s*$//;

        if (not defined $TAGS{$tag})
        {
            LogError "unrecognized tag $tag on $type $value";
            next;
        }

        $val = $TAGS{$tag}->($type, $value, $val);

        $METADATA{$type}{$value}{$tag}          = $val;
        $METADATA{$type}{$value}{objecttype}    = $type;
        $METADATA{$type}{$value}{attrid}        = $value;
    }

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

        my @arr = ();

        $SAI_ENUMS{$enumtypename}{values} = \@arr if not defined $SAI_ENUMS{$enumtypename};

        for my $ev (@{ $memberdef->{enumvalue} })
        {
            my $enumvaluename = $ev->{name}[0];

            LogDebug "$id $enumtypename $enumvaluename";

            push$SAI_ENUMS{$enumtypename}{values},$enumvaluename;

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

            ProcessDescription($enumtypename, $enumvaluename, $desc);
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

        next if not $typedeftype =~ /^enum /;

        if (not defined $SAI_ENUMS{$typedefname})
        {
            LogError "enum $typedefname has no typedef enum $typedefname";
            next;
        }

        next if not $typedefname =~ /^sai_(\w+)_attr_t$/;

        # this enum is attribute definition for object

        my $objecttype = "SAI_OBJECT_TYPE_" . uc $1;

        my %ENUM = %{ $SAI_ENUMS{$typedefname} };

        $ENUM{"objecttype"} = $objecttype;
    }
}

sub ProcessXmlFile
{
    my $file = shift;

    my $xs = XML::Simple->new();

    my $ref = $xs->XMLin($file, KeyAttr => { }, ForceArray => 1);

    my @sections = @{ $ref->{compounddef}[0]->{sectiondef} };

    for my $section (@sections)
    {
        ProcessEnumSection($section) if ($section->{kind} eq "enum");

        ProcessTypedefSection($section) if ($section->{kind} eq "typedef");
    }
}

sub WriteHeader
{
    my $content = shift;

    $HEADER_CONTENT .= $content . "\n";
}

sub WriteSource
{
    my $content = shift;

    $SOURCE_CONTENT .= $content . "\n";
}

sub WriteTest
{
    my $content = shift;

    $TEST_CONTENT .= $content . "\n";
}

sub ProcessSingleEnum
{
    my ($key, $typedef, $prefix) = @_;

    my $enum = $SAI_ENUMS{$key};

    my @values = @{$enum->{values}};

    WriteSource "const char sai_metadata_${typedef}_enum_name[] = \"$typedef\";";
    WriteSource "const $typedef sai_metadata_${typedef}_enum_values[] = {";

    for my $value (@values)
    {
        LogWarning "Value $value of $typedef is not prefixed as $prefix" if not $value =~ /^$prefix/;

        WriteSource "    $value,";
    }

    WriteSource "};";

    WriteSource "const char* sai_metadata_${typedef}_enum_values_names[] = {";

    for my $value (@values)
    {
        WriteSource "    \"$value\",";
    }

    WriteSource "    NULL";
    WriteSource "};";

    WriteSource "const char* sai_metadata_${typedef}_enum_values_short_names[] = {";

    for my $value (@values)
    {
        $value =~ s/^${prefix}//;

        WriteSource "    \"$value\",";
    }

    WriteSource "    NULL";
    WriteSource "};";

    my $count = $#values + 1;

    WriteSource "const size_t sai_metadata_${typedef}_enum_values_count = $count;";

    return $count;
}

sub WriteFile
{
    my ($file, $content) = @_;

    open (F, ">", $file) or die "$0: open $file $!";

    print F $content;

    close F;
}

sub CreateMetadataHeaderAndSource
{
    my $HEAD = "/* AUTOGENERATED FILE! DO NOT EDIT! */";

    WriteHeader $HEAD;
    WriteHeader "#include <sai.h>";
    WriteHeader "#include \"saimetadatatypes.h\"";
    WriteHeader "#include \"saimetadatautils.h\"";
    WriteHeader "#include \"saimetadatalogger.h\"";

    WriteSource $HEAD;
    WriteSource "#include <stdio.h>";
    WriteSource "#include \"saimetadata.h\"";

    WriteSource "#define DEFINE_ENUM_METADATA(x,count)\\";
    WriteSource "const sai_enum_metadata_t sai_metadata_enum_ ## x = {\\";
    WriteSource "    .name              = sai_metadata_ ## x ## _enum_name,\\";
    WriteSource "    .valuescount       = count,\\";
    WriteSource "    .values            = (const int*)sai_metadata_ ## x ## _enum_values,\\";
    WriteSource "    .valuesnames       = sai_metadata_ ## x ## _enum_values_names,\\";
    WriteSource "    .valuesshortnames  = sai_metadata_ ## x ## _enum_values_short_names,\\";
    WriteSource "};";

    for my $key (sort keys %SAI_ENUMS)
    {
        if (not $key =~ /^((sai_\w+_)t)$/)
        {
            LogWarning "Enum typedef $key is not matching SAI format";
            next;
        }

        my $count = ProcessSingleEnum($key, $1, uc $2);

        WriteHeader "extern const sai_enum_metadata_t sai_metadata_enum_$1;";
        WriteSource "DEFINE_ENUM_METADATA($1, $count);";
    }

    # all enums

    WriteHeader "extern const sai_enum_metadata_t* sai_metadata_all_enums[];";
    WriteSource "const sai_enum_metadata_t* sai_metadata_all_enums[] = {";

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

    WriteHeader "extern const sai_enum_metadata_t* sai_metadata_attr_enums[];";
    WriteSource "const sai_enum_metadata_t* sai_metadata_attr_enums[] = {";

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
        if (not grep(/^\Q$obj\E$/,@all))
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

    my @objs =@{ $objects };

    return $#objs + 1;
}

sub ProcessDefaultValueType
{
    my ($attr, $default) = @_;

    return "SAI_DEFAULT_VALUE_TYPE_NONE" if not defined $default;

    return "SAI_DEFAULT_VALUE_TYPE_CONST" if $default =~ /^SAI_NULL_OBJECT_ID$/;

    return "SAI_DEFAULT_VALUE_TYPE_SWITCH_INTERNAL" if $default =~ /^internal$/;

    return "SAI_DEFAULT_VALUE_TYPE_CONST" if $default =~ /^(true|false|const|NULL|-?\d+|0x[0-9A-F]+|SAI_\w+)$/ and not $default =~ /_ATTR_|SAI_OBJECT_TYPE_/;

    return "SAI_DEFAULT_VALUE_TYPE_EMPTY_LIST" if $default =~ /^empty$/;

    return "SAI_DEFAULT_VALUE_TYPE_VENDOR_SPECIFIC" if $default =~ /^vendor$/;

    return "SAI_DEFAULT_VALUE_TYPE_ATTR_VALUE" if $default =~ /^attrvalue SAI_\w+$/ and $default =~ /_ATTR_/;

    return "SAI_DEFAULT_VALUE_TYPE_ATTR_RANGE" if $default =~ /^attrrange SAI_\w+$/ and $default =~ /_ATTR_/;

    return "SAI_DEFAULT_VALUE_TYPE_CONST" if $default =~ /^0\.0\.0\.0$/;

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
    elsif ($default =~ /^0$/ and $type =~ /sai_acl_field_data_t (sai_u?int\d+_t)/)
    {
        WriteSource "$val = { };";
    }
    elsif ($default =~ /^0$/ and $type =~ /sai_acl_action_data_t (sai_u?int\d+_t)/)
    {
        WriteSource "$val = { };";
    }
    elsif ($default =~ /^(-?\d+|0x[0-9A-F]+)$/ and $type =~ /sai_u?int\d+_t/)
    {
        WriteSource "$val = { .$VALUE_TYPES{$type} = $default };";
    }
    elsif ($default =~ /^NULL$/ and $type =~ /^(sai_pointer_t) (sai_\w+_fn)$/)
    {
        WriteSource "$val = { .$VALUE_TYPES{$1} = $default };";

        push @pointers,$2;
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

    return "SAI_ATTR_CONDITION_TYPE_OR";
}

sub ProcessConditions
{
    my ($attr, $conditions, $enumtype) = @_;

    return "NULL" if not defined $conditions;

    my @conditions = @{ $conditions };

    my $count = 0;

    my @values = ();

    for my $cond (@conditions)
    {
        if (not $cond =~ /^(SAI_\w+) == (true|false|SAI_\w+)$/)
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
            LogError "conditional attribute $attr has condition from different object $attrid";
            return "";
        }

        WriteSource "const sai_attr_condition_t sai_metadata_condition_${attr}_$count = {";

        if ($val eq "true" or $val eq "false")
        {
            WriteSource "    .attrid = $attrid,";
            WriteSource "    .condition = { .booldata = $val }";
        }
        else
        {
            WriteSource "    .attrid = $attrid,";
            WriteSource "    .condition = { .s32 = $val }";
        }

        WriteSource "};";

        $count++;
    }

    WriteSource "const sai_attr_condition_t* sai_metadata_conditions_${attr}\[\] = {";

    $count = 0;

    for my $cond (@conditions)
    {
        WriteSource "    &sai_metadata_condition_${attr}_$count,";

        $count++;
    }

    WriteSource "    NULL";

    WriteSource "};";

    return "sai_metadata_conditions_${attr}";
}

sub ProcessConditionsLen
{
    my ($attr, $value) = @_;

    return "0" if not defined $value;

    my @conditions = @{ $value };

    return $#conditions + 1;
}

sub ProcessValidOnlyType
{
    my ($attr, $value) = @_;

    return "SAI_ATTR_CONDITION_TYPE_NONE" if not defined $value;

    return "SAI_ATTR_CONDITION_TYPE_OR";
}

sub ProcessValidOnly
{
    my ($attr, $conditions, $enumtype) = @_;

    return "NULL" if not defined $conditions;

    my @conditions = @{ $conditions };

    my $count = 0;

    my @values = ();

    for my $cond (@conditions)
    {
        if (not $cond =~ /^(SAI_\w+) == (true|false|SAI_\w+)$/)
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
            LogError "validonly attribute $attr has condition from different object $attrid";
            return "";
        }

        WriteSource "const sai_attr_condition_t sai_metadata_validonly_${attr}_$count = {";

        if ($val eq "true" or $val eq "false")
        {
            WriteSource "    .attrid = $attrid,";
            WriteSource "    .condition = { .booldata = $val }";
        }
        else
        {
            WriteSource "    .attrid = $attrid,";
            WriteSource "    .condition = { .s32 = $val }";
        }

        WriteSource "};";

        $count++;
    }

    WriteSource "const sai_attr_condition_t* sai_metadata_validonly_${attr}\[\] = {";

    $count = 0;

    for my $cond (@conditions)
    {
        WriteSource "    &sai_metadata_validonly_${attr}_$count,";

        $count++;
    }

    WriteSource "    NULL";

    WriteSource "};";

    return "sai_metadata_validonly_${attr}";
}

sub ProcessValidOnlyLen
{
    my ($attr, $value) = @_;

    return "0" if not defined $value;

    my @conditions = @{ $value };

    return $#conditions + 1;
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

sub  ProcessIsAclField
{
    my $attr = shift;

    return "true" if $attr =~ /^SAI_ACL_ENTRY_ATTR_FIELD_\w+$/;

    return "false";
}

sub  ProcessIsAclAction
{
    my $attr = shift;

    return "true" if $attr =~ /^SAI_ACL_ENTRY_ATTR_ACTION_\w+$/;

    return "false";
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

        WriteSource "const sai_attr_metadata_t sai_metadata_attr_$attr = {";

        WriteSource "    .objecttype                    = $objecttype,";
        WriteSource "    .attrid                        = $attr,";
        WriteSource "    .attridname                    = $attrname,";
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
        WriteSource "    .getsave                       = $getsave,";
        WriteSource "    .isvlan                        = $isvlan,";
        WriteSource "    .isaclfield                    = $isaclfield,";
        WriteSource "    .isaclaction                   = $isaclaction,";

        WriteSource "};";

        # check enum attributes if their names are ending on enum name

        if ($isenum eq "true" or $isenumlist eq "true")
        {
            my $en = uc($1) if $meta{type} =~/.*sai_(\w+)_t/;

            next if $attr =~ /_${en}_LIST$/;
            next if $attr =~ /_$en$/;

            $attr =~/SAI_(\w+?)_ATTR_(\w+)/;

            my $aot = $1;
            my $aend = $1;

            if ($en =~/^${aot}_(\w+)$/)
            {
                my $ending = $1;

                next if $attr =~/_$ending$/;

                LogError "enum starts by object type $aot but not ending on $ending in $en";

            }

            LogError "$meta{type} == $attr not ending on enum name $en";
        }
    }
};

sub CreateMetadata
{
    for my $key (sort keys %SAI_ENUMS)
    {
        if (not $key =~ /^(sai_(\w+)_attr_t)$/)
        {
            next;
        }

        my $typedef = $1;
        my $objtype = "SAI_OBJECT_TYPE_" . uc($2);

        ProcessSingleObjectType($typedef, $objtype);
    }
}

sub SanityCheckContent
{
    # since we generate so much metadata now
    # lets put some primitive sanity check
    # if everything we generated is fine

    my $testCount = @TESTNAMES;

    if ($testCount < 5)
    {
        LogError "there should be at least 5 test defined, got $testCount";
    }

    my $metaHeaderSize = 29337 * 0.9;
    my $metaSourceSize = 1738348 * 0.9;

    if (length($HEADER_CONTENT) < $metaHeaderSize)
    {
        LogError "generated saimetadata.h size is too small";
    }

    if (length($SOURCE_CONTENT) < $metaSourceSize)
    {
        LogError "generated saimetadata.c size is too small";
    }
}

sub WriteMetaDataFiles
{
    SanityCheckContent();

    exit 1 if ($warnings > 0 || $errors > 0);

    WriteFile("saimetadata.h", $HEADER_CONTENT);
    WriteFile("saimetadata.c", $SOURCE_CONTENT);
    WriteFile("saimetadatatest.c", $TEST_CONTENT);
}

sub ProcessAttrValueType
{
    my $filename = "saimetadatatypes.h";

    open(my $fh, '<', $filename) or die "Could not open file '$filename' $!";

    my @values = ();

    while (my $line = <$fh>)
    {
        next if not $line =~ /(SAI_ATTR_VALUE_TYPE_\w+)/;

        push@values,$1;
    }

    close $fh;

    $SAI_ENUMS{"sai_attr_value_type_t"}{values} = \@values;
}

sub ProcessSaiStatus
{
    my $filename = "../inc/saistatus.h";

    open(my $fh, '<', $filename) or die "Could not open file '$filename' $!";

    my @values = ();

    while (my $line = <$fh>)
    {
        next if not $line =~ /define\s+(SAI_STATUS_\w+).+0x00/;

        push@values,$1;
    }

    close $fh;

    $SAI_ENUMS{"sai_status_t"}{values} = \@values;
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

        WriteSource "const sai_attr_metadata_t* sai_metadata_object_type_$type\[\] = {";

        my @values = @{ $SAI_ENUMS{$type}{values} };

        for my $value (@values)
        {
            next if defined $METADATA{$type}{$value}{ignore};

            WriteSource "    &sai_metadata_attr_$value,";
        }

        WriteSource "    NULL";
        WriteSource "};";
    }

    WriteHeader "extern const sai_attr_metadata_t** sai_metadata_attr_by_object_type[];";
    WriteSource "const sai_attr_metadata_t** sai_metadata_attr_by_object_type[] = {";

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

    my $count = $#objects + 1;

    WriteHeader "extern const size_t sai_metadata_attr_by_object_type_count;";
    WriteSource "const size_t sai_metadata_attr_by_object_type_count = $count;";
}

sub CreateEnumHelperMethods
{
    for my $key (sort keys %SAI_ENUMS)
    {
        next if not $key =~ /^sai_(\w+)_t/;

        next if $key =~/_attr_t$/;

        WriteSource "const char* sai_metadata_get_$1_name(";
        WriteSource "        _In_ $key value)";
        WriteSource "{";
        WriteSource "    return sai_metadata_get_enum_value_name(&sai_metadata_enum_$key, value);";
        WriteSource "}";

        WriteHeader "extern const char* sai_metadata_get_$1_name(";
        WriteHeader "        _In_ $key value);";
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

    my @keys = keys $struct;

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

    WriteSource "const sai_struct_member_info_t* sai_metadata_struct_members_sai_${rawname}_t[] = {";

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

    my @keys = keys $struct;
    my $count = @keys;

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

    WriteSource "const sai_rev_graph_member_t* sai_metadata_${objectType}_rev_graph_members[] = {";

    for my $mn (@membernames)
    {
        WriteSource "    &$mn,";
    }

    WriteSource "    NULL,";

    WriteSource "};";

    return "sai_metadata_${objectType}_rev_graph_members";
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

        my $struct = $STRUCTS{$ot};

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

    if (not defined $struct)
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

    if (not defined $struct)
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

    if (not defined $struct)
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

    if (not defined $struct)
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
    for my $key(sort keys %APITOOBJMAP)
    {
        WriteSource "sai_${key}_api_t *sai_metadata_sai_${key}_api = NULL;";
        WriteHeader "extern sai_${key}_api_t *sai_metadata_sai_${key}_api;";
    }
}

sub CreateApisQuery
{
    WriteHeader "typedef sai_status_t (*sai_api_query_fn)(";
    WriteHeader "        _In_ sai_api_t sai_api_id,";
    WriteHeader "        _Out_ void** api_method_table);";

    WriteSource "typedef sai_status_t(*sai_create_generic_fn)(";
    WriteSource "        _Out_ sai_object_id_t* object_id,";
    WriteSource "        _In_ sai_object_id_t switch_id,";
    WriteSource "        _In_ uint32_t attr_count,";
    WriteSource "        _In_ const sai_attribute_t *attr_list);";

    WriteSource "typedef sai_status_t (*sai_remove_generic_fn)(";
    WriteSource "        _In_ sai_object_id_t object_id);";

    WriteSource "typedef sai_status_t (*sai_set_generic_attribute_fn)(";
    WriteSource "        _In_ sai_object_id_t object_id,";
    WriteSource "        _In_ const sai_attribute_t *attr);";

    WriteSource "typedef sai_status_t (*sai_get_generic_attribute_fn)(";
    WriteSource "        _In_ sai_object_id_t object_id,";
    WriteSource "        _In_ uint32_t attr_count,";
    WriteSource "        _Inout_ sai_attribute_t *attr_list);";


    # for switch we need to generate wrapper, for others we can use pointers
    # so we don't need to use meta key then


    WriteSource "int sai_metadata_apis_query(";
    WriteSource "    _In_ const sai_api_query_fn api_query)";
    WriteSource "{";
    WriteSource "    sai_status_t status = SAI_STATUS_SUCCESS;";
    WriteSource "    int count = 0;";

    WriteSource "    if (api_query == NULL)";
    WriteSource "    {";

    for my $key(sort keys %APITOOBJMAP)
    {
        WriteSource "        sai_metadata_sai_${key}_api = NULL;";
    }

    WriteSource "        return count;";
    WriteSource "    }";

    for my $key(sort keys %APITOOBJMAP)
    {
        my $api = uc("SAI_API_${key}");

        WriteSource "    status = api_query($api, (void**)&sai_metadata_sai_${key}_api);";
        WriteSource "    if (status != SAI_STATUS_SUCCESS) { count++; SAI_META_LOG_ERROR(\"failed to query api $api\"); }";
    }

    WriteSource "    return count; /* number of unsuccesfull apis */";

    WriteSource "}";

    WriteHeader "extern int sai_metadata_apis_query(";
    WriteHeader "    _In_ const sai_api_query_fn api_query);";
}

sub CreateObjectInfo
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

        my $start = "SAI_" . uc($1) . "_ATTR_START";
        my $end   = "SAI_" . uc($1) . "_ATTR_END";

        my $enum  = "&sai_metadata_enum_${type}";

        my $struct = $STRUCTS{$ot};

        #
        # here we need to only generate struct member names
        # since we use those members in rev graph entries
        # so struct members must be generated previously
        #

        my $isnonobjectid = ProcessIsNonObjectId($struct, $ot);
        my $structmembers = ProcessStructMembersName($struct, $ot ,lc($1));
        my $structmemberscount = ProcessStructMembersCount($struct, $ot);
        my $revgraph = ProcessRevGraph($ot);
        my $create = "NULL";
        my $remove = "NULL";
        my $set = "NULL";
        my $get = "NULL";

        if ($ot eq "SAI_OBJECT_TYPE_FDB_FLUSH" or $ot eq "SAI_OBJECT_TYPE_HOSTIF_PACKET")
        {
            # ok
        }
        else
        {
            $create = ProcessCreate($struct, $ot);
            $remove = ProcessRemove($struct, $ot);
            $set = ProcessSet($struct, $ot);
            $get = ProcessGet($struct, $ot);
        }

        WriteHeader "extern const sai_object_type_info_t sai_metadata_object_type_info_$ot;";

        WriteSource "const sai_object_type_info_t sai_metadata_object_type_info_$ot = {";
        WriteSource "    .objecttype         = $ot,";
        WriteSource "    .objecttypename     = \"$ot\",";
        WriteSource "    .attridstart        = $start,";
        WriteSource "    .attridend          = $end,";
        WriteSource "    .enummetadata       = $enum,";
        WriteSource "    .attrmetadata       = sai_metadata_object_type_$type,";
        WriteSource "    .isnonobjectid      = $isnonobjectid,";
        WriteSource "    .isobjectid         = !$isnonobjectid,";
        WriteSource "    .structmembers      = $structmembers,";
        WriteSource "    .structmemberscount = $structmemberscount,";
        WriteSource "    .revgraphmembers    = $revgraph,";
        WriteSource "    .create             = $create,";
        WriteSource "    .remove             = $remove,";
        WriteSource "    .set                = $set,";
        WriteSource "    .get                = $get,";
        WriteSource "};";
    }

    WriteHeader "extern const sai_object_type_info_t* sai_metadata_all_object_type_infos[];";

    WriteSource "const sai_object_type_info_t* sai_metadata_all_object_type_infos[] = {";

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

sub GetHeaderFiles
{
    opendir(my $dh, $INCLUDEDIR) || die "Can't opendir $INCLUDEDIR: $!";
    my @headers = grep { /^sai\w*\.h$/ and -f "$INCLUDEDIR/$_" } readdir($dh);
    closedir $dh;

    return @headers;
}

sub GetMetaHeaderFiles
{
    opendir(my $dh, ".") || die "Can't opendir . $!";
    my @headers = grep { /^sai\w*\.h$/ and -f "./$_" } readdir($dh);
    closedir $dh;

    return @headers;
}

sub ReadHeaderFile
{
    my $filename = shift;
    local $/ = undef;

    # first search file in meta directory

    $filename = "$INCLUDEDIR/$filename" if not -e $filename;

    open FILE, $filename or die "Couldn't open file $filename: $!";
    binmode FILE;
    my $string = <FILE>;
    close FILE;

    return $string;
}

sub GetNonObjectIdStructNames
{
    my %structs;

    my @headers = GetHeaderFiles();

    for my $header (@headers)
    {
        my $data = ReadHeaderFile($header);

        # TODO there should be better way to extract those

        while ($data =~ /sai_(?:create|set)_\w+.+?\n.+const\s+(sai_(\w+)_t)/gim)
        {
            my $name = $1;
            my $rawname = $2;

            $structs{$name} = $rawname;

            if (not $name =~ /_entry_t$/)
            {
                LogError "non object id struct name '$name'; should end on _entry_t";
                next;
            }
        }
    }

    return values %structs;
}

sub DefineTestName
{
    my $name = shift;

    push @TESTNAMES,$name;

    WriteTest "void $name(void)";
}

sub CreatePointersTest
{
    # we don't declare actual test, just global values

    for my $pointer (@pointers)
    {
        # make sure taht declared pointer is correct
        # by testing if it will compile in test

        WriteTest "$pointer var_$pointer = NULL;";
    }
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

    my @all = @{ $SAI_ENUMS{sai_object_type_t}{values} };

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

    for my $key (sort keys %SAI_ENUMS)
    {
        next if not $key =~ /^(sai_\w+_t)$/;
        next if $key =~ /^(sai_null_attr_t)$/;

        WriteTest "    TEST_ASSERT_TRUE((sizeof($1) == sizeof(int32_t)), \"invalid enum $1 size\");";
    }

    WriteTest "    TEST_ASSERT_TRUE((sizeof(sai_status_t) == sizeof(int32_t)), \"invalid sai_status_t size\");";

    WriteTest "}";
}

sub ExtractStructInfo
{
    my $struct = shift;
    my $prefix = shift;

    my %S = ();

    my $filename = "${prefix}${struct}.xml";

    $filename =~ s/_/__/g;

    my $file = "$XMLDIR/$filename"; # example: xml/struct__sai__fdb__entry__t.xml

    # read xml, we need to get each struct field and it's type and description

    my $xs = XML::Simple->new();

    my $ref = $xs->XMLin($file, KeyAttr => { }, ForceArray => 1);

    my @sections = @{ $ref->{compounddef}[0]->{sectiondef} };

    my $count = @sections;

    if ($count != 1)
    {
        LogError "expected only 1 section in $file for $struct";
        return %S;
    }

    my @members = @{ $sections[0]->{memberdef} };

    $count = @members;

    if ($count < 2)
    {
        LogError "there must be at least 2 members in struct $struct";
        return %S;
    }

    for my $member (@members)
    {
        my $name = $member->{name}[0];
        my $type = $1 if $member->{definition}[0] =~ /^(\w+)/;

        my $desc = ExtractDescription($struct, $struct, $member->{detaileddescription}[0]);

        $S{$name}{type} = $type;
        $S{$name}{desc} = $desc;
    }

    return %S;
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

    for my $member (keys %struct)
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

        $STRUCTS{$objecttype} = \%struct;
    }
}

sub CreateListOfAllAttributes
{
    # list will be used to find attribute metadata
    # based on attribute string name

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

            my %meta = %{ $METADATA{$typedef}{$attr} };

            next if defined $meta{ignore};

            $ATTRIBUTES{$attr} = 1; #"const sai_attr_metadata_t  = {";
        }
    }

    WriteSource "const sai_attr_metadata_t* sai_metadata_attr_sorted_by_id_name[] = {";
    WriteHeader "extern const sai_attr_metadata_t* sai_metadata_attr_sorted_by_id_name[];";

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

sub CheckDoxygenStyle
{
    my ($header, $line, $n) = @_;

    return if (not $line =~ /\@(\w+)/);

    my $mark = $1;

    if ($mark eq "file" and not $line =~ /\@file\s+($header)/)
    {
        LogWarning "\@file should match format: sai\\w+.h: $header $n:$line";
        return;
    }

    if ($mark eq "brief" and not $line =~ /\@brief\s+[A-Z]/)
    {
        LogWarning "\@brief should start with capital letter: $header $n:$line";
        return;
    }

    if ($mark eq "return" and not $line =~ /\@return\s+(#SAI_|[A-Z][a-z])/)
    {
        LogWarning "\@return should start with #: $header $n:$line";
        return;
    }

    if ($mark eq "param" and not $line =~ /\@param\[(in|out|inout)\] (\.\.\.|[a-z]\w+)\s+([A-Z]\w+)/)
    {
        LogWarning "\@param should be in format \@param[in|out|inout] [a-z]\\w+ [A-Z]\\w+: $header $n:$line";
        return;
    }

    if ($mark eq "defgroup" and not $line =~ /\@defgroup SAI\w* SAI - \w+/)
    {
        LogWarning "\@defgroup should be in format \@defgroup SAI\\w* SAI - \\w+: $header $n:$line";
        return;
    }
}

sub ExtractComments
{
    my $input = shift;

    my $comments = "";

    # good enough comments extractor C/C++ source

    while ($input =~ m!(".*?")|//.*?[\r\n]|/\*.*?\*/!s)
    {
        $input = $';

        $comments .= $& if not $1;
    }

    return $comments;
}

sub CheckHeaderHeader
{
    my ($data, $file) = @_;

    my $header = <<_END_
/**
 * Copyright (c) 20XX Microsoft Open Technologies, Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License"); you may
 *    not use this file except in compliance with the License. You may obtain
 *    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
 *    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 *    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 *    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 *    Microsoft would like to thank the following companies for their review and
 *    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
 *    Dell Products, L.P., Facebook, Inc
 *
_END_
;

    my $is = substr($data, 0, length($header));

    $is =~ s/ 20\d\d / 20XX /;

    return if $is eq $header;

    LogWarning "Wrong header in $file, is:\n$is\n should be:\n\n$header";
}

sub CheckFunctionsParams
{
    #
    # make sure that doxygen params match function params names
    #

    my ($data, $file) = @_;

    my $doxygenCommentPattern = '/\*\*((?:(?!\*/).)*?)\*/';
    my $fnTypeDefinition = 'typedef\s*\w+[^;]+?(\w+_fn)\s*\)([^;]+?);';
    my $globalFunction = 'sai_\w+\s*(sai_\w+)[^;]*?\(([^;]+?);';

    while ($data =~ m/$doxygenCommentPattern\s*(?:$fnTypeDefinition|$globalFunction)/gis)
    {
        my $comment = $1;
        my $fname = $2;
        my $fn = $3;

        $fname = $4 if defined $4;
        $fn = $5 if defined $5;

        my @params = $comment =~ /\@param\[\w+]\s+(\.\.\.|\w+)/gis;
        my @fnparams = $fn =~ /_(?:In|Out|Inout)_.+?(\.\.\.|\w+)\s*[,\)]/gis;

        my $params = "@params";
        my $fnparams = "@fnparams";

        if ($params ne $fnparams)
        {
            LogWarning "not matching params in function $fname: $file";
            LogWarning " doxygen '$params' vs code '$fnparams'";
        }

        if ("@params" =~ /[A-Z]/)
        {
            LogWarning "params should use small letters only '@params' in $fname: $file";
        }

        next if $fname eq "sai_remove_all_neighbor_entries_fn"; # exception

        my @paramsFlags = lc($comment) =~ /\@param\[(\w+)]/gis;
        my @fnparamsFlags = lc($fn) =~ /_(\w+)_.+?(?:\.\.\.|\w+)\s*[,\)]/gis;

        if (not "@paramsFlags" eq "@fnparamsFlags")
        {
            LogWarning "params flags not match ('@paramsFlags' vs '@fnparamsFlags') in $fname: $file";
        }

        next if not $fname =~ /_fn$/; # below don't apply for global functions

        if (not $fnparams =~ /^(\w+)(| attr| attr_count attr_list| switch_id attr_count attr_list)$/ and
            not $fname =~ /_(stats|notification)_fn$|^sai_(send|recv|bulk)_|^sai_meta/)
        {
            LogWarning "wrong param names: $fnparams: $fname";
            LogWarning " expected: $params[0](| attr| attr_count attr_list| switch_id attr_count attr_list)";
        }

        if ($fname =~ /^sai_(get|set|create|remove)_(\w+?)(_attribute)?(_stats)?_fn/)
        {
            my $pattern = $2;
            my $first = $params[0];

            if ($pattern =~ /_entry$/)
            {
                $pattern = "${pattern}_id|${pattern}";
            }
            else
            {
                $pattern = "${pattern}_id";
            }

            if (not $first =~ /^$pattern$/)
            {
                LogWarning "first param should be called ${pattern} but is $first in $fname: $file";
            }
        }
    }
}

sub CheckDoxygenCommentFormating
{
    my ($data, $file) = @_;

    while ($data =~ m%/\*\*(?:(?!\*/).)*?(\*/\n[\n]+(\s*[a-z][^\n]*))%gis)
    {
        LogWarning "empty line between doxygen comment and definition: $file: $2";
    }

    while ($data =~ m%( *)(/\*\*(?:(?!\*/).)*?\*/)%gis)
    {
        my $spaces = $1 . " ";
        my $comment = $2;

        next if $comment =~ m!^/\*\*.*\*/$!; # single line comment

        my @lines = split/\n/,$comment;

        my $first = shift @lines;
        my $last = pop @lines;

        if (not $first =~ m!^\s*/..$!)
        {
            LogWarning "first line doxygen comment should be with '/**': $file: $first";
            next;
        }

        if (not $last =~ m!^\s*\*/$!)
        {
            LogWarning "last line doxygen comment should be '*/': $file: $last";
            next;
        }

        if (not $lines[0] =~ m!\* (\@|Copyright )!)
        {
            LogWarning "first doxygen line should contain \@ tag $file: $lines[0]";
        }

        if ($lines[$#lines] =~ m!^\s*\*\s*$!)
        {
            LogWarning "last doxygen line should not be empty $file: $lines[$#lines]";
        }

        for my $line (@lines)
        {
            if (not $line =~ m!^\s*\*( |$)!)
            {
                LogWarning "multiline doxygen comments should start with '* ': $file: $line";
            }

            if (not $line =~ /^$spaces\*/)
            {
                LogWarning "doxygen comment has invalid ident: $file: $line";
            }
        }
    }

    while($data =~ m!(([^\n ])+\n */\*\*.{1,30}.+?\n)!isg)
    {
        next if $2 eq "{";

        LogWarning "doxygen comment can't be upper sticked: $file:\n$1";
    }
}

sub CheckFunctionNaming
{
    my ($header, $n, $line) = @_;

    return if not $line =~ /^\s*sai_(\w+)_fn\s+(\w+)\s*;/;

    my $typename = $1;
    my $name = $2;

    if ($typename ne $name and not $typename =~ /^bulk_/)
    {
        LogWarning "function not matching $typename vs $name in $header:$n:$line";
    }

    if (not $name =~ /^(create|remove|get|set)_\w+?(_attribute)?|clear_\w+_stats$/)
    {
        # exceptions
        return if $name =~ /^(recv_hostif_packet|send_hostif_packet|flush_fdb_entries|profile_get_value|profile_get_next_value)$/;

        LogWarning "function not follow convention in $header:$n:$line";
    }
}

sub CheckQuadApi
{
    my ($data, $file) = @_;

    return if not $data =~ m!(sai_\w+_api_t)(.+?)\1;!igs;

    my $apis = $2;

    my @fns = $apis =~ /sai_(\w+)_fn/g;

    my $fn = join" ",@fns;

    my @quad = split/\bcreate_/,$fn;

    for my $q (@quad)
    {
        next if $q eq "";

        if (not $q =~ /(\w+) remove_\1 set_\1_attribute get_\1_attribute( |$)/)
        {
            LogWarning "quad api must be in order: create remove set get: $file: $q";
        }
    }
}

sub CheckSwitchKeys
{
    my ($data, $file) = @_;

    my $keycount = $1 if $data =~ /#define\s+SAI_SWITCH_ATTR_MAX_KEY_COUNT\s+(\d+)/s;

    my $count = 0;

    while ($data =~ m!#define\s+SAI_KEY_(\w+)\s+"SAI_(\w+)"!gs)
    {
        if ($1 ne $2)
        {
            LogWarning "SAI_(KEY_)$1 should match SAI_$2";
        }

        $count++;
    }

    if ($count != $keycount)
    {
        LogWarning "SAI_SWITCH_ATTR_MAX_KEY_COUNT is $keycount, but found only $count keys";
    }
}

sub CheckStructAlignment
{
    my ($data, $file) = @_;

    return if $file eq "saitypes.h";

    while ($data =~ m!typedef\s+struct\s+_(sai_\w+_t)(.+?)\1;!igs)
    {
        my $struct = $1;
        my $inner = $2;

        # we use space in regex since \s will capture \n

        $inner =~ m/^( *)(\w.+\s+)(\w+)\s*;$/im;

        my $spaces = $1;
        my $inside = $2;
        my $name = $3;

        while ($inner =~ m/^( *)(\w.+\s+)(\w+)\s*;$/gim)
        {
            my $itemname = $2;

            if ($1 ne $spaces or (length($2) != length($inside) and $struct =~ /_api_t/))
            {
                LogWarning "$struct items has invalid column ident: $file: $itemname";
            }
        }
    }
}

sub CheckHeadersStyle
{
    #
    # Purpose of this check is to find out
    # whether header files are correctly formated
    #
    # Wrong formating includes:
    # - multiple empty lines
    # - double spaces
    # - wrong spacing idient
    #

    my @magicWords = qw/SAI IP MAC L2 ACL L3 GRE ECMP EEE FDB FD FEC ICMP I2C
        HW IEEE IP2ME L2MC LAG ARP ASIC BGP CAM CBS CB CIR CIDR CRC DLL CPU TTL
        TOS ECN DSCP TC MACST MTU NPU PFC PBS PCI PIR QOS RFC RFP SDK RSPAN
        ERSPAN SPAN SNMP SSH STP TCAM TCP UDP TPID UDF UOID VNI VR VRRP WCMP
        WWW API CCITT RARP CFI MPLS IPMC RPF WRED XON XOFF NHLFE SG/;

    # we could put that to local dictionary file

    my @spellExceptions = qw/ http www apache MERCHANTABILITY Mellanox defgroup
        Enum param attr VLAN IPv4 IPv6 Vlan inout policer Src Dst Decrement
        lookups optimizations lookup bool EtherType tx rx validonly enum sai
        loopback Multicast isvlan 6th nexthop nexthopgroup encap decap src dst
        wildcard const APIs multi multicast LAGs Linux mcast HQoS
        childs callee Callee boolean attrvalue unicast Unicast untagged
        Untagged Policer objlist BGPv6 allownull 0xFF Hostif samplepacket
        Samplepacket pkts Loopback linklocal lossless Mbps vlan ucast
        ingressing MCAST netdev AUTONEG decapsulation egressing functionalities
        rv subnet subnets Uninitialize versa VRFs Netdevice netdevs PGs CRC32
        HQOS Wildcard VLANs VLAN2 SerDes FC Wakeup warmboot Inservice PVID PHY
        metadata Metadata TODO Facebook OID OIDs deserialize
        fprintf struct stderr
        /;

    my %exceptions = map { $_ => $_ } @spellExceptions;

    my %wordsToCheck = ();
    my %wordsChecked = ();

    my @headers = GetHeaderFiles();
    my @metaheaders = GetMetaHeaderFiles();

    @headers = (@headers, @metaheaders);

    for my $header (@headers)
    {
        next if $header eq "saimetadata.h"; # skip auto generated header

        my $data = ReadHeaderFile($header);

        my $oncedef = uc ("__${header}_");

        $oncedef =~ s/\./_/g;

        my $oncedefCount = 0;

        CheckHeaderHeader($data, $header);
        CheckFunctionsParams($data, $header);
        CheckDoxygenCommentFormating($data, $header);
        CheckQuadApi($data, $header);
        CheckStructAlignment($data, $header);
        CheckSwitchKeys($data, $header) if $header eq "saiswitch.h";

        my @lines = split/\n/,$data;

        my $n = 0;

        my $empty = 0;
        my $emptydoxy = 0;

        for my $line (@lines)
        {
            $n++;

            CheckFunctionNaming($header, $n, $line);

            $oncedefCount++ if $line =~/\b$oncedef\b/;

            # detect multiple empty lines

            if ($line =~ /^$/)
            {
                $empty++;

                if ($empty > 1)
                {
                    LogWarning "header contains two empty lines one after another $header $n";
                }
            }
            else { $empty = 0 }

            # detect multiple empty lines in doxygen comments

            if ($line =~ /^\s+\*\s*$/)
            {
                $emptydoxy++;

                if ($emptydoxy > 1)
                {
                    LogWarning "header contains two empty lines in doxygen $header $n";
                }
            }
            else { $emptydoxy = 0 }

            if ($line =~ /^\s+\* / and not $line =~ /\*( {4}| {8}| )[^ ]/)
            {
                LogWarning "not expected number of spaces after * (1,4,8) $header $n:$line";
            }

            if ($line =~ /\*\s+[^ ].*  / and not $line =~ /\* \@(brief|file)/)
            {
                if (not $line =~ /const.+const\s+\w+;/)
                {
                    LogWarning "too many spaces after *\\s+ $header $n:$line";
                }
            }

            if ($line =~ /(typedef|{|}|_In\w+|_Out\w+)( [^ ].*  |  )/ and not $line =~ /typedef\s+u?int/i)
            {
                LogWarning "too many spaces $header $n:$line";
            }

            if ($line =~ m!/\*\*! and not $line =~ m!/\*\*\s*$! and not $line =~ m!/\*\*.+\*/!)
            {
                LogWarning "multiline doxygen comment should start '/**' $header $n:$line";
            }

            if ($line =~ m![^ ]\*/!)
            {
                LogWarning "coment is ending without space $header $n:$line";
            }

            if ($line =~ /^\s*sai_(\w+)_fn\s+(\w+);/)
            {
                # make struct function members to follow convention

                LogWarning "$2 should be equal to $1" if (($1 ne $2) and not($1 =~ /^bulk/))
            }

            if ($line =~ /_(?:In|Out)\w+\s+(?:sai_)?uint32_t\s*\*?(\w+)/)
            {
                my $param = $1;

                my $pattern = '^(attr_count|object_count|number_of_counters|count)$';

                if (not $param =~ /$pattern/)
                {
                    LogWarning "param $param should match $pattern $header:$n:$line";
                }
            }

            if ($line =~ /typedef.+_fn\s*\)/ and not $line =~ /typedef( \S+)+ ?\(\*\w+_fn\)\(/)
            {
                LogWarning "wrong style typedef function definition $header:$n:$line";
            }

            if ($line =~ / ([.,:;)])/ and not $line =~ /\.(1D|1Q|\.\.)/)
            {
                LogWarning "space before '$1' : $header:$n:$line";
            }

            if ($line =~ / \* / and not $line =~ /^\s*\* /)
            {
                LogWarning "floating start: $header:$n:$line";
            }

            if ($line =~ /}[^ ]/)
            {
                LogWarning "no space after '}' $header:$n:$line";
            }

            if ($line =~ /_[IO].+\w+\* /)
            {
                LogWarning "star should be next to param name $header:$n:$line";
            }

            if ($line =~ /[^ ]\s*_(In|Out|Inout)_/ and not $line =~ /^#define/)
            {
                LogWarning "each param should be in separate line $header:$n:$line";
            }

            if ($line =~ m!/\*\*\s+[a-z]!)
            {
                #LogWarning "doxygen comment should start with capital letter: $header:$n:$line";
            }

            if ($line =~ /sai_\w+_statistics_fn/)
            {
                LogWarning "statistics should use 'stats' to follow convention $header:$n:$line";
            }

            if ($line =~ /#define\s*(\w+)/ and $header ne "saitypes.h")
            {
                my $defname = $1;

                if (not $defname =~ /^(SAI_|__SAI)/)
                {
                    LogWarning "define should start with SAI_ or __SAI: $header:$n:$line";
                }
            }

            if ($line =~/\s+$/)
            {
                LogWarning "line ends in whitespace $header $n: $line";
            }

            if ($line =~ /[^\x20-\x7e]/)
            {
                LogWarning "line contains non ascii characters $header $n: $line";
            }

            if ($line =~ /typedef .+?\(\s*\*\s*(\w+)\s*\)/)
            {
                my $fname = $1;

                if (not $fname =~ /^sai_\w+_fn$/)
                {
                    LogWarning "all function declarations should be in format sai_\\w+_fn $header $n: $line";
                }
            }

            my $pattern = join"|",@magicWords;

            while ($line =~ /\b($pattern)\b/igp)
            {
                my $pre = $`;
                my $post = $';

                # force special word to be capital

                my $word = $1;

                next if $word =~ /^($pattern)$/;
                next if $line =~ /$word.h/;
                next if not $line =~ /\*/; # must contain star, so will be comment
                next if "$pre$word" =~ m!http://$word$!;

                LogWarning "Word '$word' should use capital letters $header $n:$line";
            }

            # perform aspell checking (move to separate method)

            if ($line =~ m!^\s*(\*|/\*\*)!)
            {
                while ($line =~ /\b([a-z0-9']+)\b/ig)
                {
                    my $pre = $`;
                    my $post = $';
                    my $word = $1;

                    next if $word =~ /^($pattern)$/; # capital words

                    # look into good and bad words hash to speed things up

                    next if defined $exceptions{$word};
                    next if $word =~/^sai\w+/i;
                    next if $word =~/0x\S+L/;
                    next if "$pre$word" =~/802.\d+\w+/;

                    next if defined $wordsChecked{$word};

                    $wordsChecked{$word} = 1;

                    $wordsToCheck{$word} = "$header $n:$line";
                }
            }

            if ($line =~ /\\/ and not $line =~ /\\[0\[\]]/ and not $line =~ /\\$/)
            {
                LogWarning "line contains \\ which should not be used in this way $header $n:$line";
            }

            if ($line =~ /typedef\s*(enum|struct|union).*{/)
            {
                LogWarning "move '{' to new line in typedef $header $n:$line";
            }

            CheckDoxygenStyle($header, $line, $n);

            next if $line =~ /^ \*($|[ \/])/;       # doxygen comment
            next if $line =~ /^$/;                  # empty line
            next if $line =~ /^typedef /;           # type definition
            next if $line =~ /^sai_status/;         # return codes
            next if $line =~ /^sai_object/;         # return codes
            next if $line =~ /^extern /;            # extern in metadata
            next if $line =~ /^[{}#\/]/;            # start end of struct, define, start of comment
            next if $line =~ /^ {8}(_In|_Out|\.\.\.)/;     # function arguments
            next if $line =~ /^ {4}(sai_)/i;        # sai struct entry or SAI enum
            next if $line =~ /^ {4}\/\*/;           # doxygen start
            next if $line =~ /^ {5}\*/;             # doxygen comment continue
            next if $line =~ /^ {8}sai_/;           # union entry
            next if $line =~ /^ {4}union/;          # union
            next if $line =~ /^ {4}[{}]/;           # start or end of union
            next if $line =~ /^ {4}(u?int)/;        # union entries
            next if $line =~ /^ {4}(char|bool)/;    # union entries
            next if $line =~ /^ {8}bool booldata/;  # union bool
            next if $line =~ /^ {4}(true|false)/;   # bool definition
            next if $line =~ /^ {4}(const|size_t|else)/; # const in meta headers

            next if $line =~ m![^\\]\\$!; # macro multiline

            LogWarning "Header doesn't meet style requirements (most likely ident is not 4 or 8 spaces) $header $n:$line";
        }

        if ($oncedefCount != 3)
        {
            LogWarning "$oncedef should be used 3 times in header, but used $oncedefCount";
        }
    }

    if (not -e "/usr/bin/aspell")
    {
        LogError "ASPELL IS NOT PRESENT, please install aspell";
        return;
    }

    LogInfo "Running Aspell";

    my @keys = sort keys %wordsToCheck;

    my $count = @keys;

    my $all = "@keys";

    LogInfo "Words to check: $count";

    my @result = `echo "$all" | /usr/bin/aspell -l en -a`;

    for my $res (@result)
    {
        next if not $res =~ /^\s*&\s*(\S+)/;

        my $word = $1;

        chomp $res;

        my $where = "??";

        if (not defined $wordsToCheck{$word})
        {
            for my $k (@keys)
            {
                if ($k =~/(^$word|$word$)/)
                {
                    $where = $wordsToCheck{$k};
                    last;
                }

                $where = $wordsToCheck{$k} if ($k =~/$word/);
            }
        }
        else
        {
            $where = $wordsToCheck{$word};
        }

        LogWarning "Word '$word' is misspelled $where";
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

            my %meta = %{ $METADATA{$typedef}{$attr} };

            next if not defined $meta{objects};

            # we will also include RO attributes

            my @objects = @{ $meta{objects} };

            my $attrid = $meta{attrid};

            for my $usedot (@objects)
            {
                if (not defined $REVGRAPH{$usedot})
                {
                    my @arr = ();
                    $REVGRAPH{$usedot} = \@arr;
                }

                push$REVGRAPH{$usedot},"$ot,$attrid";
            }
        }

        next if not defined $STRUCTS{$ot};

        # handle non object id types

        my %struct = %{ $STRUCTS{$ot} };

        for my $key(keys %struct)
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

                push$REVGRAPH{$usedot},"$ot,$key";
            }

        }
    }

    return %REVGRAPH;
}

sub WriteTestHeader
{
    #
    # Purpose is to write saimedatatest.c header
    #

    WriteTest "#include <stdio.h>";
    WriteTest "#include <stdlib.h>";
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

sub CreateListCountTest
{
    #
    # purpose of this test is to check if all list structs have count as first
    # item so later on we can cast any structure to extract count
    #

    DefineTestName "list_count_test";

    WriteTest "{";

    my %Union = ExtractStructInfo("sai_attribute_value_t", "union");

    WriteTest "    size_t size_ref = sizeof(sai_object_list_t);";

    for my $key (keys %Union)
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
    my $type = shift;
    my $struct = shift;

    $type = $1 if $type =~/^(\w+)\*$/; # handle pointers

    return if defined $ProcessedItems{$type};

    return if defined $SAI_ENUMS{$type}; # struct entry is enum

    return if $type eq "union"; # union is special, but all union members are flattened anyway
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

    my %S = ExtractStructInfo($type, "struct_");

    for my $key (keys %S)
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

    my %Union = ExtractStructInfo("sai_attribute_value_t", "union");

    my @primitives = qw/sai_acl_action_data_t sai_acl_field_data_t sai_pointer_t sai_object_id_t sai_object_list_t char/;

    for my $key (keys %Union)
    {
        my $type = $Union{$key}{type};

        next if $type =~/sai_u?int\d+_t/;
        next if $type =~/sai_[su]\d+_list_t/;

        next if grep(/^$type$/, @primitives);

        ProcessStructItem($type, "sai_attribute_value_t");
    }
}

#
# MAIN
#

CheckHeadersStyle();

ExtractApiToObjectMap();

for my $file (GetXmlFiles($XMLDIR))
{
    LogInfo "Processing $file";

    ProcessXmlFile("$XMLDIR/$file");
}

# since sai_status is not enum
ProcessSaiStatus();

WriteHeader "#ifndef __SAI_METADATA_H__";
WriteHeader "#define __SAI_METADATA_H__";

CreateMetadataHeaderAndSource();

CreateMetadata();

CreateMetadataForAttributes();

CreateEnumHelperMethods();

ProcessNonObjectIdObjects();

CreateStructNonObjectId();

CreateApis();

CreateApisQuery();

CreateObjectInfo();

CreateListOfAllAttributes();

CheckApiStructNames();

CheckApiDefines();

CheckAttributeValueUnion();

WriteHeader "#endif /* __SAI_METADATA_H__ */";

# Test Section

WriteTestHeader();

CreateNonObjectIdTest();

CreateSwitchIdTest();

CreateCustomRangeTest();

CreatePointersTest();

CreateEnumSizeCheckTest();

CreateListCountTest();

WriteTestMain();

WriteLoggerVariables();

WriteMetaDataFiles();
