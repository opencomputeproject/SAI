/*
 *  Copyright (C) 2014. Mellanox Technologies, Ltd. ALL RIGHTS RESERVED.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License"); you may
 *    not use this file except in compliance with the License. You may obtain
 *    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *    THIS CODE IS PROVIDED ON AN  *AS IS* BASIS, WITHOUT WARRANTIES OR
 *    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 *    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 *    FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 */

#include "sai.h"
#include "stub_sai.h"
#include "assert.h"
#include "inttypes.h"
#include <time.h>
#include <sys/time.h>
#ifndef WIN32
#include <arpa/inet.h>
#else
#include <Ws2tcpip.h>
#endif

#undef  __MODULE__
#define __MODULE__ SAI_UTILS

static sai_status_t find_functionality_attrib_index(_In_ const sai_attr_id_t          id,
                                                    _In_ const sai_attribute_entry_t *functionality_attr,
                                                    _Out_ uint32_t                   *index)
{
    uint32_t curr_index;

    STUB_LOG_ENTER();

    if (NULL == functionality_attr) {
        STUB_LOG_ERR("NULL value functionality attrib\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (NULL == index) {
        STUB_LOG_ERR("NULL value index\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    for (curr_index = 0; END_FUNCTIONALITY_ATTRIBS_ID != functionality_attr[curr_index].id; curr_index++) {
        if (id == functionality_attr[curr_index].id) {
            *index = curr_index;
            STUB_LOG_EXIT();
            return SAI_STATUS_SUCCESS;
        }
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_ITEM_NOT_FOUND;
}


sai_status_t check_attribs_metadata(_In_ uint32_t                            attr_count,
                                    _In_ const sai_attribute_t              *attr_list,
                                    _In_ const sai_attribute_entry_t        *functionality_attr,
                                    _In_ const sai_vendor_attribute_entry_t *functionality_vendor_attr,
                                    _In_ sai_operation_t                     oper)
{
    uint32_t functionality_attr_count, ii, index;
    bool    *attr_present;

    STUB_LOG_ENTER();

    if ((attr_count) && (NULL == attr_list)) {
        STUB_LOG_ERR("NULL value attr list\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (NULL == functionality_attr) {
        STUB_LOG_ERR("NULL value functionality attrib\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (NULL == functionality_vendor_attr) {
        STUB_LOG_ERR("NULL value functionality vendor attrib\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (SAI_OPERATION_MAX <= oper) {
        STUB_LOG_ERR("Invalid operation %d\n", oper);
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (SAI_OPERATION_REMOVE == oper) {
        /* No attributes expected for remove at this point */
        return SAI_STATUS_NOT_IMPLEMENTED;
    }

    if (SAI_OPERATION_SET == oper) {
        if (1 != attr_count) {
            STUB_LOG_ERR("Set operation supports only single attribute\n");
            return SAI_STATUS_INVALID_PARAMETER;
        }
    }

    for (functionality_attr_count = 0;
         END_FUNCTIONALITY_ATTRIBS_ID != functionality_attr[functionality_attr_count].id;
         functionality_attr_count++) {
        if (functionality_attr[functionality_attr_count].id !=
            functionality_vendor_attr[functionality_attr_count].id) {
            STUB_LOG_ERR("Mismatch between functionality attribute and vendor attribute index %u %u %u\n",
                         functionality_attr_count, functionality_attr[functionality_attr_count].id,
                         functionality_vendor_attr[functionality_attr_count].id);
            return SAI_STATUS_FAILURE;
        }
    }

    attr_present = (bool*)calloc(functionality_attr_count, sizeof(bool));
    if (NULL == attr_present) {
        STUB_LOG_ERR("Can't allocate memory\n");
        return SAI_STATUS_NO_MEMORY;
    }

    for (ii = 0; ii < attr_count; ii++) {
        if (SAI_STATUS_SUCCESS != find_functionality_attrib_index(attr_list[ii].id, functionality_attr, &index)) {
            STUB_LOG_ERR("Invalid attribute %d\n", attr_list[ii].id);
            free(attr_present);
            return SAI_STATUS_UNKNOWN_ATTRIBUTE_0 + ii;
        }

        if ((SAI_OPERATION_CREATE == oper) &&
            (!(functionality_attr[index].valid_for_create))) {
            STUB_LOG_ERR("Invalid attribute %s for create\n", functionality_attr[index].attrib_name);
            free(attr_present);
            return SAI_STATUS_INVALID_ATTRIBUTE_0 + ii;
        }

        if ((SAI_OPERATION_SET == oper) &&
            (!(functionality_attr[index].valid_for_set))) {
            STUB_LOG_ERR("Invalid attribute %s for set\n", functionality_attr[index].attrib_name);
            free(attr_present);
            return SAI_STATUS_INVALID_ATTRIBUTE_0 + ii;
        }

        if ((SAI_OPERATION_GET == oper) &&
            (!(functionality_attr[index].valid_for_get))) {
            STUB_LOG_ERR("Invalid attribute %s for get\n", functionality_attr[index].attrib_name);
            free(attr_present);
            return SAI_STATUS_INVALID_ATTRIBUTE_0 + ii;
        }

        if (!(functionality_vendor_attr[index].is_supported[oper])) {
            STUB_LOG_ERR("Not supported attribute %s\n", functionality_attr[index].attrib_name);
            free(attr_present);
            return SAI_STATUS_ATTR_NOT_SUPPORTED_0 + ii;
        }

        if (!(functionality_vendor_attr[index].is_implemented[oper])) {
            STUB_LOG_ERR("Not implemented attribute %s\n", functionality_attr[index].attrib_name);
            free(attr_present);
            return SAI_STATUS_ATTR_NOT_IMPLEMENTED_0 + ii;
        }

        if (attr_present[index]) {
            STUB_LOG_ERR("Attribute %s appears twice in attribute list at index %d\n",
                         functionality_attr[index].attrib_name,
                         ii);
            free(attr_present);
            return SAI_STATUS_INVALID_ATTRIBUTE_0 + ii;
        }

        if (((SAI_ATTR_VAL_TYPE_OBJLIST == functionality_attr[index].type) &&
             (NULL == attr_list[ii].value.objlist.list)) ||
            ((SAI_ATTR_VAL_TYPE_U32LIST == functionality_attr[index].type) &&
             (NULL == attr_list[ii].value.u32list.list)) ||
            ((SAI_ATTR_VAL_TYPE_S32LIST == functionality_attr[index].type) &&
             (NULL == attr_list[ii].value.s32list.list)) ||
            ((SAI_ATTR_VAL_TYPE_VLANLIST == functionality_attr[index].type) &&
             (NULL == attr_list[ii].value.vlanlist.list))) {
            STUB_LOG_ERR("Null list attribute %s at index %d\n",
                         functionality_attr[index].attrib_name,
                         ii);
            free(attr_present);
            return SAI_STATUS_INVALID_ATTR_VALUE_0 + ii;
        }

        attr_present[index] = true;
    }

    if (SAI_OPERATION_CREATE == oper) {
        for (ii = 0; ii < functionality_attr_count; ii++) {
            if ((functionality_attr[ii].mandatory_on_create) &&
                (!attr_present[ii])) {
                STUB_LOG_ERR("Missing mandatory attribute %s on create\n", functionality_attr[ii].attrib_name);
                free(attr_present);
                return SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING;
            }
        }
    }

    free(attr_present);

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

static sai_status_t set_dispatch_attrib_handler(_In_ const sai_attribute_t              *attr,
                                                _In_ const sai_attribute_entry_t        *functionality_attr,
                                                _In_ const sai_vendor_attribute_entry_t *functionality_vendor_attr,
                                                _In_ const sai_object_key_t             *key,
                                                _In_ const char                         *key_str)
{
    uint32_t     index;
    sai_status_t err;
    char         value_str[MAX_VALUE_STR_LEN];

    STUB_LOG_ENTER();

    if (NULL == attr) {
        STUB_LOG_ERR("NULL value attr\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (NULL == functionality_attr) {
        STUB_LOG_ERR("NULL value functionality attrib\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (NULL == functionality_vendor_attr) {
        STUB_LOG_ERR("NULL value functionality vendor attrib\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    assert(SAI_STATUS_SUCCESS == find_functionality_attrib_index(attr->id, functionality_attr, &index));

    if (!functionality_vendor_attr[index].setter) {
        STUB_LOG_ERR("Attribute %s not implemented on set and defined incorrectly\n",
                     functionality_attr[index].attrib_name);
        return SAI_STATUS_ATTR_NOT_IMPLEMENTED_0;
    }

    sai_value_to_str(attr->value, functionality_attr[index].type, MAX_VALUE_STR_LEN, value_str);
    STUB_LOG_NTC("Set %s, key:%s, val:%s\n", functionality_attr[index].attrib_name, key_str, value_str);
    err = functionality_vendor_attr[index].setter(key, &(attr->value), functionality_vendor_attr[index].setter_arg);

    STUB_LOG_EXIT();
    return err;
}

static sai_status_t get_dispatch_attribs_handler(_In_ uint32_t                            attr_count,
                                                 _Inout_ sai_attribute_t                 *attr_list,
                                                 _In_ const sai_attribute_entry_t        *functionality_attr,
                                                 _In_ const sai_vendor_attribute_entry_t *functionality_vendor_attr,
                                                 _In_ const sai_object_key_t             *key,
                                                 _In_ const char                         *key_str)
{
    uint32_t       ii, index;
    vendor_cache_t cache;
    sai_status_t   status;
    char           value_str[MAX_VALUE_STR_LEN];

    if ((attr_count) && (NULL == attr_list)) {
        STUB_LOG_ERR("NULL value attr list\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (NULL == functionality_attr) {
        STUB_LOG_ERR("NULL value functionality attrib\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (NULL == functionality_vendor_attr) {
        STUB_LOG_ERR("NULL value functionality vendor attrib\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    memset(&cache, 0, sizeof(cache));

    for (ii = 0; ii < attr_count; ii++) {
        assert(SAI_STATUS_SUCCESS == find_functionality_attrib_index(attr_list[ii].id, functionality_attr, &index));

        if (!functionality_vendor_attr[index].getter) {
            STUB_LOG_ERR("Attribute %s not implemented on get and defined incorrectly\n",
                         functionality_attr[index].attrib_name);
            return SAI_STATUS_ATTR_NOT_IMPLEMENTED_0 + ii;
        }

        if (SAI_STATUS_SUCCESS !=
            (status =
                 functionality_vendor_attr[index].getter(key, &(attr_list[ii].value), ii, &cache,
                                                         functionality_vendor_attr[index].getter_arg))) {
            STUB_LOG_ERR("Failed getting attrib %s\n", functionality_attr[index].attrib_name);
            return status;
        }
        sai_value_to_str(attr_list[ii].value, functionality_attr[index].type, MAX_VALUE_STR_LEN, value_str);
        STUB_LOG_NTC("Got #%u, %s, key:%s, val:%s\n", ii, functionality_attr[index].attrib_name, key_str, value_str);
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

sai_status_t find_attrib_in_list(_In_ uint32_t                       attr_count,
                                 _In_ const sai_attribute_t         *attr_list,
                                 _In_ sai_attr_id_t                  attrib_id,
                                 _Out_ const sai_attribute_value_t **attr_value,
                                 _Out_ uint32_t                     *index)
{
    uint32_t ii;

    STUB_LOG_ENTER();

    if ((attr_count) && (NULL == attr_list)) {
        STUB_LOG_ERR("NULL value attr list\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (NULL == attr_value) {
        STUB_LOG_ERR("NULL value attr value\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (NULL == index) {
        STUB_LOG_ERR("NULL value index\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    for (ii = 0; ii < attr_count; ii++) {
        if (attr_list[ii].id == attrib_id) {
            *attr_value = &(attr_list[ii].value);
            *index      = ii;
            return SAI_STATUS_SUCCESS;
        }
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_ITEM_NOT_FOUND;
}

sai_status_t sai_set_attribute(_In_ const sai_object_key_t             *key,
                               _In_ const char                         *key_str,
                               _In_ const sai_attribute_entry_t        *functionality_attr,
                               _In_ const sai_vendor_attribute_entry_t *functionality_vendor_attr,
                               _In_ const sai_attribute_t              *attr)
{
    sai_status_t status;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS !=
        (status = check_attribs_metadata(1, attr, functionality_attr, functionality_vendor_attr, SAI_OPERATION_SET))) {
        STUB_LOG_ERR("Failed attribs check, key:%s\n", key_str);
        return status;
    }

    if (SAI_STATUS_SUCCESS !=
        (status = set_dispatch_attrib_handler(attr, functionality_attr, functionality_vendor_attr, key, key_str))) {
        STUB_LOG_ERR("Failed set attrib dispatch\n");
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

sai_status_t sai_get_attributes(_In_ const sai_object_key_t             *key,
                                _In_ const char                         *key_str,
                                _In_ const sai_attribute_entry_t        *functionality_attr,
                                _In_ const sai_vendor_attribute_entry_t *functionality_vendor_attr,
                                _In_ uint32_t                            attr_count,
                                _Inout_ sai_attribute_t                 *attr_list)
{
    sai_status_t status;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS !=
        (status =
             check_attribs_metadata(attr_count, attr_list, functionality_attr, functionality_vendor_attr,
                                    SAI_OPERATION_GET))) {
        STUB_LOG_ERR("Failed attribs check, key:%s\n", key_str);
        return status;
    }

    if (SAI_STATUS_SUCCESS !=
        (status =
             get_dispatch_attribs_handler(attr_count, attr_list, functionality_attr, functionality_vendor_attr, key,
                                          key_str))) {
        STUB_LOG_ERR("Failed attribs dispatch\n");
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

static sai_status_t sai_ipv4_to_str(_In_ sai_ip4_t value,
                                    _In_ uint32_t  max_length,
                                    _Out_ char    *value_str,
                                    _Out_ int     *chars_written)
{
    inet_ntop(AF_INET, &value, value_str, max_length);

    if (NULL != chars_written) {
        *chars_written = (int)strlen(value_str);
    }

    return SAI_STATUS_SUCCESS;
}

static sai_status_t sai_ipv6_to_str(_In_ sai_ip6_t value,
                                    _In_ uint32_t  max_length,
                                    _Out_ char    *value_str,
                                    _Out_ int     *chars_written)
{
    inet_ntop(AF_INET6, &value, value_str, max_length);

    if (NULL != chars_written) {
        *chars_written = (int)strlen(value_str);
    }

    return SAI_STATUS_SUCCESS;
}

sai_status_t sai_ipaddr_to_str(_In_ sai_ip_address_t value,
                               _In_ uint32_t         max_length,
                               _Out_ char           *value_str,
                               _Out_ int            *chars_written)
{
    int res;

    if (SAI_IP_ADDR_FAMILY_IPV4 == value.addr_family) {
        sai_ipv4_to_str(value.addr.ip4, max_length, value_str, chars_written);
    } else if (SAI_IP_ADDR_FAMILY_IPV6 == value.addr_family) {
        sai_ipv6_to_str(value.addr.ip6, max_length, value_str, chars_written);
    } else {
        res = snprintf(value_str, max_length, "Invalid ipaddr family %d", value.addr_family);
        if (NULL != chars_written) {
            *chars_written = res;
        }
        return SAI_STATUS_INVALID_PARAMETER;
    }

    return SAI_STATUS_SUCCESS;
}

sai_status_t sai_ipprefix_to_str(_In_ sai_ip_prefix_t value, _In_ uint32_t max_length, _Out_ char *value_str)
{
    int      chars_written;
    uint32_t pos = 0;

    if (SAI_IP_ADDR_FAMILY_IPV4 == value.addr_family) {
        sai_ipv4_to_str(value.addr.ip4, max_length, value_str, &chars_written);
        pos += chars_written;
        if (pos > max_length) {
            return SAI_STATUS_SUCCESS;
        }
        pos += snprintf(value_str + pos, max_length - pos, " ");
        if (pos > max_length) {
            return SAI_STATUS_SUCCESS;
        }
        sai_ipv4_to_str(value.mask.ip4, max_length - pos, value_str + pos, &chars_written);
    } else if (SAI_IP_ADDR_FAMILY_IPV6 == value.addr_family) {
        sai_ipv6_to_str(value.addr.ip6, max_length, value_str, &chars_written);
        pos += chars_written;
        if (pos > max_length) {
            return SAI_STATUS_SUCCESS;
        }
        pos += snprintf(value_str + pos, max_length - pos, " ");
        if (pos > max_length) {
            return SAI_STATUS_SUCCESS;
        }
        sai_ipv6_to_str(value.mask.ip6, max_length - pos, value_str + pos, &chars_written);
    } else {
        snprintf(value_str, max_length, "Invalid addr family %d", value.addr_family);
        return SAI_STATUS_INVALID_PARAMETER;
    }

    return SAI_STATUS_SUCCESS;
}

sai_status_t sai_nexthops_to_str(_In_ uint32_t               next_hop_count,
                                 _In_ const sai_object_id_t* nexthops,
                                 _In_ uint32_t               max_length,
                                 _Out_ char                 *str)
{
    uint32_t     ii;
    uint32_t     pos = 0;
    uint32_t     nexthop_id;
    sai_status_t status;

    pos += snprintf(str, max_length, "%u hops : [", next_hop_count);
    if (pos > max_length) {
        return SAI_STATUS_SUCCESS;
    }
    for (ii = 0; ii < next_hop_count; ii++) {
        if (SAI_STATUS_SUCCESS !=
            (status = stub_object_to_type(nexthops[ii], SAI_OBJECT_TYPE_NEXT_HOP, &nexthop_id))) {
            snprintf(str + pos, max_length - pos, " invalid next hop]");
            return status;
        }

        pos += snprintf(str + pos, max_length - pos, " %u", nexthop_id);
        if (pos > max_length) {
            return SAI_STATUS_SUCCESS;
        }
    }
    snprintf(str + pos, max_length - pos, "]");

    return SAI_STATUS_SUCCESS;
}

sai_status_t sai_value_to_str(_In_ sai_attribute_value_t      value,
                              _In_ sai_attribute_value_type_t type,
                              _In_ uint32_t                   max_length,
                              _Out_ char                     *value_str)
{
    uint32_t          ii;
    uint32_t          pos = 0;
    uint32_t          count;
    stub_object_id_t *stub_object_id;

    if (NULL == value_str) {
        STUB_LOG_ERR("NULL value str");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    *value_str = '\0';

    switch (type) {
    case SAI_ATTR_VAL_TYPE_BOOL:
        snprintf(value_str, max_length, "%u", value.booldata);
        break;

    case SAI_ATTR_VAL_TYPE_CHARDATA:
        snprintf(value_str, max_length, "%s", value.chardata);
        break;

    case SAI_ATTR_VAL_TYPE_U8:
        snprintf(value_str, max_length, "%u", value.u8);
        break;

    case SAI_ATTR_VAL_TYPE_S8:
        snprintf(value_str, max_length, "%d", value.s8);
        break;

    case SAI_ATTR_VAL_TYPE_U16:
        snprintf(value_str, max_length, "%u", value.u16);
        break;

    case SAI_ATTR_VAL_TYPE_S16:
        snprintf(value_str, max_length, "%d", value.s16);
        break;

    case SAI_ATTR_VAL_TYPE_U32:
        snprintf(value_str, max_length, "%u", value.u32);
        break;

    case SAI_ATTR_VAL_TYPE_S32:
        snprintf(value_str, max_length, "%d", value.s32);
        break;

    case SAI_ATTR_VAL_TYPE_U64:
        snprintf(value_str, max_length, "%" PRIu64, value.u64);
        break;

    case SAI_ATTR_VAL_TYPE_S64:
        snprintf(value_str, max_length, "%" PRId64, value.s64);
        break;

    case SAI_ATTR_VAL_TYPE_MAC:
        snprintf(value_str, max_length, "[%02x:%02x:%02x:%02x:%02x:%02x]",
                 value.mac[0],
                 value.mac[1],
                 value.mac[2],
                 value.mac[3],
                 value.mac[4],
                 value.mac[5]);
        break;

    /* IP is in network order */
    case SAI_ATTR_VAL_TYPE_IPV4:
        sai_ipv4_to_str(value.ip4, max_length, value_str, NULL);
        break;

    case SAI_ATTR_VAL_TYPE_IPV6:
        sai_ipv6_to_str(value.ip6, max_length, value_str, NULL);
        break;

    case SAI_ATTR_VAL_TYPE_IPADDR:
        sai_ipaddr_to_str(value.ipaddr, max_length, value_str, NULL);
        break;

    case SAI_ATTR_VAL_TYPE_OID:
        stub_object_id = (stub_object_id_t*)&value.oid;
        snprintf(value_str, max_length, "%s %x",
                 SAI_TYPE_STR(sai_object_type_query(value.oid)), stub_object_id->data);
        break;

    case SAI_ATTR_VAL_TYPE_OBJLIST:
    case SAI_ATTR_VAL_TYPE_U32LIST:
    case SAI_ATTR_VAL_TYPE_S32LIST:
    case SAI_ATTR_VAL_TYPE_VLANLIST:
    case SAI_ATTR_VAL_TYPE_PORTBREAKOUT:
        if (SAI_ATTR_VAL_TYPE_PORTBREAKOUT == type) {
            pos += snprintf(value_str, max_length, "breakout mode %d.", value.portbreakout.breakout_mode);
        }
        if (pos > max_length) {
            return SAI_STATUS_SUCCESS;
        }

        count = (SAI_ATTR_VAL_TYPE_OBJLIST == type) ? value.objlist.count :
                (SAI_ATTR_VAL_TYPE_U32LIST == type) ? value.u32list.count :
                (SAI_ATTR_VAL_TYPE_S32LIST == type) ? value.s32list.count :
                (SAI_ATTR_VAL_TYPE_VLANLIST == type) ? value.vlanlist.count :
                value.portbreakout.port_list.count;
        pos += snprintf(value_str + pos, max_length - pos, "%u : [", count);
        if (pos > max_length) {
            return SAI_STATUS_SUCCESS;
        }

        for (ii = 0; ii < count; ii++) {
            if (SAI_ATTR_VAL_TYPE_OBJLIST == type) {
                pos += snprintf(value_str + pos, max_length - pos, " %" PRIx64, value.objlist.list[ii]);
            } else if (SAI_ATTR_VAL_TYPE_U32LIST == type) {
                pos += snprintf(value_str + pos, max_length - pos, " %u", value.u32list.list[ii]);
            } else if (SAI_ATTR_VAL_TYPE_S32LIST == type) {
                pos += snprintf(value_str + pos, max_length - pos, " %d", value.s32list.list[ii]);
            } else if (SAI_ATTR_VAL_TYPE_VLANLIST == type) {
                pos += snprintf(value_str + pos, max_length - pos, " %u", value.vlanlist.list[ii]);
            } else {
                pos += snprintf(value_str + pos, max_length - pos, " %" PRIx64, value.portbreakout.port_list.list[ii]);
            }
            if (pos > max_length) {
                return SAI_STATUS_SUCCESS;
            }
        }
        snprintf(value_str + pos, max_length - pos, "]");
        break;


    case SAI_ATTR_VAL_TYPE_ACLFIELD:
    case SAI_ATTR_VAL_TYPE_ACLACTION:
        /* TODO : implement if in case it is used */
        snprintf(value_str, max_length, "Not implemented value type %d", type);
        return SAI_STATUS_NOT_IMPLEMENTED;

    case SAI_ATTR_VAL_TYPE_UNDETERMINED:
    default:
        snprintf(value_str, max_length, "Invalid/Unsupported value type %d", type);
        return SAI_STATUS_INVALID_PARAMETER;
    }

    return SAI_STATUS_SUCCESS;
}

sai_status_t sai_attr_list_to_str(_In_ uint32_t                     attr_count,
                                  _In_ const sai_attribute_t       *attr_list,
                                  _In_ const sai_attribute_entry_t *functionality_attr,
                                  _In_ uint32_t                     max_length,
                                  _Out_ char                       *list_str)
{
    uint32_t ii, index, pos = 0;
    char     value_str[MAX_VALUE_STR_LEN];

    if ((attr_count) && (NULL == attr_list)) {
        STUB_LOG_ERR("NULL value attr list\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (NULL == functionality_attr) {
        STUB_LOG_ERR("NULL value functionality attrib\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (NULL == list_str) {
        STUB_LOG_ERR("NULL value str");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (0 == attr_count) {
        snprintf(list_str, max_length, "empty list");
        return SAI_STATUS_SUCCESS;
    }

    for (ii = 0; ii < attr_count; ii++) {
        assert(SAI_STATUS_SUCCESS == find_functionality_attrib_index(attr_list[ii].id, functionality_attr, &index));

        sai_value_to_str(attr_list[ii].value, functionality_attr[index].type, MAX_VALUE_STR_LEN, value_str);
        pos += snprintf(list_str + pos,
                        max_length - pos,
                        "#%u %s val:%s ",
                        ii,
                        functionality_attr[index].attrib_name,
                        value_str);
        if (pos > max_length) {
            break;
        }
    }

    return SAI_STATUS_SUCCESS;
}

sai_status_t stub_object_to_type(sai_object_id_t object_id, sai_object_type_t type, uint32_t *data)
{
    stub_object_id_t *stub_object_id = (stub_object_id_t*)&object_id;

    if (NULL == data) {
        STUB_LOG_ERR("NULL data value\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (type != stub_object_id->object_type) {
        STUB_LOG_ERR("Expected object %s got %s\n", SAI_TYPE_STR(type), SAI_TYPE_STR(stub_object_id->object_type));
        return SAI_STATUS_INVALID_PARAMETER;
    }

    *data = stub_object_id->data;
    return SAI_STATUS_SUCCESS;
}

sai_status_t stub_create_object(sai_object_type_t type, uint32_t data, sai_object_id_t *object_id)
{
    stub_object_id_t *stub_object_id = (stub_object_id_t*)object_id;

    if (NULL == object_id) {
        STUB_LOG_ERR("NULL object id value\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (type >= SAI_OBJECT_TYPE_MAX) {
        STUB_LOG_ERR("Unknown object type %d\n", type);
        return SAI_STATUS_INVALID_PARAMETER;
    }

    memset(stub_object_id, 0, sizeof(*stub_object_id));
    stub_object_id->data        = data;
    stub_object_id->object_type = type;
    return SAI_STATUS_SUCCESS;
}

static sai_status_t stub_fill_genericlist(size_t element_size, void *data, uint32_t count, void *list)
{
    /* all list objects have same field count in the beginning of the object, and then different data,
     * so can be casted to one type */
    sai_object_list_t *objlist = list;

    if (NULL == data) {
        STUB_LOG_ERR("NULL data value\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (NULL == list) {
        STUB_LOG_ERR("NULL list value\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (0 == element_size) {
        STUB_LOG_ERR("Zero element size\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (count > objlist->count) {
        STUB_LOG_ERR("Insufficient list buffer size. Allocated %u needed %u\n",
                     objlist->count, count);
        objlist->count = count;
        return SAI_STATUS_BUFFER_OVERFLOW;
    }

    objlist->count = count;
    memcpy(objlist->list, data, count * element_size);

    return SAI_STATUS_SUCCESS;
}

sai_status_t stub_fill_objlist(sai_object_id_t *data, uint32_t count, sai_object_list_t *list)
{
    return stub_fill_genericlist(sizeof(sai_object_id_t), (void*)data, count, (void*)list);
}

sai_status_t stub_fill_u32list(uint32_t *data, uint32_t count, sai_u32_list_t *list)
{
    return stub_fill_genericlist(sizeof(uint32_t), (void*)data, count, (void*)list);
}

sai_status_t stub_fill_s32list(int32_t *data, uint32_t count, sai_s32_list_t *list)
{
    return stub_fill_genericlist(sizeof(int32_t), (void*)data, count, (void*)list);
}

sai_status_t stub_fill_vlanlist(sai_vlan_id_t *data, uint32_t count, sai_vlan_list_t *list)
{
    return stub_fill_genericlist(sizeof(sai_vlan_id_t), (void*)data, count, (void*)list);
}

#define LOG_ENTRY_SIZE_MAX 1024

#ifndef _WIN32
void sai_log_cb(sai_log_level_t severity, const char *module_name, char *msg)
{
    int   level;
    char *level_str;

    /* translate SDK log level to syslog level */
    switch (severity) {
    case SAI_LOG_NOTICE:
        level     = LOG_NOTICE;
        level_str = "NOTICE";
        break;

    case SAI_LOG_INFO:
        level     = LOG_INFO;
        level_str = "INFO";
        break;

    case SAI_LOG_ERROR:
        level     = LOG_ERR;
        level_str = "ERR";
        break;

    case SAI_LOG_WARN:
        level     = LOG_WARNING;
        level_str = "WARNING";
        break;

    case SAI_LOG_DEBUG:
        level     = LOG_DEBUG;
        level_str = "DEBUG";
        break;

    default:
        level     = LOG_DEBUG;
        level_str = "DEBUG";
        break;
    }

    syslog(level, "[%s.%s] %s", module_name, level_str, msg);
}
#else
void sai_log_cb(sai_log_level_t severity, const char *module_name, char *msg)
{
    UNREFERENCED_PARAMETER(severity);
    UNREFERENCED_PARAMETER(module_name);
    UNREFERENCED_PARAMETER(msg);
}
#endif

void utils_log_vprint(const sai_log_level_t severity, const char *module_name, const char *p_str, va_list args)
{
    char buffer[LOG_ENTRY_SIZE_MAX];

    vsnprintf(buffer, LOG_ENTRY_SIZE_MAX, p_str, args);

    sai_log_cb(severity, module_name, buffer);
}

void utils_log(const sai_log_level_t severity, const char *module_name, const char *p_str, ...)
{
    va_list args;

    if (severity < SAI_LOG_INFO) {
        return;
    }

    va_start(args, p_str);
    utils_log_vprint(severity, module_name, p_str, args);
    va_end(args);
}
