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

service_method_table_t g_services;
bool                   g_initialized = false;

/*
 * Routine Description:
 *     Adapter module initialization call. This is NOT for SDK initialization.
 *
 * Arguments:
 *     [in] flags - reserved for future use, must be zero
 *     [in] services - methods table with services provided by adapter host
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t sai_api_initialize(_In_ uint64_t flags, _In_ const service_method_table_t* services)
{
    if ((NULL == services) || (NULL == services->profile_get_next_value) || (NULL == services->profile_get_value)) {
        fprintf(stderr, "Invalid services handle passed to SAI API initialize\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }
    memcpy(&g_services, services, sizeof(g_services));

    if (0 != flags) {
        fprintf(stderr, "Invalid flags passed to SAI API initialize\n");

        return SAI_STATUS_INVALID_PARAMETER;
    }

    g_initialized = true;

    return SAI_STATUS_SUCCESS;
}

/*
 * Routine Description:
 *     Retrieve a pointer to the C-style method table for desired SAI
 *     functionality as specified by the given sai_api_id.
 *
 * Arguments:
 *     [in] sai_api_id - SAI api ID
 *     [out] api_method_table - Caller allocated method table
 *           The table must remain valid until the sai_api_uninitialize() is called
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t sai_api_query(_In_ sai_api_t sai_api_id, _Out_ void** api_method_table)
{
    if (NULL == api_method_table) {
        fprintf(stderr, "NULL method table passed to SAI API initialize\n");

        return SAI_STATUS_INVALID_PARAMETER;
    }
    if (!g_initialized) {
        fprintf(stderr, "SAI API not initialized before calling API query\n");

        return SAI_STATUS_UNINITIALIZED;
    }

    switch (sai_api_id) {
    case SAI_API_SWITCH:
        *(const sai_switch_api_t**)api_method_table = &switch_api;
        return SAI_STATUS_SUCCESS;

    case SAI_API_PORT:
        *(const sai_port_api_t**)api_method_table = &port_api;
        return SAI_STATUS_SUCCESS;

    case SAI_API_FDB:
        *(const sai_fdb_api_t**)api_method_table = &fdb_api;
        return SAI_STATUS_SUCCESS;

    case SAI_API_VLAN:
        *(const sai_vlan_api_t**)api_method_table = &vlan_api;
        return SAI_STATUS_SUCCESS;

    case SAI_API_VIRTUAL_ROUTER:
        *(const sai_virtual_router_api_t**)api_method_table = &router_api;
        return SAI_STATUS_SUCCESS;

    case SAI_API_ROUTE:
        *(const sai_route_api_t**)api_method_table = &route_api;
        return SAI_STATUS_SUCCESS;

    case SAI_API_NEXT_HOP:
        *(const sai_next_hop_api_t**)api_method_table = &next_hop_api;
        return SAI_STATUS_SUCCESS;

    case SAI_API_NEXT_HOP_GROUP:
        *(const sai_next_hop_group_api_t**)api_method_table = &next_hop_group_api;
        return SAI_STATUS_SUCCESS;

    case SAI_API_ROUTER_INTERFACE:
        *(const sai_router_interface_api_t**)api_method_table = &router_interface_api;
        return SAI_STATUS_SUCCESS;

    case SAI_API_NEIGHBOR:
        *(const sai_neighbor_api_t**)api_method_table = &neighbor_api;
        return SAI_STATUS_SUCCESS;

    case SAI_API_QOS_MAPS:
        /* TODO : implement */
        return SAI_STATUS_NOT_IMPLEMENTED;

    case SAI_API_ACL:
        /* TODO : implement */
        return SAI_STATUS_NOT_IMPLEMENTED;

    case SAI_API_HOST_INTERFACE:
        *(const sai_hostif_api_t**)api_method_table = &host_interface_api;
        return SAI_STATUS_SUCCESS;

    case SAI_API_MIRROR:
        /* TODO : implement */
        return SAI_STATUS_NOT_IMPLEMENTED;

    case SAI_API_SAMPLEPACKET:
        /* TODO : implement */
        return SAI_STATUS_NOT_IMPLEMENTED;

    case SAI_API_STP:
        /* TODO : implement */
        return SAI_STATUS_NOT_IMPLEMENTED;

    case SAI_API_LAG:
        /* TODO : implement */
        return SAI_STATUS_NOT_IMPLEMENTED;

    default:
        fprintf(stderr, "Invalid API type %d\n", sai_api_id);
        return SAI_STATUS_INVALID_PARAMETER;
    }
}

/*
 * Routine Description:
 *   Uninitialization of the adapter module. SAI functionalities, retrieved via
 *   sai_api_query() cannot be used after this call.
 *
 * Arguments:
 *   None
 *
 * Return Values:
 *   SAI_STATUS_SUCCESS on success
 *   Failure status code on error
 */
sai_status_t sai_api_uninitialize(void)
{
    memset(&g_services, 0, sizeof(g_services));
    g_initialized = false;

    return SAI_STATUS_SUCCESS;
}

/*
 * Routine Description:
 *     Set log level for sai api module. The default log level is SAI_LOG_WARN.
 *
 * Arguments:
 *     [in] sai_api_id - SAI api ID
 *     [in] log_level - log level
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t sai_log_set(_In_ sai_api_t sai_api_id, _In_ sai_log_level_t log_level)
{
    switch (log_level) {
    case SAI_LOG_DEBUG:
        break;

    case SAI_LOG_INFO:
        break;

    case SAI_LOG_NOTICE:
        break;

    case SAI_LOG_WARN:
        break;

    case SAI_LOG_ERROR:
        break;

    case SAI_LOG_CRITICAL:
        break;

    default:
        fprintf(stderr, "Invalid log level %d\n", log_level);
        return SAI_STATUS_INVALID_PARAMETER;
    }

    switch (sai_api_id) {
    case SAI_API_SWITCH:
        break;

    case SAI_API_PORT:
        break;

    case SAI_API_FDB:
        break;

    case SAI_API_VLAN:
        break;

    case SAI_API_VIRTUAL_ROUTER:
        break;

    case SAI_API_ROUTE:
        break;

    case SAI_API_NEXT_HOP:
        break;

    case SAI_API_NEXT_HOP_GROUP:
        break;

    case SAI_API_ROUTER_INTERFACE:
        break;

    case SAI_API_NEIGHBOR:
        break;

    case SAI_API_QOS_MAPS:
        break;

    case SAI_API_ACL:
        break;

    case SAI_API_HOST_INTERFACE:
        break;

    case SAI_API_MIRROR:
        break;

    case SAI_API_SAMPLEPACKET:
        break;

    case SAI_API_STP:
        break;

    case SAI_API_LAG:
        break;

    default:
        fprintf(stderr, "Invalid API type %d\n", sai_api_id);
        return SAI_STATUS_INVALID_PARAMETER;
    }

    return SAI_STATUS_SUCCESS;
}

/*
 * Routine Description:
 *     Query sai object type.
 *
 * Arguments:
 *     [in] sai_object_id_t
 *
 * Return Values:
 *    Return SAI_OBJECT_TYPE_NULL when sai_object_id is not valid.
 *    Otherwise, return a valid sai object type SAI_OBJECT_TYPE_XXX
 */
sai_object_type_t sai_object_type_query(_In_ sai_object_id_t sai_object_id)
{
    sai_object_type_t type = ((stub_object_id_t*)&sai_object_id)->object_type;

    if SAI_TYPE_CHECK_RANGE(type) {
        return type;
    } else {
        fprintf(stderr, "Unknown type %d", type);
        return SAI_OBJECT_TYPE_NULL;
    }
}
