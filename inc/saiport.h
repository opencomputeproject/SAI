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
 *    FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 *    Microsoft would like to thank the following companies for their review and
 *    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
 *    Dell Products, L.P., Facebook, Inc
 *
 * @file    saiport.h
 *
 * @brief   This module defines SAI Port interface
 */

#if !defined (__SAIPORT_H_)
#define __SAIPORT_H_

#include <saitypes.h>

/**
 * @defgroup SAIPORT SAI - Port specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute data for #SAI_PORT_ATTR_TYPE
 */
typedef enum _sai_port_type_t
{
    /** Actual port. N.B. Different from the physical port. */
    SAI_PORT_TYPE_LOGICAL,

    /** CPU Port */
    SAI_PORT_TYPE_CPU,

} sai_port_type_t;

/**
* @brief Attribute data for #SAI_PORT_ATTR_BIND_MODE
*/
typedef enum _sai_port_bind_mode_t
{
    /** Port */
    SAI_PORT_BIND_MODE_PORT,

    /** Sub port */
    SAI_PORT_BIND_MODE_SUB_PORT,

} sai_port_bind_mode_t;

/**
 * @brief Attribute data for #SAI_PORT_ATTR_OPER_STATUS
 */
typedef enum _sai_port_oper_status_t
{
    /** Unknown */
    SAI_PORT_OPER_STATUS_UNKNOWN,

    /** Up */
    SAI_PORT_OPER_STATUS_UP,

    /** Down */
    SAI_PORT_OPER_STATUS_DOWN,

    /** Test Running */
    SAI_PORT_OPER_STATUS_TESTING,

    /** Not Present */
    SAI_PORT_OPER_STATUS_NOT_PRESENT

} sai_port_oper_status_t;

/**
 * @brief Defines the operational status of the port
 */
typedef struct _sai_port_oper_status_notification_t
{
    /** Port id */
    sai_object_id_t port_id;

    /** Port operational status */
    sai_port_oper_status_t port_state;

} sai_port_oper_status_notification_t;

/**
 * @brief Attribute data for #SAI_PORT_ATTR_GLOBAL_FLOW_CONTROL_MODE
 */
typedef enum _sai_port_flow_control_mode_t
{
    /** disable flow control for both tx and rx */
    SAI_PORT_FLOW_CONTROL_MODE_DISABLE,

    /** enable flow control for tx only */
    SAI_PORT_FLOW_CONTROL_MODE_TX_ONLY,

    /** enable flow control for rx only */
    SAI_PORT_FLOW_CONTROL_MODE_RX_ONLY,

    /** enable flow control for both tx and rx */
    SAI_PORT_FLOW_CONTROL_MODE_BOTH_ENABLE,

} sai_port_flow_control_mode_t;

/**
 * @brief Attribute data for #SAI_PORT_ATTR_INTERNAL_LOOPBACK_MODE
 */
typedef enum _sai_port_internal_loopback_mode_t
{
    /** disable internal loopback */
    SAI_PORT_INTERNAL_LOOPBACK_MODE_NONE,

    /** port internal loopback at phy module */
    SAI_PORT_INTERNAL_LOOPBACK_MODE_PHY,

    /** port internal loopback at mac module */
    SAI_PORT_INTERNAL_LOOPBACK_MODE_MAC

} sai_port_internal_loopback_mode_t;

/**
 * @brief Attribute data for #SAI_PORT_ATTR_MEDIA_TYPE
 */
typedef enum _sai_port_media_type_t
{
    /** Media not present */
    SAI_PORT_MEDIA_TYPE_NOT_PRESENT,

    /** Media type not known */
    SAI_PORT_MEDIA_TYPE_UNKNONWN,

    /** Media type fiber. Remote advertise medium information as fiber */
    SAI_PORT_MEDIA_TYPE_FIBER,

    /** Media type copper. Remote advertise medium information as copper */
    SAI_PORT_MEDIA_TYPE_COPPER,
} sai_port_media_type_t;

/**
 * @brief Breakout Mode types based on number
 * of SerDes lanes used in a port
 */
typedef enum _sai_port_breakout_mode_type_t
{
    /** 1 lane breakout Mode */
    SAI_PORT_BREAKOUT_MODE_TYPE_1_LANE = 0,

    /** 2 lanes breakout Mode */
    SAI_PORT_BREAKOUT_MODE_TYPE_2_LANE = 1,

    /** 4 lanes breakout Mode */
    SAI_PORT_BREAKOUT_MODE_TYPE_4_LANE = 2,

    /** Breakout mode max count */
    SAI_PORT_BREAKOUT_MODE_TYPE_MAX
} sai_port_breakout_mode_type_t;

/**
 * @brief Attribute Id in sai_set_port_attribute() and
 * sai_get_port_attribute() calls
 */
typedef enum _sai_port_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PORT_ATTR_START,

    /* READ-ONLY */

    /**
     * @brief Port Type
     *
     * @type sai_port_type_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_TYPE = SAI_PORT_ATTR_START,

    /**
     * @brief Operational Status
     *
     * @type sai_port_oper_status_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_OPER_STATUS,

    /**
     * @brief Breakout mode(s) supported
     *
     * @type sai_s32_list_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_SUPPORTED_BREAKOUT_MODE_TYPE,

    /**
     * @brief Current breakout mode
     *
     * @type sai_port_breakout_mode_type_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_CURRENT_BREAKOUT_MODE_TYPE,

    /**
     * @brief Number of queues on port
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_QOS_NUMBER_OF_QUEUES,

    /**
     * @brief List of Queues for the port
     *
     * @type sai_object_list_t
     * @objects SAI_OBJECT_TYPE_QUEUE
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_QOS_QUEUE_LIST,

    /**
     * @brief Number of Scheduler groups on port
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_QOS_NUMBER_OF_SCHEDULER_GROUPS,

    /**
     * @brief List of Scheduler groups for the port
     *
     * @type sai_object_list_t
     * @objects SAI_OBJECT_TYPE_SCHEDULER_GROUP
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_QOS_SCHEDULER_GROUP_LIST,

    /**
     * @brief Query list of supported port speed(full-duplex) in Mbps
     *
     * @type sai_u32_list_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_SUPPORTED_SPEED,

    /**
     * @brief Query list of Supported HALF-Duplex speed in Mbps
     *
     * @type sai_u32_list_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_SUPPORTED_HALF_DUPLEX_SPEED,

    /**
     * @brief Query auto-negotiation support
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_SUPPORTED_AUTO_NEG_MODE,

    /**
     * @brief Query port supported flow control mode
     *
     * @type sai_port_flow_control_mode_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_SUPPORTED_FLOW_CONTROL_MODE,

    /**
     * @brief Query port supported asymmetric pause mode
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_SUPPORTED_ASYMMETRIC_PAUSE_MODE,

    /**
     * @brief Query port supported MEDIA type
     *
     * @type sai_port_media_type_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_SUPPORTED_MEDIA_TYPE,

    /**
     * @brief Query list of supported remote port speed (Full-Duplex) in Mbps
     *
     * @type sai_u32_list_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_REMOTE_SUPPORTED_SPEED,

    /**
     * @brief Query list of Remote Port's Supported HALF-Duplex speed in Mbps
     *
     * @type sai_u32_list_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_REMOTE_SUPPORTED_HALF_DUPLEX_SPEED,

    /**
     * @brief Query Remote Port's auto-negotiation support
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_REMOTE_SUPPORTED_AUTO_NEG_MODE,

    /**
     * @brief Query Remote port supported flow control mode
     *
     * @type sai_port_flow_control_mode_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_REMOTE_SUPPORTED_FLOW_CONTROL_MODE,

    /**
     * @brief Query Remote port supported asymmetric pause mode
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_REMOTE_SUPPORTED_ASYMMETRIC_PAUSE_MODE,

    /**
     * @brief Query Remote port MEDIA type
     *
     * @type sai_port_media_type_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_REMOTE_SUPPORTED_MEDIA_TYPE,

    /**
     * @brief Query list of Advertised remote port speed (Full-Duplex) in Mbps
     *
     * @type sai_u32_list_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_REMOTE_ADVERTISED_SPEED,

    /**
     * @brief Query list of Remote Port's Advertised HALF-Duplex speed in Mbps
     *
     * @type sai_u32_list_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_REMOTE_ADVERTISED_HALF_DUPLEX_SPEED,

    /**
     * @brief Query Remote Port's auto-negotiation Advertisement
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_REMOTE_ADVERTISED_AUTO_NEG_MODE,

    /**
     * @brief Query Remote port Advertised flow control mode
     *
     * @type sai_port_flow_control_mode_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_REMOTE_ADVERTISED_FLOW_CONTROL_MODE,

    /**
     * @brief Query Remote port Advertised asymmetric pause mode
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_REMOTE_ADVERTISED_ASYMMETRIC_PAUSE_MODE,

    /**
     * @brief Query Remote port Advertised MEDIA type
     *
     * @type sai_port_media_type_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_REMOTE_ADVERTISED_MEDIA_TYPE,

    /**
     * @brief Number of ingress priority groups
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_NUMBER_OF_INGRESS_PRIORITY_GROUPS,

    /**
     * @brief list of ingress priority groups
     *
     * @type sai_object_list_t
     * @objects SAI_OBJECT_TYPE_INGRESS_PRIORITY_GROUP
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_INGRESS_PRIORITY_GROUP_LIST,

    /* READ-WRITE */

    /**
     * @brief Hardware Lane list
     *
     * @type sai_u32_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY | KEY
     */
    SAI_PORT_ATTR_HW_LANE_LIST,

    /**
     * @brief Speed in Mbps
     *
     * @type sai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_PORT_ATTR_SPEED,

    /**
     * @brief Full Duplex setting
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default true
     */
    SAI_PORT_ATTR_FULL_DUPLEX_MODE,

    /**
     * @brief Auto Negotiation configuration
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default false
     */
    SAI_PORT_ATTR_AUTO_NEG_MODE,

    /**
     * @brief Admin Mode
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PORT_ATTR_ADMIN_STATE,

    /**
     * @brief Media Type
     *
     * @type sai_port_media_type_t
     * @flags CREATE_ONLY
     * @default SAI_PORT_MEDIA_TYPE_NOT_PRESENT
     */
    SAI_PORT_ATTR_MEDIA_TYPE,

    /**
     * @brief Query/Configure list of Advertised port speed (Full-Duplex) in Mbps
     *
     * @type sai_u32_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PORT_ATTR_ADVERTISED_SPEED,

    /**
     * @brief Query/Configure list of Advertised HALF-Duplex speed in Mbps
     *
     * @type sai_u32_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PORT_ATTR_ADVERTISED_HALF_DUPLEX_SPEED,

    /**
     * @brief Query/Configure Port's Advertised auto-negotiation configuration
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PORT_ATTR_ADVERTISED_AUTO_NEG_MODE,

    /**
     * @brief Query/Configure Port's Advertised flow control mode
     *
     * @type sai_port_flow_control_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_PORT_FLOW_CONTROL_MODE_DISABLE
     */
    SAI_PORT_ATTR_ADVERTISED_FLOW_CONTROL_MODE,

    /**
     * @brief Query port's Advertised asymmetric pause mode
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PORT_ATTR_ADVERTISED_ASYMMETRIC_PAUSE_MODE,

    /**
     * @brief Query/Configure Port's Advertised media type
     *
     * @type sai_port_media_type_t
     * @flags CREATE_AND_SET
     * @default SAI_PORT_MEDIA_TYPE_UNKNONWN
     */
    SAI_PORT_ATTR_ADVERTISED_MEDIA_TYPE,

    /**
     * @brief Port VLAN ID
     *
     * Untagged ingress frames are tagged with Port VLAN ID (PVID)
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @default 1
     * @isvlan true
     */
    SAI_PORT_ATTR_PORT_VLAN_ID,

    /**
     * @brief Default VLAN Priority
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PORT_ATTR_DEFAULT_VLAN_PRIORITY,

    /**
     * @brief Ingress Filtering (Drop Frames with Unknown VLANs)
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PORT_ATTR_INGRESS_FILTERING,

    /**
     * @brief Dropping of untagged frames on ingress
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PORT_ATTR_DROP_UNTAGGED,

    /**
     * @brief Dropping of tagged frames on ingress
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PORT_ATTR_DROP_TAGGED,

    /**
     * @brief Internal loopback control
     *
     * @type sai_port_internal_loopback_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_PORT_INTERNAL_LOOPBACK_MODE_NONE
     */
    SAI_PORT_ATTR_INTERNAL_LOOPBACK_MODE,

    /**
     * @brief Update DSCP of outgoing packets
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PORT_ATTR_UPDATE_DSCP,

    /**
     * @brief MTU
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 1514
     */
    SAI_PORT_ATTR_MTU,

    /**
     * @brief Enable flood (unknown unicast or unknown multicast)
     * storm control policer on port. Set policer id = #SAI_NULL_OBJECT_ID to
     * disable policer on port.
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_POLICER
     * @flags CREATE_AND_SET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_FLOOD_STORM_CONTROL_POLICER_ID,

    /**
     * @brief Enable broadcast storm control policer on port. Set
     * Policer id = #SAI_NULL_OBJECT_ID to disable policer on port
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_POLICER
     * @flags CREATE_AND_SET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_BROADCAST_STORM_CONTROL_POLICER_ID,

    /**
     * @brief Enable multicast storm control policer on port. Set
     * policer id = #SAI_NULL_OBJECT_ID to disable policer on port
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_POLICER
     * @flags CREATE_AND_SET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_MULTICAST_STORM_CONTROL_POLICER_ID,

    /**
     * @brief Global flow control
     *
     * @type sai_port_flow_control_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_PORT_FLOW_CONTROL_MODE_DISABLE
     */
    SAI_PORT_ATTR_GLOBAL_FLOW_CONTROL_MODE,

    /**
     * @brief Port bind point for ingress ACL object
     *
     * Bind (or unbind) an ingress acl table or acl group on a port. Enable/Update
     * ingress ACL table or ACL group filtering by assigning the list of valid
     * object id. Disable ingress filtering by assigning SAI_NULL_OBJECT_ID
     * in the attribute value.
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_ACL_TABLE, SAI_OBJECT_TYPE_ACL_TABLE_GROUP
     * @flags CREATE_AND_SET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_INGRESS_ACL,

    /**
     * @brief Port bind point for egress ACL object
     *
     * Bind (or unbind) an egress acl tables or acl group on a port. Enable/Update
     * egress ACL table or ACL group filtering by assigning the list of valid
     * object id. Disable egress filtering by assigning SAI_NULL_OBJECT_ID
     * in the attribute value.
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_ACL_TABLE, SAI_OBJECT_TYPE_ACL_TABLE_GROUP
     * @flags CREATE_AND_SET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_EGRESS_ACL,

    /**
     * @brief Enable/Disable Mirror session
     *
     * Enable ingress mirroring by assigning list of mirror session object id
     * as attribute value, disable ingress mirroring by assigning object_count
     * as 0 in objlist
     *
     * @type sai_object_list_t
     * @objects SAI_OBJECT_TYPE_MIRROR_SESSION
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PORT_ATTR_INGRESS_MIRROR_SESSION,

    /**
     * @brief Enable/Disable Mirror session
     *
     * Enable egress mirroring by assigning list of mirror session object id as
     * attribute value Disable egress mirroring by assigning object_count as 0
     * in objlist
     *
     * @type sai_object_list_t
     * @objects SAI_OBJECT_TYPE_MIRROR_SESSION
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PORT_ATTR_EGRESS_MIRROR_SESSION,

    /**
     * @brief Enable/Disable Samplepacket session
     *
     * Enable ingress sampling by assigning samplepacket object id
     * Disable ingress sampling by assigning #SAI_NULL_OBJECT_ID as
     * attribute value
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_SAMPLEPACKET
     * @flags CREATE_AND_SET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_INGRESS_SAMPLEPACKET_ENABLE,

    /**
     * @brief Enable/Disable Samplepacket session
     *
     * Enable egress sampling by assigning samplepacket object id
     * Disable egress sampling by assigning #SAI_NULL_OBJECT_ID as
     * attribute value
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_SAMPLEPACKET
     * @flags CREATE_AND_SET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_EGRESS_SAMPLEPACKET_ENABLE,

    /**
     * @brief Attach/Detach policer to port
     *
     * Set policer id = #SAI_NULL_OBJECT_ID to disable policer on port
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_POLICER
     * @flags CREATE_AND_SET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_POLICER_ID,

    /**
     * @brief Port default Traffic class Mapping
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PORT_ATTR_QOS_DEFAULT_TC,

    /**
     * @brief Enable DOT1P -> TC MAP on port
     *
     * MAP id = #SAI_NULL_OBJECT_ID to disable map on port.
     * To enable/disable trust Dot1p, Map ID should be add/remove on port.
     * Default no map
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @flags CREATE_AND_SET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_DOT1P_TO_TC_MAP,

    /**
     * @brief Enable DOT1P -> COLOR MAP on port
     *
     * MAP id = #SAI_NULL_OBJECT_ID to disable map on port.
     * To enable/disable trust Dot1p, Map ID should be add/remove on port.
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @flags CREATE_AND_SET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_DOT1P_TO_COLOR_MAP,

    /**
     * @brief Enable DSCP -> TC MAP on port
     *
     * MAP id = #SAI_NULL_OBJECT_ID to disable map on port.
     * To enable/disable trust DSCP, Map ID should be add/remove on port.
     * Default no map
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @flags CREATE_AND_SET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_DSCP_TO_TC_MAP,

    /**
     * @brief Enable DSCP -> COLOR MAP on port
     *
     * MAP id = #SAI_NULL_OBJECT_ID to disable map on port.
     * To enable/disable trust DSCP, Map ID should be add/remove on port.
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @flags CREATE_AND_SET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_DSCP_TO_COLOR_MAP,

    /**
     * @brief Enable TC -> Queue MAP on port
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on port.
     * Default no map i.e All packets to queue 0
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @flags CREATE_AND_SET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_TC_TO_QUEUE_MAP,

    /**
     * @brief Enable TC AND COLOR -> DOT1P MAP
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on port.
     * Default no map
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @flags CREATE_AND_SET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_TC_AND_COLOR_TO_DOT1P_MAP,

    /**
     * @brief Enable TC AND COLOR -> DSCP MAP
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on port.
     * Default no map
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @flags CREATE_AND_SET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_TC_AND_COLOR_TO_DSCP_MAP,

    /**
     * @brief Enable TC -> Priority Group MAP
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on port.
     * Default no map
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @flags CREATE_AND_SET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_TC_TO_PRIORITY_GROUP_MAP,

    /**
     * @brief Enable PFC Priority -> Priority Group MAP
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on port.
     * Default no map
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @flags CREATE_AND_SET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_PFC_PRIORITY_TO_PRIORITY_GROUP_MAP,

    /**
     * @brief Enable PFC Priority -> Queue MAP
     * Map id = #SAI_NULL_OBJECT_ID to disable map on port.
     * Default no map
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @flags CREATE_AND_SET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_PFC_PRIORITY_TO_QUEUE_MAP,

    /**
     * @brief Attach WRED to port
     *
     * ID = #SAI_NULL_OBJECT_ID to disable WRED.
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_WRED
     * @flags CREATE_AND_SET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_WRED_PROFILE_ID,

    /**
     * @brief Scheduler for port, Default no limits.
     *
     * #SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE & #SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE
     * attributes alone valid. Rest will be ignored
     *
     * @type sai_object_id_t
     * @objects SAI_OBJECT_TYPE_SCHEDULER
     * @flags CREATE_AND_SET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_SCHEDULER_PROFILE_ID,

    /**
     * @brief Ingress buffer profiles for port
     *
     * There can be up to #SAI_SWITCH_ATTR_INGRESS_BUFFER_POOL_NUM profiles
     *
     * @type sai_object_list_t
     * @objects SAI_OBJECT_TYPE_BUFFER_PROFILE
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PORT_ATTR_QOS_INGRESS_BUFFER_PROFILE_LIST,

    /**
     * @brief Egress buffer profiles for port
     *
     * There can be up to #SAI_SWITCH_ATTR_EGRESS_BUFFER_POOL_NUM profiles
     *
     * @type sai_object_list_t
     * @objects SAI_OBJECT_TYPE_BUFFER_PROFILE
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PORT_ATTR_QOS_EGRESS_BUFFER_PROFILE_LIST,

    /**
     * @brief Bit vector enable/disable port PFC
     *
     * Valid from bit 0 to bit 7
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PORT_ATTR_PRIORITY_FLOW_CONTROL,

    /**
     * @brief User based Meta Data
     *
     * Value Range #SAI_SWITCH_ATTR_PORT_USER_META_DATA_RANGE
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PORT_ATTR_META_DATA,

    /**
     * @brief Egress block port list
     *
     * Traffic ingressing on this port and egressing out of the ports in the
     * given port list will be dropped.
     *
     * @type sai_object_list_t
     * @objects SAI_OBJECT_TYPE_PORT
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PORT_ATTR_EGRESS_BLOCK_PORT_LIST,

    /**
     * @brief Port Hardware Configuration Profile ID
     *
     * Port can require different hardware configuration based on the attached
     * media type, cable length etc. A Profile ID maps to a Port Hardware
     * configuration settings that needs to be applied on the Port.
     * This attribute need not be implemented and can be ignored if the port
     * doesn't require any specific hardware settings based on media type/cable.
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default vendor
     */
    SAI_PORT_ATTR_HW_PROFILE_ID,

    /**
     * @brief Port EEE Configuration
     *
     * Energy Efficient Ethernet(EEE) is an IEEE 802.3 az standard aiming to
     * reduce power consumptions on Ethernet ports (native copper ports).
     * Enable the EEE on port level
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PORT_ATTR_EEE_ENABLE,

    /**
     * @brief Port EEE IDLE time configuration
     *
     * Time (in microsecs) to move to Low power state (No traffic), at the end of which MAC transitions to Low power state.
     * MAX value set more benefit.
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @default 2500
     */
    SAI_PORT_ATTR_EEE_IDLE_TIME,

    /**
     * @brief Port EEE Wakeup time configuration
     *
     * Time(in microsecs) to wait before transmitter is leaving Low Power Mode State. Min value set avoid latency.
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @default 5
     */
    SAI_PORT_ATTR_EEE_WAKE_TIME,

    /**
    * @brief Port bind mode
    *
    * @type sai_port_bind_mode_t
    * @flags CREATE_AND_SET
    * @default SAI_PORT_BIND_MODE_PORT
    */
    SAI_PORT_ATTR_BIND_MODE,

    /**
     * @brief End of attributes
     */
    SAI_PORT_ATTR_END,

    /** Custom range base value */
    SAI_PORT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PORT_ATTR_CUSTOM_RANGE_END

} sai_port_attr_t;

/**
 * @brief Port counter IDs in sai_get_port_stats() call
 */
typedef enum _sai_port_stat_t
{
    /** sai port stat if in octets */
    SAI_PORT_STAT_IF_IN_OCTETS,

    /** sai port stat if in ucast pkts */
    SAI_PORT_STAT_IF_IN_UCAST_PKTS,

    /** sai port stat if in non ucast pkts */
    SAI_PORT_STAT_IF_IN_NON_UCAST_PKTS,

    /** sai port stat if in discards */
    SAI_PORT_STAT_IF_IN_DISCARDS,

    /** sai port stat if in errors */
    SAI_PORT_STAT_IF_IN_ERRORS,

    /** sai port stat if in unknown protos */
    SAI_PORT_STAT_IF_IN_UNKNOWN_PROTOS,

    /** sai port stat if in broadcast pkts */
    SAI_PORT_STAT_IF_IN_BROADCAST_PKTS,

    /** sai port stat if in multicast pkts */
    SAI_PORT_STAT_IF_IN_MULTICAST_PKTS,

    /** sai port stat if in vlan discards */
    SAI_PORT_STAT_IF_IN_VLAN_DISCARDS,

    /** sai port stat if out octets */
    SAI_PORT_STAT_IF_OUT_OCTETS,

    /** sai port stat if out ucast pkts */
    SAI_PORT_STAT_IF_OUT_UCAST_PKTS,

    /** sai port stat if out non ucast pkts */
    SAI_PORT_STAT_IF_OUT_NON_UCAST_PKTS,

    /** sai port stat if out discards */
    SAI_PORT_STAT_IF_OUT_DISCARDS,

    /** sai port stat if out errors */
    SAI_PORT_STAT_IF_OUT_ERRORS,

    /** sai port stat if out qlen */
    SAI_PORT_STAT_IF_OUT_QLEN,

    /** sai port stat if out broadcast pkts */
    SAI_PORT_STAT_IF_OUT_BROADCAST_PKTS,

    /** sai port stat if out multicast pkts */
    SAI_PORT_STAT_IF_OUT_MULTICAST_PKTS,

    /** sai port stat ether stats drop events */
    SAI_PORT_STAT_ETHER_STATS_DROP_EVENTS,

    /** sai port stat ether stats multicast pkts */
    SAI_PORT_STAT_ETHER_STATS_MULTICAST_PKTS,

    /** sai port stat ether stats broadcast pkts */
    SAI_PORT_STAT_ETHER_STATS_BROADCAST_PKTS,

    /** sai port stat ether stats undersize pkts */
    SAI_PORT_STAT_ETHER_STATS_UNDERSIZE_PKTS,

    /** sai port stat ether stats fragments */
    SAI_PORT_STAT_ETHER_STATS_FRAGMENTS,

    /** sai port stat ether stats pkts 64 octets */
    SAI_PORT_STAT_ETHER_STATS_PKTS_64_OCTETS,

    /** sai port stat ether stats pkts 65 to 127 octets */
    SAI_PORT_STAT_ETHER_STATS_PKTS_65_TO_127_OCTETS,

    /** sai port stat ether stats pkts 128 to 255 octets */
    SAI_PORT_STAT_ETHER_STATS_PKTS_128_TO_255_OCTETS,

    /** sai port stat ether stats pkts 256 to 511 octets */
    SAI_PORT_STAT_ETHER_STATS_PKTS_256_TO_511_OCTETS,

    /** sai port stat ether stats pkts 512 to 1023 octets */
    SAI_PORT_STAT_ETHER_STATS_PKTS_512_TO_1023_OCTETS,

    /** sai port stat ether stats pkts 1024 to 1518 octets */
    SAI_PORT_STAT_ETHER_STATS_PKTS_1024_TO_1518_OCTETS,

    /** sai port stat ether stats pkts 1519 to 2047 octets */
    SAI_PORT_STAT_ETHER_STATS_PKTS_1519_TO_2047_OCTETS,

    /** sai port stat ether stats pkts 2048 to 4095 octets */
    SAI_PORT_STAT_ETHER_STATS_PKTS_2048_TO_4095_OCTETS,

    /** sai port stat ether stats pkts 4096 to 9216 octets */
    SAI_PORT_STAT_ETHER_STATS_PKTS_4096_TO_9216_OCTETS,

    /** sai port stat ether stats pkts 9217 to 16383 octets */
    SAI_PORT_STAT_ETHER_STATS_PKTS_9217_TO_16383_OCTETS,

    /** sai port stat ether stats oversize pkts */
    SAI_PORT_STAT_ETHER_STATS_OVERSIZE_PKTS,

    /** sai port stat ether rx oversize pkts */
    SAI_PORT_STAT_ETHER_RX_OVERSIZE_PKTS,

    /** sai port stat ether tx oversize pkts */
    SAI_PORT_STAT_ETHER_TX_OVERSIZE_PKTS,

    /** sai port stat ether stats jabbers */
    SAI_PORT_STAT_ETHER_STATS_JABBERS,

    /** sai port stat ether stats octets */
    SAI_PORT_STAT_ETHER_STATS_OCTETS,

    /** sai port stat ether stats pkts */
    SAI_PORT_STAT_ETHER_STATS_PKTS,

    /** sai port stat ether stats collisions */
    SAI_PORT_STAT_ETHER_STATS_COLLISIONS,

    /** sai port stat ether stats crc align errors */
    SAI_PORT_STAT_ETHER_STATS_CRC_ALIGN_ERRORS,

    /** sai port stat ether stats tx no errors */
    SAI_PORT_STAT_ETHER_STATS_TX_NO_ERRORS,

    /** sai port stat ether stats rx no errors */
    SAI_PORT_STAT_ETHER_STATS_RX_NO_ERRORS,

    /** sai port stat ip in receives */
    SAI_PORT_STAT_IP_IN_RECEIVES,

    /** sai port stat ip in octets */
    SAI_PORT_STAT_IP_IN_OCTETS,

    /** sai port stat ip in ucast pkts */
    SAI_PORT_STAT_IP_IN_UCAST_PKTS,

    /** sai port stat ip in non ucast pkts */
    SAI_PORT_STAT_IP_IN_NON_UCAST_PKTS,

    /** sai port stat ip in discards */
    SAI_PORT_STAT_IP_IN_DISCARDS,

    /** sai port stat ip out octets */
    SAI_PORT_STAT_IP_OUT_OCTETS,

    /** sai port stat ip out ucast pkts */
    SAI_PORT_STAT_IP_OUT_UCAST_PKTS,

    /** sai port stat ip out non ucast pkts */
    SAI_PORT_STAT_IP_OUT_NON_UCAST_PKTS,

    /** sai port stat ip out discards */
    SAI_PORT_STAT_IP_OUT_DISCARDS,

    /** sai port stat ipv6 in receives */
    SAI_PORT_STAT_IPV6_IN_RECEIVES,

    /** sai port stat ipv6 in octets */
    SAI_PORT_STAT_IPV6_IN_OCTETS,

    /** sai port stat ipv6 in ucast pkts */
    SAI_PORT_STAT_IPV6_IN_UCAST_PKTS,

    /** sai port stat ipv6 in non ucast pkts */
    SAI_PORT_STAT_IPV6_IN_NON_UCAST_PKTS,

    /** sai port stat ipv6 in mcast pkts */
    SAI_PORT_STAT_IPV6_IN_MCAST_PKTS,

    /** sai port stat ipv6 in discards */
    SAI_PORT_STAT_IPV6_IN_DISCARDS,

    /** sai port stat ipv6 out octets */
    SAI_PORT_STAT_IPV6_OUT_OCTETS,

    /** sai port stat ipv6 out ucast pkts */
    SAI_PORT_STAT_IPV6_OUT_UCAST_PKTS,

    /** sai port stat ipv6 out non ucast pkts */
    SAI_PORT_STAT_IPV6_OUT_NON_UCAST_PKTS,

    /** sai port stat ipv6 out mcast pkts */
    SAI_PORT_STAT_IPV6_OUT_MCAST_PKTS,

    /** sai port stat ipv6 out discards */
    SAI_PORT_STAT_IPV6_OUT_DISCARDS,

    /** get/set WRED green packet count [uint64_t] */
    SAI_PORT_STAT_GREEN_DISCARD_DROPPED_PACKETS,

    /** get/set WRED green byte count [uint64_t] */
    SAI_PORT_STAT_GREEN_DISCARD_DROPPED_BYTES,

    /** get/set WRED yellow packet count [uint64_t] */
    SAI_PORT_STAT_YELLOW_DISCARD_DROPPED_PACKETS,

    /** get/set WRED yellow byte count [uint64_t] */
    SAI_PORT_STAT_YELLOW_DISCARD_DROPPED_BYTES,

    /** get/set WRED red packet count [uint64_t] */
    SAI_PORT_STAT_RED_DISCARD_DROPPED_PACKETS,

    /** get/set WRED red byte count [uint64_t] */
    SAI_PORT_STAT_RED_DISCARD_DROPPED_BYTES,

    /** get/set WRED dropped packets count [uint64_t] */
    SAI_PORT_STAT_DISCARD_DROPPED_PACKETS,

    /** get/set WRED dropped bytes count [uint64_t] */
    SAI_PORT_STAT_DISCARD_DROPPED_BYTES,

    /** get/set packets marked by ECN count [uint64_t] */
    SAI_PORT_STAT_ECN_MARKED_PACKETS,

    /** packet size based packets count rt stat ether in pkts 64 octets */
    SAI_PORT_STAT_ETHER_IN_PKTS_64_OCTETS,

    /** sai port stat ether in pkts 65 to 127 octets */
    SAI_PORT_STAT_ETHER_IN_PKTS_65_TO_127_OCTETS,

    /** sai port stat ether in pkts 128 to 255 octets */
    SAI_PORT_STAT_ETHER_IN_PKTS_128_TO_255_OCTETS,

    /** sai port stat ether in pkts 256 to 511 octets */
    SAI_PORT_STAT_ETHER_IN_PKTS_256_TO_511_OCTETS,

    /** sai port stat ether in pkts 512 to 1023 octets */
    SAI_PORT_STAT_ETHER_IN_PKTS_512_TO_1023_OCTETS,

    /** sai port stat ether in pkts 1024 to 1518 octets */
    SAI_PORT_STAT_ETHER_IN_PKTS_1024_TO_1518_OCTETS,

    /** sai port stat ether in pkts 1519 to 2047 octets */
    SAI_PORT_STAT_ETHER_IN_PKTS_1519_TO_2047_OCTETS,

    /** sai port stat ether in pkts 2048 to 4095 octets */
    SAI_PORT_STAT_ETHER_IN_PKTS_2048_TO_4095_OCTETS,

    /** sai port stat ether in pkts 4096 to 9216 octets */
    SAI_PORT_STAT_ETHER_IN_PKTS_4096_TO_9216_OCTETS,

    /** sai port stat ether in pkts 9217 to 16383 octets */
    SAI_PORT_STAT_ETHER_IN_PKTS_9217_TO_16383_OCTETS,

    /** sai port stat ether out pkts 64 octets */
    SAI_PORT_STAT_ETHER_OUT_PKTS_64_OCTETS,

    /** sai port stat ether out pkts 65 to 127 octets */
    SAI_PORT_STAT_ETHER_OUT_PKTS_65_TO_127_OCTETS,

    /** sai port stat ether out pkts 128 to 255 octets */
    SAI_PORT_STAT_ETHER_OUT_PKTS_128_TO_255_OCTETS,

    /** sai port stat ether out pkts 256 to 511 octets */
    SAI_PORT_STAT_ETHER_OUT_PKTS_256_TO_511_OCTETS,

    /** sai port stat ether out pkts 512 to 1023 octets */
    SAI_PORT_STAT_ETHER_OUT_PKTS_512_TO_1023_OCTETS,

    /** sai port stat ether out pkts 1024 to 1518 octets */
    SAI_PORT_STAT_ETHER_OUT_PKTS_1024_TO_1518_OCTETS,

    /** sai port stat ether out pkts 1519 to 2047 octets */
    SAI_PORT_STAT_ETHER_OUT_PKTS_1519_TO_2047_OCTETS,

    /** sai port stat ether out pkts 2048 to 4095 octets */
    SAI_PORT_STAT_ETHER_OUT_PKTS_2048_TO_4095_OCTETS,

    /** sai port stat ether out pkts 4096 to 9216 octets */
    SAI_PORT_STAT_ETHER_OUT_PKTS_4096_TO_9216_OCTETS,

    /** sai port stat ether out pkts 9217 to 16383 octets */
    SAI_PORT_STAT_ETHER_OUT_PKTS_9217_TO_16383_OCTETS,

    /** get current port occupancy in bytes [uint64_t] */
    SAI_PORT_STAT_CURR_OCCUPANCY_BYTES,

    /** get watermark port occupancy in bytes [uint64_t] */
    SAI_PORT_STAT_WATERMARK_BYTES,

    /** get current port shared occupancy in bytes [uint64_t] */
    SAI_PORT_STAT_SHARED_CURR_OCCUPANCY_BYTES,

    /** get watermark port shared occupancy in bytes [uint64_t] */
    SAI_PORT_STAT_SHARED_WATERMARK_BYTES,

    /** get the number of pause frames received on the port [uint64_t] */
    SAI_PORT_STAT_PAUSE_RX_PKTS,

    /** get the number of pause frames transmitted on the port [uint64_t] */
    SAI_PORT_STAT_PAUSE_TX_PKTS,

    /** PFC Packet Counters for RX and TX per PFC priority [uint64_t] */
    SAI_PORT_STAT_PFC_0_RX_PKTS,

    /** sai port stat pfc 0 tx pkts */
    SAI_PORT_STAT_PFC_0_TX_PKTS,

    /** sai port stat pfc 1 rx pkts */
    SAI_PORT_STAT_PFC_1_RX_PKTS,

    /** sai port stat pfc 1 tx pkts */
    SAI_PORT_STAT_PFC_1_TX_PKTS,

    /** sai port stat pfc 2 rx pkts */
    SAI_PORT_STAT_PFC_2_RX_PKTS,

    /** sai port stat pfc 2 tx pkts */
    SAI_PORT_STAT_PFC_2_TX_PKTS,

    /** sai port stat pfc 3 rx pkts */
    SAI_PORT_STAT_PFC_3_RX_PKTS,

    /** sai port stat pfc 3 tx pkts */
    SAI_PORT_STAT_PFC_3_TX_PKTS,

    /** sai port stat pfc 4 rx pkts */
    SAI_PORT_STAT_PFC_4_RX_PKTS,

    /** sai port stat pfc 4 tx pkts */
    SAI_PORT_STAT_PFC_4_TX_PKTS,

    /** sai port stat pfc 5 rx pkts */
    SAI_PORT_STAT_PFC_5_RX_PKTS,

    /** sai port stat pfc 5 tx pkts */
    SAI_PORT_STAT_PFC_5_TX_PKTS,

    /** sai port stat pfc 6 rx pkts */
    SAI_PORT_STAT_PFC_6_RX_PKTS,

    /** sai port stat pfc 6 tx pkts */
    SAI_PORT_STAT_PFC_6_TX_PKTS,

    /** sai port stat pfc 7 rx pkts */
    SAI_PORT_STAT_PFC_7_RX_PKTS,

    /** sai port stat pfc 7 tx pkts */
    SAI_PORT_STAT_PFC_7_TX_PKTS,

    /** Number of times port state changed from
     * high power mode to low power mode in TX direction [uint64_t] */
    SAI_PORT_STAT_EEE_TX_EVENT_COUNT,

    /** Number of times port state changed from
     * high power mode to low power mode in RX direction [uint64_t] */
    SAI_PORT_STAT_EEE_RX_EVENT_COUNT,

    /** Port Low power mode duration(micro secs) in TX direction [uint64_t].
     * This Duration is accumulative since EEE enable on port/from last clear stats*/
    SAI_PORT_STAT_EEE_TX_DURATION,

    /** Port Low power mode duration(micro secs) in RX direction [uint64_t]
     * This Duration is accumulative since EEE enable on port/from last clear stats*/
    SAI_PORT_STAT_EEE_RX_DURATION,

} sai_port_stat_t;

/**
 * @brief Create port
 *
 * @param[out] port_id Port id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_create_port_fn)(
        _Out_ sai_object_id_t *port_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);
/**
 * @brief Remove port
 *
 * @param[in] port_id Port id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_remove_port_fn)(
        _In_ sai_object_id_t port_id);

/**
 * @brief Set port attribute value.
 *
 * @param[in] port_id Port id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_set_port_attribute_fn)(
        _In_ sai_object_id_t port_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get port attribute value.
 *
 * @param[in] port_id Port id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_port_attribute_fn)(
        _In_ sai_object_id_t port_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get port statistics counters.
 *
 * @param[in] port_id Port id
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] number_of_counters Number of counters in the array
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_get_port_stats_fn)(
        _In_ sai_object_id_t port_id,
        _In_ const sai_port_stat_t *counter_ids,
        _In_ uint32_t number_of_counters,
        _Out_ uint64_t *counters);

/**
 * @brief Clear port statistics counters.
 *
 * @param[in] port_id Port id
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] number_of_counters Number of counters in the array
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_clear_port_stats_fn)(
        _In_ sai_object_id_t port_id,
        _In_ const sai_port_stat_t *counter_ids,
        _In_ uint32_t number_of_counters);

/**
 * @brief Clear port's all statistics counters.
 *
 * @param[in] port_id Port id
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
typedef sai_status_t (*sai_clear_port_all_stats_fn)(
        _In_ sai_object_id_t port_id);

/**
 * @brief Port state change notification
 *
 * Passed as a parameter into sai_initialize_switch()
 *
 * @param[in] count Number of notifications
 * @param[in] data Array of port operational status
 */
typedef void (*sai_port_state_change_notification_fn)(
        _In_ uint32_t count,
        _In_ sai_port_oper_status_notification_t *data);

/**
 * @brief Port methods table retrieved with sai_api_query()
 */
typedef struct _sai_port_api_t
{
    sai_create_port_fn              create_port;
    sai_remove_port_fn              remove_port;
    sai_set_port_attribute_fn       set_port_attribute;
    sai_get_port_attribute_fn       get_port_attribute;
    sai_get_port_stats_fn           get_port_stats;
    sai_clear_port_stats_fn         clear_port_stats;
    sai_clear_port_all_stats_fn     clear_port_all_stats;

} sai_port_api_t;

/**
 * @}
 */
#endif /** __SAIPORT_H_ */
