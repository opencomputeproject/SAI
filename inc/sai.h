/**
 * Copyright (c) 2014 Microsoft Open Technologies, Inc.
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
 *    Dell Products, L.P., Facebook, Inc., Marvell International Ltd.
 *
 * @file    sai.h
 *
 * @brief   This module defines an entry point into Switch Abstraction Interface (SAI)
 */

#if !defined (__SAI_H_)
#define __SAI_H_

#include "saiacl.h"
#include "saibridge.h"
#include "saibuffer.h"
#include "saicounter.h"
#include "saifdb.h"
#include "saihash.h"
#include "saihostif.h"
#include "saiipmcgroup.h"
#include "saiipmc.h"
#include "saiipsec.h"
#include "sail2mcgroup.h"
#include "sail2mc.h"
#include "sailag.h"
#include "saimacsec.h"
#include "saimcastfdb.h"
#include "saimirror.h"
#include "saimpls.h"
#include "saineighbor.h"
#include "sainexthopgroup.h"
#include "sainexthop.h"
#include "saiobject.h"
#include "saipolicer.h"
#include "saiport.h"
#include "saiqosmap.h"
#include "saiqueue.h"
#include "sairoute.h"
#include "sairouterinterface.h"
#include "sairpfgroup.h"
#include "saisamplepacket.h"
#include "saischedulergroup.h"
#include "saischeduler.h"
#include "saisrv6.h"
#include "saistatus.h"
#include "saistp.h"
#include "saiswitch.h"
#include "saisystemport.h"
#include "saitam.h"
#include "saitunnel.h"
#include "saitypes.h"
#include "saiudf.h"
#include "saivirtualrouter.h"
#include "saivlan.h"
#include "saiwred.h"
#include "saidtel.h"
#include "saibfd.h"
#include "sainat.h"
#include "saiisolationgroup.h"
#include "saidebugcounter.h"
#include "saimymac.h"
#include "saiversion.h"

/**
 * @defgroup SAI SAI - Entry point specific API definitions.
 *
 * @{
 */

/**
 * @brief Defined API sets have assigned IDs.
 *
 * If specific API method table changes in any way (method signature, number of
 * methods), a new ID needs to be created (e.g. VLAN2) and old API still may
 * need to be supported for compatibility with older adapter hosts.
 */
typedef enum _sai_api_t
{
    SAI_API_UNSPECIFIED      =  0, /**< unspecified API */
    SAI_API_SWITCH           =  1, /**< sai_switch_api_t */
    SAI_API_PORT             =  2, /**< sai_port_api_t */
    SAI_API_FDB              =  3, /**< sai_fdb_api_t */
    SAI_API_VLAN             =  4, /**< sai_vlan_api_t */
    SAI_API_VIRTUAL_ROUTER   =  5, /**< sai_virtual_router_api_t */
    SAI_API_ROUTE            =  6, /**< sai_route_api_t */
    SAI_API_NEXT_HOP         =  7, /**< sai_next_hop_api_t */
    SAI_API_NEXT_HOP_GROUP   =  8, /**< sai_next_hop_group_api_t */
    SAI_API_ROUTER_INTERFACE =  9, /**< sai_router_interface_api_t */
    SAI_API_NEIGHBOR         = 10, /**< sai_neighbor_api_t */
    SAI_API_ACL              = 11, /**< sai_acl_api_t */
    SAI_API_HOSTIF           = 12, /**< sai_hostif_api_t */
    SAI_API_MIRROR           = 13, /**< sai_mirror_api_t */
    SAI_API_SAMPLEPACKET     = 14, /**< sai_samplepacket_api_t */
    SAI_API_STP              = 15, /**< sai_stp_api_t */
    SAI_API_LAG              = 16, /**< sai_lag_api_t */
    SAI_API_POLICER          = 17, /**< sai_policer_api_t */
    SAI_API_WRED             = 18, /**< sai_wred_api_t */
    SAI_API_QOS_MAP          = 19, /**< sai_qos_map_api_t */
    SAI_API_QUEUE            = 20, /**< sai_queue_api_t */
    SAI_API_SCHEDULER        = 21, /**< sai_scheduler_api_t */
    SAI_API_SCHEDULER_GROUP  = 22, /**< sai_scheduler_group_api_t */
    SAI_API_BUFFER           = 23, /**< sai_buffer_api_t */
    SAI_API_HASH             = 24, /**< sai_hash_api_t */
    SAI_API_UDF              = 25, /**< sai_udf_api_t */
    SAI_API_TUNNEL           = 26, /**< sai_tunnel_api_t */
    SAI_API_L2MC             = 27, /**< sai_l2mc_api_t */
    SAI_API_IPMC             = 28, /**< sai_ipmc_api_t */
    SAI_API_RPF_GROUP        = 29, /**< sai_rpf_group_api_t */
    SAI_API_L2MC_GROUP       = 30, /**< sai_l2mc_group_api_t */
    SAI_API_IPMC_GROUP       = 31, /**< sai_ipmc_group_api_t */
    SAI_API_MCAST_FDB        = 32, /**< sai_mcast_fdb_api_t */
    SAI_API_BRIDGE           = 33, /**< sai_bridge_api_t */
    SAI_API_TAM              = 34, /**< sai_tam_api_t */
    SAI_API_SRV6             = 35, /**< sai_srv6_api_t */
    SAI_API_MPLS             = 36, /**< sai_mpls_api_t */
    SAI_API_DTEL             = 37, /**< sai_dtel_api_t (experimental) */
    SAI_API_BFD              = 38, /**< sai_bfd_api_t */
    SAI_API_ISOLATION_GROUP  = 39, /**< sai_isolation_group_api_t */
    SAI_API_NAT              = 40, /**< sai_nat_api_t */
    SAI_API_COUNTER          = 41, /**< sai_counter_api_t */
    SAI_API_DEBUG_COUNTER    = 42, /**< sai_debug_counter_api_t */
    SAI_API_MACSEC           = 43, /**< sai_macsec_api_t */
    SAI_API_SYSTEM_PORT      = 44, /**< sai_system_port_api_t */
    SAI_API_MY_MAC           = 45, /**< sai_my_mac_api_t */
    SAI_API_IPSEC            = 46, /**< sai_ipsec_api_t */
    SAI_API_MAX,                   /**< total number of APIs */
} sai_api_t;

/**
 * @brief Defines log level
 */
typedef enum _sai_log_level_t
{
    /** Log Level Debug */
    SAI_LOG_LEVEL_DEBUG            = 0,

    /** Log Level Info */
    SAI_LOG_LEVEL_INFO             = 1,

    /** Log Level Notice */
    SAI_LOG_LEVEL_NOTICE           = 2,

    /** Log level Warning */
    SAI_LOG_LEVEL_WARN             = 3,

    /** Log Level Error */
    SAI_LOG_LEVEL_ERROR            = 4,

    /** Log Level Critical */
    SAI_LOG_LEVEL_CRITICAL         = 5

} sai_log_level_t;

typedef const char* (*sai_profile_get_value_fn)(
        _In_ sai_switch_profile_id_t profile_id,
        _In_ const char *variable);

typedef int (*sai_profile_get_next_value_fn)(
        _In_ sai_switch_profile_id_t profile_id,
        _Out_ const char **variable,
        _Out_ const char **value);

/**
 * @brief Method table that contains function pointers for services exposed by
 * adapter host for adapter.
 */
typedef struct _sai_service_method_table_t
{
    /**
     * @brief Get variable value given its name
     */
    sai_profile_get_value_fn        profile_get_value;

    /**
     * @brief Enumerate all the K/V pairs in a profile.
     *
     * Pointer to NULL passed as variable restarts enumeration. Function
     * returns 0 if next value exists, -1 at the end of the list.
     */
    sai_profile_get_next_value_fn   profile_get_next_value;

} sai_service_method_table_t;

/**
 * @brief Adapter module initialization call
 *
 * This is NOT for SDK initialization.
 *
 * @param[in] flags Reserved for future use, must be zero
 * @param[in] services Methods table with services provided by adapter host
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
sai_status_t sai_api_initialize(
        _In_ uint64_t flags,
        _In_ const sai_service_method_table_t *services);

/**
 * @brief Retrieve a pointer to the C-style method table for desired SAI
 * functionality as specified by the given sai_api_id.
 *
 * @param[in] api SAI API ID
 * @param[out] api_method_table Caller allocated method table. The table must
 * remain valid until the sai_api_uninitialize() is called.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
sai_status_t sai_api_query(
        _In_ sai_api_t api,
        _Out_ void **api_method_table);

/**
 * @brief Uninitialize adapter module. SAI functionalities,
 * retrieved via sai_api_query() cannot be used after this call.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
sai_status_t sai_api_uninitialize(void);

/**
 * @brief Set log level for SAI API module
 *
 * The default log level is #SAI_LOG_LEVEL_WARN.
 *
 * @param[in] api SAI API ID
 * @param[in] log_level Log level
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
sai_status_t sai_log_set(
        _In_ sai_api_t api,
        _In_ sai_log_level_t log_level);

/**
 * @brief Query SAI object type.
 *
 * @param[in] object_id Object id
 *
 * @return #SAI_OBJECT_TYPE_NULL when sai_object_id is not valid.
 * Otherwise, return a valid SAI object type SAI_OBJECT_TYPE_XXX.
 */
sai_object_type_t sai_object_type_query(
        _In_ sai_object_id_t object_id);

/**
 * @brief Query SAI switch id.
 *
 * @param[in] object_id Object id
 *
 * @return #SAI_NULL_OBJECT_ID when sai_object_id is not valid.
 * Otherwise, return a valid SAI_OBJECT_TYPE_SWITCH object on which
 * provided object id belongs. If valid switch id object is provided
 * as input parameter it should return itself.
 */
sai_object_id_t sai_switch_id_query(
        _In_ sai_object_id_t object_id);

/**
 * @brief Generate dump file. The dump file may include SAI state information and vendor SDK information.
 *
 * @param[in] dump_file_name Full path for dump file
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
sai_status_t sai_dbg_generate_dump(
        _In_ const char *dump_file_name);

/**
 * @brief Get SAI object type resource availability.
 *
 * @param[in] switch_id SAI Switch object id
 * @param[in] object_type SAI object type
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list List of attributes that to distinguish resource
 * @param[out] count Available objects left
 *
 * @return #SAI_STATUS_NOT_SUPPORTED if the given object type does not support resource accounting.
 * Otherwise, return #SAI_STATUS_SUCCESS.
 */
sai_status_t sai_object_type_get_availability(
        _In_ sai_object_id_t switch_id,
        _In_ sai_object_type_t object_type,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list,
        _Out_ uint64_t *count);

/**
 * @}
 */
#endif /** __SAI_H_ */
