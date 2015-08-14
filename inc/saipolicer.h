/*
* Copyright (c) 2015 Dell Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License"); you may
* not use this file except in compliance with the License. You may obtain
* a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
*
* THIS CODE IS PROVIDED ON AN  *AS IS* BASIS, WITHOUT WARRANTIES OR
* CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
* LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
* FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
*
* See the Apache Version 2.0 License for specific language governing
* permissions and limitations under the License.
*
* @file saipolicer.h
*
* @brief This file contains Qos Policer functionality.
************************************************************************/

#if !defined (__SAIPOLICER_H_)
#define __SAIPOLICER_H_

#include "saitypes.h"

/** \defgroup SAIPOLICER  SAI - Qos Policer specific API definitions.
 *
 *  \{
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

    /* -- */
    /* Custom range base value */
    SAI_METER_TYPE_CUSTOM_RANGE_BASE = 0x10000000

} sai_meter_type_t;

/** @brief Enum defining mode of the policer object*/
typedef enum _sai_policer_mode_t
{
    /**RFC 2697, Single Rate Three color marker, CIR, CBS and PBS, G, Y and R*/
    SAI_POLICER_MODE_Sr_TCM = 0x00000000,

    /** RFC 2698,  Two Rate Three color marker, CIR, CBS , PIR and PBS, G, Y and R*/
    SAI_POLICER_MODE_Tr_TCM = 0x00000001,

    /** Storm control mode
     * Single Rate Two color CIR, CBS, G and R */
    SAI_POLICER_MODE_STORM_CONTROL = 0x00000002,

    /* -- */
    /* Custom range base value */
    SAI_POLICER_MODE_CUSTOM_RANGE_BASE = 0x10000000
} sai_policer_mode_t;


/** @brief Enum defining Policer color source */
typedef enum _sai_policer_color_source_t
{
    /** Previous coloring schemes are ignored */
    SAI_POLICER_COLOR_SOURCE_BLIND = 0x00000000,

    /** Previous coloring schemes are used */
    SAI_POLICER_COLOR_SOURCE_AWARE = 0x00000001,

    /* -- */
    /* Custom range base value */
    SAI_POLICER_COLOR_CUSTOM_RANGE_BASE = 0x10000000
} sai_policer_color_source_t;


/** @brief Enum defining Policer Attributes */
typedef enum _sai_policer_attr_t
{

    /** Policer Meter Type [sai_meter_type_t ]
     *  MANDATORY_ON_CREATE,  CREATE_ONLY
     */
    SAI_POLICER_ATTR_METER_TYPE = 0x00000000,

    /** Policer mode [sai_policer_mode_t],
     * MANDATORY_ON_CREATE,  CREATE_ONLY */
    SAI_POLICER_ATTR_MODE = 0x00000001,

    /** Policer Color Source [sai_policer_color_source_t],
     * Default is SAI_POLICER_COLOR_SOURCE_AWARE  */
    SAI_POLICER_ATTR_COLOR_SOURCE = 0x00000002,

    /** Committed burst size bytes/packets based on
     * SAI_POLICER_ATTR_METER_TYPE [uint64_t]*/
    SAI_POLICER_ATTR_CBS = 0x00000003,

    /** Committed information rate BPS/PPS based on
     * SAI_POLICER_ATTR_METER_TYPE [uint64_t]*/
    SAI_POLICER_ATTR_CIR = 0x00000004,

    /** Peak burst size bytes/packets based on
     * SAI_POLICER_ATTR_METER_TYPE [uint64_t]*/
    SAI_POLICER_ATTR_PBS = 0x00000005,

    /** Peak information rate BPS/PPS based on
     * SAI_POLICER_ATTR_METER_TYPE [uint64_t]
     * Mandatory only for SAI_POLICER_MODE_Tr_TCM */
    SAI_POLICER_ATTR_PIR = 0x00000006,

    /** Action to take for Green color packets
     * [sai_packet_action_t],
     * Default action SAI_PACKET_ACTION_FORWARD
     */
    SAI_POLICER_ATTR_GREEN_PACKET_ACTION = 0x00000007,

    /** Action to take for Yellow color packets
     * [sai_packet_action_t],
     * Default action SAI_PACKET_ACTION_FORWARD
     */
    SAI_POLICER_ATTR_YELLOW_PACKET_ACTION = 0x00000008,

    /** Action to take for RED color packets [sai_packet_action_t],
     * Default action SAI_PACKET_ACTION_FORWARD
     * For storm control action should be used as RED_PACKET_ACTION */
    SAI_POLICER_ATTR_RED_PACKET_ACTION = 0x00000009,

    /** Enable/disable counter [sai_s32_list_t of sai_policer_stat_counter_t].
     * Default[disabled], Modify List Needs full new set*/
    SAI_POLICER_ATTR_ENABLE_COUNTER_LIST = 0x0000000a,

    /* -- */
    /* Custom range base value */
    SAI_POLICER_ATTR_CUSTOM_RANGE_BASE = 0x10000000

} sai_policer_attr_t;



/** @brief Enum defining policer statistics */
typedef enum _sai_policer_stat_counter_t
{
    /** get/set packet count [uint64_t] */
    SAI_POLICER_STAT_PACKETS = 0x00000000,

    /** get/set byte count [uint64_t] */
    SAI_POLICER_STAT_ATTR_BYTES = 0x00000001,

    /** get/set green packet count [uint64_t] */
    SAI_POLICER_STAT_GREEN_PACKETS = 0x00000002,

    /** get/set green byte count [uint64_t] */
    SAI_POLICER_STAT_GREEN_BYTES = 0x00000003,

    /** get/set yellow packet count [uint64_t] */
    SAI_POLICER_STAT_YELLOW_PACKETS = 0x00000004,

    /** get/set yellow byte count [uint64_t] */
    SAI_POLICER_STAT_YELLOW_BYTES = 0x00000005,

    /** get/set red packet count [uint64_t] */
    SAI_POLICER_STAT_RED_PACKETS = 0x00000006,

    /** get/set red byte count [uint64_t] */
    SAI_POLICER_STAT_RED_BYTES = 0x00000007,

    /* -- */
    /* Custom range base value */
    SAI_POLICER_STAT_CUSTOM_RANGE_BASE = 0x10000000

} sai_policer_stat_counter_t;





/**
 * @brief Create Policer
 *
 * @param[out] policer_id - the policer id
 * @param[in] attr_count - number of attributes
 * @param[in] attr_list - array of attributes
 *
 * @return SAI_STATUS_SUCCESS on success
 *         Failure status code on error
 */
typedef sai_status_t (*sai_create_policer_fn)(
    _Out_ sai_object_id_t *policer_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list);


/**
 * @brief Delete policer
 *
 * @param[in] policer_id - Policer id
 *
 * @return  SAI_STATUS_SUCCESS on success
 *          Failure status code on error
 */
typedef sai_status_t (*sai_remove_policer_fn)(
    _In_ sai_object_id_t policer_id);



/**
 * @brief  Set Policer attribute
 *
 * @param[in] policer_id - Policer id
 * @param[in] attr - attribute
 *
 * @return SAI_STATUS_SUCCESS on success
 *         Failure status code on error
 */
typedef sai_status_t (*sai_set_policer_attribute_fn)(
    _In_ sai_object_id_t policer_id,
    _In_ const sai_attribute_t *attr
    );



/**
 * @brief  Get Policer attribute
 *
 * @param[in] policer_id - policer id
 * @param[in] attr_count - number of attributes
 * @param[inout] attr_list - array of attributes
 *
 * @return SAI_STATUS_SUCCESS on success
 *         Failure status code on error
 */
typedef sai_status_t (*sai_get_policer_attribute_fn)(
    _In_ sai_object_id_t policer_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );


/**
 * @brief  Get Policer Statistics
 *
 * @param[in] policer_id - policer id
 * @param[in] counter_ids - array of counter ids
 * @param[in] number_of_counters - number of counters in the array
 * @param[out] counters - array of resulting counter values.
 *
 * @return SAI_STATUS_SUCCESS on success
 *         Failure status code on error
 */
typedef sai_status_t (*sai_get_policer_stats_fn)(
    _In_ sai_object_id_t policer_id,
    _In_ const sai_policer_stat_counter_t *counter_ids,
    _In_ uint32_t number_of_counters,
    _Out_ uint64_t* counters
   );

/**
 * @brief Policer methods table retrieved with sai_api_query()
 */
typedef struct _sai_policer_api_t
{
    sai_create_policer_fn                 create_policer;
    sai_remove_policer_fn                 remove_policer;
    sai_set_policer_attribute_fn          set_policer_attribute;
    sai_get_policer_attribute_fn          get_policer_attribute;
    sai_get_policer_stats_fn              get_policer_statistics;
} sai_policer_api_t;

/**
 *\}
 */

#endif
