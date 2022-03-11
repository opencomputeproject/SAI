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

    /** Fabric Port */
    SAI_PORT_TYPE_FABRIC,

    /** Recycle Port */
    SAI_PORT_TYPE_RECYCLE,

} sai_port_type_t;

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
    /**
     * @brief Port id.
     *
     * @objects SAI_OBJECT_TYPE_PORT, SAI_OBJECT_TYPE_BRIDGE_PORT, SAI_OBJECT_TYPE_LAG
     */
    sai_object_id_t port_id;

    /** Port operational status */
    sai_port_oper_status_t port_state;

} sai_port_oper_status_notification_t;

/**
 * @brief Attribute data for #SAI_PORT_ATTR_GLOBAL_FLOW_CONTROL_MODE
 */
typedef enum _sai_port_flow_control_mode_t
{
    /** Disable flow control for both tx and rx */
    SAI_PORT_FLOW_CONTROL_MODE_DISABLE,

    /** Enable flow control for tx only */
    SAI_PORT_FLOW_CONTROL_MODE_TX_ONLY,

    /** Enable flow control for rx only */
    SAI_PORT_FLOW_CONTROL_MODE_RX_ONLY,

    /** Enable flow control for both tx and rx */
    SAI_PORT_FLOW_CONTROL_MODE_BOTH_ENABLE,

} sai_port_flow_control_mode_t;

/**
 * @brief Attribute data for #SAI_PORT_ATTR_INTERNAL_LOOPBACK_MODE
 * To be deprecated, use sai_port_loopback_mode_t instead.
 */
typedef enum _sai_port_internal_loopback_mode_t
{
    /** Disable internal loopback */
    SAI_PORT_INTERNAL_LOOPBACK_MODE_NONE,

    /** Port internal loopback at PHY module */
    SAI_PORT_INTERNAL_LOOPBACK_MODE_PHY,

    /** Port internal loopback at MAC module */
    SAI_PORT_INTERNAL_LOOPBACK_MODE_MAC

} sai_port_internal_loopback_mode_t;

/**
 * @brief Attribute data for #SAI_PORT_ATTR_LOOPBACK_MODE
 */
typedef enum _sai_port_loopback_mode_t
{
    /** Disable loopback */
    SAI_PORT_LOOPBACK_MODE_NONE,

    /** Port loopback at PHY module */
    SAI_PORT_LOOPBACK_MODE_PHY,

    /** Port loopback at MAC module */
    SAI_PORT_LOOPBACK_MODE_MAC,

    /** Port loopback at PHY remote end */
    SAI_PORT_LOOPBACK_MODE_PHY_REMOTE
} sai_port_loopback_mode_t;

/**
 * @brief Attribute data for #SAI_PORT_ATTR_MEDIA_TYPE
 */
typedef enum _sai_port_media_type_t
{
    /** Media not present */
    SAI_PORT_MEDIA_TYPE_NOT_PRESENT,

    /** Media type not known */
    SAI_PORT_MEDIA_TYPE_UNKNOWN,

    /** Media type fiber. Remote advertise medium information as fiber */
    SAI_PORT_MEDIA_TYPE_FIBER,

    /** Media type copper. Remote advertise medium information as copper */
    SAI_PORT_MEDIA_TYPE_COPPER,

    /** Media type Back plane. */
    SAI_PORT_MEDIA_TYPE_BACKPLANE,
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
 * @brief Attribute data for #SAI_PORT_ATTR_FEC_MODE
 */
typedef enum _sai_port_fec_mode_t
{
    /** No FEC */
    SAI_PORT_FEC_MODE_NONE,

    /** Enable RS-FEC - 25G, 50G, 100G ports. The specific RS-FEC mode will be automatically determined. */
    SAI_PORT_FEC_MODE_RS,

    /** Enable FC-FEC - 10G, 25G, 40G, 50G ports */
    SAI_PORT_FEC_MODE_FC,
} sai_port_fec_mode_t;

/**
 * @brief Attribute data for #SAI_PORT_ATTR_FEC_MODE_EXTENDED
 */
typedef enum _sai_port_fec_mode_extended_t
{
    /** No FEC */
    SAI_PORT_FEC_MODE_EXTENDED_NONE,

    /** Enable RS-528 FEC (CL91) - 25G, 50G, 100G ports */
    SAI_PORT_FEC_MODE_EXTENDED_RS528,

    /** Enable RS544-FEC - 100G PAM4, 200G ports */
    SAI_PORT_FEC_MODE_EXTENDED_RS544,

    /** Enable RS544-FEC (interleaved) - 100G, 200G, 400G ports */
    SAI_PORT_FEC_MODE_EXTENDED_RS544_INTERLEAVED,

    /** Enable FC-FEC (CL74) - 10G, 25G, 40G, 50G ports */
    SAI_PORT_FEC_MODE_EXTENDED_FC,
} sai_port_fec_mode_extended_t;

/**
 * @brief Priority flow control mode
 */
typedef enum _sai_port_priority_flow_control_mode_t
{
    /** Same value for RX/TX */
    SAI_PORT_PRIORITY_FLOW_CONTROL_MODE_COMBINED,

    /** Separate values for RX/TX */
    SAI_PORT_PRIORITY_FLOW_CONTROL_MODE_SEPARATE,

} sai_port_priority_flow_control_mode_t;

/**
 * @brief PTP mode
 */
typedef enum _sai_port_ptp_mode_t
{
    /** No special processing for PTP packets */
    SAI_PORT_PTP_MODE_NONE,

    /** Single-step Timestamp mode for the PTP packets */
    SAI_PORT_PTP_MODE_SINGLE_STEP_TIMESTAMP,

    /** Two-step Timestamp mode for the PTP packets */
    SAI_PORT_PTP_MODE_TWO_STEP_TIMESTAMP,

} sai_port_ptp_mode_t;

/**
 * @brief Attribute data for #SAI_PORT_ATTR_INTERFACE_TYPE
 * Used for selecting electrical interface with specific electrical pin and signal quality
 */
typedef enum _sai_port_interface_type_t
{
    /** Interface type none */
    SAI_PORT_INTERFACE_TYPE_NONE,

    /** Interface type CR */
    SAI_PORT_INTERFACE_TYPE_CR,

    /** Interface type CR2 */
    SAI_PORT_INTERFACE_TYPE_CR2,

    /** Interface type CR4 */
    SAI_PORT_INTERFACE_TYPE_CR4,

    /** Interface type SR */
    SAI_PORT_INTERFACE_TYPE_SR,

    /** Interface type SR2 */
    SAI_PORT_INTERFACE_TYPE_SR2,

    /** Interface type SR4 */
    SAI_PORT_INTERFACE_TYPE_SR4,

    /** Interface type LR */
    SAI_PORT_INTERFACE_TYPE_LR,

    /** Interface type LR4 */
    SAI_PORT_INTERFACE_TYPE_LR4,

    /** Interface type KR */
    SAI_PORT_INTERFACE_TYPE_KR,

    /** Interface type KR4 */
    SAI_PORT_INTERFACE_TYPE_KR4,

    /** Interface type CAUI */
    SAI_PORT_INTERFACE_TYPE_CAUI,

    /** Interface type GMII */
    SAI_PORT_INTERFACE_TYPE_GMII,

    /** Interface type SFI */
    SAI_PORT_INTERFACE_TYPE_SFI,

    /** Interface type XLAUI */
    SAI_PORT_INTERFACE_TYPE_XLAUI,

    /** Interface type KR2 */
    SAI_PORT_INTERFACE_TYPE_KR2,

    /** Interface type CAUI */
    SAI_PORT_INTERFACE_TYPE_CAUI4,

    /** Interface type XAUI */
    SAI_PORT_INTERFACE_TYPE_XAUI,

    /** Interface type XFI */
    SAI_PORT_INTERFACE_TYPE_XFI,

    /** Interface type XGMII */
    SAI_PORT_INTERFACE_TYPE_XGMII,

    /** Interface type CR8 */
    SAI_PORT_INTERFACE_TYPE_CR8,

    /** Interface type KR8 */
    SAI_PORT_INTERFACE_TYPE_KR8,

    /** Interface type SR8 */
    SAI_PORT_INTERFACE_TYPE_SR8,

    /** Interface type LR8 */
    SAI_PORT_INTERFACE_TYPE_LR8,

    /** Interface type MAX */
    SAI_PORT_INTERFACE_TYPE_MAX,

} sai_port_interface_type_t;

/**
 * @brief Attribute data for #SAI_PORT_ATTR_LINK_TRAINING_FAILURE_STATUS
 * Used for Link Training failure status and error codes
 */
typedef enum _sai_port_link_training_failure_status_t
{
    /** No Error detected */
    SAI_PORT_LINK_TRAINING_FAILURE_STATUS_NO_ERROR,

    /** Failure detected */
    SAI_PORT_LINK_TRAINING_FAILURE_STATUS_FRAME_LOCK_ERROR,

    /** SNR lower than threshold */
    SAI_PORT_LINK_TRAINING_FAILURE_STATUS_SNR_LOWER_THRESHOLD,

    /** Link training timeout */
    SAI_PORT_LINK_TRAINING_FAILURE_STATUS_TIME_OUT
} sai_port_link_training_failure_status_t;

/**
 * @brief Attribute data for #SAI_PORT_ATTR_LINK_TRAINING_RX_STATUS
 * Used for receiver status for link training
 */
typedef enum _sai_port_link_training_rx_status_t
{
    /** Receiver not trained */
    SAI_PORT_LINK_TRAINING_RX_STATUS_NOT_TRAINED,

    /** Receiver trained */
    SAI_PORT_LINK_TRAINING_RX_STATUS_TRAINED,
} sai_port_link_training_rx_status_t;

/**
 * @brief Attribute data for #SAI_PORT_ATTR_PRBS_CONFIG
 * PRBS configuration to enable transmitter, receiver or both
 */
typedef enum _sai_port_prbs_config_t
{
    /** PRBS Disable */
    SAI_PORT_PRBS_CONFIG_DISABLE,

    /** Enable both PRBS Transmitter and Receiver */
    SAI_PORT_PRBS_CONFIG_ENABLE_TX_RX,

    /** Enable PRBS Receiver */
    SAI_PORT_PRBS_CONFIG_ENABLE_RX,

    /** Enable PRBS Transmitter */
    SAI_PORT_PRBS_CONFIG_ENABLE_TX
} sai_port_prbs_config_t;

/**
 * @brief Attribute data for #SAI_PORT_CONNECTOR_ATTR_FAILOVER_MODE
 * Used for Failover mode configuration on port
 */
typedef enum _sai_port_connector_failover_mode_t
{
    /** Failover mode disable */
    SAI_PORT_CONNECTOR_FAILOVER_MODE_DISABLE,

    /** Configure Failover mode on primary port */
    SAI_PORT_CONNECTOR_FAILOVER_MODE_PRIMARY,

    /** Configure Failover mode on secondary port */
    SAI_PORT_CONNECTOR_FAILOVER_MODE_SECONDARY
} sai_port_connector_failover_mode_t;

/**
 * @brief Attribute data for #SAI_PORT_ATTR_MDIX_MODE_STATUS
 * Used for MDIX mode status
 */
typedef enum _sai_port_mdix_mode_status_t
{
    /** MDIX mode status straight */
    SAI_PORT_MDIX_MODE_STATUS_STRAIGHT,

    /**  MDIX mode status cross over */
    SAI_PORT_MDIX_MODE_STATUS_CROSSOVER
} sai_port_mdix_mode_status_t;

/**
 * @brief Attribute data for #SAI_PORT_ATTR_MDIX_MODE_CONFIG
 * Used for MDIX mode configuration
 */
typedef enum _sai_port_mdix_mode_config_t
{
    /** MDIX mode status auto */
    SAI_PORT_MDIX_MODE_CONFIG_AUTO,

    /** MDIX mode status straight */
    SAI_PORT_MDIX_MODE_CONFIG_STRAIGHT,

    /**  MDIX mode status cross over */
    SAI_PORT_MDIX_MODE_CONFIG_CROSSOVER
} sai_port_mdix_mode_config_t;

/**
 * @brief Attribute data for #SAI_PORT_ATTR_AUTO_NEG_CONFIG_MODE
 * Used for auto negotiation mode to configure master or slave mode
 */
typedef enum _sai_port_auto_neg_config_mode_t
{
    /** Auto neg configuration mode disabled */
    SAI_PORT_AUTO_NEG_CONFIG_MODE_DISABLED,

    /** Auto neg mode auto */
    SAI_PORT_AUTO_NEG_CONFIG_MODE_AUTO,

    /** Auto neg mode slave */
    SAI_PORT_AUTO_NEG_CONFIG_MODE_SLAVE,

    /** Auto neg mode master */
    SAI_PORT_AUTO_NEG_CONFIG_MODE_MASTER
} sai_port_auto_neg_config_mode_t;

/**
 * @brief Attribute data for #SAI_PORT_ATTR_MODULE_TYPE
 * Used for configuring Fiber module type
 */
typedef enum _sai_port_module_type_t
{
    /** Module Type Fiber */
    SAI_PORT_MODULE_TYPE_1000BASE_X,

    /** Module Type 100FX */
    SAI_PORT_MODULE_TYPE_100FX,

    /** Module Type SGMII-Slave */
    SAI_PORT_MODULE_TYPE_SGMII_SLAVE,
} sai_port_module_type_t;

/**
 * @brief Attribute data for #SAI_PORT_ATTR_DUAL_MEDIA
 * Used to configure media type for dual media supported PHY
 */
typedef enum _sai_port_dual_media_t
{
    /**  Dual media not supported */
    SAI_PORT_DUAL_MEDIA_NONE,

    /**  Force Copper mode, Fiber is inactive/disabled */
    SAI_PORT_DUAL_MEDIA_COPPER_ONLY,

    /**  Force Fiber mode, Copper is inactive/disabled */
    SAI_PORT_DUAL_MEDIA_FIBER_ONLY,

    /**  Both Copper and Fiber supported, but Copper preferred */
    SAI_PORT_DUAL_MEDIA_COPPER_PREFERRED,

    /**  Both Copper and Fiber supported, but Fiber preferred */
    SAI_PORT_DUAL_MEDIA_FIBER_PREFERRED
} sai_port_dual_media_t;

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
     * @type sai_s32_list_t sai_port_breakout_mode_type_t
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
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_QUEUE
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
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_SCHEDULER_GROUP
     */
    SAI_PORT_ATTR_QOS_SCHEDULER_GROUP_LIST,

    /**
     * @brief The sum of the headroom size of the ingress priority groups belonging to this port
     *        should not exceed the SAI_PORT_ATTR_QOS_MAXIMUM_HEADROOM_SIZE value.
     *
     * This attribute is applicable only for per-port, per-PG headroom model
     * (which means SAI_BUFFER_POOL_ATTR_XOFF_SIZE is zero)
     *
     * For the platforms which don't have this limitation, 0 should be returned.
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_QOS_MAXIMUM_HEADROOM_SIZE,

    /**
     * @brief Query list of supported port speed(full-duplex) in Mbps
     *
     * @type sai_u32_list_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_SUPPORTED_SPEED,

    /**
     * @brief Query list of supported port FEC mode
     *
     * @type sai_s32_list_t sai_port_fec_mode_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_SUPPORTED_FEC_MODE,

    /**
     * @brief Query extended list of supported port FEC modes
     *
     * @type sai_s32_list_t sai_port_fec_mode_extended_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_SUPPORTED_FEC_MODE_EXTENDED,

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
     * @brief Query list of Advertised remote port speed (Full-Duplex) in Mbps
     *
     * @type sai_u32_list_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_REMOTE_ADVERTISED_SPEED,

    /**
     * @brief Query list of Advertised remote port FEC control
     *
     * @type sai_s32_list_t sai_port_fec_mode_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_REMOTE_ADVERTISED_FEC_MODE,

    /**
     * @brief Query extended list of Advertised remote port FEC control
     *
     * @type sai_s32_list_t sai_port_fec_mode_extended_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_REMOTE_ADVERTISED_FEC_MODE_EXTENDED,

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
     * @brief Query Remote port Advertised OUI Code
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_REMOTE_ADVERTISED_OUI_CODE,

    /**
     * @brief Number of ingress priority groups
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_NUMBER_OF_INGRESS_PRIORITY_GROUPS,

    /**
     * @brief List of ingress priority groups
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_INGRESS_PRIORITY_GROUP
     */
    SAI_PORT_ATTR_INGRESS_PRIORITY_GROUP_LIST,

    /**
     * @brief List of port's lanes eye values
     *
     * @type sai_port_eye_values_list_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_EYE_VALUES,

    /**
     * @brief Operational speed in Mbps
     *
     * If port is down, the returned value should be zero.
     * If auto negotiation is on, the returned value should be the negotiated speed.
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_OPER_SPEED,

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
     * On get, returns the configured port speed.
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
     * @flags CREATE_AND_SET
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
     * @flags CREATE_AND_SET
     * @default SAI_PORT_MEDIA_TYPE_NOT_PRESENT
     */
    SAI_PORT_ATTR_MEDIA_TYPE,

    /**
     * @brief Query/Configure list of Advertised port speed (Full-Duplex) in Mbps
     *
     * Used when auto negotiation is on. Empty list means all supported values are enabled.
     *
     * @type sai_u32_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PORT_ATTR_ADVERTISED_SPEED,

    /**
     * @brief Query/Configure list of Advertised port FEC Mode
     *
     * @type sai_s32_list_t sai_port_fec_mode_t
     * @flags CREATE_AND_SET
     * @default empty
     * @validonly SAI_PORT_ATTR_USE_EXTENDED_FEC == false
     */
    SAI_PORT_ATTR_ADVERTISED_FEC_MODE,

    /**
     * @brief Query/Configure extended list of Advertised port FEC Mode
     *
     * @type sai_s32_list_t sai_port_fec_mode_extended_t
     * @flags CREATE_AND_SET
     * @default empty
     * @validonly SAI_PORT_ATTR_USE_EXTENDED_FEC == true
     */
    SAI_PORT_ATTR_ADVERTISED_FEC_MODE_EXTENDED,

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
     * @default SAI_PORT_MEDIA_TYPE_UNKNOWN
     */
    SAI_PORT_ATTR_ADVERTISED_MEDIA_TYPE,

    /**
     * @brief Query/Configure Port's Advertised OUI code
     *
     * Organizationally Unique Identifier for 25G/50G auto negotiation.
     * Default is 0x6A737D for Ethernet Consortium
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0x6A737D
     * @validonly SAI_PORT_ATTR_SPEED == 25000 or SAI_PORT_ATTR_SPEED == 50000
     */
    SAI_PORT_ATTR_ADVERTISED_OUI_CODE,

    /**
     * @brief Port VLAN ID
     *
     * Untagged ingress frames are tagged with Port VLAN ID (PVID).
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan true
     * @default 1
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
     * To be deprecated, use #SAI_PORT_ATTR_LOOPBACK_MODE
     *
     * @type sai_port_internal_loopback_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_PORT_INTERNAL_LOOPBACK_MODE_NONE
     */
    SAI_PORT_ATTR_INTERNAL_LOOPBACK_MODE,

    /**
     * @brief Forward Error Correction (FEC) extended control
     *
     * Enables use of extended FEC controls as opposed to their non-extended
     * counterparts.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PORT_ATTR_USE_EXTENDED_FEC,

    /**
     * @brief Forward Error Correction (FEC) control
     *
     * @type sai_port_fec_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_PORT_FEC_MODE_NONE
     * @validonly SAI_PORT_ATTR_USE_EXTENDED_FEC == false
     */
    SAI_PORT_ATTR_FEC_MODE,

    /**
     * @brief Forward Error Correction (FEC) extended control
     *
     * @type sai_port_fec_mode_extended_t
     * @flags CREATE_AND_SET
     * @default SAI_PORT_FEC_MODE_EXTENDED_NONE
     * @validonly SAI_PORT_ATTR_USE_EXTENDED_FEC == true
     */
    SAI_PORT_ATTR_FEC_MODE_EXTENDED,

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
     * storm control policer on port.
     *
     * Set policer id = #SAI_NULL_OBJECT_ID to disable policer on port.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_POLICER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_FLOOD_STORM_CONTROL_POLICER_ID,

    /**
     * @brief Enable broadcast storm control policer on port.
     *
     * Set Policer id = #SAI_NULL_OBJECT_ID to disable policer on port.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_POLICER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_BROADCAST_STORM_CONTROL_POLICER_ID,

    /**
     * @brief Enable multicast storm control policer on port.
     *
     * Set policer id = #SAI_NULL_OBJECT_ID to disable policer on port.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_POLICER
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
     * Bind (or unbind) an ingress ACL table or ACL group on a port.
     * Enable/Update ingress ACL table or ACL group filtering by assigning
     * a valid object id. Disable ingress filtering by assigning
     * SAI_NULL_OBJECT_ID in the attribute value.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_ACL_TABLE, SAI_OBJECT_TYPE_ACL_TABLE_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_INGRESS_ACL,

    /**
     * @brief Port bind point for egress ACL object
     *
     * Bind (or unbind) an egress ACL tables or ACL group on a port.
     * Enable/Update egress ACL table or ACL group filtering by assigning
     * a valid object id. Disable egress filtering by assigning
     * SAI_NULL_OBJECT_ID in the attribute value.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_ACL_TABLE, SAI_OBJECT_TYPE_ACL_TABLE_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_EGRESS_ACL,

    /**
     * @brief Port bind point for ingress MACsec ACL object
     *
     * Bind (or unbind) an ingress MACsec ACL table on a port.
     * Enable/Update ingress MACsec ACL table filtering by assigning
     * a valid object id. Disable ingress filtering by assigning
     * SAI_NULL_OBJECT_ID in the attribute value.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_ACL_TABLE
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_INGRESS_MACSEC_ACL,

    /**
     * @brief Port bind point for egress MACsec ACL object
     *
     * Bind (or unbind) an egress MACsec ACL tables on a port.
     * Enable/Update egress MACsec ACL table filtering by assigning
     * a valid object id. Disable egress filtering by assigning
     * SAI_NULL_OBJECT_ID in the attribute value.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_ACL_TABLE
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_EGRESS_MACSEC_ACL,

    /**
     * @brief List of MACsec ports
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_MACSEC_PORT
     */
    SAI_PORT_ATTR_MACSEC_PORT_LIST,

    /**
     * @brief Enable/Disable Mirror session
     *
     * Enable ingress mirroring by assigning list of mirror session object id
     * as attribute value, disable ingress mirroring by assigning object_count
     * as 0 in objlist.
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_MIRROR_SESSION
     * @default empty
     */
    SAI_PORT_ATTR_INGRESS_MIRROR_SESSION,

    /**
     * @brief Enable/Disable Mirror session
     *
     * Enable egress mirroring by assigning list of mirror session object id as
     * attribute value Disable egress mirroring by assigning object_count as 0
     * in objlist.
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_MIRROR_SESSION
     * @default empty
     */
    SAI_PORT_ATTR_EGRESS_MIRROR_SESSION,

    /**
     * @brief Enable/Disable Samplepacket session
     *
     * Enable ingress sampling by assigning samplepacket object id Disable
     * ingress sampling by assigning #SAI_NULL_OBJECT_ID as attribute value.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_SAMPLEPACKET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_INGRESS_SAMPLEPACKET_ENABLE,

    /**
     * @brief Enable/Disable Samplepacket session
     *
     * Enable egress sampling by assigning samplepacket object id Disable
     * egress sampling by assigning #SAI_NULL_OBJECT_ID as attribute value.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_SAMPLEPACKET
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_EGRESS_SAMPLEPACKET_ENABLE,

    /**
     * @brief Enable/Disable Samplepacket session
     *
     * Enable sample ingress mirroring by assigning list of mirror object ids Disable
     * sample ingress mirroring by assigning object_count as 0 in objlist
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_MIRROR_SESSION
     * @default empty
     */
    SAI_PORT_ATTR_INGRESS_SAMPLE_MIRROR_SESSION,

    /**
     * @brief Enable/Disable Samplepacket session
     *
     * Enable sample egress mirroring by assigning list of mirror object ids Disable
     * sample egress mirroring by assigning object_count as 0 in objlist
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_MIRROR_SESSION
     * @default empty
     */
    SAI_PORT_ATTR_EGRESS_SAMPLE_MIRROR_SESSION,

    /**
     * @brief Attach/Detach policer to port
     *
     * Set policer id = #SAI_NULL_OBJECT_ID to disable policer on port.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_POLICER
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
     * Map id = #SAI_NULL_OBJECT_ID to disable map on port.
     * To enable/disable trust Dot1p, map ID should be added/removed on port.
     * Default no map.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_DOT1P_TO_TC_MAP,

    /**
     * @brief Enable DOT1P -> COLOR MAP on port
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on port.
     * To enable/disable trust Dot1p, map ID should be added/removed on port.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_DOT1P_TO_COLOR_MAP,

    /**
     * @brief Enable DSCP -> TC MAP on port
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on port.
     * To enable/disable trust DSCP, map ID should be added/removed on port.
     * Default no map.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_DSCP_TO_TC_MAP,

    /**
     * @brief Enable DSCP -> COLOR MAP on port
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on port.
     * To enable/disable trust DSCP, map ID should be added/removed on port.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_DSCP_TO_COLOR_MAP,

    /**
     * @brief Enable TC -> Queue MAP on port
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on port.
     * Default no map, i.e. all packets to queue 0.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_TC_TO_QUEUE_MAP,

    /**
     * @brief Enable TC AND COLOR -> DOT1P MAP
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on port.
     * Default no map.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_TC_AND_COLOR_TO_DOT1P_MAP,

    /**
     * @brief Enable TC AND COLOR -> DSCP MAP
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on port.
     * Default no map.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
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
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
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
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_PFC_PRIORITY_TO_PRIORITY_GROUP_MAP,

    /**
     * @brief Enable PFC Priority -> Queue MAP.
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on port.
     * Default no map
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_PFC_PRIORITY_TO_QUEUE_MAP,

    /**
     * @brief Scheduler for port, Default no limits.
     *
     * #SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE & #SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE
     * attributes alone valid. Rest will be ignored.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_SCHEDULER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_SCHEDULER_PROFILE_ID,

    /**
     * @brief Ingress buffer profiles for port
     *
     * There can be up to #SAI_SWITCH_ATTR_INGRESS_BUFFER_POOL_NUM profiles.
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_BUFFER_PROFILE
     * @default empty
     */
    SAI_PORT_ATTR_QOS_INGRESS_BUFFER_PROFILE_LIST,

    /**
     * @brief Egress buffer profiles for port
     *
     * There can be up to #SAI_SWITCH_ATTR_EGRESS_BUFFER_POOL_NUM profiles.
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_BUFFER_PROFILE
     * @default empty
     */
    SAI_PORT_ATTR_QOS_EGRESS_BUFFER_PROFILE_LIST,

    /**
     * @brief Combined or separate Bit vectors for port PFC RX/TX
     *
     * @type sai_port_priority_flow_control_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_PORT_PRIORITY_FLOW_CONTROL_MODE_COMBINED
     */
    SAI_PORT_ATTR_PRIORITY_FLOW_CONTROL_MODE,

    /**
     * @brief Bit vector enable/disable port PFC
     *
     * Valid from bit 0 to bit 7, for combined RX/TX control mode
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_PORT_ATTR_PRIORITY_FLOW_CONTROL_MODE == SAI_PORT_PRIORITY_FLOW_CONTROL_MODE_COMBINED
     */
    SAI_PORT_ATTR_PRIORITY_FLOW_CONTROL,

    /**
     * @brief Bit vector enable/disable port PFC RX
     *
     * Valid from bit 0 to bit 7, for separate RX/TX control mode
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_PORT_ATTR_PRIORITY_FLOW_CONTROL_MODE == SAI_PORT_PRIORITY_FLOW_CONTROL_MODE_SEPARATE
     */
    SAI_PORT_ATTR_PRIORITY_FLOW_CONTROL_RX,

    /**
     * @brief Bit vector enable/disable port PFC TX
     *
     * Valid from bit 0 to bit 7, for separate RX/TX control mode
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_PORT_ATTR_PRIORITY_FLOW_CONTROL_MODE == SAI_PORT_PRIORITY_FLOW_CONTROL_MODE_SEPARATE
     */
    SAI_PORT_ATTR_PRIORITY_FLOW_CONTROL_TX,

    /**
     * @brief User based Meta Data
     *
     * Value Range #SAI_SWITCH_ATTR_PORT_USER_META_DATA_RANGE.
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_PORT_ATTR_META_DATA,

    /**
     * @brief Egress block port list
     *
     * Needs to be deprecated. Isolation group can be used instead.
     *
     * Traffic ingressing on this port and egressing out of the ports in the
     * given port list will be dropped.
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_PORT
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
     * Energy Efficient Ethernet(EEE) is an IEEE 802.3az standard aiming to
     * reduce power consumption on Ethernet ports (native copper ports).
     * Enable the EEE on port level.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PORT_ATTR_EEE_ENABLE,

    /**
     * @brief Port EEE IDLE time configuration
     *
     * Time (in microseconds) to move to Low power state (No traffic), at the
     * end of which MAC transitions to Low power state. Max value set more
     * benefit.
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 2500
     */
    SAI_PORT_ATTR_EEE_IDLE_TIME,

    /**
     * @brief Port EEE Wakeup time configuration
     *
     * Time (in microseconds) to wait before transmitter is leaving Low Power
     * Mode State. Min value set avoid latency.
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 5
     */
    SAI_PORT_ATTR_EEE_WAKE_TIME,

    /**
     * @brief List of port pools for the port
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_PORT_POOL
     */
    SAI_PORT_ATTR_PORT_POOL_LIST,

    /**
     * @brief Isolation group id
     *
     * Packets ingressing on the port should not be forwarded to the
     * members present in the isolation group.The isolation group type
     * should be SAI_ISOLATION_GROUP_TYPE_PORT.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_ISOLATION_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_ISOLATION_GROUP,

    /**
     * @brief Port packet transmission enable
     *
     * Enable/Disable packet transmission of a port. When packet transmission
     * is disabled on a port, packets are still subject to regular ingress and egress
     * admission control to determine the actions on a packet: whether it is
     * dropped (immediately or after timeout), or whether it is kept in buffers
     * internal to the switch before packet transmission is enabled on the port.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default true
     */
    SAI_PORT_ATTR_PKT_TX_ENABLE,

    /**
     * @brief Port bind point for TAM object
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_TAM
     * @default empty
     */
    SAI_PORT_ATTR_TAM_OBJECT,

    /**
     * @brief Port serdes control pre-emphasis
     *
     * Deprecated. Use SAI_OBJECT_TYPE_PORT_SERDES
     * List of port serdes pre-emphasis values. The values are of type sai_u32_list_t
     * where the count is number lanes in a port and the list specifies list of values
     * to be applied to each lane.
     *
     * @type sai_u32_list_t
     * @flags CREATE_AND_SET
     * @default internal
     * @deprecated true
     */
    SAI_PORT_ATTR_SERDES_PREEMPHASIS,

    /**
     * @brief Port serdes control idriver
     *
     * Deprecated. Use SAI_OBJECT_TYPE_PORT_SERDES
     * List of port serdes idriver values. The values are of type sai_u32_list_t
     * where the count is number lanes in a port and the list specifies list of values
     * to be applied to each lane.
     *
     * @type sai_u32_list_t
     * @flags CREATE_AND_SET
     * @default internal
     * @deprecated true
     */
    SAI_PORT_ATTR_SERDES_IDRIVER,

    /**
     * @brief Port serdes control ipredriver
     *
     * Deprecated. Use SAI_OBJECT_TYPE_PORT_SERDES
     * List of port serdes ipredriver values. The values are of type sai_u32_list_t
     * where the count is number lanes in a port and the list specifies list of values
     * to be applied to each lane.
     *
     * @type sai_u32_list_t
     * @flags CREATE_AND_SET
     * @default internal
     * @deprecated true
     */
    SAI_PORT_ATTR_SERDES_IPREDRIVER,

    /**
     * @brief Enable/Disable Port Link Training
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PORT_ATTR_LINK_TRAINING_ENABLE,

    /**
     * @brief Configure PTP mode on the port
     *
     * @type sai_port_ptp_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_PORT_PTP_MODE_NONE
     */
    SAI_PORT_ATTR_PTP_MODE,

    /**
     * @brief Configure Interface type
     *
     * @type sai_port_interface_type_t
     * @flags CREATE_AND_SET
     * @default SAI_PORT_INTERFACE_TYPE_NONE
     */
    SAI_PORT_ATTR_INTERFACE_TYPE,

    /**
     * @brief Configure advertised interface type list
     *
     * Used when auto negotiation is on. Empty list means all supported values are enabled.
     *
     * @type sai_s32_list_t sai_port_interface_type_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PORT_ATTR_ADVERTISED_INTERFACE_TYPE,

    /**
     * @brief Configure port reference clock in hertz
     *
     * @type sai_uint64_t
     * @flags CREATE_ONLY
     * @default internal
     */
    SAI_PORT_ATTR_REFERENCE_CLOCK,

    /**
     * @brief Port PRBS Polynomial
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default internal
     */
    SAI_PORT_ATTR_PRBS_POLYNOMIAL,

    /**
     * @brief Serdes object ID for the port
     *
     * @type sai_object_id_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_PORT_SERDES
     * @default internal
     */
    SAI_PORT_ATTR_PORT_SERDES_ID,

    /**
     * @brief Link training failure status and error codes
     *
     * @type sai_port_link_training_failure_status_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_LINK_TRAINING_FAILURE_STATUS,

    /**
     * @brief Status whether the receiver trained or not trained to receive data
     *
     * @type sai_port_link_training_rx_status_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_LINK_TRAINING_RX_STATUS,

    /**
     * @brief Attribute data for #SAI_PORT_ATTR_PRBS_CONFIG
     *
     * PRBS configuration to enable transmitter, receiver or both
     *
     * @type sai_port_prbs_config_t
     * @flags CREATE_AND_SET
     * @default SAI_PORT_PRBS_CONFIG_DISABLE
     */
    SAI_PORT_ATTR_PRBS_CONFIG,

    /**
     * @brief Attribute data for #SAI_PORT_ATTR_PRBS_LOCK_STATUS
     *
     * PRBS lock status: 1 for locked, 0 for unlocked
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_PRBS_LOCK_STATUS,

    /**
     * @brief Attribute data for #SAI_PORT_ATTR_PRBS_LOCK_LOSS_STATUS
     *
     * PRBS unlocked status since last read: 1 for lock loss, 0 for no lock loss
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_PRBS_LOCK_LOSS_STATUS,

    /**
     * @brief Attribute data for #SAI_PORT_ATTR_PRBS_RX_STATUS
     *
     * @type sai_port_prbs_rx_status_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_PRBS_RX_STATUS,

    /**
     * @brief Attribute data for #SAI_PORT_ATTR_PRBS_RX_STATE
     * Used for clear on read status/count register.
     * Adapter should return SAI_STATUS_NOT_SUPPORTED if not supported.
     *
     * @type sai_prbs_rx_state_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_PRBS_RX_STATE,

    /**
     * @brief Attribute data for #SAI_PORT_ATTR_AUTO_NEG_STATUS
     *
     * Auto negotiation (AN) done state: 0 for AN in progress, 0 for AN done
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_AUTO_NEG_STATUS,

    /**
     * @brief To enable/disable Decrement TTL
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PORT_ATTR_DISABLE_DECREMENT_TTL,

    /**
     * @brief Enable EXP -> TC MAP on port
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on port.
     * To enable/disable trust EXP, map ID should be added/removed on port.
     * Default no map.
     * If present overrides SAI_SWITCH_ATTR_QOS_MPLS_EXP_TO_TC_MAP.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_MPLS_EXP_TO_TC_MAP,

    /**
     * @brief Enable EXP -> COLOR MAP on port
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on port.
     * To enable/disable trust EXP, map ID should be added/removed on port.
     * Default no map.
     * If present overrides SAI_SWITCH_ATTR_QOS_MPLS_EXP_TO_COLOR_MAP.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_MPLS_EXP_TO_COLOR_MAP,

    /**
     * @brief Enable TC AND COLOR -> EXP MAP
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on port.
     * Default no map.
     * If present overrides SAI_SWITCH_ATTR_QOS_TC_AND_COLOR_TO_MPLS_EXP_MAP.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_TC_AND_COLOR_TO_MPLS_EXP_MAP,

    /**
     * @brief TPID
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0x8100
     */
    SAI_PORT_ATTR_TPID,

    /**
     * @brief Port Down Error Status
     *
     * @type sai_port_err_status_list_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_ERR_STATUS_LIST,

    /**
     * @brief Fabric port Attached
     *
     * Signifies if the fabric port is attached
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_FABRIC_ATTACHED,

    /**
     * @brief Attached Switch type.
     *
     * Signifies the attached switch type
     *
     * @type sai_switch_type_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_FABRIC_ATTACHED_SWITCH_TYPE,

    /**
     * @brief Attached Switch ID.
     *
     * Signifies the attached switch ID
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_FABRIC_ATTACHED_SWITCH_ID,

    /**
     * @brief Attached Port Index.
     *
     * Signifies the attached port index
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_FABRIC_ATTACHED_PORT_INDEX,

    /**
     * @brief Fabric port reachability
     *
     * @type sai_fabric_port_reachability_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_FABRIC_REACHABILITY,

    /**
     * @brief System port for the port
     *
     * @type sai_object_id_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_SYSTEM_PORT
     */
    SAI_PORT_ATTR_SYSTEM_PORT,

    /**
     * @brief FEC mode auto-negotiation override status
     *
     * If set to true, any auto-negotiated FEC mode will be
     * overridden by the value configured in SAI_PORT_ATTR_FEC_MODE
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PORT_ATTR_AUTO_NEG_FEC_MODE_OVERRIDE,

    /**
     * @brief Internal or External loopback control
     *
     * @type sai_port_loopback_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_PORT_LOOPBACK_MODE_NONE
     */
    SAI_PORT_ATTR_LOOPBACK_MODE,

    /**
     * @brief MDIX mode status for the port
     *
     * @type sai_port_mdix_mode_status_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_MDIX_MODE_STATUS,

    /**
     * @brief MDIX mode configuration for the port
     *
     * @type sai_port_mdix_mode_config_t
     * @flags CREATE_AND_SET
     * @default SAI_PORT_MDIX_MODE_CONFIG_AUTO
     */
    SAI_PORT_ATTR_MDIX_MODE_CONFIG,

    /**
     * @brief Configure auto negotiation configuration mode for the port
     *
     * @type sai_port_auto_neg_config_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_PORT_AUTO_NEG_CONFIG_MODE_DISABLED
     * @validonly SAI_PORT_ATTR_AUTO_NEG_MODE == true
     */
    SAI_PORT_ATTR_AUTO_NEG_CONFIG_MODE,

    /**
     * @brief Enable auto detection between 1000X and SGMII slave mode
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     * @validonly SAI_PORT_ATTR_MEDIA_TYPE == SAI_PORT_MEDIA_TYPE_FIBER and SAI_PORT_ATTR_SPEED == 1000
     */
    SAI_PORT_ATTR_1000X_SGMII_SLAVE_AUTODETECT,

    /**
     * @brief Configure Fiber module type
     *
     * @type sai_port_module_type_t
     * @flags CREATE_AND_SET
     * @default SAI_PORT_MODULE_TYPE_1000BASE_X
     * @validonly SAI_PORT_ATTR_MEDIA_TYPE == SAI_PORT_MEDIA_TYPE_FIBER and SAI_PORT_ATTR_SPEED == 1000
     */
    SAI_PORT_ATTR_MODULE_TYPE,

    /**
     * @brief Configure media types for dual media supported PHY
     *
     * @type sai_port_dual_media_t
     * @flags CREATE_AND_SET
     * @default SAI_PORT_DUAL_MEDIA_NONE
     */
    SAI_PORT_ATTR_DUAL_MEDIA,

    /**
     * @brief Query the Auto Negotiated Resolved FEC
     *
     * Auto negotiated FEC status is applicable on Auto Negotiation good state
     *
     * @type sai_port_fec_mode_extended_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_AUTO_NEG_FEC_MODE_EXTENDED,

    /**
     * @brief Configures inter frame gap for an ethernet port
     *
     * IPG is mandated by IEEE 802.3 to ensure reliable communication between two Ethernet devices.
     * The minimum recommended inter-packet gap(IPG) by IEEE 802.3 is 96 bits (12 bytes)
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 96
     */
    SAI_PORT_ATTR_IPG,

    /**
     * @brief Forward or terminate the global flow control(802.3X) frame
     *
     * If true, flow control frames are switched between ports.
     * If false, flow control frames are terminated by the switch.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PORT_ATTR_GLOBAL_FLOW_CONTROL_FORWARD,

    /**
     * @brief Forward or terminate the PFC(802.1Qbb) frame
     *
     * If true, flow control frames are switched between ports.
     * If false, flow control frames are terminated by the switch.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PORT_ATTR_PRIORITY_FLOW_CONTROL_FORWARD,

    /**
     * @brief Enable DSCP -> Forwarding Class MAP on port
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on port.
     * Default no map.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_DSCP_TO_FORWARDING_CLASS_MAP,

    /**
     * @brief Enable EXP -> Forwarding Class MAP on port
     *
     * Map id = #SAI_NULL_OBJECT_ID to disable map on port.
     * Default no map.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_QOS_MAP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_ATTR_QOS_MPLS_EXP_TO_FORWARDING_CLASS_MAP,

    /**
     * @brief Associated IPsec port
     *
     * @type sai_object_id_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_IPSEC_PORT
     */
    SAI_PORT_ATTR_IPSEC_PORT,

    /**
     * @brief  PFC Deadlock Detection timer interval range
     *
     * @type sai_u32_range_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_PFC_TC_DLD_INTERVAL_RANGE,

    /**
     * @brief PFC Deadlock Detection timer interval in milliseconds.
     *
     * If the monitored queue is in XOFF state for more than this duration then
     * its considered to be in a PFC deadlock state and recovery process is kicked off.
     * Note: Use TC (Traffic Class) value as key and timer interval as value.
     *
     * @type sai_map_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PORT_ATTR_PFC_TC_DLD_INTERVAL,

    /**
     * @brief  PFC Deadlock Recovery timer interval range
     *
     * @type sai_u32_range_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_PFC_TC_DLR_INTERVAL_RANGE,

    /**
     * @brief PFC Deadlock Recovery timer interval in milliseconds.
     *
     * The PFC deadlock recovery process will run for this amount of time and then normal
     * state will resume. If the system remains in a deadlock state then the detection and
     * recovery will resume again after the configured detection timer interval.
     * Note: Use TC (Traffic Class) value as key and timer interval as value.
     *
     * @type sai_map_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PORT_ATTR_PFC_TC_DLR_INTERVAL,

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
 *
 * @flags ranges
 */
typedef enum _sai_port_stat_t
{
    /** SAI port stat if in octets */
    SAI_PORT_STAT_IF_IN_OCTETS,

    /** SAI port stat if in ucast pkts */
    SAI_PORT_STAT_IF_IN_UCAST_PKTS,

    /** SAI port stat if in non ucast pkts */
    SAI_PORT_STAT_IF_IN_NON_UCAST_PKTS,

    /** SAI port stat if in discards */
    SAI_PORT_STAT_IF_IN_DISCARDS,

    /** SAI port stat if in errors */
    SAI_PORT_STAT_IF_IN_ERRORS,

    /** SAI port stat if in unknown protocols */
    SAI_PORT_STAT_IF_IN_UNKNOWN_PROTOS,

    /** SAI port stat if in broadcast pkts */
    SAI_PORT_STAT_IF_IN_BROADCAST_PKTS,

    /** SAI port stat if in multicast pkts */
    SAI_PORT_STAT_IF_IN_MULTICAST_PKTS,

    /** SAI port stat if in vlan discards */
    SAI_PORT_STAT_IF_IN_VLAN_DISCARDS,

    /** SAI port stat if out octets */
    SAI_PORT_STAT_IF_OUT_OCTETS,

    /** SAI port stat if out ucast pkts */
    SAI_PORT_STAT_IF_OUT_UCAST_PKTS,

    /** SAI port stat if out non ucast pkts */
    SAI_PORT_STAT_IF_OUT_NON_UCAST_PKTS,

    /** SAI port stat if out discards */
    SAI_PORT_STAT_IF_OUT_DISCARDS,

    /** SAI port stat if out errors */
    SAI_PORT_STAT_IF_OUT_ERRORS,

    /** SAI port stat if out queue length */
    SAI_PORT_STAT_IF_OUT_QLEN,

    /** SAI port stat if out broadcast pkts */
    SAI_PORT_STAT_IF_OUT_BROADCAST_PKTS,

    /** SAI port stat if out multicast pkts */
    SAI_PORT_STAT_IF_OUT_MULTICAST_PKTS,

    /** SAI port stat ether stats drop events */
    SAI_PORT_STAT_ETHER_STATS_DROP_EVENTS,

    /** SAI port stat ether stats multicast pkts */
    SAI_PORT_STAT_ETHER_STATS_MULTICAST_PKTS,

    /** SAI port stat ether stats broadcast pkts */
    SAI_PORT_STAT_ETHER_STATS_BROADCAST_PKTS,

    /** SAI port stat ether stats undersized pkts */
    SAI_PORT_STAT_ETHER_STATS_UNDERSIZE_PKTS,

    /** SAI port stat ether stats fragments */
    SAI_PORT_STAT_ETHER_STATS_FRAGMENTS,

    /** SAI port stat ether stats pkts 64 octets */
    SAI_PORT_STAT_ETHER_STATS_PKTS_64_OCTETS,

    /** SAI port stat ether stats pkts 65 to 127 octets */
    SAI_PORT_STAT_ETHER_STATS_PKTS_65_TO_127_OCTETS,

    /** SAI port stat ether stats pkts 128 to 255 octets */
    SAI_PORT_STAT_ETHER_STATS_PKTS_128_TO_255_OCTETS,

    /** SAI port stat ether stats pkts 256 to 511 octets */
    SAI_PORT_STAT_ETHER_STATS_PKTS_256_TO_511_OCTETS,

    /** SAI port stat ether stats pkts 512 to 1023 octets */
    SAI_PORT_STAT_ETHER_STATS_PKTS_512_TO_1023_OCTETS,

    /** SAI port stat ether stats pkts 1024 to 1518 octets */
    SAI_PORT_STAT_ETHER_STATS_PKTS_1024_TO_1518_OCTETS,

    /** SAI port stat ether stats pkts 1519 to 2047 octets */
    SAI_PORT_STAT_ETHER_STATS_PKTS_1519_TO_2047_OCTETS,

    /** SAI port stat ether stats pkts 2048 to 4095 octets */
    SAI_PORT_STAT_ETHER_STATS_PKTS_2048_TO_4095_OCTETS,

    /** SAI port stat ether stats pkts 4096 to 9216 octets */
    SAI_PORT_STAT_ETHER_STATS_PKTS_4096_TO_9216_OCTETS,

    /** SAI port stat ether stats pkts 9217 to 16383 octets */
    SAI_PORT_STAT_ETHER_STATS_PKTS_9217_TO_16383_OCTETS,

    /** SAI port stat ether stats oversize pkts */
    SAI_PORT_STAT_ETHER_STATS_OVERSIZE_PKTS,

    /** SAI port stat ether rx oversize pkts */
    SAI_PORT_STAT_ETHER_RX_OVERSIZE_PKTS,

    /** SAI port stat ether tx oversize pkts */
    SAI_PORT_STAT_ETHER_TX_OVERSIZE_PKTS,

    /** SAI port stat ether stats jabbers */
    SAI_PORT_STAT_ETHER_STATS_JABBERS,

    /** SAI port stat ether stats octets */
    SAI_PORT_STAT_ETHER_STATS_OCTETS,

    /** SAI port stat ether stats pkts */
    SAI_PORT_STAT_ETHER_STATS_PKTS,

    /** SAI port stat ether stats collisions */
    SAI_PORT_STAT_ETHER_STATS_COLLISIONS,

    /** SAI port stat ether stats CRC align errors */
    SAI_PORT_STAT_ETHER_STATS_CRC_ALIGN_ERRORS,

    /** SAI port stat ether stats tx no errors */
    SAI_PORT_STAT_ETHER_STATS_TX_NO_ERRORS,

    /** SAI port stat ether stats rx no errors */
    SAI_PORT_STAT_ETHER_STATS_RX_NO_ERRORS,

    /** SAI port stat IP in receives */
    SAI_PORT_STAT_IP_IN_RECEIVES,

    /** SAI port stat IP in octets */
    SAI_PORT_STAT_IP_IN_OCTETS,

    /** SAI port stat IP in ucast pkts */
    SAI_PORT_STAT_IP_IN_UCAST_PKTS,

    /** SAI port stat IP in non ucast pkts */
    SAI_PORT_STAT_IP_IN_NON_UCAST_PKTS,

    /** SAI port stat IP in discards */
    SAI_PORT_STAT_IP_IN_DISCARDS,

    /** SAI port stat IP out octets */
    SAI_PORT_STAT_IP_OUT_OCTETS,

    /** SAI port stat IP out ucast pkts */
    SAI_PORT_STAT_IP_OUT_UCAST_PKTS,

    /** SAI port stat IP out non ucast pkts */
    SAI_PORT_STAT_IP_OUT_NON_UCAST_PKTS,

    /** SAI port stat IP out discards */
    SAI_PORT_STAT_IP_OUT_DISCARDS,

    /** SAI port stat IPv6 in receives */
    SAI_PORT_STAT_IPV6_IN_RECEIVES,

    /** SAI port stat IPv6 in octets */
    SAI_PORT_STAT_IPV6_IN_OCTETS,

    /** SAI port stat IPv6 in ucast pkts */
    SAI_PORT_STAT_IPV6_IN_UCAST_PKTS,

    /** SAI port stat IPv6 in non ucast pkts */
    SAI_PORT_STAT_IPV6_IN_NON_UCAST_PKTS,

    /** SAI port stat IPv6 in mcast pkts */
    SAI_PORT_STAT_IPV6_IN_MCAST_PKTS,

    /** SAI port stat IPv6 in discards */
    SAI_PORT_STAT_IPV6_IN_DISCARDS,

    /** SAI port stat IPv6 out octets */
    SAI_PORT_STAT_IPV6_OUT_OCTETS,

    /** SAI port stat IPv6 out ucast pkts */
    SAI_PORT_STAT_IPV6_OUT_UCAST_PKTS,

    /** SAI port stat IPv6 out non ucast pkts */
    SAI_PORT_STAT_IPV6_OUT_NON_UCAST_PKTS,

    /** SAI port stat IPv6 out mcast pkts */
    SAI_PORT_STAT_IPV6_OUT_MCAST_PKTS,

    /** SAI port stat IPv6 out discards */
    SAI_PORT_STAT_IPV6_OUT_DISCARDS,

    /** Get/set WRED green packet count [uint64_t] */
    SAI_PORT_STAT_GREEN_WRED_DROPPED_PACKETS,

    /** Get/set WRED green byte count [uint64_t] */
    SAI_PORT_STAT_GREEN_WRED_DROPPED_BYTES,

    /** Get/set WRED yellow packet count [uint64_t] */
    SAI_PORT_STAT_YELLOW_WRED_DROPPED_PACKETS,

    /** Get/set WRED yellow byte count [uint64_t] */
    SAI_PORT_STAT_YELLOW_WRED_DROPPED_BYTES,

    /** Get/set WRED red packet count [uint64_t] */
    SAI_PORT_STAT_RED_WRED_DROPPED_PACKETS,

    /** Get/set WRED red byte count [uint64_t] */
    SAI_PORT_STAT_RED_WRED_DROPPED_BYTES,

    /** Get/set WRED dropped packets count [uint64_t] */
    SAI_PORT_STAT_WRED_DROPPED_PACKETS,

    /** Get/set WRED dropped bytes count [uint64_t] */
    SAI_PORT_STAT_WRED_DROPPED_BYTES,

    /** Get/set packets marked by ECN count [uint64_t] */
    SAI_PORT_STAT_ECN_MARKED_PACKETS,

    /** Packet size based packets count rt stat ether in pkts 64 octets */
    SAI_PORT_STAT_ETHER_IN_PKTS_64_OCTETS,

    /** SAI port stat ether in pkts 65 to 127 octets */
    SAI_PORT_STAT_ETHER_IN_PKTS_65_TO_127_OCTETS,

    /** SAI port stat ether in pkts 128 to 255 octets */
    SAI_PORT_STAT_ETHER_IN_PKTS_128_TO_255_OCTETS,

    /** SAI port stat ether in pkts 256 to 511 octets */
    SAI_PORT_STAT_ETHER_IN_PKTS_256_TO_511_OCTETS,

    /** SAI port stat ether in pkts 512 to 1023 octets */
    SAI_PORT_STAT_ETHER_IN_PKTS_512_TO_1023_OCTETS,

    /** SAI port stat ether in pkts 1024 to 1518 octets */
    SAI_PORT_STAT_ETHER_IN_PKTS_1024_TO_1518_OCTETS,

    /** SAI port stat ether in pkts 1519 to 2047 octets */
    SAI_PORT_STAT_ETHER_IN_PKTS_1519_TO_2047_OCTETS,

    /** SAI port stat ether in pkts 2048 to 4095 octets */
    SAI_PORT_STAT_ETHER_IN_PKTS_2048_TO_4095_OCTETS,

    /** SAI port stat ether in pkts 4096 to 9216 octets */
    SAI_PORT_STAT_ETHER_IN_PKTS_4096_TO_9216_OCTETS,

    /** SAI port stat ether in pkts 9217 to 16383 octets */
    SAI_PORT_STAT_ETHER_IN_PKTS_9217_TO_16383_OCTETS,

    /** SAI port stat ether out pkts 64 octets */
    SAI_PORT_STAT_ETHER_OUT_PKTS_64_OCTETS,

    /** SAI port stat ether out pkts 65 to 127 octets */
    SAI_PORT_STAT_ETHER_OUT_PKTS_65_TO_127_OCTETS,

    /** SAI port stat ether out pkts 128 to 255 octets */
    SAI_PORT_STAT_ETHER_OUT_PKTS_128_TO_255_OCTETS,

    /** SAI port stat ether out pkts 256 to 511 octets */
    SAI_PORT_STAT_ETHER_OUT_PKTS_256_TO_511_OCTETS,

    /** SAI port stat ether out pkts 512 to 1023 octets */
    SAI_PORT_STAT_ETHER_OUT_PKTS_512_TO_1023_OCTETS,

    /** SAI port stat ether out pkts 1024 to 1518 octets */
    SAI_PORT_STAT_ETHER_OUT_PKTS_1024_TO_1518_OCTETS,

    /** SAI port stat ether out pkts 1519 to 2047 octets */
    SAI_PORT_STAT_ETHER_OUT_PKTS_1519_TO_2047_OCTETS,

    /** SAI port stat ether out pkts 2048 to 4095 octets */
    SAI_PORT_STAT_ETHER_OUT_PKTS_2048_TO_4095_OCTETS,

    /** SAI port stat ether out pkts 4096 to 9216 octets */
    SAI_PORT_STAT_ETHER_OUT_PKTS_4096_TO_9216_OCTETS,

    /** SAI port stat ether out pkts 9217 to 16383 octets */
    SAI_PORT_STAT_ETHER_OUT_PKTS_9217_TO_16383_OCTETS,

    /** Get in port current occupancy in bytes [uint64_t] */
    SAI_PORT_STAT_IN_CURR_OCCUPANCY_BYTES,

    /** Get in port watermark occupancy in bytes [uint64_t] */
    SAI_PORT_STAT_IN_WATERMARK_BYTES,

    /** Get in port current shared occupancy in bytes [uint64_t] */
    SAI_PORT_STAT_IN_SHARED_CURR_OCCUPANCY_BYTES,

    /** Get in port watermark shared occupancy in bytes [uint64_t] */
    SAI_PORT_STAT_IN_SHARED_WATERMARK_BYTES,

    /** Get out port current occupancy in bytes [uint64_t] */
    SAI_PORT_STAT_OUT_CURR_OCCUPANCY_BYTES,

    /** Get out port watermark occupancy in bytes [uint64_t] */
    SAI_PORT_STAT_OUT_WATERMARK_BYTES,

    /** Get out port current shared occupancy in bytes [uint64_t] */
    SAI_PORT_STAT_OUT_SHARED_CURR_OCCUPANCY_BYTES,

    /** Get out port watermark shared occupancy in bytes [uint64_t] */
    SAI_PORT_STAT_OUT_SHARED_WATERMARK_BYTES,

    /** Get in port packet drops due to buffers [uint64_t] */
    SAI_PORT_STAT_IN_DROPPED_PKTS,

    /** Get out port packet drops due to buffers [uint64_t] */
    SAI_PORT_STAT_OUT_DROPPED_PKTS,

    /** Get the number of pause frames received on the port [uint64_t] */
    SAI_PORT_STAT_PAUSE_RX_PKTS,

    /** Get the number of pause frames transmitted on the port [uint64_t] */
    SAI_PORT_STAT_PAUSE_TX_PKTS,

    /** PFC Packet Counters for RX and TX per PFC priority [uint64_t] */
    SAI_PORT_STAT_PFC_0_RX_PKTS,

    /** SAI port stat PFC 0 tx pkts */
    SAI_PORT_STAT_PFC_0_TX_PKTS,

    /** SAI port stat PFC 1 rx pkts */
    SAI_PORT_STAT_PFC_1_RX_PKTS,

    /** SAI port stat PFC 1 tx pkts */
    SAI_PORT_STAT_PFC_1_TX_PKTS,

    /** SAI port stat PFC 2 rx pkts */
    SAI_PORT_STAT_PFC_2_RX_PKTS,

    /** SAI port stat PFC 2 tx pkts */
    SAI_PORT_STAT_PFC_2_TX_PKTS,

    /** SAI port stat PFC 3 rx pkts */
    SAI_PORT_STAT_PFC_3_RX_PKTS,

    /** SAI port stat PFC 3 tx pkts */
    SAI_PORT_STAT_PFC_3_TX_PKTS,

    /** SAI port stat PFC 4 rx pkts */
    SAI_PORT_STAT_PFC_4_RX_PKTS,

    /** SAI port stat PFC 4 tx pkts */
    SAI_PORT_STAT_PFC_4_TX_PKTS,

    /** SAI port stat PFC 5 rx pkts */
    SAI_PORT_STAT_PFC_5_RX_PKTS,

    /** SAI port stat PFC 5 tx pkts */
    SAI_PORT_STAT_PFC_5_TX_PKTS,

    /** SAI port stat PFC 6 rx pkts */
    SAI_PORT_STAT_PFC_6_RX_PKTS,

    /** SAI port stat PFC 6 tx pkts */
    SAI_PORT_STAT_PFC_6_TX_PKTS,

    /** SAI port stat PFC 7 rx pkts */
    SAI_PORT_STAT_PFC_7_RX_PKTS,

    /** SAI port stat PFC 7 tx pkts */
    SAI_PORT_STAT_PFC_7_TX_PKTS,

    /**
     * @brief PFC pause duration for RX and TX per PFC priority [uint64_t]
     *
     * RX pause duration for certain priority is a the duration quanta in ingress pause
     * frame for that priority (a pause frame received by the switch).
     * While TX pause duration for certain priority is the duration quanta in egress pause
     * frame for that priority (a pause frame sent by the switch).
     */
    SAI_PORT_STAT_PFC_0_RX_PAUSE_DURATION,

    /** SAI port stat PFC 0 tx duration */
    SAI_PORT_STAT_PFC_0_TX_PAUSE_DURATION,

    /** SAI port stat PFC 1 rx duration */
    SAI_PORT_STAT_PFC_1_RX_PAUSE_DURATION,

    /** SAI port stat PFC 1 tx duration */
    SAI_PORT_STAT_PFC_1_TX_PAUSE_DURATION,

    /** SAI port stat PFC 2 rx duration */
    SAI_PORT_STAT_PFC_2_RX_PAUSE_DURATION,

    /** SAI port stat PFC 2 tx duration */
    SAI_PORT_STAT_PFC_2_TX_PAUSE_DURATION,

    /** SAI port stat PFC 3 rx duration */
    SAI_PORT_STAT_PFC_3_RX_PAUSE_DURATION,

    /** SAI port stat PFC 3 tx duration */
    SAI_PORT_STAT_PFC_3_TX_PAUSE_DURATION,

    /** SAI port stat PFC 4 rx duration */
    SAI_PORT_STAT_PFC_4_RX_PAUSE_DURATION,

    /** SAI port stat PFC 4 tx duration */
    SAI_PORT_STAT_PFC_4_TX_PAUSE_DURATION,

    /** SAI port stat PFC 5 rx duration */
    SAI_PORT_STAT_PFC_5_RX_PAUSE_DURATION,

    /** SAI port stat PFC 5 tx duration */
    SAI_PORT_STAT_PFC_5_TX_PAUSE_DURATION,

    /** SAI port stat PFC 6 rx duration */
    SAI_PORT_STAT_PFC_6_RX_PAUSE_DURATION,

    /** SAI port stat PFC 6 tx duration */
    SAI_PORT_STAT_PFC_6_TX_PAUSE_DURATION,

    /** SAI port stat PFC 7 rx duration */
    SAI_PORT_STAT_PFC_7_RX_PAUSE_DURATION,

    /** SAI port stat PFC 7 tx duration */
    SAI_PORT_STAT_PFC_7_TX_PAUSE_DURATION,

    /**
     * @brief PFC pause duration for RX and TX per PFC priority in micro seconds [uint64_t]
     *
     * RX pause duration for certain priority is a the duration in micro seconds converted
     * from quanta in ingress pause frame for that priority (a pause frame received by the
     * switch).
     * While TX pause duration for certain priority is the duration in micro seconds converted
     * from quanta in egress pause frame for that priority (a pause frame sent by the switch).
     */
    SAI_PORT_STAT_PFC_0_RX_PAUSE_DURATION_US,

    /** SAI port stat PFC 0 tx duration in micro seconds */
    SAI_PORT_STAT_PFC_0_TX_PAUSE_DURATION_US,

    /** SAI port stat PFC 1 rx duration in micro seconds */
    SAI_PORT_STAT_PFC_1_RX_PAUSE_DURATION_US,

    /** SAI port stat PFC 1 tx duration in micro seconds */
    SAI_PORT_STAT_PFC_1_TX_PAUSE_DURATION_US,

    /** SAI port stat PFC 2 rx duration in micro seconds */
    SAI_PORT_STAT_PFC_2_RX_PAUSE_DURATION_US,

    /** SAI port stat PFC 2 tx duration in micro seconds */
    SAI_PORT_STAT_PFC_2_TX_PAUSE_DURATION_US,

    /** SAI port stat PFC 3 rx duration in micro seconds */
    SAI_PORT_STAT_PFC_3_RX_PAUSE_DURATION_US,

    /** SAI port stat PFC 3 tx duration in micro seconds */
    SAI_PORT_STAT_PFC_3_TX_PAUSE_DURATION_US,

    /** SAI port stat PFC 4 rx duration in micro seconds */
    SAI_PORT_STAT_PFC_4_RX_PAUSE_DURATION_US,

    /** SAI port stat PFC 4 tx duration in micro seconds */
    SAI_PORT_STAT_PFC_4_TX_PAUSE_DURATION_US,

    /** SAI port stat PFC 5 rx duration in micro seconds */
    SAI_PORT_STAT_PFC_5_RX_PAUSE_DURATION_US,

    /** SAI port stat PFC 5 tx duration in micro seconds */
    SAI_PORT_STAT_PFC_5_TX_PAUSE_DURATION_US,

    /** SAI port stat PFC 6 rx duration in micro seconds */
    SAI_PORT_STAT_PFC_6_RX_PAUSE_DURATION_US,

    /** SAI port stat PFC 6 tx duration in micro seconds */
    SAI_PORT_STAT_PFC_6_TX_PAUSE_DURATION_US,

    /** SAI port stat PFC 7 rx duration in micro seconds */
    SAI_PORT_STAT_PFC_7_RX_PAUSE_DURATION_US,

    /** SAI port stat PFC 7 tx duration in micro seconds */
    SAI_PORT_STAT_PFC_7_TX_PAUSE_DURATION_US,

    /** PFC based ON to OFF pause transitions counter per PFC priority [uint64_t] */
    SAI_PORT_STAT_PFC_0_ON2OFF_RX_PKTS,

    /** SAI port stat PFC 1 on to off rx pkts */
    SAI_PORT_STAT_PFC_1_ON2OFF_RX_PKTS,

    /** SAI port stat PFC 2 on to off rx pkts */
    SAI_PORT_STAT_PFC_2_ON2OFF_RX_PKTS,

    /** SAI port stat PFC 3 on to off rx pkts */
    SAI_PORT_STAT_PFC_3_ON2OFF_RX_PKTS,

    /** SAI port stat PFC 4 on to off rx pkts */
    SAI_PORT_STAT_PFC_4_ON2OFF_RX_PKTS,

    /** SAI port stat PFC 5 on to off rx pkts */
    SAI_PORT_STAT_PFC_5_ON2OFF_RX_PKTS,

    /** SAI port stat PFC 6 on to off rx pkts */
    SAI_PORT_STAT_PFC_6_ON2OFF_RX_PKTS,

    /** SAI port stat PFC 7 on to off rx pkts */
    SAI_PORT_STAT_PFC_7_ON2OFF_RX_PKTS,

    /** Frames received that are not an integral number of octets in length and do not pass the FCS check */
    SAI_PORT_STAT_DOT3_STATS_ALIGNMENT_ERRORS,

    /** Frames received that are an integral number of octets in length but do not pass the FCS check */
    SAI_PORT_STAT_DOT3_STATS_FCS_ERRORS,

    /** Frames that are involved in a single collision, and are subsequently transmitted successfully */
    SAI_PORT_STAT_DOT3_STATS_SINGLE_COLLISION_FRAMES,

    /** Frames that are involved in a more than one collision collision, and are subsequently transmitted successfully */
    SAI_PORT_STAT_DOT3_STATS_MULTIPLE_COLLISION_FRAMES,

    /** Number of times that the SQE TEST ERROR is received */
    SAI_PORT_STAT_DOT3_STATS_SQE_TEST_ERRORS,

    /** Frames for which the first transmission attempt is delayed because the medium is busy */
    SAI_PORT_STAT_DOT3_STATS_DEFERRED_TRANSMISSIONS,

    /** Number of times that a collision is detected later than one slot time into the transmission of a packet */
    SAI_PORT_STAT_DOT3_STATS_LATE_COLLISIONS,

    /** Frames for which transmission fails due to excessive collisions */
    SAI_PORT_STAT_DOT3_STATS_EXCESSIVE_COLLISIONS,

    /** Frames for which transmission fails due to an internal MAC sublayer transmit error */
    SAI_PORT_STAT_DOT3_STATS_INTERNAL_MAC_TRANSMIT_ERRORS,

    /** Number of times that the carrier sense condition was lost or never asserted when attempting to transmit a frame */
    SAI_PORT_STAT_DOT3_STATS_CARRIER_SENSE_ERRORS,

    /** Frames received that exceed the maximum permitted frame size */
    SAI_PORT_STAT_DOT3_STATS_FRAME_TOO_LONGS,

    /** Frames for which reception fails due to an internal MAC sublayer receive error */
    SAI_PORT_STAT_DOT3_STATS_INTERNAL_MAC_RECEIVE_ERRORS,

    /** Number of times there was an invalid data symbol, incremented at most once per carrier event */
    SAI_PORT_STAT_DOT3_STATS_SYMBOL_ERRORS,

    /** MAC Control frames received that contain an opcode that is not supported by this device */
    SAI_PORT_STAT_DOT3_CONTROL_IN_UNKNOWN_OPCODES,

    /**
     * @brief Number of times port state changed from
     * high power mode to low power mode in TX direction [uint64_t]
     */
    SAI_PORT_STAT_EEE_TX_EVENT_COUNT,

    /**
     * @brief Number of times port state changed from
     * high power mode to low power mode in RX direction [uint64_t]
     */
    SAI_PORT_STAT_EEE_RX_EVENT_COUNT,

    /**
     * @brief Port Low power mode duration(micro secs) in TX direction [uint64_t].
     *
     * This Duration is accumulative since EEE enable on port/from last clear stats.
     */
    SAI_PORT_STAT_EEE_TX_DURATION,

    /**
     * @brief Port Low power mode duration(micro secs) in RX direction [uint64_t]
     *
     * This Duration is accumulative since EEE enable on port/from last clear stats.
     */
    SAI_PORT_STAT_EEE_RX_DURATION,

    /** PRBS Error Count */
    SAI_PORT_STAT_PRBS_ERROR_COUNT,

    /** SAI port stat if in FEC correctable pkts */
    SAI_PORT_STAT_IF_IN_FEC_CORRECTABLE_FRAMES,

    /** SAI port stat if in FEC not correctable pkts */
    SAI_PORT_STAT_IF_IN_FEC_NOT_CORRECTABLE_FRAMES,

    /** SAI port stat if in FEC symbol errors */
    SAI_PORT_STAT_IF_IN_FEC_SYMBOL_ERRORS,

    /** Fabric port stat in data units */
    SAI_PORT_STAT_IF_IN_FABRIC_DATA_UNITS,

    /** Fabric port stat out data units */
    SAI_PORT_STAT_IF_OUT_FABRIC_DATA_UNITS,

    /** Port stat in drop reasons range start */
    SAI_PORT_STAT_IN_DROP_REASON_RANGE_BASE = 0x00001000,

    /** Get in port packet drops configured by debug counter API at index 0 */
    SAI_PORT_STAT_IN_CONFIGURED_DROP_REASONS_0_DROPPED_PKTS = SAI_PORT_STAT_IN_DROP_REASON_RANGE_BASE,

    /** Get in port packet drops configured by debug counter API at index 1 */
    SAI_PORT_STAT_IN_CONFIGURED_DROP_REASONS_1_DROPPED_PKTS,

    /** Get in port packet drops configured by debug counter API at index 2 */
    SAI_PORT_STAT_IN_CONFIGURED_DROP_REASONS_2_DROPPED_PKTS,

    /** Get in port packet drops configured by debug counter API at index 3 */
    SAI_PORT_STAT_IN_CONFIGURED_DROP_REASONS_3_DROPPED_PKTS,

    /** Get in port packet drops configured by debug counter API at index 4 */
    SAI_PORT_STAT_IN_CONFIGURED_DROP_REASONS_4_DROPPED_PKTS,

    /** Get in port packet drops configured by debug counter API at index 5 */
    SAI_PORT_STAT_IN_CONFIGURED_DROP_REASONS_5_DROPPED_PKTS,

    /** Get in port packet drops configured by debug counter API at index 6 */
    SAI_PORT_STAT_IN_CONFIGURED_DROP_REASONS_6_DROPPED_PKTS,

    /** Get in port packet drops configured by debug counter API at index 7 */
    SAI_PORT_STAT_IN_CONFIGURED_DROP_REASONS_7_DROPPED_PKTS,

    /** Port stat in drop reasons range end */
    SAI_PORT_STAT_IN_DROP_REASON_RANGE_END = 0x00001fff,

    /** Port stat out drop reasons range start */
    SAI_PORT_STAT_OUT_DROP_REASON_RANGE_BASE = 0x00002000,

    /** Get out port packet drops configured by debug counter API at index 0 */
    SAI_PORT_STAT_OUT_CONFIGURED_DROP_REASONS_0_DROPPED_PKTS = SAI_PORT_STAT_OUT_DROP_REASON_RANGE_BASE,

    /** Get out port packet drops configured by debug counter API at index 1 */
    SAI_PORT_STAT_OUT_CONFIGURED_DROP_REASONS_1_DROPPED_PKTS,

    /** Get out port packet drops configured by debug counter API at index 2 */
    SAI_PORT_STAT_OUT_CONFIGURED_DROP_REASONS_2_DROPPED_PKTS,

    /** Get out port packet drops configured by debug counter API at index 3 */
    SAI_PORT_STAT_OUT_CONFIGURED_DROP_REASONS_3_DROPPED_PKTS,

    /** Get out port packet drops configured by debug counter API at index 4 */
    SAI_PORT_STAT_OUT_CONFIGURED_DROP_REASONS_4_DROPPED_PKTS,

    /** Get out port packet drops configured by debug counter API at index 5 */
    SAI_PORT_STAT_OUT_CONFIGURED_DROP_REASONS_5_DROPPED_PKTS,

    /** Get out port packet drops configured by debug counter API at index 6 */
    SAI_PORT_STAT_OUT_CONFIGURED_DROP_REASONS_6_DROPPED_PKTS,

    /** Get out port packet drops configured by debug counter API at index 7 */
    SAI_PORT_STAT_OUT_CONFIGURED_DROP_REASONS_7_DROPPED_PKTS,

    /** Port stat out drop reasons range end */
    SAI_PORT_STAT_OUT_DROP_REASON_RANGE_END = 0x00002fff,

} sai_port_stat_t;

/**
 * @brief Create port
 *
 * @param[out] port_id Port id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
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
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_port_fn)(
        _In_ sai_object_id_t port_id);

/**
 * @brief Set port attribute value.
 *
 * @param[in] port_id Port id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
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
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_port_attribute_fn)(
        _In_ sai_object_id_t port_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get port statistics counters. Deprecated for backward compatibility.
 *
 * @param[in] port_id Port id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_port_stats_fn)(
        _In_ sai_object_id_t port_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get port statistics counters extended.
 *
 * @param[in] port_id Port id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_port_stats_ext_fn)(
        _In_ sai_object_id_t port_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear port statistics counters.
 *
 * @param[in] port_id Port id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_port_stats_fn)(
        _In_ sai_object_id_t port_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief Clear port's all statistics counters.
 *
 * @param[in] port_id Port id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_port_all_stats_fn)(
        _In_ sai_object_id_t port_id);

/**
 * @brief Port state change notification
 *
 * Passed as a parameter into sai_initialize_switch()
 *
 * @count data[count]
 *
 * @param[in] count Number of notifications
 * @param[in] data Array of port operational status
 */
typedef void (*sai_port_state_change_notification_fn)(
        _In_ uint32_t count,
        _In_ const sai_port_oper_status_notification_t *data);

/**
 * @brief List of Port buffer pool attributes
 */
typedef enum _sai_port_pool_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PORT_POOL_ATTR_START,

    /**
     * @brief Port ID
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_PORT
     */
    SAI_PORT_POOL_ATTR_PORT_ID = SAI_PORT_POOL_ATTR_START,

    /**
     * @brief Buffer pool id
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_BUFFER_POOL
     */
    SAI_PORT_POOL_ATTR_BUFFER_POOL_ID,

    /**
     * @brief Attach WRED to port pool
     *
     * ID = #SAI_NULL_OBJECT_ID to disable WRED.
     *
     * WRED Drop/ECN marking based on port thresholds will happen only
     * when one of queue referring to this port configured
     * with non default value for SAI_QUEUE_ATTR_WRED_PROFILE_ID.
     * ID = #SAI_NULL_OBJECT_ID to disable WRED
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_WRED
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_POOL_ATTR_QOS_WRED_PROFILE_ID,

    /**
     * @brief End of attributes
     */
    SAI_PORT_POOL_ATTR_END,

    /** Custom range base value */
    SAI_PORT_POOL_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PORT_POOL_ATTR_CUSTOM_RANGE_END

} sai_port_pool_attr_t;

/**
 * @brief Port pool counter IDs in sai_get_port_pool_stats() call
 */
typedef enum _sai_port_pool_stat_t
{
    /** SAI port stat if octets */
    SAI_PORT_POOL_STAT_IF_OCTETS,

    /** Get/set WRED green dropped packet count [uint64_t] */
    SAI_PORT_POOL_STAT_GREEN_WRED_DROPPED_PACKETS,

    /** Get/set WRED green dropped byte count [uint64_t] */
    SAI_PORT_POOL_STAT_GREEN_WRED_DROPPED_BYTES,

    /** Get/set WRED yellow dropped packet count [uint64_t] */
    SAI_PORT_POOL_STAT_YELLOW_WRED_DROPPED_PACKETS,

    /** Get/set WRED yellow dropped byte count [uint64_t] */
    SAI_PORT_POOL_STAT_YELLOW_WRED_DROPPED_BYTES,

    /** Get/set WRED red dropped packet count [uint64_t] */
    SAI_PORT_POOL_STAT_RED_WRED_DROPPED_PACKETS,

    /** Get/set WRED red dropped byte count [uint64_t] */
    SAI_PORT_POOL_STAT_RED_WRED_DROPPED_BYTES,

    /** Get/set WRED dropped packets count [uint64_t] */
    SAI_PORT_POOL_STAT_WRED_DROPPED_PACKETS,

    /** Get/set WRED dropped bytes count [uint64_t] */
    SAI_PORT_POOL_STAT_WRED_DROPPED_BYTES,

    /** Get/set WRED green marked packet count [uint64_t] */
    SAI_PORT_POOL_STAT_GREEN_WRED_ECN_MARKED_PACKETS,

    /** Get/set WRED green marked byte count [uint64_t] */
    SAI_PORT_POOL_STAT_GREEN_WRED_ECN_MARKED_BYTES,

    /** Get/set WRED yellow marked packet count [uint64_t] */
    SAI_PORT_POOL_STAT_YELLOW_WRED_ECN_MARKED_PACKETS,

    /** Get/set WRED yellow marked byte count [uint64_t] */
    SAI_PORT_POOL_STAT_YELLOW_WRED_ECN_MARKED_BYTES,

    /** Get/set WRED red marked packet count [uint64_t] */
    SAI_PORT_POOL_STAT_RED_WRED_ECN_MARKED_PACKETS,

    /** Get/set WRED red marked byte count [uint64_t] */
    SAI_PORT_POOL_STAT_RED_WRED_ECN_MARKED_BYTES,

    /** Get/set WRED marked packets count [uint64_t] */
    SAI_PORT_POOL_STAT_WRED_ECN_MARKED_PACKETS,

    /** Get/set WRED marked bytes count [uint64_t] */
    SAI_PORT_POOL_STAT_WRED_ECN_MARKED_BYTES,

    /** Get in port current occupancy bytes [uint64_t] */
    SAI_PORT_POOL_STAT_CURR_OCCUPANCY_BYTES,

    /** Get in port watermark occupancy bytes [uint64_t] */
    SAI_PORT_POOL_STAT_WATERMARK_BYTES,

    /** Get in port current shared occupancy bytes [uint64_t] */
    SAI_PORT_POOL_STAT_SHARED_CURR_OCCUPANCY_BYTES,

    /** Get in port watermark shared occupancy bytes [uint64_t] */
    SAI_PORT_POOL_STAT_SHARED_WATERMARK_BYTES,

    /** Get in port packet drops due to buffers [uint64_t] */
    SAI_PORT_POOL_STAT_DROPPED_PKTS,

} sai_port_pool_stat_t;

/**
 * @brief Create port pool
 *
 * @param[out] port_pool_id Port pool id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_port_pool_fn)(
        _Out_ sai_object_id_t *port_pool_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove port pool
 *
 * @param[in] port_pool_id Port pool id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_port_pool_fn)(
        _In_ sai_object_id_t port_pool_id);

/**
 * @brief Set port pool attribute value.
 *
 * @param[in] port_pool_id Port pool id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_port_pool_attribute_fn)(
        _In_ sai_object_id_t port_pool_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get port pool attribute value.
 *
 * @param[in] port_pool_id Port pool id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_port_pool_attribute_fn)(
        _In_ sai_object_id_t port_pool_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Get port pool statistics counters. Deprecated for backward compatibility.
 *
 * @param[in] port_pool_id Port pool id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_port_pool_stats_fn)(
        _In_ sai_object_id_t port_pool_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters);

/**
 * @brief Get port pool statistics counters extended.
 *
 * @param[in] port_pool_id Port pool id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_port_pool_stats_ext_fn)(
        _In_ sai_object_id_t port_pool_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters);

/**
 * @brief Clear port pool statistics counters.
 *
 * @param[in] port_pool_id Port pool id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_clear_port_pool_stats_fn)(
        _In_ sai_object_id_t port_pool_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids);

/**
 * @brief List of Port Serdes attributes
 */
typedef enum _sai_port_serdes_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PORT_SERDES_ATTR_START,

    /**
     * @brief Port ID
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_PORT
     */
    SAI_PORT_SERDES_ATTR_PORT_ID = SAI_PORT_SERDES_ATTR_START,

    /**
     * @brief Port serdes control pre-emphasis
     *
     * List of port serdes pre-emphasis values. The values are of type sai_s32_list_t
     * where the count is number lanes in a port and the list specifies list of values
     * to be applied to each lane.
     *
     * @type sai_s32_list_t
     * @flags CREATE_ONLY
     * @default internal
     */
    SAI_PORT_SERDES_ATTR_PREEMPHASIS,

    /**
     * @brief Port serdes control idriver
     *
     * List of port serdes idriver values. The values are of type sai_s32_list_t
     * where the count is number lanes in a port and the list specifies list of values
     * to be applied to each lane.
     *
     * @type sai_s32_list_t
     * @flags CREATE_ONLY
     * @default internal
     */
    SAI_PORT_SERDES_ATTR_IDRIVER,

    /**
     * @brief Port serdes control pre-emphasis
     *
     * List of port serdes ipredriver values. The values are of type sai_s32_list_t
     * where the count is number lanes in a port and the list specifies list of values
     * to be applied to each lane.
     *
     * @type sai_s32_list_t
     * @flags CREATE_ONLY
     * @default internal
     */
    SAI_PORT_SERDES_ATTR_IPREDRIVER,

    /**
     * @brief Port serdes control TX FIR PRE1 filter
     *
     * List of port serdes TX fir precursor1 tap-filter values.
     * The values are of type sai_s32_list_t where the count is number lanes in
     * a port and the list specifies list of values to be applied to each lane.
     *
     * @type sai_s32_list_t
     * @flags CREATE_ONLY
     * @default internal
     */
    SAI_PORT_SERDES_ATTR_TX_FIR_PRE1,

    /**
     * @brief Port serdes control TX FIR PRE2 filter
     *
     * List of port serdes TX fir precursor2 tap-filter values.
     * The values are of type sai_s32_list_t where the count is number lanes in
     * a port and the list specifies list of values to be applied to each lane.
     *
     * @type sai_s32_list_t
     * @flags CREATE_ONLY
     * @default internal
     */
    SAI_PORT_SERDES_ATTR_TX_FIR_PRE2,

    /**
     * @brief Port serdes control TX FIR PRE3 filter
     *
     * List of port serdes TX fir precursor3 tap-filter values.
     * The values are of type sai_s32_list_t where the count is number lanes in
     * a port and the list specifies list of values to be applied to each lane.
     *
     * @type sai_s32_list_t
     * @flags CREATE_ONLY
     * @default internal
     */
    SAI_PORT_SERDES_ATTR_TX_FIR_PRE3,

    /**
     * @brief Port serdes control TX FIR MAIN filter
     *
     * List of port serdes TX fir maincursor tap-filter values.
     * The values are of type sai_s32_list_t where the count is number lanes in
     * a port and the list specifies list of values to be applied to each lane.
     *
     * @type sai_s32_list_t
     * @flags CREATE_ONLY
     * @default internal
     */
    SAI_PORT_SERDES_ATTR_TX_FIR_MAIN,

    /**
     * @brief Port serdes control TX FIR POST1 filter
     *
     * List of port serdes TX fir postcursor1 tap-filter values.
     * The values are of type sai_s32_list_t where the count is number lanes in
     * a port and the list specifies list of values to be applied to each lane.
     *
     * @type sai_s32_list_t
     * @flags CREATE_ONLY
     * @default internal
     */
    SAI_PORT_SERDES_ATTR_TX_FIR_POST1,

    /**
     * @brief Port serdes control TX FIR POST2 filter
     *
     * List of port serdes TX fir postcursor2 tap-filter values.
     * The values are of type sai_s32_list_t where the count is number lanes in
     * a port and the list specifies list of values to be applied to each lane.
     *
     * @type sai_s32_list_t
     * @flags CREATE_ONLY
     * @default internal
     */
    SAI_PORT_SERDES_ATTR_TX_FIR_POST2,

    /**
     * @brief Port serdes control TX FIR POST3 filter
     *
     * List of port serdes TX fir postcursor3 tap-filter values.
     * The values are of type sai_s32_list_t where the count is number lanes in
     * a port and the list specifies list of values to be applied to each lane.
     *
     * @type sai_s32_list_t
     * @flags CREATE_ONLY
     * @default internal
     */
    SAI_PORT_SERDES_ATTR_TX_FIR_POST3,

    /**
     * @brief Port serdes control TX FIR attenuation
     *
     * List of port serdes TX fir attn values.
     * The values are of type sai_s32_list_t where the count is number lanes in
     * a port and the list specifies list of values to be applied to each lane.
     *
     * @type sai_s32_list_t
     * @flags CREATE_ONLY
     * @default internal
     */
    SAI_PORT_SERDES_ATTR_TX_FIR_ATTN,

    /**
     * @brief End of attributes
     */
    SAI_PORT_SERDES_ATTR_END,

    /** Custom range base value */
    SAI_PORT_SERDES_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PORT_SERDES_ATTR_CUSTOM_RANGE_END

} sai_port_serdes_attr_t;

/**
 * @brief Create port serdes
 *
 * @param[out] port_serdes_id Port serdes id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_port_serdes_fn)(
        _Out_ sai_object_id_t *port_serdes_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove port serdes
 *
 * @param[in] port_serdes_id Port serdes id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_port_serdes_fn)(
        _In_ sai_object_id_t port_serdes_id);

/**
 * @brief Set Port serdes attribute value.
 *
 * @param[in] port_serdes_id Port serdes id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_port_serdes_attribute_fn)(
        _In_ sai_object_id_t port_serdes_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get Port serdes attribute value.
 *
 * @param[in] port_serdes_id Port serdes id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_port_serdes_attribute_fn)(
        _In_ sai_object_id_t port_serdes_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief List of Port connector attributes
 */
typedef enum _sai_port_connector_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PORT_CONNECTOR_ATTR_START,

    /**
     * @brief Port ID
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY | KEY
     * @objects SAI_OBJECT_TYPE_PORT
     */
    SAI_PORT_CONNECTOR_ATTR_SYSTEM_SIDE_PORT_ID = SAI_PORT_CONNECTOR_ATTR_START,

    /**
     * @brief Port ID
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY | KEY
     * @objects SAI_OBJECT_TYPE_PORT
     */
    SAI_PORT_CONNECTOR_ATTR_LINE_SIDE_PORT_ID,

    /**
     * @brief System Side Port ID
     *
     * @type sai_object_id_t
     * @flags CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_PORT
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_CONNECTOR_ATTR_SYSTEM_SIDE_FAILOVER_PORT_ID,

    /**
     * @brief Line Side Port ID
     *
     * @type sai_object_id_t
     * @flags CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_PORT
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_PORT_CONNECTOR_ATTR_LINE_SIDE_FAILOVER_PORT_ID,

    /**
     * @brief Configure the failover mode on port
     *
     * @type sai_port_connector_failover_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_PORT_CONNECTOR_FAILOVER_MODE_DISABLE
     */
    SAI_PORT_CONNECTOR_ATTR_FAILOVER_MODE,

    /**
     * @brief End of attributes
     */
    SAI_PORT_CONNECTOR_ATTR_END,

    /** Custom range base value */
    SAI_PORT_CONNECTOR_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PORT_CONNECTOR_ATTR_CUSTOM_RANGE_END

} sai_port_connector_attr_t;

/**
 * @brief Create port connector
 * Port connector uses to define logical relation between system side port to line side port.
 *
 * @param[out] port_connector_id Port connector id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_port_connector_fn)(
        _Out_ sai_object_id_t *port_connector_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove port connector
 *
 * @param[in] port_connector_id Port connector id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_port_connector_fn)(
        _In_ sai_object_id_t port_connector_id);

/**
 * @brief Set port connector attribute value.
 *
 * @param[in] port_connector_id Port connector id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_port_connector_attribute_fn)(
        _In_ sai_object_id_t port_connector_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get port connector attribute value.
 *
 * @param[in] port_connector_id Port connector id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_port_connector_attribute_fn)(
        _In_ sai_object_id_t port_connector_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Port methods table retrieved with sai_api_query()
 */
typedef struct _sai_port_api_t
{
    sai_create_port_fn                     create_port;
    sai_remove_port_fn                     remove_port;
    sai_set_port_attribute_fn              set_port_attribute;
    sai_get_port_attribute_fn              get_port_attribute;
    sai_get_port_stats_fn                  get_port_stats;
    sai_get_port_stats_ext_fn              get_port_stats_ext;
    sai_clear_port_stats_fn                clear_port_stats;
    sai_clear_port_all_stats_fn            clear_port_all_stats;
    sai_create_port_pool_fn                create_port_pool;
    sai_remove_port_pool_fn                remove_port_pool;
    sai_set_port_pool_attribute_fn         set_port_pool_attribute;
    sai_get_port_pool_attribute_fn         get_port_pool_attribute;
    sai_get_port_pool_stats_fn             get_port_pool_stats;
    sai_get_port_pool_stats_ext_fn         get_port_pool_stats_ext;
    sai_clear_port_pool_stats_fn           clear_port_pool_stats;
    sai_create_port_connector_fn           create_port_connector;
    sai_remove_port_connector_fn           remove_port_connector;
    sai_set_port_connector_attribute_fn    set_port_connector_attribute;
    sai_get_port_connector_attribute_fn    get_port_connector_attribute;
    sai_create_port_serdes_fn              create_port_serdes;
    sai_remove_port_serdes_fn              remove_port_serdes;
    sai_set_port_serdes_attribute_fn       set_port_serdes_attribute;
    sai_get_port_serdes_attribute_fn       get_port_serdes_attribute;
} sai_port_api_t;

/**
 * @}
 */
#endif /** __SAIPORT_H_ */
