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

#if !defined (__STUBAI_H_)
#define __STUBSAI_H_

#include <sai.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <syslog.h>
#include <stdarg.h>
#include <assert.h>

extern service_method_table_t           g_services;
extern const sai_route_api_t            route_api;
extern const sai_virtual_router_api_t   router_api;
extern const sai_switch_api_t           switch_api;
extern const sai_port_api_t             port_api;
extern const sai_fdb_api_t              fdb_api;
extern const sai_neighbor_api_t         neighbor_api;
extern const sai_next_hop_api_t         next_hop_api;
extern const sai_next_hop_group_api_t   next_hop_group_api;
extern const sai_router_interface_api_t router_interface_api;
extern const sai_vlan_api_t             vlan_api;
extern const sai_hostif_api_t           host_interface_api;

/*
 *  SAI operation type
 *  Values must start with 0 base and be without gaps
 */
typedef enum sai_operation_t {
    SAI_OPERATION_CREATE,
    SAI_OPERATION_REMOVE,
    SAI_OPERATION_SET,
    SAI_OPERATION_GET,
    SAI_OPERATION_MAX
} sai_operation_t;

/*
 *  Attribute value types
 */
typedef enum _sai_attribute_value_type_t {
    SAI_ATTR_VAL_TYPE_UNDETERMINED,
    SAI_ATTR_VAL_TYPE_BOOL,
    SAI_ATTR_VAL_TYPE_CHARDATA,
    SAI_ATTR_VAL_TYPE_U8,
    SAI_ATTR_VAL_TYPE_S8,
    SAI_ATTR_VAL_TYPE_U16,
    SAI_ATTR_VAL_TYPE_S16,
    SAI_ATTR_VAL_TYPE_U32,
    SAI_ATTR_VAL_TYPE_S32,
    SAI_ATTR_VAL_TYPE_U64,
    SAI_ATTR_VAL_TYPE_S64,
    SAI_ATTR_VAL_TYPE_MAC,
    SAI_ATTR_VAL_TYPE_IPV4,
    SAI_ATTR_VAL_TYPE_IPV6,
    SAI_ATTR_VAL_TYPE_IPADDR,
    SAI_ATTR_VAL_TYPE_OID,
    SAI_ATTR_VAL_TYPE_OBJLIST,
    SAI_ATTR_VAL_TYPE_U32LIST,
    SAI_ATTR_VAL_TYPE_S32LIST,
    SAI_ATTR_VAL_TYPE_VLANLIST,
    SAI_ATTR_VAL_TYPE_ACLFIELD,
    SAI_ATTR_VAL_TYPE_ACLACTION,
    SAI_ATTR_VAL_TYPE_PORTBREAKOUT
} sai_attribute_value_type_t;
typedef struct _sai_attribute_entry_t {
    sai_attr_id_t              id;
    bool                       mandatory_on_create;
    bool                       valid_for_create;
    bool                       valid_for_set;
    bool                       valid_for_get;
    const char                *attrib_name;
    sai_attribute_value_type_t type;
} sai_attribute_entry_t;
typedef struct _stub_object_id_t {
    sai_uint8_t  object_type;
    sai_uint8_t  reserved[3];
    sai_uint32_t data;
} stub_object_id_t;

#define SAI_TYPE_CHECK_RANGE(type) (type < SAI_OBJECT_TYPE_MAX)

#define SAI_TYPE_STR(type) SAI_TYPE_CHECK_RANGE(type) ? sai_type2str_arr[type] : "Unknown object type"

static __attribute__((__used__)) const char *sai_type2str_arr[] = {
    /* SAI_OBJECT_TYPE_NULL = 0 */
    "NULL type",

    /*SAI_OBJECT_TYPE_PORT = 1 */
    "Port type",

    /*SAI_OBJECT_TYPE_LAG = 2 */
    "LAG type",

    /*SAI_OBJECT_TYPE_VIRTUAL_ROUTER = 3 */
    "Virtual router type",

    /* SAI_OBJECT_TYPE_NEXT_HOP = 4 */
    "Next hop type",

    /* SAI_OBJECT_TYPE_NEXT_HOP_GROUP = 5 */
    "Next hop group type",

    /* SAI_OBJECT_TYPE_ROUTER_INTERFACE = 6 */
    "Router interface type",

    /* SAI_OBJECT_TYPE_ACL_TABLE = 7 */
    "ACL table type",

    /* SAI_OBJECT_TYPE_ACL_ENTRY = 8 */
    "ACL entry type",

    /* SAI_OBJECT_TYPE_ACL_COUNTER = 9 */
    "ACL counter type",

    /* SAI_OBJECT_TYPE_HOST_INTERFACE = 10 */
    "Host interface type",

    /* SAI_OBJECT_TYPE_MIRROR = 11 */
    "Mirror type",

    /* SAI_OBJECT_TYPE_SAMPLEPACKET = 12 */
    "Sample packet type",

    /* SAI_OBJECT_TYPE_STP_INSTANCE = 13 */
    "Stp instance type"

    /* SAI_OBJECT_TYPE_MAX = 14 */
};

typedef union {
    const sai_fdb_entry_t          * fdb_entry;
    const sai_neighbor_entry_t     * neighbor_entry;
    const sai_unicast_route_entry_t* unicast_route_entry;
    const sai_vlan_id_t              vlan_id;
    const sai_object_id_t            object_id;
} sai_object_key_t;
typedef sai_status_t (*sai_attribute_set_fn)(_In_ const sai_object_key_t *key, _In_ const sai_attribute_value_t *value,
                                             void *arg);
typedef union {
    int dummy;
} vendor_cache_t;
typedef sai_status_t (*sai_attribute_get_fn)(_In_ const sai_object_key_t *key, _Inout_ sai_attribute_value_t *value,
                                             _In_ uint32_t attr_index, _Inout_ vendor_cache_t *cache, void *arg);
typedef struct _sai_vendor_attribute_entry_t {
    sai_attr_id_t        id;
    bool                 is_implemented[SAI_OPERATION_MAX];
    bool                 is_supported[SAI_OPERATION_MAX];
    sai_attribute_get_fn getter;
    void                *getter_arg;
    sai_attribute_set_fn setter;
    void                *setter_arg;
} sai_vendor_attribute_entry_t;

#define END_FUNCTIONALITY_ATTRIBS_ID 0xFFFFFFFF

sai_status_t check_attribs_metadata(_In_ uint32_t                            attr_count,
                                    _In_ const sai_attribute_t              *attr_list,
                                    _In_ const sai_attribute_entry_t        *functionality_attr,
                                    _In_ const sai_vendor_attribute_entry_t *functionality_vendor_attr,
                                    _In_ sai_operation_t                     oper);

sai_status_t find_attrib_in_list(_In_ uint32_t                       attr_count,
                                 _In_ const sai_attribute_t         *attr_list,
                                 _In_ sai_attr_id_t                  attrib_id,
                                 _Out_ const sai_attribute_value_t **attr_value,
                                 _Out_ uint32_t                     *index);

sai_status_t sai_set_attribute(_In_ const sai_object_key_t             *key,
                               _In_ const char                         *key_str,
                               _In_ const sai_attribute_entry_t        *functionality_attr,
                               _In_ const sai_vendor_attribute_entry_t *functionality_vendor_attr,
                               _In_ const sai_attribute_t              *attr);

sai_status_t sai_get_attributes(_In_ const sai_object_key_t             *key,
                                _In_ const char                         *key_str,
                                _In_ const sai_attribute_entry_t        *functionality_attr,
                                _In_ const sai_vendor_attribute_entry_t *functionality_vendor_attr,
                                _In_ uint32_t                            attr_count,
                                _Inout_ sai_attribute_t                 *attr_list);

#define MAX_KEY_STR_LEN        100
#define MAX_VALUE_STR_LEN      100
#define MAX_LIST_VALUE_STR_LEN 1000

#define PORT_NUMBER 32

sai_status_t sai_value_to_str(_In_ sai_attribute_value_t      value,
                              _In_ sai_attribute_value_type_t type,
                              _In_ uint32_t                   max_length,
                              _Out_ char                     *value_str);
sai_status_t sai_attr_list_to_str(_In_ uint32_t                     attr_count,
                                  _In_ const sai_attribute_t       *attr_list,
                                  _In_ const sai_attribute_entry_t *functionality_attr,
                                  _In_ uint32_t                     max_length,
                                  _Out_ char                       *list_str);
sai_status_t sai_ipprefix_to_str(_In_ sai_ip_prefix_t value, _In_ uint32_t max_length, _Out_ char *value_str);
sai_status_t sai_ipaddr_to_str(_In_ sai_ip_address_t value,
                               _In_ uint32_t         max_length,
                               _Out_ char           *value_str,
                               _Out_ int            *chars_written);
sai_status_t sai_nexthops_to_str(_In_ uint32_t               next_hop_count,
                                 _In_ const sai_object_id_t* nexthops,
                                 _In_ uint32_t               max_length,
                                 _Out_ char                 *str);
sai_status_t stub_object_to_type(sai_object_id_t object_id, sai_object_type_t type, uint32_t *data);
sai_status_t stub_create_object(sai_object_type_t type, uint32_t data, sai_object_id_t *object_id);

void db_init_next_hop_group();
sai_status_t db_get_next_hop_group(_In_ uint32_t next_hop_group_id, _Out_ sai_object_list_t *next_hop_list);
void db_init_vlan();

sai_status_t stub_fill_objlist(sai_object_id_t *data, uint32_t count, sai_object_list_t *list);
sai_status_t stub_fill_u32list(uint32_t *data, uint32_t count, sai_u32_list_t *list);
sai_status_t stub_fill_s32list(int32_t *data, uint32_t count, sai_s32_list_t *list);
sai_status_t stub_fill_vlanlist(sai_vlan_id_t *data, uint32_t count, sai_vlan_list_t *list);

void utils_log(const sai_log_level_t severity, const char *module_name, const char *p_str, ...);

#define QUOTEME_(x) #x                        /* add "" to x */
#define QUOTEME(x)  QUOTEME_(x)

#define STUB_ASSERT(exp) assert((exp))

#ifndef _WIN32

#define UNREFERENCED_PARAMETER(X)
#define UTILS_LOG(level, fmt, arg ...)                                \
    do {                                            \
        utils_log(level, QUOTEME(__MODULE__), "%s[%d]- %s: " fmt,            \
                  __FILE__, __LINE__, __FUNCTION__, ## arg);            \
    } while (0)

#define STUB_LOG_ENTER()           UTILS_LOG(SAI_LOG_DEBUG, "%s: [\n", __FUNCTION__)
#define STUB_LOG_EXIT()            UTILS_LOG(SAI_LOG_DEBUG, "%s: ]\n", __FUNCTION__)
#define STUB_LOG_DBG(fmt, arg ...) UTILS_LOG(SAI_LOG_DEBUG, fmt, ## arg)
#define STUB_LOG_INF(fmt, arg ...) UTILS_LOG(SAI_LOG_INFO, fmt, ## arg)
#define STUB_LOG_WRN(fmt, arg ...) UTILS_LOG(SAI_LOG_WARN, fmt, ## arg)
#define STUB_LOG_ERR(fmt, arg ...) UTILS_LOG(SAI_LOG_ERROR, fmt, ## arg)
#define STUB_LOG_NTC(fmt, arg ...) UTILS_LOG(SAI_LOG_NOTICE, fmt, ## arg)

#else /* WIN32 */

#define PRId64 "lld"
#include <windows.h>
#define UTILS_LOG(level, fmt, ...)                                \
    do {                                            \
        utils_log(level, QUOTEME(__MODULE__), "%s[%d]- %s: " fmt,            \
                  __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);            \
    } while (0)

#define STUB_LOG_ENTER()       UTILS_LOG(SAI_LOG_DEBUG, "%s: [\n", __FUNCTION__)
#define STUB_LOG_EXIT()        UTILS_LOG(SAI_LOG_DEBUG, "%s: ]\n", __FUNCTION__)
#define STUB_LOG_DBG(fmt, ...) UTILS_LOG(SAI_LOG_DEBUG, fmt, __VA_ARGS__)
#define STUB_LOG_INF(fmt, ...) UTILS_LOG(SAI_LOG_INFO, fmt, __VA_ARGS__)
#define STUB_LOG_WRN(fmt, ...) UTILS_LOG(SAI_LOG_WARN, fmt, __VA_ARGS__)
#define STUB_LOG_ERR(fmt, ...) UTILS_LOG(SAI_LOG_ERROR, fmt, __VA_ARGS__)
#define STUB_LOG_NTC(fmt, ...) UTILS_LOG(SAI_LOG_NOTICE, fmt, __VA_ARGS__)

#endif

#endif /* __STUBSAI_H_ */
