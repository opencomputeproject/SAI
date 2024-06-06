/**
 * Copyright (c) 2018 Microsoft Open Technologies, Inc.
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
 * @file    saiswitchextensions.h
 *
 * @brief   This module defines switch extensions of the Switch Abstraction Interface (SAI)
 */

#ifndef __SAISWITCHEXTENSIONS_H_
#define __SAISWITCHEXTENSIONS_H_

#include <saiswitch.h>
#include <saitypesextensions.h>

/**
 * @brief DASH capability HA scope level
 */
typedef enum _sai_dash_caps_ha_scope_level_t
{
    /** Card level HA scope */
    SAI_DASH_CAPS_HA_SCOPE_LEVEL_CARD,

    /** ENI level HA scope */
    SAI_DASH_CAPS_HA_SCOPE_LEVEL_ENI,
} sai_dash_caps_ha_scope_level_t;

/**
 * @brief HA set event type
 */
typedef enum _sai_ha_set_event_t
{
    /** Data plane channel goes up. */
    SAI_HA_SET_EVENT_DP_CHANNEL_UP,

    /** Data plane channel goes down. */
    SAI_HA_SET_EVENT_DP_CHANNEL_DOWN,

} sai_ha_set_event_t;

/**
 * @brief Notification data format received from SAI HA set callback
 */
typedef struct _sai_ha_set_event_data_t
{
    /** Event type */
    sai_ha_set_event_t event_type;

    /** HA set id */
    sai_object_id_t ha_set_id;

} sai_ha_set_event_data_t;

/**
 * @brief HA set event notification
 *
 * Passed as a parameter into sai_initialize_switch()
 *
 * @count data[count]
 *
 * @param[in] count Number of notifications
 * @param[in] data Array of HA set events
 */
typedef void (*sai_ha_set_event_notification_fn)(
        _In_ uint32_t count,
        _In_ const sai_ha_set_event_data_t *data);

/**
 * @brief HA scope event type
 */
typedef enum _sai_ha_scope_event_t
{
    /** HA scope state changed */
    SAI_HA_SCOPE_EVENT_STATE_CHANGED,

    /** Flow reconcile is needed */
    SAI_HA_SCOPE_EVENT_FLOW_RECONCILE_NEEDED,
} sai_ha_scope_event_t;

/**
 * @brief Notification data format received from SAI HA scope callback
 */
typedef struct _sai_ha_scope_event_data_t
{
    /** Event type */
    sai_ha_scope_event_t event_type;

    /** HA scope id */
    sai_object_id_t ha_scope_id;

    /** HA role */
    sai_dash_ha_role_t ha_role;

    /** Flow version */
    sai_uint32_t flow_version;

} sai_ha_scope_event_data_t;

/**
 * @brief HA scope event notification
 *
 * Passed as a parameter into sai_initialize_switch()
 *
 * @count data[count]
 *
 * @param[in] count Number of notifications
 * @param[in] data Array of HA scope events
 */
typedef void (*sai_ha_scope_event_notification_fn)(
        _In_ uint32_t count,
        _In_ const sai_ha_scope_event_data_t *data);

/**
 * @brief SAI switch attribute extensions.
 *
 * @flags free
 */
typedef enum _sai_switch_attr_extensions_t
{
    SAI_SWITCH_ATTR_EXTENSIONS_RANGE_START = 0x20000000,

    /**
     * @brief Maximum number of meter buckets per ENI.
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_DASH_CAPS_MAX_METER_BUCKET_COUNT_PER_ENI = SAI_SWITCH_ATTR_EXTENSIONS_RANGE_START,

    /**
     * @brief DASH capability HA scope level.
     *
     * It indicates on which level HA scope can be supported, such as ENI or Card.
     *
     * @type sai_dash_caps_ha_scope_level_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_DASH_CAPS_HA_SCOPE_LEVEL,

    /**
     * @brief DASH capability HA owner needed.
     *
     * If true, the DASH host need to own driving the HA state machine, otherwise the DASH
     * implementation can drive the HA state machine by itself.
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_DASH_CAPS_HA_OWNER_NEEDED,

    /**
     * @brief DASH HA set event notification
     *
     * Use sai_ha_set_event_notification_fn as notification function.
     *
     * @type sai_pointer_t sai_ha_set_event_notification_fn
     * @flags CREATE_AND_SET
     * @default NULL
     */
    SAI_SWITCH_ATTR_HA_SET_EVENT_NOTIFY,

    /**
     * @brief DASH HA set scope notification
     *
     * Use sai_ha_scope_event_notification_fn as notification function.
     *
     * @type sai_pointer_t sai_ha_scope_event_notification_fn
     * @flags CREATE_AND_SET
     * @default NULL
     */
    SAI_SWITCH_ATTR_HA_SCOPE_EVENT_NOTIFY,

    SAI_SWITCH_ATTR_EXTENSIONS_RANGE_END

} sai_switch_attr_extensions_t;

#endif /* __SAISWITCHEXTENSIONS_H_ */
