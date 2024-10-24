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
 * @file    saipolicer.h
 *
 * @brief   This module defines SAI QOS Policer interface
 */

#if !defined (__SAIPOLICER_H_)
#define __SAIPOLICER_H_

#include <saitypes.h>

/**
 * @defgroup SAIPOLICER SAI - QOS Policer specific API definitions
 *
 * @{
 */

/**
 * @brief Enum defining types of meters
 */
typedef enum _sai_meter_type_t
{
    /** Metering is done based on packets */
    SAI_METER_TYPE_PACKETS = 0x00000000,

    /** Metering is done based on bytes */
    SAI_METER_TYPE_BYTES = 0x00000001,

    /** Custom range base value */
    SAI_METER_TYPE_CUSTOM_RANGE_BASE = 0x10000000

} sai_meter_type_t;

/**
 * @brief Enum defining mode of the policer object
 */
typedef enum _sai_policer_mode_t
{
    /** RFC 2697, Single Rate Three color marker, CIR, CBS and PBS, G, Y and R */
    SAI_POLICER_MODE_SR_TCM = 0x00000000,

    /** RFC 2698, Two Rate Three color marker, CIR, CBS, PIR and PBS, G, Y and R */
    SAI_POLICER_MODE_TR_TCM = 0x00000001,

    /** Storm control mode. Single Rate Two color CIR, CBS, G and R */
    SAI_POLICER_MODE_STORM_CONTROL = 0x00000002,

    /** Custom range base value */
    SAI_POLICER_MODE_CUSTOM_RANGE_BASE = 0x10000000

} sai_policer_mode_t;

/**
 * @brief Enum defining Policer color source
 */
typedef enum _sai_policer_color_source_t
{
    /** Previous coloring schemes are ignored */
    SAI_POLICER_COLOR_SOURCE_BLIND = 0x00000000,

    /** Previous coloring schemes are used */
    SAI_POLICER_COLOR_SOURCE_AWARE = 0x00000001,

    /** Custom range base value */
    SAI_POLICER_COLOR_SOURCE_CUSTOM_RANGE_BASE = 0x10000000

} sai_policer_color_source_t;

/**
 * @brief Enum defining Policer Attributes
 */
typedef enum _sai_policer_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_POLICER_ATTR_START = 0x00000000,

    /**
     * @brief Policer Meter Type
     *
     * @type sai_meter_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_POLICER_ATTR_METER_TYPE = SAI_POLICER_ATTR_START,

    /**
     * @brief Policer mode
     *
     * @type sai_policer_mode_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_POLICER_ATTR_MODE = 0x00000001,

    /**
     * @brief Policer Color Source
     *
     * @type sai_policer_color_source_t
     * @flags CREATE_ONLY
     * @default SAI_POLICER_COLOR_SOURCE_AWARE
     */
    SAI_POLICER_ATTR_COLOR_SOURCE = 0x00000002,

    /**
     * @brief Committed burst size bytes/packets based on
     * #SAI_POLICER_ATTR_METER_TYPE
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_POLICER_ATTR_CBS = 0x00000003,

    /**
     * @brief Committed information rate BPS/PPS based on
     * #SAI_POLICER_ATTR_METER_TYPE
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_POLICER_ATTR_CIR = 0x00000004,

    /**
     * @brief Peak burst size bytes/packets based on
     * #SAI_POLICER_ATTR_METER_TYPE
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_POLICER_ATTR_PBS = 0x00000005,

    /**
     * @brief Peak information rate BPS/PPS based on
     * #SAI_POLICER_ATTR_METER_TYPE
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_POLICER_ATTR_MODE == SAI_POLICER_MODE_TR_TCM
     */
    SAI_POLICER_ATTR_PIR = 0x00000006,

    /**
     * @brief Action to take for Green color packets
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_FORWARD
     */
    SAI_POLICER_ATTR_GREEN_PACKET_ACTION = 0x00000007,

    /**
     * @brief Action to take for Yellow color packets
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_FORWARD
     */
    SAI_POLICER_ATTR_YELLOW_PACKET_ACTION = 0x00000008,

    /**
     * @brief Action to take for RED color packets
     *
     * For storm control action should be used as red packet action.
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_FORWARD
     */
    SAI_POLICER_ATTR_RED_PACKET_ACTION = 0x00000009,

    /**
     * @brief Enable/disable counter
     *
     * Default disabled. Modify list needs full new set.
     *
     * @type sai_s32_list_t sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_POLICER_ATTR_ENABLE_COUNTER_PACKET_ACTION_LIST = 0x0000000a,

    /**
     * @brief Policer pool stage
     *
     * @type sai_object_stage_t
     * @flags CREATE_ONLY
     * @default SAI_OBJECT_STAGE_BOTH
     */
    SAI_POLICER_ATTR_OBJECT_STAGE = 0x0000000b,

    /**
     * @brief Set policer statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_POLICER_ATTR_STATS_COUNT_MODE,

    /**
     * @brief Attach counter object list
     *
     * Counter object should be of type Selective.
     * Fill (#SAI_COUNTER_ATTR_TYPE with #SAI_COUNTER_TYPE_SELECTIVE).
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @default empty
     */
    SAI_POLICER_ATTR_SELECTIVE_COUNTER_LIST,

    /**
     * @brief End of attributes
     */
    SAI_POLICER_ATTR_END,

    /** Custom range base value */
    SAI_POLICER_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_POLICER_ATTR_CUSTOM_RANGE_END

} sai_policer_attr_t;

/**
 * @brief Enum defining policer statistics
 */
typedef enum _sai_policer_stat_t
{
    /** Get/set packet count [uint64_t] */
    SAI_POLICER_STAT_PACKETS = 0x00000000,

    /** Get/set byte count [uint64_t] */
    SAI_POLICER_STAT_ATTR_BYTES = 0x00000001,

    /** Get/set green packet count [uint64_t] */
    SAI_POLICER_STAT_GREEN_PACKETS = 0x00000002,

    /** Get/set green byte count [uint64_t] */
    SAI_POLICER_STAT_GREEN_BYTES = 0x00000003,

    /** Get/set yellow packet count [uint64_t] */
    SAI_POLICER_STAT_YELLOW_PACKETS = 0x00000004,

    /** Get/set yellow byte count [uint64_t] */
    SAI_POLICER_STAT_YELLOW_BYTES = 0x00000005,

    /** Get/set red packet count [uint64_t] */
    SAI_POLICER_STAT_RED_PACKETS = 0x00000006,

    /** Get/set red byte count [uint64_t] */
    SAI_POLICER_STAT_RED_BYTES = 0x00000007,

    /** Custom range base value */
    SAI_POLICER_STAT_CUSTOM_RANGE_BASE = 0x10000000

} sai_policer_stat_t;

/**
 * @brief Create Policer
 *
 * @param[out] policer_id The policer id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_policer_fn)(
        _Out_ sai_object_id_t *policer_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Delete policer
 *
 * @param[in] policer_id Policer id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_policer_fn)(
        _In_ sai_object_id_t policer_id);

/**
 * @brief Set Policer attribute
 *
 * @param[in] policer_id Policer id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_policer_attribute_fn)(
        _In_ sai_object_id_t policer_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get Policer attribute
 *
 * @param[in] policer_id Policer id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_policer_attribute_fn)(
        _In_ sai_object_id_t policer_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get Policer Statistics. Deprecated for backward compatibility.
 *
 * @param[in] policer_id Policer id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_policer_stats_fn)(
        _In_ sai_object_id_t policer_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get Policer Statistics extended
 *
 * @param[in] policer_id Policer id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_policer_stats_ext_fn)(
        _In_ sai_object_id_t policer_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear Policer statistics counters.
 *
 * @param[in] policer_id Policer id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_policer_stats_fn)(
        _In_ sai_object_id_t policer_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief Policer methods table retrieved with sai_api_query()
 */
typedef struct _sai_policer_api_t
{
    sai_create_policer_fn                 create_policer;
    sai_remove_policer_fn                 remove_policer;
    sai_set_policer_attribute_fn          set_policer_attribute;
    sai_get_policer_attribute_fn          get_policer_attribute;
    sai_get_policer_stats_fn              get_policer_stats;
    sai_get_policer_stats_ext_fn          get_policer_stats_ext;
    sai_clear_policer_stats_fn            clear_policer_stats;

} sai_policer_api_t;

/**
 * @}
 */
#endif /** __SAIPOLICER_H_ */
