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
 * @file    saimpls.h
 *
 * @brief   This module defines SAI MPLS interface
 */

#if !defined (__SAIMPLS_H_)
#define __SAIMPLS_H_

#include <saitypes.h>

/**
 * @defgroup SAIMPLS SAI - MPLS specific API definitions
 *
 * @{
 */
typedef enum _sai_inseg_entry_psc_type_t
{
    /**
     * @brief EXP of MPLS label infers both TC and COLOR
     */
    SAI_INSEG_ENTRY_PSC_TYPE_ELSP,

    /**
     * @brief MPLS label infers TC and EXP of MPLS label infers COLOR
     */
    SAI_INSEG_ENTRY_PSC_TYPE_LLSP
} sai_inseg_entry_psc_type_t;

typedef enum _sai_inseg_entry_pop_ttl_mode_t
{
    /**
     * @brief Uniform mode
     *
     * TTL of inner header is computed based on TTL of outer header on pop.
     */
    SAI_INSEG_ENTRY_POP_TTL_MODE_UNIFORM,

    /**
     * @brief Pipe mode
     *
     * TTL of inner header is left unchanged on pop.
     */
    SAI_INSEG_ENTRY_POP_TTL_MODE_PIPE
} sai_inseg_entry_pop_ttl_mode_t;

typedef enum _sai_inseg_entry_pop_qos_mode_t
{
    /**
     * @brief Uniform mode
     *
     * DSCP or EXP of inner header is computed based on TC AND COLOR of outer header on pop.
     */
    SAI_INSEG_ENTRY_POP_QOS_MODE_UNIFORM,

    /**
     * @brief Uniform mode
     *
     * DSCP or EXP of inner header is left unchanged on pop.
     */
    SAI_INSEG_ENTRY_POP_QOS_MODE_PIPE
} sai_inseg_entry_pop_qos_mode_t;

/**
 * @brief Attribute Id for SAI in segment
 */
typedef enum _sai_inseg_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_INSEG_ENTRY_ATTR_START,

    /**
     * @brief Number of pops
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_INSEG_ENTRY_ATTR_NUM_OF_POP = SAI_INSEG_ENTRY_ATTR_START,

    /**
     * @brief Packet action
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_FORWARD
     */
    SAI_INSEG_ENTRY_ATTR_PACKET_ACTION,

    /**
     * @brief Packet priority for trap/log actions
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_INSEG_ENTRY_ATTR_TRAP_PRIORITY,

    /**
     * @brief The next hop id
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_NEXT_HOP, SAI_OBJECT_TYPE_NEXT_HOP_GROUP, SAI_OBJECT_TYPE_ROUTER_INTERFACE
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_INSEG_ENTRY_ATTR_NEXT_HOP_ID,

    /**
     * @brief Define PSC type for a label.
     *
     * Defines how to infer both TC and COLOR
     *
     * @type sai_inseg_entry_psc_type_t
     * @flags CREATE_AND_SET
     * @default SAI_INSEG_ENTRY_PSC_TYPE_ELSP
     */
    SAI_INSEG_ENTRY_ATTR_PSC_TYPE,

    /**
     * @brief TC for a label.
     *
     * Associate TC by a label (override TC provided by QOS MAP)
     * Mainly used for L-LSP tunnels, where label infers TC and EXP infers COLOR
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_INSEG_ENTRY_ATTR_PSC_TYPE == SAI_INSEG_ENTRY_PSC_TYPE_LLSP
     */
    SAI_INSEG_ENTRY_ATTR_QOS_TC,

    /**
     * @brief Enable EXP -> TC MAP on label.
     *
     * Associate TC by a QOS MAP
     * Mainly used for E-LSP tunnels, where EXP infers both TC and COLOR
     * Overrides SAI_SWITCH_ATTR_QOS_MPLS_EXP_TO_TC_MAP and SAI_PORT_ATTR_QOS_MPLS_EXP_TO_TC_MAP
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     * @validonly SAI_INSEG_ENTRY_ATTR_PSC_TYPE == SAI_INSEG_ENTRY_PSC_TYPE_ELSP
     */
    SAI_INSEG_ENTRY_ATTR_MPLS_EXP_TO_TC_MAP,

    /**
     * @brief Enable EXP -> COLOR MAP on label.
     *
     * Associate COLOR by a QOS MAP
     * Overrides SAI_SWITCH_ATTR_QOS_MPLS_EXP_TO_COLOR_MAP and SAI_PORT_ATTR_QOS_MPLS_EXP_TO_COLOR_MAP
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_INSEG_ENTRY_ATTR_MPLS_EXP_TO_COLOR_MAP,

    /**
     * @brief Define TTL setting for PHP or POP
     *
     * @type sai_inseg_entry_pop_ttl_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_INSEG_ENTRY_POP_TTL_MODE_UNIFORM
     */
    SAI_INSEG_ENTRY_ATTR_POP_TTL_MODE,

    /**
     * @brief Define QOS setting for PHP or POP
     *
     * @type sai_inseg_entry_pop_qos_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_INSEG_ENTRY_POP_QOS_MODE_UNIFORM
     */
    SAI_INSEG_ENTRY_ATTR_POP_QOS_MODE,

    /**
     * @brief End of attributes
     */
    SAI_INSEG_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_INSEG_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_INSEG_ENTRY_ATTR_CUSTOM_RANGE_END

} sai_inseg_entry_attr_t;

/**
 * @brief In segment entry
 */
typedef struct _sai_inseg_entry_t
{
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /**
     * @brief MPLS label
     */
    sai_label_id_t label;

} sai_inseg_entry_t;

/**
 * @brief Create In Segment entry
 *
 * @param[in] inseg_entry InSegment entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_inseg_entry_fn)(
        _In_ const sai_inseg_entry_t *inseg_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove In Segment entry
 *
 * @param[in] inseg_entry InSegment entry
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_inseg_entry_fn)(
        _In_ const sai_inseg_entry_t *inseg_entry);

/**
 * @brief Set In Segment attribute value
 *
 * @param[in] inseg_entry InSegment entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_inseg_entry_attribute_fn)(
        _In_ const sai_inseg_entry_t *inseg_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get In Segment attribute value
 *
 * @param[in] inseg_entry InSegment entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_inseg_entry_attribute_fn)(
        _In_ const sai_inseg_entry_t *inseg_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk create In Segment entry
 *
 * @param[in] object_count Number of objects to create
 * @param[in] inseg_entry List of object to create
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to create.
 * @param[in] attr_list List of attributes for every object.
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are created or
 * #SAI_STATUS_FAILURE when any of the objects fails to create. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_create_inseg_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_inseg_entry_t *inseg_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk remove In Segment entry
 *
 * @param[in] object_count Number of objects to remove
 * @param[in] inseg_entry List of objects to remove
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_remove_inseg_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_inseg_entry_t *inseg_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk set attribute on In Segment entry
 *
 * @param[in] object_count Number of objects to set attribute
 * @param[in] inseg_entry List of objects to set attribute
 * @param[in] attr_list List of attributes to set on objects, one attribute per object
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_set_inseg_entry_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_inseg_entry_t *inseg_entry,
        _In_ const sai_attribute_t *attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk get attribute on In Segment entry
 *
 * @param[in] object_count Number of objects to set attribute
 * @param[in] inseg_entry List of objects to set attribute
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get
 * @param[inout] attr_list List of attributes to set on objects, one attribute per object
 * @param[in] mode Bulk operation error handling mode
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_inseg_entry_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_inseg_entry_t *inseg_entry,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief MPLS methods table retrieved with sai_api_query()
 */
typedef struct _sai_mpls_api_t
{
    sai_create_inseg_entry_fn                      create_inseg_entry;
    sai_remove_inseg_entry_fn                      remove_inseg_entry;
    sai_set_inseg_entry_attribute_fn               set_inseg_entry_attribute;
    sai_get_inseg_entry_attribute_fn               get_inseg_entry_attribute;

    sai_bulk_create_inseg_entry_fn                 create_inseg_entries;
    sai_bulk_remove_inseg_entry_fn                 remove_inseg_entries;
    sai_bulk_set_inseg_entry_attribute_fn          set_inseg_entries_attribute;
    sai_bulk_get_inseg_entry_attribute_fn          get_inseg_entries_attribute;

} sai_mpls_api_t;

/**
 * @}
 */
#endif /** __SAIMPLS_H_ */
