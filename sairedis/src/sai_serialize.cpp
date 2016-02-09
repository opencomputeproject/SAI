
#include "sai_serialize.h"

sai_serialization_map_t g_serialization_map = sai_get_serialization_map();
sai_object_type_to_string_map_t g_object_type_map = sai_get_object_type_map();

sai_serialization_map_t sai_get_serialization_map()
{
    sai_serialization_map_t map;

    map[SAI_OBJECT_TYPE_PORT][SAI_PORT_ATTR_SPEED] = SAI_SERIALIZATION_TYPE_UINT32;
    map[SAI_OBJECT_TYPE_PORT][SAI_PORT_ATTR_OPER_STATUS] = SAI_SERIALIZATION_TYPE_INT32;
    map[SAI_OBJECT_TYPE_PORT][SAI_PORT_ATTR_PORT_VLAN_ID] = SAI_SERIALIZATION_TYPE_UINT16;
    map[SAI_OBJECT_TYPE_PORT][SAI_PORT_ATTR_FDB_LEARNING] = SAI_SERIALIZATION_TYPE_INT32;

    map[SAI_OBJECT_TYPE_NEXT_HOP][SAI_NEXT_HOP_ATTR_TYPE] = SAI_SERIALIZATION_TYPE_INT32;
    map[SAI_OBJECT_TYPE_NEXT_HOP][SAI_NEXT_HOP_ATTR_IP] = SAI_SERIALIZATION_TYPE_IP_ADDRESS;
    map[SAI_OBJECT_TYPE_NEXT_HOP][SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID] = SAI_SERIALIZATION_TYPE_OBJECT_ID;

    map[SAI_OBJECT_TYPE_NEXT_HOP_GROUP][SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_COUNT] = SAI_SERIALIZATION_TYPE_UINT32;
    map[SAI_OBJECT_TYPE_NEXT_HOP_GROUP][SAI_NEXT_HOP_GROUP_ATTR_TYPE] = SAI_SERIALIZATION_TYPE_INT32;
    map[SAI_OBJECT_TYPE_NEXT_HOP_GROUP][SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_LIST] = SAI_SERIALIZATION_TYPE_OBJECT_LIST;

    map[SAI_OBJECT_TYPE_ROUTER_INTERFACE][SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID] = SAI_SERIALIZATION_TYPE_OBJECT_ID;
    map[SAI_OBJECT_TYPE_ROUTER_INTERFACE][SAI_ROUTER_INTERFACE_ATTR_TYPE] = SAI_SERIALIZATION_TYPE_INT32;
    map[SAI_OBJECT_TYPE_ROUTER_INTERFACE][SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS] = SAI_SERIALIZATION_TYPE_MAC;
    map[SAI_OBJECT_TYPE_ROUTER_INTERFACE][SAI_ROUTER_INTERFACE_ATTR_VLAN_ID] = SAI_SERIALIZATION_TYPE_MAC;

    map[SAI_OBJECT_TYPE_HOST_INTERFACE][SAI_HOSTIF_ATTR_TYPE] = SAI_SERIALIZATION_TYPE_INT32;
    map[SAI_OBJECT_TYPE_HOST_INTERFACE][SAI_HOSTIF_ATTR_RIF_OR_PORT_ID] = SAI_SERIALIZATION_TYPE_OBJECT_ID;
    map[SAI_OBJECT_TYPE_HOST_INTERFACE][SAI_HOSTIF_ATTR_NAME] = SAI_SERIALIZATION_TYPE_CHARDATA;

    map[SAI_OBJECT_TYPE_NEIGHBOR][SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS] = SAI_SERIALIZATION_TYPE_MAC;

    map[SAI_OBJECT_TYPE_ROUTE][SAI_ROUTE_ATTR_PACKET_ACTION] = SAI_SERIALIZATION_TYPE_INT32;
    map[SAI_OBJECT_TYPE_ROUTE][SAI_ROUTE_ATTR_TRAP_PRIORITY] = SAI_SERIALIZATION_TYPE_UINT8;
    map[SAI_OBJECT_TYPE_ROUTE][SAI_ROUTE_ATTR_NEXT_HOP_ID] = SAI_SERIALIZATION_TYPE_OBJECT_ID;
    map[SAI_OBJECT_TYPE_ROUTE][SAI_ROUTE_ATTR_META_DATA] = SAI_SERIALIZATION_TYPE_UINT32;

    map[SAI_OBJECT_TYPE_SWITCH][SAI_SWITCH_ATTR_SWITCHING_MODE] = SAI_SERIALIZATION_TYPE_BOOL;
    map[SAI_OBJECT_TYPE_SWITCH][SAI_SWITCH_ATTR_PORT_NUMBER] = SAI_SERIALIZATION_TYPE_UINT32;
    map[SAI_OBJECT_TYPE_SWITCH][SAI_SWITCH_ATTR_PORT_LIST] = SAI_SERIALIZATION_TYPE_OBJECT_LIST;
    map[SAI_OBJECT_TYPE_SWITCH][SAI_SWITCH_ATTR_PORT_MAX_MTU] = SAI_SERIALIZATION_TYPE_UINT32;
    map[SAI_OBJECT_TYPE_SWITCH][SAI_SWITCH_ATTR_CPU_PORT] = SAI_SERIALIZATION_TYPE_OBJECT_ID;

    map[SAI_OBJECT_TYPE_FDB][SAI_FDB_ENTRY_ATTR_TYPE] = SAI_SERIALIZATION_TYPE_INT32;

    map[SAI_OBJECT_TYPE_VLAN][SAI_VLAN_ATTR_MAX_LEARNED_ADDRESSES] = SAI_SERIALIZATION_TYPE_UINT32;

    return map;
}

sai_object_type_to_string_map_t sai_get_object_type_map()
{
    sai_object_type_to_string_map_t map;

    map[SAI_OBJECT_TYPE_NULL] = TO_STR(SAI_OBJECT_TYPE_NULL);
    map[SAI_OBJECT_TYPE_PORT] = TO_STR(SAI_OBJECT_TYPE_PORT);
    map[SAI_OBJECT_TYPE_LAG] = TO_STR(SAI_OBJECT_TYPE_LAG);
    map[SAI_OBJECT_TYPE_VIRTUAL_ROUTER] = TO_STR(SAI_OBJECT_TYPE_VIRTUAL_ROUTER);
    map[SAI_OBJECT_TYPE_NEXT_HOP] = TO_STR(SAI_OBJECT_TYPE_NEXT_HOP);
    map[SAI_OBJECT_TYPE_NEXT_HOP_GROUP] = TO_STR(SAI_OBJECT_TYPE_NEXT_HOP_GROUP);
    map[SAI_OBJECT_TYPE_ROUTER_INTERFACE] = TO_STR(SAI_OBJECT_TYPE_ROUTER_INTERFACE);
    map[SAI_OBJECT_TYPE_ACL_TABLE] = TO_STR(SAI_OBJECT_TYPE_ACL_TABLE);
    map[SAI_OBJECT_TYPE_ACL_ENTRY] = TO_STR(SAI_OBJECT_TYPE_ACL_ENTRY);
    map[SAI_OBJECT_TYPE_ACL_COUNTER] = TO_STR(SAI_OBJECT_TYPE_ACL_COUNTER);
    map[SAI_OBJECT_TYPE_HOST_INTERFACE] = TO_STR(SAI_OBJECT_TYPE_HOST_INTERFACE);
    map[SAI_OBJECT_TYPE_MIRROR] = TO_STR(SAI_OBJECT_TYPE_MIRROR);
    map[SAI_OBJECT_TYPE_SAMPLEPACKET] = TO_STR(SAI_OBJECT_TYPE_SAMPLEPACKET);
    map[SAI_OBJECT_TYPE_STP_INSTANCE] = TO_STR(SAI_OBJECT_TYPE_STP_INSTANCE);
    map[SAI_OBJECT_TYPE_TRAP_GROUP] = TO_STR(SAI_OBJECT_TYPE_TRAP_GROUP);
    map[SAI_OBJECT_TYPE_ACL_TABLE_GROUP] = TO_STR(SAI_OBJECT_TYPE_ACL_TABLE_GROUP);
    map[SAI_OBJECT_TYPE_POLICER] = TO_STR(SAI_OBJECT_TYPE_POLICER);
    map[SAI_OBJECT_TYPE_WRED] = TO_STR(SAI_OBJECT_TYPE_WRED);
    map[SAI_OBJECT_TYPE_QOS_MAPS] = TO_STR(SAI_OBJECT_TYPE_QOS_MAPS);
    map[SAI_OBJECT_TYPE_QUEUE] = TO_STR(SAI_OBJECT_TYPE_QUEUE);
    map[SAI_OBJECT_TYPE_SCHEDULER] = TO_STR(SAI_OBJECT_TYPE_SCHEDULER);
    map[SAI_OBJECT_TYPE_SCHEDULER_GROUP] = TO_STR(SAI_OBJECT_TYPE_SCHEDULER_GROUP);
    map[SAI_OBJECT_TYPE_BUFFER_POOL] = TO_STR(SAI_OBJECT_TYPE_BUFFER_POOL);
    map[SAI_OBJECT_TYPE_BUFFER_PROFILE] = TO_STR(SAI_OBJECT_TYPE_BUFFER_PROFILE);
    map[SAI_OBJECT_TYPE_PRIORITY_GROUP] = TO_STR(SAI_OBJECT_TYPE_PRIORITY_GROUP);
    map[SAI_OBJECT_TYPE_LAG_MEMBER] = TO_STR(SAI_OBJECT_TYPE_LAG_MEMBER);
    map[SAI_OBJECT_TYPE_HASH] = TO_STR(SAI_OBJECT_TYPE_HASH);
    map[SAI_OBJECT_TYPE_UDF] = TO_STR(SAI_OBJECT_TYPE_UDF);
    map[SAI_OBJECT_TYPE_UDF_MATCH] = TO_STR(SAI_OBJECT_TYPE_UDF_MATCH);
    map[SAI_OBJECT_TYPE_UDF_GROUP] = TO_STR(SAI_OBJECT_TYPE_UDF_GROUP);
    map[SAI_OBJECT_TYPE_FDB] = TO_STR(SAI_OBJECT_TYPE_FDB);
    map[SAI_OBJECT_TYPE_SWITCH] = TO_STR(SAI_OBJECT_TYPE_SWITCH);
    map[SAI_OBJECT_TYPE_TRAP] = TO_STR(SAI_OBJECT_TYPE_TRAP);
    map[SAI_OBJECT_TYPE_TRAP_USER_DEF] = TO_STR(SAI_OBJECT_TYPE_TRAP_USER_DEF);
    map[SAI_OBJECT_TYPE_NEIGHBOR] = TO_STR(SAI_OBJECT_TYPE_NEIGHBOR);
    map[SAI_OBJECT_TYPE_ROUTE] = TO_STR(SAI_OBJECT_TYPE_ROUTE);
    map[SAI_OBJECT_TYPE_VLAN] = TO_STR(SAI_OBJECT_TYPE_VLAN);

    return map;
}

sai_status_t sai_get_object_type_string(sai_object_type_t object_type, std::string &str_object_type)
{
    auto it = g_object_type_map.find(object_type);

    if (it == g_object_type_map.end())
    {
        fprintf(stderr, "serialization object not found type not found");
        return SAI_STATUS_NOT_IMPLEMENTED;
    }

    str_object_type = it->second;

    return SAI_STATUS_SUCCESS;
}

sai_status_t sai_get_serialization_type(
        _In_ const sai_object_type_t object_type,
        _In_ const sai_attr_id_t attr_id,
        _Out_ sai_attr_serialization_type_t &serialization_type)
{
    auto it = g_serialization_map.find(object_type);

    if (it == g_serialization_map.end())
    {
        fprintf(stderr, "serialization object not found type not found");
        return SAI_STATUS_NOT_IMPLEMENTED;
    }

    std::map<sai_attr_id_t, sai_attr_serialization_type_t> &map = it->second;

    auto mit = map.find(attr_id);

    if (mit == map.end())
    {
        fprintf(stderr, "serialization attribute not found");
        return SAI_STATUS_NOT_IMPLEMENTED;
    }

    serialization_type = mit->second;

    return SAI_STATUS_SUCCESS;
}

sai_status_t sai_serialize_attr_id(
        _In_ const sai_attribute_t &attr,
        _Out_ std::string &s)
{
    sai_serialize_primitive(attr.id, s);

    return SAI_STATUS_SUCCESS;
}

sai_status_t sai_serialize_attr_value(
        _In_ const sai_attr_serialization_type_t type,
        _In_ const sai_attribute_t &attr,
        _Out_ std::string &s)
{
    switch (type)
    {
        case SAI_SERIALIZATION_TYPE_BOOL:
            sai_serialize_primitive(attr.value.booldata, s);
            break;

        case SAI_SERIALIZATION_TYPE_CHARDATA:
            sai_serialize_primitive(attr.value.chardata, s);
            break;

        case SAI_SERIALIZATION_TYPE_UINT8:
            sai_serialize_primitive(attr.value.u8, s);
            break;

        case SAI_SERIALIZATION_TYPE_INT8:
            sai_serialize_primitive(attr.value.s8, s);
            break;

        case SAI_SERIALIZATION_TYPE_UINT16:
            sai_serialize_primitive(attr.value.u16, s);
            break;

        case SAI_SERIALIZATION_TYPE_INT16:
            sai_serialize_primitive(attr.value.s16, s);
            break;

        case SAI_SERIALIZATION_TYPE_UINT32:
            sai_serialize_primitive(attr.value.u32, s);
            break;

        case SAI_SERIALIZATION_TYPE_INT32:
            sai_serialize_primitive(attr.value.s32, s);
            break;

        case SAI_SERIALIZATION_TYPE_UINT64:
            sai_serialize_primitive(attr.value.u64, s);
            break;

        case SAI_SERIALIZATION_TYPE_INT64:
            sai_serialize_primitive(attr.value.s64, s);
            break;

        case SAI_SERIALIZATION_TYPE_MAC:
            sai_serialize_primitive(attr.value.mac, s);
            break;

        case SAI_SERIALIZATION_TYPE_IP4:
            sai_serialize_primitive(attr.value.ip4, s);
            break;

        case SAI_SERIALIZATION_TYPE_IP6:
            sai_serialize_primitive(attr.value.ip6, s);
            break;

        case SAI_SERIALIZATION_TYPE_IP_ADDRESS:
            sai_serialize_primitive(attr.value.ipaddr, s);
            break;

        case SAI_SERIALIZATION_TYPE_OBJECT_ID:
            sai_serialize_primitive(attr.value.oid, s);
            break;

        case SAI_SERIALIZATION_TYPE_OBJECT_LIST:
            sai_serialize_list(attr.value.objlist, s);
            break;

        case SAI_SERIALIZATION_TYPE_UINT8_LIST:
            sai_serialize_list(attr.value.u8list, s);
            break;

        case SAI_SERIALIZATION_TYPE_INT8_LIST:
            sai_serialize_list(attr.value.s8list, s);
            break;

        case SAI_SERIALIZATION_TYPE_UINT16_LIST:
            sai_serialize_list(attr.value.u16list, s);
            break;

        case SAI_SERIALIZATION_TYPE_INT16_LIST:
            sai_serialize_list(attr.value.s16list, s);
            break;

        case SAI_SERIALIZATION_TYPE_UINT32_LIST:
            sai_serialize_list(attr.value.u32list, s);
            break;

        case SAI_SERIALIZATION_TYPE_INT32_LIST:
            sai_serialize_list(attr.value.s32list, s);
            break;

        case SAI_SERIALIZATION_TYPE_UINT32_RANGE:
            sai_serialize_primitive(attr.value.u32range, s);
            break;

        case SAI_SERIALIZATION_TYPE_INT32_RANGE:
            sai_serialize_primitive(attr.value.s32range, s);
            break;

        case SAI_SERIALIZATION_TYPE_VLAN_LIST:
            sai_serialize_list(attr.value.vlanlist, s);
            break;

        case SAI_SERIALIZATION_TYPE_VLAN_PORT_LIST:
            sai_serialize_list(attr.value.vlanportlist, s);
            break;

        case SAI_SERIALIZATION_TYPE_PORT_BREAKOUT:
            sai_serialize_primitive(attr.value.portbreakout.breakout_mode, s);
            sai_serialize_list(attr.value.portbreakout.port_list, s);
            break;

        case SAI_SERIALIZATION_TYPE_QOS_MAP_LIST:
            sai_serialize_list(attr.value.qosmap, s);
            break;

            /* ACL FIELD DATA */

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_UINT8:
            sai_serialize_primitive(attr.value.aclfield.enable, s);
            sai_serialize_primitive(attr.value.aclfield.mask.u8, s);
            sai_serialize_primitive(attr.value.aclfield.data.u8, s);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_INT8:
            sai_serialize_primitive(attr.value.aclfield.enable, s);
            sai_serialize_primitive(attr.value.aclfield.mask.s8, s);
            sai_serialize_primitive(attr.value.aclfield.data.s8, s);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_UINT16:
            sai_serialize_primitive(attr.value.aclfield.enable, s);
            sai_serialize_primitive(attr.value.aclfield.mask.u16, s);
            sai_serialize_primitive(attr.value.aclfield.data.u16, s);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_INT16:
            sai_serialize_primitive(attr.value.aclfield.enable, s);
            sai_serialize_primitive(attr.value.aclfield.mask.s16, s);
            sai_serialize_primitive(attr.value.aclfield.data.s16, s);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_UINT32:
            sai_serialize_primitive(attr.value.aclfield.enable, s);
            sai_serialize_primitive(attr.value.aclfield.mask.u16, s);
            sai_serialize_primitive(attr.value.aclfield.data.u16, s);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_INT32:
            sai_serialize_primitive(attr.value.aclfield.enable, s);
            sai_serialize_primitive(attr.value.aclfield.mask.s32, s);
            sai_serialize_primitive(attr.value.aclfield.data.s32, s);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_MAC:
            sai_serialize_primitive(attr.value.aclfield.enable, s);
            sai_serialize_primitive(attr.value.aclfield.mask.mac, s);
            sai_serialize_primitive(attr.value.aclfield.data.mac, s);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_IP4:
            sai_serialize_primitive(attr.value.aclfield.enable, s);
            sai_serialize_primitive(attr.value.aclfield.mask.ip4, s);
            sai_serialize_primitive(attr.value.aclfield.data.ip4, s);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_IP6:
            sai_serialize_primitive(attr.value.aclfield.enable, s);
            sai_serialize_primitive(attr.value.aclfield.mask.ip6, s);
            sai_serialize_primitive(attr.value.aclfield.data.ip6, s);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_OBJECT_ID:
            sai_serialize_primitive(attr.value.aclfield.enable, s);
            sai_serialize_primitive(attr.value.aclfield.data.oid, s);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_OBJECT_LIST:
            sai_serialize_primitive(attr.value.aclfield.enable, s);
            sai_serialize_list(attr.value.aclfield.data.objlist, s);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_UINT8_LIST:
            sai_serialize_primitive(attr.value.aclfield.enable, s);
            sai_serialize_list(attr.value.aclfield.mask.u8list, s);
            sai_serialize_list(attr.value.aclfield.data.u8list, s);
            break;

            /* ACL ACTION DATA */

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_UINT8:
            sai_serialize_primitive(attr.value.aclaction.enable, s);
            sai_serialize_primitive(attr.value.aclaction.parameter.u8, s);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_INT8:
            sai_serialize_primitive(attr.value.aclaction.enable, s);
            sai_serialize_primitive(attr.value.aclaction.parameter.s8, s);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_UINT16:
            sai_serialize_primitive(attr.value.aclaction.enable, s);
            sai_serialize_primitive(attr.value.aclaction.parameter.u16, s);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_INT16:
            sai_serialize_primitive(attr.value.aclaction.enable, s);
            sai_serialize_primitive(attr.value.aclaction.parameter.s16, s);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_UINT32:
            sai_serialize_primitive(attr.value.aclaction.enable, s);
            sai_serialize_primitive(attr.value.aclaction.parameter.u32, s);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_INT32:
            sai_serialize_primitive(attr.value.aclaction.enable, s);
            sai_serialize_primitive(attr.value.aclaction.parameter.s32, s);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_MAC:
            sai_serialize_primitive(attr.value.aclaction.enable, s);
            sai_serialize_primitive(attr.value.aclaction.parameter.mac, s);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_IPV4:
            sai_serialize_primitive(attr.value.aclaction.enable, s);
            sai_serialize_primitive(attr.value.aclaction.parameter.ip4, s);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_IPV6:
            sai_serialize_primitive(attr.value.aclaction.enable, s);
            sai_serialize_primitive(attr.value.aclaction.parameter.ip6, s);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_OBJECT_ID:
            sai_serialize_primitive(attr.value.aclaction.enable, s);
            sai_serialize_primitive(attr.value.aclaction.parameter.oid, s);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_OBJECT_LIST:
            sai_serialize_primitive(attr.value.aclaction.enable, s);
            sai_serialize_list(attr.value.aclaction.parameter.objlist, s);
            break;

        default:
            return SAI_STATUS_NOT_IMPLEMENTED;
    }

    return SAI_STATUS_SUCCESS;
}

sai_status_t sai_serialize_attr(
        _In_ const sai_attr_serialization_type_t type,
        _In_ const sai_attribute_t &attr,
        _Out_ std::string &s)
{
    sai_serialize_attr_id(attr, s);

    return sai_serialize_attr_value(type, attr, s);
}

int char_to_int(
        _In_ const char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';

    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;

    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;

    std::stringstream ss;
    ss << "Unable to convert char '" << c << "' (" << (int)c << ") to int";

    throw ss.str();
}

sai_status_t sai_deserialize_attr_value(
        _In_ std::string &s,
        _In_ int &index,
        _In_ const sai_attr_serialization_type_t type,
        _Out_ sai_attribute_t &attr)
{
    switch (type)
    {
        case SAI_SERIALIZATION_TYPE_BOOL:
            sai_deserialize_primitive(s, index, attr.value.booldata);
            break;

        case SAI_SERIALIZATION_TYPE_CHARDATA:
            sai_deserialize_primitive(s, index, attr.value.chardata);
            break;

        case SAI_SERIALIZATION_TYPE_UINT8:
            sai_deserialize_primitive(s, index, attr.value.u8);
            break;

        case SAI_SERIALIZATION_TYPE_INT8:
            sai_deserialize_primitive(s, index, attr.value.s8);
            break;

        case SAI_SERIALIZATION_TYPE_UINT16:
            sai_deserialize_primitive(s, index, attr.value.u16);
            break;

        case SAI_SERIALIZATION_TYPE_INT16:
            sai_deserialize_primitive(s, index, attr.value.s16);
            break;

        case SAI_SERIALIZATION_TYPE_UINT32:
            sai_deserialize_primitive(s, index, attr.value.u32);
            break;

        case SAI_SERIALIZATION_TYPE_INT32:
            sai_deserialize_primitive(s, index, attr.value.s32);
            break;

        case SAI_SERIALIZATION_TYPE_UINT64:
            sai_deserialize_primitive(s, index, attr.value.u64);
            break;

        case SAI_SERIALIZATION_TYPE_INT64:
            sai_deserialize_primitive(s, index, attr.value.s64);
            break;

        case SAI_SERIALIZATION_TYPE_MAC:
            sai_deserialize_primitive(s, index, attr.value.mac);
            break;

        case SAI_SERIALIZATION_TYPE_IP4:
            sai_deserialize_primitive(s, index, attr.value.ip4);
            break;

        case SAI_SERIALIZATION_TYPE_IP6:
            sai_deserialize_primitive(s, index, attr.value.ip6);
            break;

        case SAI_SERIALIZATION_TYPE_IP_ADDRESS:
            sai_deserialize_primitive(s, index, attr.value.ipaddr);
            break;

        case SAI_SERIALIZATION_TYPE_OBJECT_ID:
            sai_deserialize_primitive(s, index, attr.value.oid);
            break;

        case SAI_SERIALIZATION_TYPE_OBJECT_LIST:
            sai_deserialize_list(s, index, attr.value.objlist);
            break;

        case SAI_SERIALIZATION_TYPE_UINT8_LIST:
            sai_deserialize_list(s, index, attr.value.u8list);
            break;

        case SAI_SERIALIZATION_TYPE_INT8_LIST:
            sai_deserialize_list(s, index, attr.value.s8list);
            break;

        case SAI_SERIALIZATION_TYPE_UINT16_LIST:
            sai_deserialize_list(s, index, attr.value.u16list);
            break;

        case SAI_SERIALIZATION_TYPE_INT16_LIST:
            sai_deserialize_list(s, index, attr.value.s16list);
            break;

        case SAI_SERIALIZATION_TYPE_UINT32_LIST:
            sai_deserialize_list(s, index, attr.value.u32list);
            break;

        case SAI_SERIALIZATION_TYPE_INT32_LIST:
            sai_deserialize_list(s, index, attr.value.s32list);
            break;

        case SAI_SERIALIZATION_TYPE_UINT32_RANGE:
            sai_deserialize_primitive(s, index, attr.value.u32range);
            break;

        case SAI_SERIALIZATION_TYPE_INT32_RANGE:
            sai_deserialize_primitive(s, index, attr.value.s32range);
            break;

        case SAI_SERIALIZATION_TYPE_VLAN_LIST:
            sai_deserialize_list(s, index, attr.value.vlanlist);
            break;

        case SAI_SERIALIZATION_TYPE_VLAN_PORT_LIST:
            sai_deserialize_list(s, index, attr.value.vlanportlist);
            break;

        case SAI_SERIALIZATION_TYPE_PORT_BREAKOUT:
            sai_deserialize_primitive(s, index, attr.value.portbreakout.breakout_mode);
            sai_deserialize_list(s, index, attr.value.portbreakout.port_list);
            break;

        case SAI_SERIALIZATION_TYPE_QOS_MAP_LIST:
            sai_deserialize_list(s, index, attr.value.qosmap);
            break;

            /* ACL FIELD DATA */

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_UINT8:
            sai_deserialize_primitive(s, index, attr.value.aclfield.enable);
            sai_deserialize_primitive(s, index, attr.value.aclfield.mask.u8);
            sai_deserialize_primitive(s, index, attr.value.aclfield.data.u8);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_INT8:
            sai_deserialize_primitive(s, index, attr.value.aclfield.enable);
            sai_deserialize_primitive(s, index, attr.value.aclfield.mask.s8);
            sai_deserialize_primitive(s, index, attr.value.aclfield.data.s8);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_UINT16:
            sai_deserialize_primitive(s, index, attr.value.aclfield.enable);
            sai_deserialize_primitive(s, index, attr.value.aclfield.mask.u16);
            sai_deserialize_primitive(s, index, attr.value.aclfield.data.u16);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_INT16:
            sai_deserialize_primitive(s, index, attr.value.aclfield.enable);
            sai_deserialize_primitive(s, index, attr.value.aclfield.mask.s16);
            sai_deserialize_primitive(s, index, attr.value.aclfield.data.s16);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_UINT32:
            sai_deserialize_primitive(s, index, attr.value.aclfield.enable);
            sai_deserialize_primitive(s, index, attr.value.aclfield.mask.u16);
            sai_deserialize_primitive(s, index, attr.value.aclfield.data.u16);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_INT32:
            sai_deserialize_primitive(s, index, attr.value.aclfield.enable);
            sai_deserialize_primitive(s, index, attr.value.aclfield.mask.s32);
            sai_deserialize_primitive(s, index, attr.value.aclfield.data.s32);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_MAC:
            sai_deserialize_primitive(s, index, attr.value.aclfield.enable);
            sai_deserialize_primitive(s, index, attr.value.aclfield.mask.mac);
            sai_deserialize_primitive(s, index, attr.value.aclfield.data.mac);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_IP4:
            sai_deserialize_primitive(s, index, attr.value.aclfield.enable);
            sai_deserialize_primitive(s, index, attr.value.aclfield.mask.ip4);
            sai_deserialize_primitive(s, index, attr.value.aclfield.data.ip4);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_IP6:
            sai_deserialize_primitive(s, index, attr.value.aclfield.enable);
            sai_deserialize_primitive(s, index, attr.value.aclfield.mask.ip6);
            sai_deserialize_primitive(s, index, attr.value.aclfield.data.ip6);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_OBJECT_ID:
            sai_deserialize_primitive(s, index, attr.value.aclfield.enable);
            sai_deserialize_primitive(s, index, attr.value.aclfield.data.oid);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_OBJECT_LIST:
            sai_deserialize_primitive(s, index, attr.value.aclfield.enable);
            sai_deserialize_list(s, index, attr.value.aclfield.data.objlist);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_UINT8_LIST:
            sai_deserialize_primitive(s, index, attr.value.aclfield.enable);
            sai_deserialize_list(s, index, attr.value.aclfield.mask.u8list);
            sai_deserialize_list(s, index, attr.value.aclfield.data.u8list);
            break;

            /* ACL ACTION DATA */

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_UINT8:
            sai_deserialize_primitive(s, index, attr.value.aclaction.enable);
            sai_deserialize_primitive(s, index, attr.value.aclaction.parameter.u8);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_INT8:
            sai_deserialize_primitive(s, index, attr.value.aclaction.enable);
            sai_deserialize_primitive(s, index, attr.value.aclaction.parameter.s8);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_UINT16:
            sai_deserialize_primitive(s, index, attr.value.aclaction.enable);
            sai_deserialize_primitive(s, index, attr.value.aclaction.parameter.u16);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_INT16:
            sai_deserialize_primitive(s, index, attr.value.aclaction.enable);
            sai_deserialize_primitive(s, index, attr.value.aclaction.parameter.s16);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_UINT32:
            sai_deserialize_primitive(s, index, attr.value.aclaction.enable);
            sai_deserialize_primitive(s, index, attr.value.aclaction.parameter.u32);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_INT32:
            sai_deserialize_primitive(s, index, attr.value.aclaction.enable);
            sai_deserialize_primitive(s, index, attr.value.aclaction.parameter.s32);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_MAC:
            sai_deserialize_primitive(s, index, attr.value.aclaction.enable);
            sai_deserialize_primitive(s, index, attr.value.aclaction.parameter.mac);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_IPV4:
            sai_deserialize_primitive(s, index, attr.value.aclaction.enable);
            sai_deserialize_primitive(s, index, attr.value.aclaction.parameter.ip4);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_IPV6:
            sai_deserialize_primitive(s, index, attr.value.aclaction.enable);
            sai_deserialize_primitive(s, index, attr.value.aclaction.parameter.ip6);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_OBJECT_ID:
            sai_deserialize_primitive(s, index, attr.value.aclaction.enable);
            sai_deserialize_primitive(s, index, attr.value.aclaction.parameter.oid);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_OBJECT_LIST:
            sai_deserialize_primitive(s, index, attr.value.aclaction.enable);
            sai_deserialize_list(s, index, attr.value.aclaction.parameter.objlist);
            break;

        default:
            return SAI_STATUS_NOT_IMPLEMENTED;
    }

    return SAI_STATUS_SUCCESS;
}

sai_status_t sai_deserialize_free_attribute_value(
        _In_ const sai_attr_serialization_type_t type,
        _In_ sai_attribute_t &attr)
{
    // if we allocated list, then we need to free it

    switch (type)
    {
        case SAI_SERIALIZATION_TYPE_BOOL:
            break;

        case SAI_SERIALIZATION_TYPE_CHARDATA:
            break;

        case SAI_SERIALIZATION_TYPE_UINT8:
            break;

        case SAI_SERIALIZATION_TYPE_INT8:
            break;

        case SAI_SERIALIZATION_TYPE_UINT16:
            break;

        case SAI_SERIALIZATION_TYPE_INT16:
            break;

        case SAI_SERIALIZATION_TYPE_UINT32:
            break;

        case SAI_SERIALIZATION_TYPE_INT32:
            break;

        case SAI_SERIALIZATION_TYPE_UINT64:
            break;

        case SAI_SERIALIZATION_TYPE_INT64:
            break;

        case SAI_SERIALIZATION_TYPE_MAC:
            break;

        case SAI_SERIALIZATION_TYPE_IP4:
            break;

        case SAI_SERIALIZATION_TYPE_IP6:
            break;

        case SAI_SERIALIZATION_TYPE_IP_ADDRESS:
            break;

        case SAI_SERIALIZATION_TYPE_OBJECT_ID:
            break;

        case SAI_SERIALIZATION_TYPE_OBJECT_LIST:
            sai_free_list(attr.value.objlist);
            break;

        case SAI_SERIALIZATION_TYPE_UINT8_LIST:
            sai_free_list(attr.value.u8list);
            break;

        case SAI_SERIALIZATION_TYPE_INT8_LIST:
            sai_free_list(attr.value.s8list);
            break;

        case SAI_SERIALIZATION_TYPE_UINT16_LIST:
            sai_free_list(attr.value.u16list);
            break;

        case SAI_SERIALIZATION_TYPE_INT16_LIST:
            sai_free_list(attr.value.s16list);
            break;

        case SAI_SERIALIZATION_TYPE_UINT32_LIST:
            sai_free_list(attr.value.u32list);
            break;

        case SAI_SERIALIZATION_TYPE_INT32_LIST:
            sai_free_list(attr.value.s32list);
            break;

        case SAI_SERIALIZATION_TYPE_UINT32_RANGE:
            break;

        case SAI_SERIALIZATION_TYPE_INT32_RANGE:
            break;

        case SAI_SERIALIZATION_TYPE_VLAN_LIST:
            sai_free_list(attr.value.vlanlist);
            break;

        case SAI_SERIALIZATION_TYPE_VLAN_PORT_LIST:
            sai_free_list(attr.value.vlanportlist);
            break;

        case SAI_SERIALIZATION_TYPE_PORT_BREAKOUT:
            sai_free_list(attr.value.portbreakout.port_list);
            break;

        case SAI_SERIALIZATION_TYPE_QOS_MAP_LIST:
            sai_free_list(attr.value.qosmap);
            break;

            /* ACL FIELD DATA */

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_UINT8:
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_INT8:
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_UINT16:
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_INT16:
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_UINT32:
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_INT32:
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_MAC:
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_IP4:
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_IP6:
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_OBJECT_ID:
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_OBJECT_LIST:
            sai_free_list(attr.value.aclfield.data.objlist);
            break;

        case SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_UINT8_LIST:
            sai_free_list(attr.value.aclfield.mask.u8list);
            sai_free_list(attr.value.aclfield.data.u8list);
            break;

            /* ACL ACTION DATA */

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_UINT8:
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_INT8:
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_UINT16:
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_INT16:
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_UINT32:
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_INT32:
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_MAC:
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_IPV4:
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_IPV6:
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_OBJECT_ID:
            break;

        case SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_OBJECT_LIST:
            sai_free_list(attr.value.aclaction.parameter.objlist);
            break;

        default:
            return SAI_STATUS_NOT_IMPLEMENTED;
    }

    return SAI_STATUS_SUCCESS;
}
