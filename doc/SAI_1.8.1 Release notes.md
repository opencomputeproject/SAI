# SAI 1.8.1 Release Notes

The Switch Abstraction Interface(SAI) defines the APIs to provide a vendor-independent way of controlling forwarding elements, such as a switching ASIC, an NPU or a software switch in a uniform manner. This release document covers the SAI API changes from SAI tag 1.7.1 to SAI tag 1.8.1. The previous release notes corresponding to SAI tag 1.7.1 is available at [SAI 1.7.1 release notes](https://github.com/opencomputeproject/SAI/blob/master/doc/SAI_1.7.1_ReleaseNotes.md) 

This document explains the new SAI features as well as the enhancements and the bug fixes on existing features. 


### Add SAI_MACSEC_ATTR_SUPPORTED_CIPHER_SUITE_LIST 

Changed SAI_MACSEC_ATTR to use sai_macsec_cipher_suite_t making it consistent 
with the style change in SAI_MACSEC_SC_ATTR

```````````````
saimacsec.h 

     * @brief List of supported cipher-suites
     *
     * @type sai_s32_list_t sai_macsec_cipher_suite_t
     * @flags READ_ONLY
     */
    SAI_MACSEC_ATTR_SUPPORTED_CIPHER_SUITE_LIST,

    /**
     * @brief Indicates if 32-bit Packer Number (PN) is supported.  This is deprecated,
     * subsumed under SAI_MACSEC_ATTR_SUPPORTED_CIPHER_SUITE_LIST.

     * subsumed under SAI_MACSEC_ATTR_SUPPORTED_CIPHER_SUITE_LIST.
     
```````````````

The PR related to this feature is available at [PR#1172](https://github.com/opencomputeproject/SAI/pull/1172)

### Separate XPN configuration attribute from read-only attribute 

Added a new read-only current XPN (available for both ingress and egress) and separate from the egress attribute for setting initial XPN value.
Renamed ingress set-and-clear attribute MINIMUM_XPN to MINIMUM_INGRESS_XPN.

```````````````

SAI_MACSEC_SA_ATTR_CONFIGURED_EGRESS_XPN,

    /**
     * @brief MACsec current packet number (PN/XPN). For ingress, largest
     * received packet number. For egress, 1 less than the next packet number.
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_MACSEC_SA_ATTR_CURRENT_XPN,

    /** @ignore - for backward compatibility */
    SAI_MACSEC_SA_ATTR_XPN = SAI_MACSEC_SA_ATTR_CURRENT_XPN,

```````````````

The PR related to this feature is available at [PR#1169](https://github.com/opencomputeproject/SAI/pull/1169)

### Do not call sai_metadata_sai get APIs if they are not allocated 

New asic - fabric - doesn't have attributes like VLAN, HOSTIF etc. So Broadcom SAI will not allocate these APIs or function pointers of those API objects are not allocated. This causes attempts that try to get/read these attributes to fail. So sai discovery for fabric will crash.
the solution for this is to modify parse.pl to verify sai_metadata_sai if they are allocated or not before trying to attributes. This implementation will change only for getting attributes. Create, Set, Remove may also need to be changed, but this is out of the scope of this PR.
The below code changes have been made in parse.pl for sub ProcessGet.

````````````

        WriteSource "if (!sai_metadata_sai_${api}_api || !sai_metadata_sai_${api}_api->get_${small}_attribute)";
        WriteSource "{";
        WriteSource "return SAI_STATUS_NOT_SUPPORTED;";
        WriteSource "}";
	 }
	 else
 	 {
        WriteSource "if (!sai_metadata_sai_${api}_api || !sai_metadata_sai_${api}_api->get_${small}_attribute)";
        WriteSource "{";
        WriteSource "return SAI_STATUS_NOT_SUPPORTED;";
        WriteSource "}";

````````````

The PR related to this feature is available at PR#[1182](https://github.com/opencomputeproject/SAI/pull/1182)

### Enhancements for MPLS support 

This implementation defines missing support for MPLS as compared to equivalent IPv4/IPv6 support:

*	bulking APIs for MPLS in-segment entries 
*	per-RIF enable/disable of MPLS family

SAI bulk  create/remove/get/set has been added part of saimpls.h 

```````````````

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


```````````````

The PR related to this feature is available at PR#[1181](https://github.com/opencomputeproject/SAI/pull/1181)

### Switch scoped tunnel attributes 

This implementation is to add switch attributes for switch level configuration of tunnel ECN modes. Encap and Decap ecn modes can be configured individually with separately assigned ECN mappers for user defined ECN mode.There is switch level tunnel object which can be set for various attributes per tunnel type.

SAI tunnel encap/decap ecn modes have been added for saiswitch.h 

```````````

/**
 * @brief Defines tunnel type
 */
typedef enum _sai_tunnel_type_t
{
    SAI_TUNNEL_TYPE_IPINIP,

    SAI_TUNNEL_TYPE_IPINIP_GRE,

    SAI_TUNNEL_TYPE_VXLAN,

    SAI_TUNNEL_TYPE_MPLS,

} sai_tunnel_type_t;

/**
 * @brief Defines VXLAN tunnel UDP source port mode
 */
typedef enum _sai_tunnel_vxlan_udp_sport_mode_t
{
    /**
     * @brief User define value
     */
    SAI_TUNNEL_VXLAN_UDP_SPORT_MODE_USER_DEFINED,

    /**
     * @brief RFC6335 Computed hash value in range 49152-65535
     */
    SAI_TUNNEL_VXLAN_UDP_SPORT_MODE_EPHEMERAL,
} sai_tunnel_vxlan_udp_sport_mode_t;

/**
 * @brief Defines tunnel encap ECN mode
 */
typedef enum _sai_tunnel_encap_ecn_mode_t
{
    /**
     * @brief Normal mode behavior defined in RFC 6040
     * section 4.1 copy from inner
     */
    SAI_TUNNEL_ENCAP_ECN_MODE_STANDARD,

    /**
     * @brief User defined behavior.
     */
    SAI_TUNNEL_ENCAP_ECN_MODE_USER_DEFINED

} sai_tunnel_encap_ecn_mode_t;

/**
 * @brief Defines tunnel decap ECN mode
 */
typedef enum _sai_tunnel_decap_ecn_mode_t
{
    /**
     * @brief Behavior defined in RFC 6040 section 4.2
     */
    SAI_TUNNEL_DECAP_ECN_MODE_STANDARD,

    /**
     * @brief Copy from outer ECN
     */
    SAI_TUNNEL_DECAP_ECN_MODE_COPY_FROM_OUTER,

    /**
     * @brief User defined behavior
     */
    SAI_TUNNEL_DECAP_ECN_MODE_USER_DEFINED

} sai_tunnel_decap_ecn_mode_t;

/**
 * @brief Defines tunnel attributes at switch level.
 * SAI_OBJECT_TYPE_SWITCH_TUNNEL object provides
 * per tunnel type global configuration.
 * SAI_OBJECT_TYPE_TUNNEL object configuration
 * overrides the switch scoped global configuration.
 */
typedef enum _sai_switch_tunnel_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_SWITCH_TUNNEL_ATTR_START,

    /**
     * @brief Tunnel type key
     *
     * @type sai_tunnel_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY | KEY
     * @isresourcetype true
     */
    SAI_SWITCH_TUNNEL_ATTR_TUNNEL_TYPE = SAI_SWITCH_TUNNEL_ATTR_START,

    /**
     * @brief Packet action when a packet ingress and gets routed back to same tunnel
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_FORWARD
     */
    SAI_SWITCH_TUNNEL_ATTR_LOOPBACK_PACKET_ACTION,


``````````````````````

The PR related to this feature is available at PR#[1173](https://github.com/opencomputeproject/SAI/pull/1173)

### Add SAI_SWITCH_ATTR_SWITCH_HARDWARE_INFO format for GB MDIO sysfs access 

SAI_SWITCH_ATTR_SWITCH_HARDWARE_INFO is the right attribute to be used to provide the each Phy information for mdio sysfs driver support

*	SAI_SWITCH_ATTR_SWITCH_HARDWARE_INFO shall contain the interface_name and phy_id with “/” as the separator

*	The data should be formatted as {intf_name}/{phy_id} : Ex) eth0/1

*	GB Phy SAI is expected to parse the SAI_SWITCH_ATTR_SWITCH_HARDWARE_INFO to determine the phy_id along with interface name for each phySAI

typedef sai_s8_list_t attribute has been modified with below code for saiswitch.h file

```````````````
* Example: Like PCI location, I2C address, MDIO address, MDIO bus SysFS information etc.

* For the MDIO SysFS driver support, the interface name and phy_id should be

* set and separated by "/", which should be formatted as {interface_name}/{phy_id}

```````````````

The PR related to this feature is available at PR#[1171](https://github.com/opencomputeproject/SAI/pull/1171)

### Override VRF 

New virtual router boolean attribute is introduced to indicate that this is an override VRF and overrides other VRs

The below code is added part of typedef sai_switch_attr_t for saiswitch.h file.

```````````````

    /**
     * @brief Default SAI Override Virtual Router ID
     *
     * Must return #SAI_STATUS_OBJECT_IN_USE when try to delete this VR ID.
     *
     * @type sai_object_id_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
     * @default internal
     */
    SAI_SWITCH_ATTR_DEFAULT_OVERRIDE_VIRTUAL_ROUTER_ID,

``````````````````````

The PR related to this feature is available at PR#[1186](https://github.com/opencomputeproject/SAI/pull/1186)

### Add attribute to query available packet DMA pool size 

This change implements the addition of more attribute for querying available packet DMA pool size. This can be used for accounting for and debugging allocations from packet DMA pool.

The below code is added part of typedef sai_switch_attr_t for saiswitch.h file.

``````````````````````

 /**
     * @brief The size of the available packet DMA pool memory in bytes
     * This can be used in conjunction with total packet DMA pool
     * size to account/debug % of memory available.
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_PACKET_AVAILABLE_DMA_MEMORY_POOL_SIZE,

``````````````````````

The PR related to this feature is available at PR#[1198](https://github.com/opencomputeproject/SAI/pull/1198)

### Allow multiple sai_macsec objects associated with 1 sai_switch object 

This change allows the multiple objects associationed with sai switch object by changing the type in saiswitch.h file for typedef sai_switch_attr_t

```````````````
/**
     * @brief MACsec object list for this switch.
     *
***     * @type sai_object_list_t   ***
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_MACSEC
     * @default empty
     */
***    SAI_SWITCH_ATTR_MACSEC_OBJECT_LIST, ***

``````````````````````

The PR related to this feature is available at PR#[1199](https://github.com/opencomputeproject/SAI/pull/1199)

### SAI versioning 

This implementation is for the proposal of SAI versioning

The below code has been added part of saiversion.h file.

``````````````````````

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
 * @file    saiversion.h
 *
 * @brief   Define the current version
 */

#if !defined (__SAIVERSION_H_)
#define __SAIVERSION_H_

#define SAI_MAJOR 1
#define SAI_MINOR 7
#define SAI_REVISION 1

#define SAI_VERSION(major, minor, revision) (10000 * (major) + 100 * (minor) + (revision))

#define SAI_API_VERSION SAI_VERSION(SAI_MAJOR, SAI_MINOR, SAI_REVISION)

``````````````````````

The PR related to this feature is available at PR#[1183](https://github.com/opencomputeproject/SAI/pull/1183)

### Packet header based VRF classification 

This adds Pre-Ingress ACL stage and also adds Set VRF action along with doc to describe the use case.

The following code are added for saiacl.h, saiswitch.h and saitypes.h files.

```````````````````````

*** saiacl.h ***

added part of typedef sai_acl_action_type_t

/** Associate with virtual router */
    SAI_ACL_ACTION_TYPE_SET_VRF


added part of typedef sai_acl_entry_attr_t

 /**
     * @brief Set virtual router
     *
     * @type sai_acl_action_data_t sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
     * @default disabled
     */
    SAI_ACL_ENTRY_ATTR_ACTION_SET_VRF,

    /**
     * @brief End of Rule Actions
     */
    SAI_ACL_ENTRY_ATTR_ACTION_END = SAI_ACL_ENTRY_ATTR_ACTION_SET_VRF,


*** saiswitch.h ***

  /**
     * @brief Switch/Global bind point for Pre-ingress ACL object
     *
     * Bind (or unbind) an Pre-ingress ACL table or ACL group globally. Enable/Update
     * Pre-ingress ACL table or ACL group filtering by assigning the list of valid
     * object id. Disable pre-ingress filtering by assigning SAI_NULL_OBJECT_ID
     * in the attribute value.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_ACL_TABLE, SAI_OBJECT_TYPE_ACL_TABLE_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_SWITCH_ATTR_PRE_INGRESS_ACL,

*** saitypes.h ***

 /** Pre-ingress Stage */
    SAI_ACL_STAGE_PRE_INGRESS,


```````````````````````

The PR related to this feature is available at PR#[1185](https://github.com/opencomputeproject/SAI/pull/1185)

#### PRBS RX State Data Type 

This implementation is to Read Only attribute for getting Rx error count. There are multiple SAI attributes/stat-id (viz., SAI_PORT_ATTR_PRBS_RX_STATE, SAI_PORT_ATTR_PRBS_RX_STATUS, SAI_PORT_PRBS_RX_STATUS_LOCK_WITH_ERRORS) for PORT PRBS status. It depends on the underlying ASIC as to which of these attributes are applicable. The suggestion was that the application would first query SAI_PORT_ATTR_PRBS_RX_STATE. If the SAI implementation returns that the attribute is unsupported, the application would then fallback to query the existing SAI_PORT_ATTR_PRBS_RX_STATUS and SAI_PORT_PRBS_RX_STATUS_LOCK_WITH_ERRORS.

Below is the code change for this implementation for saiport.h, saitypes.h, saimetadatatypes.h files.

``````````````````````

*** saiport.h ***

Added the code under typedef sai_port_attr_t

 /**
     * @brief Attribute data for #SAI_PORT_ATTR_PRBS_RX_STATE
     * Used for clear on read status/count register.
     * Adapter should return SAI_STATUS_NOT_SUPPORTED if not supported.
     *
     * @type sai_prbs_rx_state_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_PRBS_RX_STATE,


Added the code under typedef struct _sai_ip_prefix_t


/**
 * @brief Attribute data for #SAI_PORT_ATTR_PRBS_RX_STATUS
 */
typedef enum _sai_port_prbs_rx_status_t
{
    /** PRBS is locked and error_count is 0 */
    SAI_PORT_PRBS_RX_STATUS_OK,

    /** PRBS is locked, but there are errors */
    SAI_PORT_PRBS_RX_STATUS_LOCK_WITH_ERRORS,

    /** PRBS not locked */
    SAI_PORT_PRBS_RX_STATUS_NOT_LOCKED,

    /** PRBS locked but there is loss of lock since last call */
    SAI_PORT_PRBS_RX_STATUS_LOST_LOCK,

} sai_port_prbs_rx_status_t;

typedef struct _sai_prbs_rx_state_t
{
    sai_port_prbs_rx_status_t rx_status;

    uint32_t error_count;
} sai_prbs_rx_state_t;


Modified typedef union _sai_attribute_value_t for 

  /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_PRBS_RX_STATE */
    sai_prbs_rx_state_t rx_state;

*** saimetadatatypes.h ***

Added code under typedef sai_attr_value_type_t

   /**
     * @brief Attribute value is PRBS RX state
     */
    SAI_ATTR_VALUE_TYPE_PRBS_RX_STATE,

``````````````````````

The PR related to this feature is available at PR#[1179](https://github.com/opencomputeproject/SAI/pull/1179)

### Add switch attributes for hash offset configuration. 

This implements the ability to specify the hash offset (rotation) in addition to the hash seed for ecmp and LAG.

Added below code for saiswitch.h file.

`````````````````

typedef sai_switch_attr_t

 /**
     * @brief SAI ECMP default hash offset
     *
     * When set, the output of the ECMP hash calculation will be rotated right
     * by the specified number of bits.
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_OFFSET,

typedef sai_switch_attr_t

  /**
     * @brief SAI LAG default hash offset
     *
     * When set, the output of the LAG hash calculation will be rotated right
     * by the specified number of bits.
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_SWITCH_ATTR_LAG_DEFAULT_HASH_OFFSET,

``````````````````````

The PR related to this feature is available at PR#[1195](https://github.com/opencomputeproject/SAI/pull/1195)

### Add source/dest/double NAPT entry available attributes 

This implements the attributes for Source/Destination/Double NAPT entries in saiswitch. Attributes to query Available SNAT, DNAT, DOUBLE_NAT entries are already part of saiswitch. These new attributes are for port-based NAT entries. Some hardware use separate table for NAPT entries and these new attributes can be used to query available entries for port-based SNAT/DNAT/Double_nat entries.

Added below code for saiswitch.h file for typedef sai_switch_attr_t

``````````````````````

   /**
     * @brief Available SNAPT entries
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_SNAPT_ENTRY,

    /**
     * @brief Available DNAPT entries
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_DNAPT_ENTRY,

    /**
     * @brief Available Double NAPT entries
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_AVAILABLE_DOUBLE_NAPT_ENTRY,

``````````````````````


The PR related to this feature is available at PR#[1194](https://github.com/opencomputeproject/SAI/pull/1194)

### Updated SAI_NEIGHBOR_ENTRY_ATTR_ENCAP_INDEX and deprecated SAI_NEIGHBOR_ENTRY_ATTR_ENCAP_IMPOSE_INDEX 


The below modification has been done for saineighbor.h file

``````````````````````

  * @type sai_uint32_t
     * @flags CREATE_AND_SET
***     * @default internal  ***
     */
    SAI_NEIGHBOR_ENTRY_ATTR_ENCAP_INDEX,

    /**
     * @brief Encapsulation index is imposed.  *** This is deprecated ***


 *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
***      * @deprecated true  ***
     */
    SAI_NEIGHBOR_ENTRY_ATTR_ENCAP_IMPOSE_INDEX,

``````````````````````

The PR related to this feature is available at PR#[1202](https://github.com/opencomputeproject/SAI/pull/1202)

### Tunnel loopback packet action as resource 

The attribute SAI_TUNNEL_ATTR_LOOPBACK_PACKET_ACTION is marked as resource and can be queried using the sai_object_type_get_availability API.

*	This API will return number of vxlan tunnels which can further with packet action as drop.
*	SONiC when invokes this API with out any tunnels configured
*	SAI will return 4
*	SONiC will configure 1 tunnel with packet action as drop and call the query API.
*	SAI will return 3 and so forth


The PR related to this feature is available at PR#[1163](https://github.com/opencomputeproject/SAI/pull/1163)

### Add IPv6 flow label hash attribute. 

This implements the add IPv6 flow label for hashing support.

The below code is added part of typedef sai_native_hash_field_t for saihash.h file.

``````````````````````

 /** Native hash field IPv6 flow label */
    SAI_NATIVE_HASH_FIELD_IPV6_FLOW_LABEL,


``````````````````````

The PR related to this feature is available at PR#[1192](https://github.com/opencomputeproject/SAI/pull/1192)

### Remove obsolete stub folder 

This request is to remove the stub folder which was part of SAI. Hence stub_sai.h file was removed.


The PR related to this feature is available at PR#[1168](https://github.com/opencomputeproject/SAI/pull/1168)

### Support for ACL extensions in metadata 

SAI sanitychecker doesn't expect any extensions to be developed for ACL attributes. This implementation aims to add support by adding checks whether the 'under-check-attribute' is an extension attribute. The value range for an attribute between SAI_ACL_ENTRY_ATTR_ACTION_START and SAI_ACL_ENTRY_ATTR_ACTION_END is valid only for non-extensions. The extensions are to have attributes beyond SAI_ACL_ENTRY_ATTR_ACTION_END. This implementation allows the range check to be conditional for extension attributes.


The PR related to this feature is available at PR#[1178](https://github.com/opencomputeproject/SAI/pull/1178)

