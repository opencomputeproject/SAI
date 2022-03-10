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
 * @file    saiexperimentaldash.h
 *
 * @brief   This module defines SAI P4 extension  interface
 */

#if !defined (__SAIEXPERIMENTALDASH_H_)
#define __SAIEXPERIMENTALDASH_H_

#include <saitypes.h>

/**
 * @defgroup SAIEXPERIMENTALDASH SAI - Extension specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute data for #SAI_INBOUND_ROUTING_ENTRY_ATTR_ACTION
 */
typedef enum _sai_inbound_routing_entry_action_t
{
    SAI_INBOUND_ROUTING_ENTRY_ACTION_VXLAN_DECAP,

    SAI_INBOUND_ROUTING_ENTRY_ACTION_VXLAN_DECAP_PA_VALIDATE,

    SAI_INBOUND_ROUTING_ENTRY_ACTION_DENY,

} sai_inbound_routing_entry_action_t;

/**
 * @brief Attribute data for #SAI_PA_VALIDATION_ENTRY_ATTR_ACTION
 */
typedef enum _sai_pa_validation_entry_action_t
{
    SAI_PA_VALIDATION_ENTRY_ACTION_PERMIT,

    SAI_PA_VALIDATION_ENTRY_ACTION_DENY,

} sai_pa_validation_entry_action_t;

/**
 * @brief Attribute data for #SAI_DASH_ACL_ATTR_ACTION
 */
typedef enum _sai_dash_acl_action_t
{
    SAI_DASH_ACL_ACTION_PERMIT,

    SAI_DASH_ACL_ACTION_PERMIT_AND_CONTINUE,

    SAI_DASH_ACL_ACTION_DENY,

    SAI_DASH_ACL_ACTION_DENY_AND_CONTINUE,

} sai_dash_acl_action_t;

/**
 * @brief Attribute data for #SAI_DASH_ACL_ATTR_STAGE
 */
typedef enum _sai_dash_acl_stage_t
{
    SAI_DASH_ACL_STAGE_OUTBOUND_ACL_STAGE1,

    SAI_DASH_ACL_STAGE_OUTBOUND_ACL_STAGE2,

    SAI_DASH_ACL_STAGE_OUTBOUND_ACL_STAGE3,

    SAI_DASH_ACL_STAGE_INBOUND_ACL_STAGE1,

    SAI_DASH_ACL_STAGE_INBOUND_ACL_STAGE2,

    SAI_DASH_ACL_STAGE_INBOUND_ACL_STAGE3,

} sai_dash_acl_stage_t;

/**
 * @brief Entry for direction_lookup_entry
 */
typedef struct _sai_direction_lookup_entry_t
{
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /**
     * @brief Exact matched key VNI
     */
    sai_uint32_t VNI;

} sai_direction_lookup_entry_t;

/**
 * @brief Attribute ID for direction_lookup_entry
 */
typedef enum _sai_direction_lookup_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_DIRECTION_LOOKUP_ENTRY_ATTR_START,

    /**
     * @brief Action set_direction parameter DIRECTION
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_DIRECTION_LOOKUP_ENTRY_ATTR_DIRECTION = SAI_DIRECTION_LOOKUP_ENTRY_ATTR_START,

    /**
     * @brief End of attributes
     */
    SAI_DIRECTION_LOOKUP_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_DIRECTION_LOOKUP_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_DIRECTION_LOOKUP_ENTRY_ATTR_CUSTOM_RANGE_END,

} sai_direction_lookup_entry_attr_t;

/**
 * @brief Entry for eni_lookup_to_vm_entry
 */
typedef struct _sai_eni_lookup_to_vm_entry_t
{
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /**
     * @brief Exact matched key dst_mac
     */
    sai_mac_t dst_mac;

} sai_eni_lookup_to_vm_entry_t;

/**
 * @brief Attribute ID for eni_lookup_to_vm_entry
 */
typedef enum _sai_eni_lookup_to_vm_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_ENI_LOOKUP_TO_VM_ENTRY_ATTR_START,

    /**
     * @brief Action set_eni parameter ENI
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_ENI_LOOKUP_TO_VM_ENTRY_ATTR_ENI = SAI_ENI_LOOKUP_TO_VM_ENTRY_ATTR_START,

    /**
     * @brief End of attributes
     */
    SAI_ENI_LOOKUP_TO_VM_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_ENI_LOOKUP_TO_VM_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_ENI_LOOKUP_TO_VM_ENTRY_ATTR_CUSTOM_RANGE_END,

} sai_eni_lookup_to_vm_entry_attr_t;

/**
 * @brief Entry for inbound_routing_entry
 */
typedef struct _sai_inbound_routing_entry_t
{
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /**
     * @brief Exact matched key VNI
     */
    sai_uint32_t VNI;

} sai_inbound_routing_entry_t;

/**
 * @brief Attribute ID for inbound_routing_entry
 */
typedef enum _sai_inbound_routing_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_INBOUND_ROUTING_ENTRY_ATTR_START,

    /**
     * @brief Action
     *
     * @type sai_inbound_routing_entry_action_t
     * @flags CREATE_AND_SET
     * @default SAI_INBOUND_ROUTING_ENTRY_ACTION_VXLAN_DECAP
     */
    SAI_INBOUND_ROUTING_ENTRY_ATTR_ACTION = SAI_INBOUND_ROUTING_ENTRY_ATTR_START,

    /**
     * @brief End of attributes
     */
    SAI_INBOUND_ROUTING_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_INBOUND_ROUTING_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_INBOUND_ROUTING_ENTRY_ATTR_CUSTOM_RANGE_END,

} sai_inbound_routing_entry_attr_t;

/**
 * @brief Entry for pa_validation_entry
 */
typedef struct _sai_pa_validation_entry_t
{
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /**
     * @brief Exact matched key ENI
     */
    sai_uint8_t ENI;

    /**
     * @brief Exact matched key sip
     */
    sai_ip_address_t sip;

    /**
     * @brief Exact matched key VNI
     */
    sai_uint32_t VNI;

} sai_pa_validation_entry_t;

/**
 * @brief Attribute ID for pa_validation_entry
 */
typedef enum _sai_pa_validation_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PA_VALIDATION_ENTRY_ATTR_START,

    /**
     * @brief Action
     *
     * @type sai_pa_validation_entry_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PA_VALIDATION_ENTRY_ACTION_PERMIT
     */
    SAI_PA_VALIDATION_ENTRY_ATTR_ACTION = SAI_PA_VALIDATION_ENTRY_ATTR_START,

    /**
     * @brief End of attributes
     */
    SAI_PA_VALIDATION_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_PA_VALIDATION_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PA_VALIDATION_ENTRY_ATTR_CUSTOM_RANGE_END,

} sai_pa_validation_entry_attr_t;

/**
 * @brief Entry for outbound_eni_lookup_from_vm_entry
 */
typedef struct _sai_outbound_eni_lookup_from_vm_entry_t
{
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /**
     * @brief Exact matched key src_mac
     */
    sai_mac_t src_mac;

} sai_outbound_eni_lookup_from_vm_entry_t;

/**
 * @brief Attribute ID for outbound_eni_lookup_from_vm_entry
 */
typedef enum _sai_outbound_eni_lookup_from_vm_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_OUTBOUND_ENI_LOOKUP_FROM_VM_ENTRY_ATTR_START,

    /**
     * @brief Action set_eni parameter ENI
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_OUTBOUND_ENI_LOOKUP_FROM_VM_ENTRY_ATTR_ENI = SAI_OUTBOUND_ENI_LOOKUP_FROM_VM_ENTRY_ATTR_START,

    /**
     * @brief End of attributes
     */
    SAI_OUTBOUND_ENI_LOOKUP_FROM_VM_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_OUTBOUND_ENI_LOOKUP_FROM_VM_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_OUTBOUND_ENI_LOOKUP_FROM_VM_ENTRY_ATTR_CUSTOM_RANGE_END,

} sai_outbound_eni_lookup_from_vm_entry_attr_t;

/**
 * @brief Entry for outbound_eni_to_vni_entry
 */
typedef struct _sai_outbound_eni_to_vni_entry_t
{
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /**
     * @brief Exact matched key ENI
     */
    sai_uint8_t ENI;

} sai_outbound_eni_to_vni_entry_t;

/**
 * @brief Attribute ID for outbound_eni_to_vni_entry
 */
typedef enum _sai_outbound_eni_to_vni_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_OUTBOUND_ENI_TO_VNI_ENTRY_ATTR_START,

    /**
     * @brief Action set_vni parameter VNI
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_OUTBOUND_ENI_TO_VNI_ENTRY_ATTR_VNI = SAI_OUTBOUND_ENI_TO_VNI_ENTRY_ATTR_START,

    /**
     * @brief End of attributes
     */
    SAI_OUTBOUND_ENI_TO_VNI_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_OUTBOUND_ENI_TO_VNI_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_OUTBOUND_ENI_TO_VNI_ENTRY_ATTR_CUSTOM_RANGE_END,

} sai_outbound_eni_to_vni_entry_attr_t;

/**
 * @brief Attribute ID for dash_acl
 */
typedef enum _sai_dash_acl_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_DASH_ACL_ATTR_START,

    /**
     * @brief Action
     *
     * @type sai_dash_acl_action_t
     * @flags CREATE_AND_SET
     * @default SAI_DASH_ACL_ACTION_PERMIT
     */
    SAI_DASH_ACL_ATTR_ACTION = SAI_DASH_ACL_ATTR_START,

    /**
     * @brief Exact matched key ENI
     *
     * @type sai_uint8_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_DASH_ACL_ATTR_ENI,

    /**
     * @brief List matched key dip
     *
     * @type sai_ip_address_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_DASH_ACL_ATTR_DIP,

    /**
     * @brief List matched key sip
     *
     * @type sai_ip_address_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_DASH_ACL_ATTR_SIP,

    /**
     * @brief List matched key protocol
     *
     * @type sai_u8_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_DASH_ACL_ATTR_PROTOCOL,

    /**
     * @brief Range_list matched key src_port
     *
     * @type sai_u16_range_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_DASH_ACL_ATTR_SRC_PORT,

    /**
     * @brief Range_list matched key dst_port
     *
     * @type sai_u16_range_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_DASH_ACL_ATTR_DST_PORT,

    /**
     * @brief Stage
     *
     * @type sai_dash_acl_stage_t
     * @flags CREATE_AND_SET
     * @default SAI_DASH_ACL_STAGE_OUTBOUND_ACL_STAGE1
     */
    SAI_DASH_ACL_ATTR_STAGE,

    /**
     * @brief End of attributes
     */
    SAI_DASH_ACL_ATTR_END,

    /** Custom range base value */
    SAI_DASH_ACL_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_DASH_ACL_ATTR_CUSTOM_RANGE_END,

} sai_dash_acl_attr_t;

/**
 * @brief Entry for outbound_routing_entry
 */
typedef struct _sai_outbound_routing_entry_t
{
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /**
     * @brief Exact matched key ENI
     */
    sai_uint8_t ENI;

    /**
     * @brief LPM matched key destination
     */
    sai_ip_prefix_t destination;

} sai_outbound_routing_entry_t;

/**
 * @brief Attribute ID for outbound_routing_entry
 */
typedef enum _sai_outbound_routing_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_OUTBOUND_ROUTING_ENTRY_ATTR_START,

    /**
     * @brief Action route_vnet parameter DEST_VNET_VNI
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_OUTBOUND_ROUTING_ENTRY_ATTR_DEST_VNET_VNI = SAI_OUTBOUND_ROUTING_ENTRY_ATTR_START,

    /**
     * @brief End of attributes
     */
    SAI_OUTBOUND_ROUTING_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_OUTBOUND_ROUTING_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_OUTBOUND_ROUTING_ENTRY_ATTR_CUSTOM_RANGE_END,

} sai_outbound_routing_entry_attr_t;

/**
 * @brief Entry for outbound_ca_to_pa_entry
 */
typedef struct _sai_outbound_ca_to_pa_entry_t
{
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /**
     * @brief Exact matched key dest_vni
     */
    sai_uint8_t dest_vni;

    /**
     * @brief Exact matched key dip
     */
    sai_ip_address_t dip;

} sai_outbound_ca_to_pa_entry_t;

/**
 * @brief Attribute ID for outbound_ca_to_pa_entry
 */
typedef enum _sai_outbound_ca_to_pa_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_START,

    /**
     * @brief Action set_tunnel_mapping parameter UNDERLAY_DIP
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_UNDERLAY_DIP = SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_START,

    /**
     * @brief Action set_tunnel_mapping parameter OVERLAY_DMAC
     *
     * @type sai_mac_t
     * @flags CREATE_AND_SET
     * @default 0:0:0:0:0:0
     */
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_OVERLAY_DMAC,

    /**
     * @brief Action set_tunnel_mapping parameter USE_DST_VNI
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_USE_DST_VNI,

    /**
     * @brief End of attributes
     */
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_OUTBOUND_CA_TO_PA_ENTRY_ATTR_CUSTOM_RANGE_END,

} sai_outbound_ca_to_pa_entry_attr_t;

/**
 * @brief Entry for inbound_eni_to_vm_entry
 */
typedef struct _sai_inbound_eni_to_vm_entry_t
{
    /**
     * @brief Switch ID
     *
     * @objects SAI_OBJECT_TYPE_SWITCH
     */
    sai_object_id_t switch_id;

    /**
     * @brief Exact matched key ENI
     */
    sai_uint8_t ENI;

} sai_inbound_eni_to_vm_entry_t;

/**
 * @brief Attribute ID for inbound_eni_to_vm_entry
 */
typedef enum _sai_inbound_eni_to_vm_entry_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_INBOUND_ENI_TO_VM_ENTRY_ATTR_START,

    /**
     * @brief Action set_vm_id parameter VM_ID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     */
    SAI_INBOUND_ENI_TO_VM_ENTRY_ATTR_VM_ID = SAI_INBOUND_ENI_TO_VM_ENTRY_ATTR_START,

    /**
     * @brief End of attributes
     */
    SAI_INBOUND_ENI_TO_VM_ENTRY_ATTR_END,

    /** Custom range base value */
    SAI_INBOUND_ENI_TO_VM_ENTRY_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_INBOUND_ENI_TO_VM_ENTRY_ATTR_CUSTOM_RANGE_END,

} sai_inbound_eni_to_vm_entry_attr_t;

/**
 * @brief Attribute ID for inbound_vm
 */
typedef enum _sai_inbound_vm_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_INBOUND_VM_ATTR_START,

    /**
     * @brief Action set_vm_attributes parameter UNDERLAY_DMAC
     *
     * @type sai_mac_t
     * @flags CREATE_AND_SET
     * @default 0:0:0:0:0:0
     */
    SAI_INBOUND_VM_ATTR_UNDERLAY_DMAC = SAI_INBOUND_VM_ATTR_START,

    /**
     * @brief Action set_vm_attributes parameter UNDERLAY_DIP
     *
     * @type sai_ip_address_t
     * @flags CREATE_AND_SET
     * @default 0.0.0.0
     */
    SAI_INBOUND_VM_ATTR_UNDERLAY_DIP,

    /**
     * @brief Action set_vm_attributes parameter VNI
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_INBOUND_VM_ATTR_VNI,

    /**
     * @brief End of attributes
     */
    SAI_INBOUND_VM_ATTR_END,

    /** Custom range base value */
    SAI_INBOUND_VM_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_INBOUND_VM_ATTR_CUSTOM_RANGE_END,

} sai_inbound_vm_attr_t;

/**
 * @brief Create direction_lookup_entry
 *
 * @param[in] direction_lookup_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_direction_lookup_entry_fn)(
        _In_ const sai_direction_lookup_entry_t *direction_lookup_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove direction_lookup_entry
 *
 * @param[in] direction_lookup_entry Entry
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_direction_lookup_entry_fn)(
        _In_ const sai_direction_lookup_entry_t *direction_lookup_entry);

/**
 * @brief Set attribute for direction_lookup_entry
 *
 * @param[in] direction_lookup_entry Entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_direction_lookup_entry_attribute_fn)(
        _In_ const sai_direction_lookup_entry_t *direction_lookup_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for direction_lookup_entry
 *
 * @param[in] direction_lookup_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_direction_lookup_entry_attribute_fn)(
        _In_ const sai_direction_lookup_entry_t *direction_lookup_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk create direction_lookup_entry
 *
 * @param[in] object_count Number of objects to create
 * @param[in] direction_lookup_entry List of object to create
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
typedef sai_status_t (*sai_bulk_create_direction_lookup_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_direction_lookup_entry_t *direction_lookup_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk remove direction_lookup_entry
 *
 * @param[in] object_count Number of objects to remove
 * @param[in] direction_lookup_entry List of objects to remove
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_remove_direction_lookup_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_direction_lookup_entry_t *direction_lookup_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Create eni_lookup_to_vm_entry
 *
 * @param[in] eni_lookup_to_vm_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_eni_lookup_to_vm_entry_fn)(
        _In_ const sai_eni_lookup_to_vm_entry_t *eni_lookup_to_vm_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove eni_lookup_to_vm_entry
 *
 * @param[in] eni_lookup_to_vm_entry Entry
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_eni_lookup_to_vm_entry_fn)(
        _In_ const sai_eni_lookup_to_vm_entry_t *eni_lookup_to_vm_entry);

/**
 * @brief Set attribute for eni_lookup_to_vm_entry
 *
 * @param[in] eni_lookup_to_vm_entry Entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_eni_lookup_to_vm_entry_attribute_fn)(
        _In_ const sai_eni_lookup_to_vm_entry_t *eni_lookup_to_vm_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for eni_lookup_to_vm_entry
 *
 * @param[in] eni_lookup_to_vm_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_eni_lookup_to_vm_entry_attribute_fn)(
        _In_ const sai_eni_lookup_to_vm_entry_t *eni_lookup_to_vm_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk create eni_lookup_to_vm_entry
 *
 * @param[in] object_count Number of objects to create
 * @param[in] eni_lookup_to_vm_entry List of object to create
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
typedef sai_status_t (*sai_bulk_create_eni_lookup_to_vm_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_eni_lookup_to_vm_entry_t *eni_lookup_to_vm_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk remove eni_lookup_to_vm_entry
 *
 * @param[in] object_count Number of objects to remove
 * @param[in] eni_lookup_to_vm_entry List of objects to remove
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_remove_eni_lookup_to_vm_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_eni_lookup_to_vm_entry_t *eni_lookup_to_vm_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Create inbound_routing_entry
 *
 * @param[in] inbound_routing_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_inbound_routing_entry_fn)(
        _In_ const sai_inbound_routing_entry_t *inbound_routing_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove inbound_routing_entry
 *
 * @param[in] inbound_routing_entry Entry
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_inbound_routing_entry_fn)(
        _In_ const sai_inbound_routing_entry_t *inbound_routing_entry);

/**
 * @brief Set attribute for inbound_routing_entry
 *
 * @param[in] inbound_routing_entry Entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_inbound_routing_entry_attribute_fn)(
        _In_ const sai_inbound_routing_entry_t *inbound_routing_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for inbound_routing_entry
 *
 * @param[in] inbound_routing_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_inbound_routing_entry_attribute_fn)(
        _In_ const sai_inbound_routing_entry_t *inbound_routing_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk create inbound_routing_entry
 *
 * @param[in] object_count Number of objects to create
 * @param[in] inbound_routing_entry List of object to create
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
typedef sai_status_t (*sai_bulk_create_inbound_routing_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_inbound_routing_entry_t *inbound_routing_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk remove inbound_routing_entry
 *
 * @param[in] object_count Number of objects to remove
 * @param[in] inbound_routing_entry List of objects to remove
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_remove_inbound_routing_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_inbound_routing_entry_t *inbound_routing_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Create pa_validation_entry
 *
 * @param[in] pa_validation_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_pa_validation_entry_fn)(
        _In_ const sai_pa_validation_entry_t *pa_validation_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove pa_validation_entry
 *
 * @param[in] pa_validation_entry Entry
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_pa_validation_entry_fn)(
        _In_ const sai_pa_validation_entry_t *pa_validation_entry);

/**
 * @brief Set attribute for pa_validation_entry
 *
 * @param[in] pa_validation_entry Entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_pa_validation_entry_attribute_fn)(
        _In_ const sai_pa_validation_entry_t *pa_validation_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for pa_validation_entry
 *
 * @param[in] pa_validation_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_pa_validation_entry_attribute_fn)(
        _In_ const sai_pa_validation_entry_t *pa_validation_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk create pa_validation_entry
 *
 * @param[in] object_count Number of objects to create
 * @param[in] pa_validation_entry List of object to create
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
typedef sai_status_t (*sai_bulk_create_pa_validation_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_pa_validation_entry_t *pa_validation_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk remove pa_validation_entry
 *
 * @param[in] object_count Number of objects to remove
 * @param[in] pa_validation_entry List of objects to remove
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_remove_pa_validation_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_pa_validation_entry_t *pa_validation_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Create outbound_eni_lookup_from_vm_entry
 *
 * @param[in] outbound_eni_lookup_from_vm_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_outbound_eni_lookup_from_vm_entry_fn)(
        _In_ const sai_outbound_eni_lookup_from_vm_entry_t *outbound_eni_lookup_from_vm_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove outbound_eni_lookup_from_vm_entry
 *
 * @param[in] outbound_eni_lookup_from_vm_entry Entry
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_outbound_eni_lookup_from_vm_entry_fn)(
        _In_ const sai_outbound_eni_lookup_from_vm_entry_t *outbound_eni_lookup_from_vm_entry);

/**
 * @brief Set attribute for outbound_eni_lookup_from_vm_entry
 *
 * @param[in] outbound_eni_lookup_from_vm_entry Entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_outbound_eni_lookup_from_vm_entry_attribute_fn)(
        _In_ const sai_outbound_eni_lookup_from_vm_entry_t *outbound_eni_lookup_from_vm_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for outbound_eni_lookup_from_vm_entry
 *
 * @param[in] outbound_eni_lookup_from_vm_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_outbound_eni_lookup_from_vm_entry_attribute_fn)(
        _In_ const sai_outbound_eni_lookup_from_vm_entry_t *outbound_eni_lookup_from_vm_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk create outbound_eni_lookup_from_vm_entry
 *
 * @param[in] object_count Number of objects to create
 * @param[in] outbound_eni_lookup_from_vm_entry List of object to create
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
typedef sai_status_t (*sai_bulk_create_outbound_eni_lookup_from_vm_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_outbound_eni_lookup_from_vm_entry_t *outbound_eni_lookup_from_vm_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk remove outbound_eni_lookup_from_vm_entry
 *
 * @param[in] object_count Number of objects to remove
 * @param[in] outbound_eni_lookup_from_vm_entry List of objects to remove
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_remove_outbound_eni_lookup_from_vm_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_outbound_eni_lookup_from_vm_entry_t *outbound_eni_lookup_from_vm_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Create outbound_eni_to_vni_entry
 *
 * @param[in] outbound_eni_to_vni_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_outbound_eni_to_vni_entry_fn)(
        _In_ const sai_outbound_eni_to_vni_entry_t *outbound_eni_to_vni_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove outbound_eni_to_vni_entry
 *
 * @param[in] outbound_eni_to_vni_entry Entry
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_outbound_eni_to_vni_entry_fn)(
        _In_ const sai_outbound_eni_to_vni_entry_t *outbound_eni_to_vni_entry);

/**
 * @brief Set attribute for outbound_eni_to_vni_entry
 *
 * @param[in] outbound_eni_to_vni_entry Entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_outbound_eni_to_vni_entry_attribute_fn)(
        _In_ const sai_outbound_eni_to_vni_entry_t *outbound_eni_to_vni_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for outbound_eni_to_vni_entry
 *
 * @param[in] outbound_eni_to_vni_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_outbound_eni_to_vni_entry_attribute_fn)(
        _In_ const sai_outbound_eni_to_vni_entry_t *outbound_eni_to_vni_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk create outbound_eni_to_vni_entry
 *
 * @param[in] object_count Number of objects to create
 * @param[in] outbound_eni_to_vni_entry List of object to create
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
typedef sai_status_t (*sai_bulk_create_outbound_eni_to_vni_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_outbound_eni_to_vni_entry_t *outbound_eni_to_vni_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk remove outbound_eni_to_vni_entry
 *
 * @param[in] object_count Number of objects to remove
 * @param[in] outbound_eni_to_vni_entry List of objects to remove
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_remove_outbound_eni_to_vni_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_outbound_eni_to_vni_entry_t *outbound_eni_to_vni_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Create dash_acl
 *
 * @param[out] dash_acl_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_dash_acl_fn)(
        _Out_ sai_object_id_t *dash_acl_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove dash_acl
 *
 * @param[in] dash_acl_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_dash_acl_fn)(
        _In_ sai_object_id_t dash_acl_id);

/**
 * @brief Set attribute for dash_acl
 *
 * @param[in] dash_acl_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_dash_acl_attribute_fn)(
        _In_ sai_object_id_t dash_acl_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for dash_acl
 *
 * @param[in] dash_acl_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_dash_acl_attribute_fn)(
        _In_ sai_object_id_t dash_acl_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Create outbound_routing_entry
 *
 * @param[in] outbound_routing_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_outbound_routing_entry_fn)(
        _In_ const sai_outbound_routing_entry_t *outbound_routing_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove outbound_routing_entry
 *
 * @param[in] outbound_routing_entry Entry
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_outbound_routing_entry_fn)(
        _In_ const sai_outbound_routing_entry_t *outbound_routing_entry);

/**
 * @brief Set attribute for outbound_routing_entry
 *
 * @param[in] outbound_routing_entry Entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_outbound_routing_entry_attribute_fn)(
        _In_ const sai_outbound_routing_entry_t *outbound_routing_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for outbound_routing_entry
 *
 * @param[in] outbound_routing_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_outbound_routing_entry_attribute_fn)(
        _In_ const sai_outbound_routing_entry_t *outbound_routing_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk create outbound_routing_entry
 *
 * @param[in] object_count Number of objects to create
 * @param[in] outbound_routing_entry List of object to create
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
typedef sai_status_t (*sai_bulk_create_outbound_routing_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_outbound_routing_entry_t *outbound_routing_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk remove outbound_routing_entry
 *
 * @param[in] object_count Number of objects to remove
 * @param[in] outbound_routing_entry List of objects to remove
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_remove_outbound_routing_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_outbound_routing_entry_t *outbound_routing_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Create outbound_ca_to_pa_entry
 *
 * @param[in] outbound_ca_to_pa_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_outbound_ca_to_pa_entry_fn)(
        _In_ const sai_outbound_ca_to_pa_entry_t *outbound_ca_to_pa_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove outbound_ca_to_pa_entry
 *
 * @param[in] outbound_ca_to_pa_entry Entry
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_outbound_ca_to_pa_entry_fn)(
        _In_ const sai_outbound_ca_to_pa_entry_t *outbound_ca_to_pa_entry);

/**
 * @brief Set attribute for outbound_ca_to_pa_entry
 *
 * @param[in] outbound_ca_to_pa_entry Entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_outbound_ca_to_pa_entry_attribute_fn)(
        _In_ const sai_outbound_ca_to_pa_entry_t *outbound_ca_to_pa_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for outbound_ca_to_pa_entry
 *
 * @param[in] outbound_ca_to_pa_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_outbound_ca_to_pa_entry_attribute_fn)(
        _In_ const sai_outbound_ca_to_pa_entry_t *outbound_ca_to_pa_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk create outbound_ca_to_pa_entry
 *
 * @param[in] object_count Number of objects to create
 * @param[in] outbound_ca_to_pa_entry List of object to create
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
typedef sai_status_t (*sai_bulk_create_outbound_ca_to_pa_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_outbound_ca_to_pa_entry_t *outbound_ca_to_pa_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk remove outbound_ca_to_pa_entry
 *
 * @param[in] object_count Number of objects to remove
 * @param[in] outbound_ca_to_pa_entry List of objects to remove
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_remove_outbound_ca_to_pa_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_outbound_ca_to_pa_entry_t *outbound_ca_to_pa_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Create inbound_eni_to_vm_entry
 *
 * @param[in] inbound_eni_to_vm_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_inbound_eni_to_vm_entry_fn)(
        _In_ const sai_inbound_eni_to_vm_entry_t *inbound_eni_to_vm_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove inbound_eni_to_vm_entry
 *
 * @param[in] inbound_eni_to_vm_entry Entry
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_inbound_eni_to_vm_entry_fn)(
        _In_ const sai_inbound_eni_to_vm_entry_t *inbound_eni_to_vm_entry);

/**
 * @brief Set attribute for inbound_eni_to_vm_entry
 *
 * @param[in] inbound_eni_to_vm_entry Entry
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_inbound_eni_to_vm_entry_attribute_fn)(
        _In_ const sai_inbound_eni_to_vm_entry_t *inbound_eni_to_vm_entry,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for inbound_eni_to_vm_entry
 *
 * @param[in] inbound_eni_to_vm_entry Entry
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_inbound_eni_to_vm_entry_attribute_fn)(
        _In_ const sai_inbound_eni_to_vm_entry_t *inbound_eni_to_vm_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Bulk create inbound_eni_to_vm_entry
 *
 * @param[in] object_count Number of objects to create
 * @param[in] inbound_eni_to_vm_entry List of object to create
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
typedef sai_status_t (*sai_bulk_create_inbound_eni_to_vm_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_inbound_eni_to_vm_entry_t *inbound_eni_to_vm_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk remove inbound_eni_to_vm_entry
 *
 * @param[in] object_count Number of objects to remove
 * @param[in] inbound_eni_to_vm_entry List of objects to remove
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_remove_inbound_eni_to_vm_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_inbound_eni_to_vm_entry_t *inbound_eni_to_vm_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Create inbound_vm
 *
 * @param[out] inbound_vm_id Entry id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_inbound_vm_fn)(
        _Out_ sai_object_id_t *inbound_vm_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove inbound_vm
 *
 * @param[in] inbound_vm_id Entry id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_inbound_vm_fn)(
        _In_ sai_object_id_t inbound_vm_id);

/**
 * @brief Set attribute for inbound_vm
 *
 * @param[in] inbound_vm_id Entry id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_inbound_vm_attribute_fn)(
        _In_ sai_object_id_t inbound_vm_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get attribute for inbound_vm
 *
 * @param[in] inbound_vm_id Entry id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_inbound_vm_attribute_fn)(
        _In_ sai_object_id_t inbound_vm_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

typedef struct _sai_dash_api_t
{
    sai_create_direction_lookup_entry_fn                      create_direction_lookup_entry;
    sai_remove_direction_lookup_entry_fn                      remove_direction_lookup_entry;
    sai_set_direction_lookup_entry_attribute_fn               set_direction_lookup_entry_attribute;
    sai_get_direction_lookup_entry_attribute_fn               get_direction_lookup_entry_attribute;
    sai_bulk_create_direction_lookup_entry_fn                 create_direction_lookup_entries;
    sai_bulk_remove_direction_lookup_entry_fn                 remove_direction_lookup_entries;
    sai_create_eni_lookup_to_vm_entry_fn                      create_eni_lookup_to_vm_entry;
    sai_remove_eni_lookup_to_vm_entry_fn                      remove_eni_lookup_to_vm_entry;
    sai_set_eni_lookup_to_vm_entry_attribute_fn               set_eni_lookup_to_vm_entry_attribute;
    sai_get_eni_lookup_to_vm_entry_attribute_fn               get_eni_lookup_to_vm_entry_attribute;
    sai_bulk_create_eni_lookup_to_vm_entry_fn                 create_eni_lookup_to_vm_entries;
    sai_bulk_remove_eni_lookup_to_vm_entry_fn                 remove_eni_lookup_to_vm_entries;
    sai_create_inbound_routing_entry_fn                       create_inbound_routing_entry;
    sai_remove_inbound_routing_entry_fn                       remove_inbound_routing_entry;
    sai_set_inbound_routing_entry_attribute_fn                set_inbound_routing_entry_attribute;
    sai_get_inbound_routing_entry_attribute_fn                get_inbound_routing_entry_attribute;
    sai_bulk_create_inbound_routing_entry_fn                  create_inbound_routing_entries;
    sai_bulk_remove_inbound_routing_entry_fn                  remove_inbound_routing_entries;
    sai_create_pa_validation_entry_fn                         create_pa_validation_entry;
    sai_remove_pa_validation_entry_fn                         remove_pa_validation_entry;
    sai_set_pa_validation_entry_attribute_fn                  set_pa_validation_entry_attribute;
    sai_get_pa_validation_entry_attribute_fn                  get_pa_validation_entry_attribute;
    sai_bulk_create_pa_validation_entry_fn                    create_pa_validation_entries;
    sai_bulk_remove_pa_validation_entry_fn                    remove_pa_validation_entries;
    sai_create_outbound_eni_lookup_from_vm_entry_fn           create_outbound_eni_lookup_from_vm_entry;
    sai_remove_outbound_eni_lookup_from_vm_entry_fn           remove_outbound_eni_lookup_from_vm_entry;
    sai_set_outbound_eni_lookup_from_vm_entry_attribute_fn    set_outbound_eni_lookup_from_vm_entry_attribute;
    sai_get_outbound_eni_lookup_from_vm_entry_attribute_fn    get_outbound_eni_lookup_from_vm_entry_attribute;
    sai_bulk_create_outbound_eni_lookup_from_vm_entry_fn      create_outbound_eni_lookup_from_vm_entries;
    sai_bulk_remove_outbound_eni_lookup_from_vm_entry_fn      remove_outbound_eni_lookup_from_vm_entries;
    sai_create_outbound_eni_to_vni_entry_fn                   create_outbound_eni_to_vni_entry;
    sai_remove_outbound_eni_to_vni_entry_fn                   remove_outbound_eni_to_vni_entry;
    sai_set_outbound_eni_to_vni_entry_attribute_fn            set_outbound_eni_to_vni_entry_attribute;
    sai_get_outbound_eni_to_vni_entry_attribute_fn            get_outbound_eni_to_vni_entry_attribute;
    sai_bulk_create_outbound_eni_to_vni_entry_fn              create_outbound_eni_to_vni_entries;
    sai_bulk_remove_outbound_eni_to_vni_entry_fn              remove_outbound_eni_to_vni_entries;
    sai_create_dash_acl_fn                                    create_dash_acl;
    sai_remove_dash_acl_fn                                    remove_dash_acl;
    sai_set_dash_acl_attribute_fn                             set_dash_acl_attribute;
    sai_get_dash_acl_attribute_fn                             get_dash_acl_attribute;
    sai_bulk_object_create_fn                                 create_dash_acls;
    sai_bulk_object_remove_fn                                 remove_dash_acls;
    sai_create_outbound_routing_entry_fn                      create_outbound_routing_entry;
    sai_remove_outbound_routing_entry_fn                      remove_outbound_routing_entry;
    sai_set_outbound_routing_entry_attribute_fn               set_outbound_routing_entry_attribute;
    sai_get_outbound_routing_entry_attribute_fn               get_outbound_routing_entry_attribute;
    sai_bulk_create_outbound_routing_entry_fn                 create_outbound_routing_entries;
    sai_bulk_remove_outbound_routing_entry_fn                 remove_outbound_routing_entries;
    sai_create_outbound_ca_to_pa_entry_fn                     create_outbound_ca_to_pa_entry;
    sai_remove_outbound_ca_to_pa_entry_fn                     remove_outbound_ca_to_pa_entry;
    sai_set_outbound_ca_to_pa_entry_attribute_fn              set_outbound_ca_to_pa_entry_attribute;
    sai_get_outbound_ca_to_pa_entry_attribute_fn              get_outbound_ca_to_pa_entry_attribute;
    sai_bulk_create_outbound_ca_to_pa_entry_fn                create_outbound_ca_to_pa_entries;
    sai_bulk_remove_outbound_ca_to_pa_entry_fn                remove_outbound_ca_to_pa_entries;
    sai_create_inbound_eni_to_vm_entry_fn                     create_inbound_eni_to_vm_entry;
    sai_remove_inbound_eni_to_vm_entry_fn                     remove_inbound_eni_to_vm_entry;
    sai_set_inbound_eni_to_vm_entry_attribute_fn              set_inbound_eni_to_vm_entry_attribute;
    sai_get_inbound_eni_to_vm_entry_attribute_fn              get_inbound_eni_to_vm_entry_attribute;
    sai_bulk_create_inbound_eni_to_vm_entry_fn                create_inbound_eni_to_vm_entries;
    sai_bulk_remove_inbound_eni_to_vm_entry_fn                remove_inbound_eni_to_vm_entries;
    sai_create_inbound_vm_fn                                  create_inbound_vm;
    sai_remove_inbound_vm_fn                                  remove_inbound_vm;
    sai_set_inbound_vm_attribute_fn                           set_inbound_vm_attribute;
    sai_get_inbound_vm_attribute_fn                           get_inbound_vm_attribute;
    sai_bulk_object_create_fn                                 create_inbound_vms;
    sai_bulk_object_remove_fn                                 remove_inbound_vms;
} sai_dash_api_t;

/**
 * @}
 */
#endif /** __SAIEXPERIMENTALDASH_H_ */
