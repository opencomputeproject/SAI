#!/usr/bin/perl

use strict;
use warnings;
use diagnostics;

use XML::Simple qw(:strict);
use Getopt::Std;
use Data::Dumper;

# COLOR DEFINITIONS
my $colorDefault       = "\033[01;00m";
my $colorGreenBlue     = "\033[104;92m";
my $colorBlackYellow   = "\033[103;30m";
my $colorBlackRed      = "\033[31;7m";
my $colorRed           = "\033[66;91m";
my $colorGreen         = "\033[66;92m";
my $colorYellow        = "\033[66;93m";
my $colorBlue          = "\033[66;94m";
my $colorAqua          = "\033[66;96m";

my $errors = 0;
my $warnings = 0;
my $XMLDIR = "xml";
my $INCLUDEDIR = "../inc/";
my %SAI_ENUMS = ();
my %METADATA = ();
my %STRUCTS = ();
my %options =();

my $HEADER_CONTENT = "";
my $SOURCE_CONTENT = "";

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
    print "${colorGreen}@_${colorDefault}\n";
}

sub LogWarning
{
    $warnings++;
    print "${colorYellow}@_${colorDefault}\n";
}
sub LogError
{
    $errors++;
    print "${colorRed}@_${colorDefault}\n";
}

sub LogDebug
{
    print "${colorBlue}@_${colorDefault}\n" if $optionPrintDebug;
}

$SIG{__DIE__} = sub
{
    LogError "${colorBlackRed} FATAL ERROR === MUST FIX === : @_";
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

    if ($val =~/^(empty|special|vendor|const)/)
    {
        return $val;
    }

    if ($val =~/^(inherit|attrvalue) SAI_\w+_ATTR_\w+$/)
    {
        return $val;
    }

    if ($val =~/^(true|false|NULL|SAI_\w+|\d+)$/ and not $val =~ /_ATTR_|OBJECT_TYPE/)
    {
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

    $desc =~ s/@@/\n@@/g;

    while ($desc =~ /@@(\w+)(.+)/g)
    {
        my $tag = $1;
        my $val = $2;

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

        }

        # remove unnecessary attributes
        my @values = @{ $SAI_ENUMS{$enumtypename}{values} };
        @values = grep(!/^SAI_\w+_(START|END)$/, @values);
        @values = grep(!/^SAI_\w+(CUSTOM_RANGE_BASE)$/, @values);
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

sub ProcessSingleEnum
{
    my ($key, $typedef, $prefix) = @_;

    my $enum = $SAI_ENUMS{$key};

    my @values = @{$enum->{values}};

    WriteSource "const char metadata_${typedef}_enum_name[] = \"$typedef\";";
    WriteSource "const $typedef metadata_${typedef}_enum_values[] = {";

    for my $value (@values)
    {
        LogWarning "Value $value of $typedef is not prefixed as $prefix" if not $value =~ /^$prefix/;

        WriteSource "    $value,";
    }

    WriteSource "};";

    WriteSource "const char* metadata_${typedef}_enum_values_names[] = {";

    for my $value (@values)
    {
        WriteSource "    \"$value\",";
    }

    WriteSource "    NULL";
    WriteSource "};";

    WriteSource "const char* metadata_${typedef}_enum_values_short_names[] = {";

    for my $value (@values)
    {
        $value =~ s/^${prefix}//;

        WriteSource "    \"$value\",";
    }

    WriteSource "    NULL";
    WriteSource "};";

    my $count = $#values + 1;

    WriteSource "const size_t metadata_${typedef}_enum_values_count = $count;";

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

    WriteSource $HEAD;
    WriteSource "#include <stdio.h>";
    WriteSource "#include \"saimetadata.h\"";

    WriteSource "#define DEFINE_ENUM_METADATA(x,count)\\";
    WriteSource "const sai_enum_metadata_t metadata_enum_ ## x = {\\";
    WriteSource "    .name              = metadata_ ## x ## _enum_name,\\";
    WriteSource "    .valuescount       = count,\\";
    WriteSource "    .values            = (const int*)metadata_ ## x ## _enum_values,\\";
    WriteSource "    .valuesnames       = metadata_ ## x ## _enum_values_names,\\";
    WriteSource "    .valuesshortnames  = metadata_ ## x ## _enum_values_short_names,\\";
    WriteSource "};";

    for my $key (sort keys %SAI_ENUMS)
    {
        if (not $key =~ /^((sai_\w+_)t)$/)
        {
            LogWarning "Enum typedef $key is not matching SAI format";
            next;
        }

        my $count = ProcessSingleEnum($key, $1, uc $2);

        WriteHeader "extern const sai_enum_metadata_t metadata_enum_$1;";
        WriteSource "DEFINE_ENUM_METADATA($1, $count);";
    }

    # all enums

    WriteHeader "extern const sai_enum_metadata_t* metadata_all_enums[];";
    WriteSource "const sai_enum_metadata_t* metadata_all_enums[] = {";

    for my $key (sort keys %SAI_ENUMS)
    {
        if (not $key =~ /^((sai_\w+_)t)$/)
        {
            LogWarning "Enum typedef $key is not matching SAI format";
            next;
        }

        my $typedef = $1;

        WriteSource "    &metadata_enum_$typedef,";
    }

    WriteSource "    NULL";
    WriteSource "};";

    my $count = keys %SAI_ENUMS;

    WriteHeader "extern const size_t metadata_all_enums_count;";
    WriteSource "const size_t metadata_all_enums_count = $count;";

    WriteHeader "extern const sai_enum_metadata_t* metadata_attr_enums[];";
    WriteSource "const sai_enum_metadata_t* metadata_attr_enums[] = {";

    $count = 0;

    for my $key (sort keys %SAI_ENUMS)
    {
        if (not $key =~ /^(sai_\w+_attr_t)$/)
        {
            next;
        }

        my $typedef = $1;

        WriteSource "    &metadata_enum_$typedef,";

        $count++;
    }

    WriteSource "    NULL";
    WriteSource "};";

    WriteHeader "extern const size_t metadata_attr_enums_count;";
    WriteSource "const size_t metadata_attr_enums_count = $count;";

    # attr enums as object types for sanity check

    WriteSource "const sai_object_type_t metadata_object_types[] = {";

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

    WriteSource "const sai_object_type_t metadata_${attr}_allowed_objects[] = {";

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

    return "metadata_${attr}_allowed_objects";
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

    return "SAI_DEFAULT_VALUE_TYPE_CONST" if $default =~ /^(true|false|const|NULL|\d+|SAI_\w+)$/ and not $default =~ /_ATTR_|SAI_OBJECT_TYPE_/;

    return "SAI_DEFAULT_VALUE_TYPE_INHERIT" if $default =~ /^inherit SAI_\w+$/ and $default =~ /_ATTR_/;

    return "SAI_DEFAULT_VALUE_TYPE_EMPTY_LIST" if $default =~ /^empty$/;

    return "SAI_DEFAULT_VALUE_TYPE_VENDOR_SPECIFIC" if $default =~ /^vendor$/;

    return "SAI_DEFAULT_VALUE_TYPE_ATTR_VALUE" if $default =~ /^attrvalue SAI_\w+$/ and $default =~ /_ATTR_/;

    return "SAI_DEFAULT_VALUE_TYPE_ATTR_RANGE" if $default =~ /^attrrange SAI_\w+$/ and $default =~ /_ATTR_/;

    LogError "invalid default value type '$default' on $attr";

    return "";
}

sub ProcessDefaultValue
{
    my ($attr, $default, $type) = @_;

    return "NULL" if not defined $default;

    my $val = "const sai_attribute_value_t metadata_${attr}_default_value";

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
    elsif ($default =~ /^\d+$/ and $type =~ /sai_u?int\d+_t/)
    {
        WriteSource "$val = { .$VALUE_TYPES{$type} = $default };";
    }
    elsif ($default =~ /^NULL$/ and $type =~ /sai_pointer_t/)
    {
        WriteSource "$val = { .$VALUE_TYPES{$type} = $default };";
    }
    elsif ($default =~ /^inherit/)
    {
        WriteSource "$val = { };";
    }
    elsif ($default =~ /^(attrvalue|attrrange|vendor|empty|const)/)
    {
        return "NULL";
    }
    else
    {
        LogError "invalid default value '$default' on $attr ($type)";
    }

    return "&metadata_${attr}_default_value";
}

sub ProcessDefaultValueObjectType
{
    my ($attr, $value, $type) = @_;

    $value = "" if not defined $value;

    return "SAI_OBJECT_TYPE_$2" if $value =~ /^(inherit|attrvalue|attrrange) SAI_(\w+)_ATTR_\w+$/;

    return "SAI_OBJECT_TYPE_NULL";
}

sub ProcessDefaultValueAttrId
{
    my ($attr, $value, $type) = @_;

    $value = "" if not defined $value;

    return $2 if $value =~ /^(inherit|attrvalue|attrrange) ((SAI_\w+)_ATTR_\w+)$/;

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

    return "&metadata_enum_$1" if $type =~ /^(sai_\w+_t)$/ and not defined $VALUE_TYPES{$type};
    return "&metadata_enum_$1" if $type =~ /^sai_acl_field_data_t (sai_\w+_t)$/ and not defined $ACL_FIELD_TYPES{$1};
    return "&metadata_enum_$1" if $type =~ /^sai_acl_action_data_t (sai_\w+_t)$/ and not defined $ACL_ACTION_TYPES{$1};
    return "&metadata_enum_$1" if $type =~ /^sai_s32_list_t (sai_\w+_t)$/;

    return "NULL";
}

sub ProcessIsVlan
{
    my ($attr, $value) = @_;

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

        WriteSource "const sai_attr_condition_t metadata_condition_${attr}_$count = {";

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

    WriteSource "const sai_attr_condition_t* metadata_conditions_${attr}\[\] = {";

    $count = 0;

    for my $cond (@conditions)
    {
        WriteSource "    &metadata_condition_${attr}_$count,";

        $count++;
    }

    WriteSource "    NULL";

    WriteSource "};";

    return "metadata_conditions_${attr}";
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

        WriteSource "const sai_attr_condition_t metadata_validonly_${attr}_$count = {";

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

    WriteSource "const sai_attr_condition_t* metadata_validonly_${attr}\[\] = {";

    $count = 0;

    for my $cond (@conditions)
    {
        WriteSource "    &metadata_validonly_${attr}_$count,";

        $count++;
    }

    WriteSource "    NULL";

    WriteSource "};";

    return "metadata_validonly_${attr}";
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
        my $isvlan          = ProcessIsVlan($attr, $meta{isvlan});
        my $getsave         = ProcessGetSave($attr, $meta{getsave});

        WriteSource "const sai_attr_metadata_t metadata_attr_$attr = {";

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
        WriteSource "    .validonlytype                 = $validonlytype,";
        WriteSource "    .validonly                     = $validonly,";
        WriteSource "    .validonlylength               = $validonlylen,";
        WriteSource "    .getsave                       = $getsave,";
        WriteSource "    .isvlan                        = $isvlan,";

        WriteSource "};";

        # check enum attributes if their names are ending on enum name

        if ($isenum eq "true" or $isenumlist eq "true")
        {
            my $en = uc($1) if $meta{type} =~/.*sai_(\S+)_t/;

            next if $attr =~ /_${en}_LIST$/;
            next if $attr =~ /_$en$/;

            $attr =~/SAI_(\S+?)_ATTR_(\S+)/;

            my $aot = $1;
            my $aend = $1;

            if ($en =~/^${aot}_(\S+)$/)
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

sub WriteMetaDataFiles
{
    exit 1 if ($warnings > 0 || $errors > 0);

    WriteFile("saimetadata.h", $HEADER_CONTENT);
    WriteFile("saimetadata.c", $SOURCE_CONTENT);
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

        WriteSource "const sai_attr_metadata_t* metadata_object_type_$type\[\] = {";

        my @values = @{ $SAI_ENUMS{$type}{values} };

        for my $value (@values)
        {
            next if defined $METADATA{$type}{$value}{ignore};

            WriteSource "    &metadata_attr_$value,";
        }

        WriteSource "    NULL";
        WriteSource "};";
    }

    WriteHeader "extern const sai_attr_metadata_t** metadata_attr_by_object_type[];";
    WriteSource "const sai_attr_metadata_t** metadata_attr_by_object_type[] = {";

    for my $ot (@objects)
    {
        if (not $ot =~ /^SAI_OBJECT_TYPE_(\w+)$/)
        {
            LogError "invalid obejct type '$ot'";
            next;
        }

        my $type = "sai_" . lc($1) . "_attr_t";

        WriteSource "    metadata_object_type_$type,";
    }

    WriteSource "    NULL";
    WriteSource "};";

    my $count = $#objects + 1;

    WriteHeader "extern const size_t metadata_attr_by_object_type_count;";
    WriteSource "const size_t metadata_attr_by_object_type_count = $count;";
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
        WriteSource "    return sai_metadata_get_enum_value_name(&metadata_enum_$key, value);";
        WriteSource "}";

        WriteHeader "extern const char* sai_metadata_get_$1_name(";
        WriteHeader "        _In_ $key value);";
    }

    WriteHeader "extern const char* sai_metadata_get_enum_value_name(";
    WriteHeader "        _In_ const sai_enum_metadata_t* metadata,";
    WriteHeader "        _In_ int value);";
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


    WriteSource "const sai_object_type_t metadata_struct_member_sai_${rawname}_t_${key}_allowed_objects[] = {";

    my $objects = $struct->{objects};

    for my $obj (@{ $objects })
    {
        WriteSource "    $obj,";
    }

    WriteSource "};";

    return "metadata_struct_member_sai_${rawname}_t_${key}_allowed_objects";
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

        WriteSource "const sai_struct_member_info_t struct_member_sai_${rawname}_t_$key = {";

        WriteSource "    .membervaluetype           = $valuetype,";
        WriteSource "    .membername                = \"$key\",";
        WriteSource "    .isvlan                    = $isvlan,";
        WriteSource "    .allowedobjecttypes        = $objects,";
        WriteSource "    .allowedobjecttypeslength  = $objectlen,";

        # TODO add enum type is value is enum
        # TODO allow null

        WriteSource "};";
    }

    WriteSource "const sai_struct_member_info_t* struct_members_sai_${rawname}_t[] = {";

    for my $key (@keys)
    {
        WriteSource "    &struct_member_sai_${rawname}_t_$key,";
    }

    WriteSource "    NULL";
    WriteSource "};";

    return "struct_members_sai_${rawname}_t";
}

sub ProcessStructMembersCount
{
    my $struct = shift;

    return "0" if not defined $struct;

    my @keys = keys $struct;
    my $count = @keys;

    return $count;
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

        my $enum  = "&metadata_enum_${type}";

        my $struct = $STRUCTS{$ot};

        my $isnonobjectid = ProcessIsNonObjectId($struct, $ot);
        my $structmembers = ProcessStructMembers($struct, $ot ,lc($1));
        my $structmemberscount = ProcessStructMembersCount($struct, $ot);

        WriteHeader "extern const sai_object_type_info_t sai_object_type_info_$ot;";

        WriteSource "const sai_object_type_info_t sai_object_type_info_$ot = {";
        WriteSource "    .objecttype         = $ot,";
        WriteSource "    .attridstart        = $start,";
        WriteSource "    .attridend          = $end,";
        WriteSource "    .enummetadata       = $enum,";
        WriteSource "    .attrmetadata       = metadata_object_type_$type,";
        WriteSource "    .isnonobjectid      = $isnonobjectid,";
        WriteSource "    .structmembers      = $structmembers,";
        WriteSource "    .structmemberscount = $structmemberscount,";
        WriteSource "};";
    }

    WriteHeader "extern const sai_object_type_info_t* sai_all_object_type_infos[];";

    WriteSource "const sai_object_type_info_t* sai_all_object_type_infos[] = {";

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

        WriteSource "    &sai_object_type_info_$ot,";
    }

    WriteSource "    NULL";
    WriteSource "};";
}

sub GetHeaderFiles
{
    opendir(my $dh, $INCLUDEDIR) || die "Can't opendir $INCLUDEDIR: $!";
    my @headers = grep { /^sai\S+\.h$/ && -f "$INCLUDEDIR/$_" } readdir($dh);
    closedir $dh;

    return @headers;
}

sub ReadHeaderFile
{
    my $filename = shift;
    local $/ = undef;
    open FILE, "$INCLUDEDIR/$filename" or die "Couldn't open file $INCLUDEDIR/$filename: $!";
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

        while ($data =~ /sai_(?:create|set)_\S+.+?\n.+const\s+(sai_(\w+)_t)/gim)
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

sub CreateNonObjectIdTest
{
    WriteSource "void non_object_id_test(void)";
    WriteSource "{";

    WriteSource "    sai_object_key_t ok;";
    WriteSource "    void *p;";

    my @rawnames = GetNonObjectIdStructNames();

    WriteSource "    p = &ok.key.object_id;";
    WriteSource "    printf(\"%p\",p);";

    for my $rawname (@rawnames)
    {
        WriteSource "    p = &ok.key.$rawname;";
        WriteSource "    printf(\"%p\",p);";
    }

    WriteSource "}";
}

sub ExtractStructInfo
{
    my $struct = shift;

    my %S = ();

    my $filename = "struct_${struct}.xml";

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
        my $type = $1 if $member->{definition}[0] =~ /^(\S+)/;

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

    my %struct = ExtractStructInfo($structname);

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

    WriteSource "const sai_attr_metadata_t* metadata_attr_sorted_by_id_name[] = {";
    WriteHeader "extern const sai_attr_metadata_t* metadata_attr_sorted_by_id_name[];";

    my @keys = sort keys %ATTRIBUTES;

    for my $attr (@keys)
    {
        WriteSource "    &metadata_attr_$attr,";
    }

    my $count = @keys;

    WriteSource "    NULL";
    WriteSource "};";

    WriteSource "const size_t metadata_attr_sorted_by_id_name_count = $count;";
    WriteHeader "extern const size_t metadata_attr_sorted_by_id_name_count;";
}

#
# MAIN
#

for my $file (GetXmlFiles($XMLDIR))
{
    LogInfo "Processing $file";

    ProcessXmlFile("$XMLDIR/$file");
}

# since sai_status is not enum
ProcessSaiStatus();

WriteHeader "#ifndef __SAI_METADATA_TYPES__";
WriteHeader "#define __SAI_METADATA_TYPES__";

CreateMetadataHeaderAndSource();

CreateMetadata();

CreateMetadataForAttributes();

CreateEnumHelperMethods();

CreateNonObjectIdTest();

ProcessNonObjectIdObjects();

CreateObjectInfo();

CreateListOfAllAttributes();

WriteHeader "#endif /* __SAI_METADATA_TYPES__ */";

WriteMetaDataFiles();
