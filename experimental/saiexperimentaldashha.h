/**
 * Copyright (c) 2024 Microsoft Open Technologies, Inc.
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
 * @file    saiexperimentaldashha.h
 *
 * @brief   This module defines SAI P4 extension  interface
 */

#if !defined (__SAIEXPERIMENTALDASHHA_H_)
#define __SAIEXPERIMENTALDASHHA_H_

#include <saitypes.h>

/**
 * @defgroup SAIEXPERIMENTALDASH_HA SAI - Extension specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute ID for DASH HA
 */
typedef enum _sai_dash_ha_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_DASH_HA_ATTR_START,

    /**
     * @brief HA Local IP for control traffic
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_DASH_HA_ATTR_CONTROL_LOCAL_IP = SAI_DASH_HA_ATTR_START,

    /**
     * @brief HA Peer IP for control traffic
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_DASH_HA_ATTR_CONTROL_PEER_IP,

    /**
     * @brief List of HA VIPs
     *
     * @type sai_dash_ha_vip_info_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_DASH_HA_ATTR_VIP_INFO_LIST,

    /**
     * @brief HA heartbeat interval time in ms
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_DASH_HA_ATTR_HEARTBEAT_INTERVAL,

    /**
     * @brief Number of HA heartbeats missed before declaring Peer unreachable
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_DASH_HA_ATTR_HEARTBEAT_COUNT,

    /**
     * @brief Time in ms required for network to converge after switchover
     *
     * @par Any traffic directed to wrong DPU after this time will be blackholed
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 100
     */
    SAI_DASH_HA_ATTR_VIP_CONVERGENCE_TIMEOUT,

    /**
     * @brief List of HA VIP status
     *
     * @type sai_dash_ha_vip_status_list_t
     * @flags READ_ONLY
     */
    SAI_DASH_HA_ATTR_VIP_STATUS_LIST,

    /**
     * @brief End of attributes
     */
    SAI_DASH_HA_ATTR_END,

    /** Custom range base value */
    SAI_DASH_HA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_DASH_HA_ATTR_CUSTOM_RANGE_END

} sai_dash_ha_attr_t;

/**
 * @brief Key for DASH HA FSM APIs
 */
typedef struct _sai_dash_ha_fsm_entry_t
{
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_DASH_HA
     */
    sai_object_id_t dash_ha_id;

    /**
     * @brief VIP ID to which this API applies
     *
     * @par If VIP ID is not specified then this applies to all VIP IDs
     * on the DPU
     */
    uint32_t vip_id;
} sai_dash_ha_fsm_entry_t;

/**
 * @brief Attribute ID for DASH HA FSM API
 */
typedef enum _sai_dash_ha_fsm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_DASH_HA_FSM_ATTR_START,

    /**
     * @brief Force flag to ignore optimizations
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_DASH_HA_FSM_ATTR_FORCE = SAI_DASH_HA_FSM_ATTR_START,

    /**
     * @brief End of attributes
     */
    SAI_DASH_HA_FSM_ATTR_END,

    /** Custom range base value */
    SAI_DASH_HA_FSM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_DASH_HA_FSM_ATTR_CUSTOM_RANGE_END

} sai_dash_ha_fsm_attr_t;

/**
 * @brief Counter IDs in sai_get_dash_ha_stats() call
 */
typedef enum _sai_dash_ha_stat_t
{
    /** DASH HA stat heartbeat messages sent */
    SAI_DASH_HA_STAT_HEARTBEAT_MSG_SENT,

    /** DASH HA stat heartbeat messages received */
    SAI_DASH_HA_STAT_HEARTBEAT_MSG_RECEIVED,

    /** DASH HA stat flow sync requests sent */
    SAI_DASH_HA_STAT_FLOW_SYNC_REQ_SENT,

    /** DASH HA stat flow sync acknowledgements sent */
    SAI_DASH_HA_STAT_FLOW_SYNC_ACK_RECEIVED,

} sai_dash_ha_stat_t;

/**
 * @brief DASH HA event type
 */
typedef enum _sai_dash_ha_fsm_event_t
{
    /** HA Flow reconcile API needs to be invoked */
    SAI_DASH_HA_FSM_EVENT_FLOW_RECONCILE_REQUIRED,

    /** HA Stop followed by HA Start API needs to be invoked */
    SAI_DASH_HA_FSM_EVENT_STOP_START_REQUIRED,

} sai_dash_ha_fsm_event_t;

/**
 * @brief Notification data format received from SAI DASH HA event callback
 */
typedef struct _sai_dash_ha_fsm_event_notification_data_t
{
    /** Event type */
    sai_dash_ha_fsm_event_t event_type;

    /** DASH HA entry */
    sai_dash_ha_fsm_entry_t ha_entry;

} sai_dash_ha_fsm_event_notification_data_t;

/**
 * @brief Create and return a DASH HA object
 *
 * @param[out] dash_ha_id HA object id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_dash_ha_fn)(
        _Out_ sai_object_id_t *dash_ha_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove DASH HA object
 *
 * @param[in] dash_ha_id HA object id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_dash_ha_fn)(
        _In_ sai_object_id_t dash_ha_id);

/**
 * @brief Set DASH HA attribute
 *
 * @param[in] dash_ha_id HA object id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_dash_ha_attribute_fn)(
        _In_ sai_object_id_t dash_ha_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get DASH HA attribute
 *
 * @param[in] dash_ha_id HA object id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_dash_ha_attribute_fn)(
        _In_ sai_object_id_t dash_ha_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Kick-start the HA finite state machine after HA configuration
 *        has been pushed
 *
 * @param[in] ha_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 * #SAI_STATUS_OBJECT_IN_USE will be returned if this operation could
 * cause traffic disruption. Retry with Force attribute set to go
 * ahead with start.
 */
typedef sai_status_t (*sai_start_dash_ha_fsm_fn)(
        _In_ const sai_dash_ha_fsm_entry_t *ha_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Shutdown the HA finite state machine to take out a DPU from HA pairing
 *
 * @param[in] ha_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 * #SAI_STATUS_OBJECT_IN_USE will be returned if its not in a safe state
 * for shutdown. Retry with Force attribute set to go ahead with stop.
 */
typedef sai_status_t (*sai_stop_dash_ha_fsm_fn)(
        _In_ const sai_dash_ha_fsm_entry_t *ha_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Activate the HA admin role
 *
 * @param[in] ha_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_activate_admin_role_dash_ha_fsm_fn)(
        _In_ const sai_dash_ha_fsm_entry_t *ha_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Initiate controlled switchover to the HA peer
 *
 * @param[in] ha_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 * #SAI_STATUS_OBJECT_IN_USE will be returned if this operation could
 * cause traffic disruption. Retry with Force attribute set to go
 * ahead with switchover.
 */
typedef sai_status_t (*sai_initiate_switchover_dash_ha_fsm_fn)(
        _In_ const sai_dash_ha_fsm_entry_t *ha_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Reconcile synced flows with current local configuration
 *
 * @param[in] ha_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_flow_reconcile_dash_ha_fsm_fn)(
        _In_ const sai_dash_ha_fsm_entry_t *ha_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Get DASH HA statistics counters.
 *
 * @param[in] dash_ha_id HA object id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_dash_ha_stats_fn)(
        _In_ sai_object_id_t dash_ha_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get DASH HA statistics counters extended.
 *
 * @param[in] dash_ha_id HA object id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_dash_ha_stats_ext_fn)(
        _In_ sai_object_id_t dash_ha_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear DASH HA statistics counters.
 *
 * @param[in] dash_ha_id HA object id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_dash_ha_stats_fn)(
        _In_ sai_object_id_t dash_ha_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief Clear all DASH HA statistics counters.
 *
 * @param[in] dash_ha_id HA object id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_dash_ha_all_stats_fn)(
        _In_ sai_object_id_t dash_ha_id);

/**
 * @brief DASH HA FSM notifications
 *
 * @count data[count]
 *
 * @param[in] count Number of notifications
 * @param[in] data Pointer to DASH HA FSM event notification data array
 */
typedef void (*sai_dash_ha_fsm_event_notification_fn)(
        _In_ uint32_t count,
        _In_ const sai_dash_ha_fsm_event_notification_data_t *data);

typedef struct _sai_dash_ha_api_t
{
    sai_create_dash_ha_fn                      create_dash_ha;
    sai_remove_dash_ha_fn                      remove_dash_ha;
    sai_set_dash_ha_attribute_fn               set_dash_ha_attribute;
    sai_get_dash_ha_attribute_fn               get_dash_ha_attribute;
    sai_get_dash_ha_stats_fn                   get_dash_ha_stats;
    sai_get_dash_ha_stats_ext_fn               get_dash_ha_stats_ext;
    sai_clear_dash_ha_stats_fn                 clear_dash_ha_stats;
    sai_clear_dash_ha_all_stats_fn             clear_dash_ha_all_stats;
    sai_start_dash_ha_fsm_fn                   start_dash_ha_fsm;
    sai_stop_dash_ha_fsm_fn                    stop_dash_ha_fsm;
    sai_activate_admin_role_dash_ha_fsm_fn     activate_admin_role_dash_ha_fsm;
    sai_initiate_switchover_dash_ha_fsm_fn     initiate_switchover_dash_ha_fsm;
    sai_flow_reconcile_dash_ha_fsm_fn          flow_reconcile_dash_ha_fsm;
} sai_dash_ha_api_t;

/**
 * @}
 */
#endif /** __SAIEXPERIMENTALDASHHA_H_ */
