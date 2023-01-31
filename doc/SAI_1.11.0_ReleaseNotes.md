# SAI 1.11.0 Release Notes

The Switch Abstraction Interface(SAI) defines the APIs to provide a vendor-independent way of controlling forwarding elements, such as a switching ASIC, an NPU or a software switch in a uniform manner. This release document covers the SAI API changes from SAI tag 1.10.2 to SAI tag 1.11.0. The previous release notes corresponding to SAI tag 1.10.2 is available at [SAI 1.10.2 release notes](https://github.com/opencomputeproject/SAI/blob/master/doc/SAI_1.10.2_ReleaseNotes.md) 

This document explains the new SAI features as well as the enhancements and the bug fixes on existing features. 


### SAI Generic Programmable Extensions header

This feature adds the header files and relevant meta code changes to SAI. 

```

sai.h
=====

#include "saigenericprogrammable.h"
SAI_API_GENERIC_PROGRAMMABLE = 47, /**<sai_generic_programmable_t */


saigenericprogrammable.h
========================

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
 * @file    saigenericprogrammable.h
 *
 * @brief   This module defines SAI Genetic Programmable Extensions (GPE)
 */

#if !defined (__SAIGENERICPROGRAMMABLE_H_)
#define __SAIGENERICPROGRAMMABLE_H_

#include <saitypes.h>

/**
 * @defgroup SAIGENERICPROGRAMMABLE SAI - GPE specific API definitions
 *
 * @{
 */

/**
 * @brief Attribute Id for Generic Programmable extension
 */
typedef enum _sai_generic_programmable_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_GENERIC_PROGRAMMABLE_ATTR_START,

    /**
     * @brief HW block name to program the entry
     *
     * @type sai_s8_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_GENERIC_PROGRAMMABLE_ATTR_OBJECT_NAME = SAI_GENERIC_PROGRAMMABLE_ATTR_START,

    /**
     * @brief JSON string carrying HW block entry information
     *
     * @type sai_json_t
     * @flags CREATE_AND_SET
     * @default vendor
     */
    SAI_GENERIC_PROGRAMMABLE_ATTR_ENTRY,
	/**
     * @brief Attach a counter
     *
     * When it is empty, then packet hits won't be counted
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_GENERIC_PROGRAMMABLE_ATTR_COUNTER_ID,

    /**
     * @brief End of attributes
     */
    SAI_GENERIC_PROGRAMMABLE_ATTR_END,

    /** Custom range base value */
    SAI_GENERIC_PROGRAMMABLE_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_GENERIC_PROGRAMMABLE_ATTR_CUSTOM_RANGE_END

} sai_generic_programmable_attr_t;

/**
 * @brief Create a Generic programmable entry
 *
 * @param[out] generic_programmable_id The OID returned per entry per HW block
 * @param[in] switch_id The Switch Object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_generic_programmable_fn)(
        _Out_ sai_object_id_t *generic_programmable_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Delete a Generic programmable entry
 *
 * @param[in] generic_programmable_id The table id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_generic_programmable_fn)(
        _In_ sai_object_id_t generic_programmable_id);

/**
 * @brief Set Generic programmable Table entry attribute
 *
 * @param[in] generic_programmable_id The table id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_generic_programmable_attribute_fn)(
        _In_ sai_object_id_t generic_programmable_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get Generic programmable entry attribute
 *
 * @param[in] generic_programmable_id The table id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_generic_programmable_attribute_fn)(
        _In_ sai_object_id_t generic_programmable_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Generic extensions methods table retrieved with sai_api_query()
 */
typedef struct _sai_generic_programmable_api_t
{
    sai_create_generic_programmable_fn            create_generic_programmable;
    sai_remove_generic_programmable_fn            remove_generic_programmable;
    sai_set_generic_programmable_attribute_fn     set_generic_programmable_attribute;
    sai_get_generic_programmable_attribute_fn     get_generic_programmable_attribute;
} sai_generic_programmable_api_t;

/**
 * @}
 */
#endif /** __SAIGENERICPROGRAMMABLE_H_ */


saitypes.h
==========

SAI_OBJECT_TYPE_GENERIC_PROGRAMMABLE = 102,

/**
 * @brief JSON data type
 * "attributes": [
 * {
 *    "attribute_name": {
 *        "sai_metadata": {
 *        "sai_attr_value_type": "<SAI_ATTR_VALUE_TYPE_T>",
 *        "brief": "Brief Attribute Description",
 *        "sai_attr_flags": "<SAI_ATTR_FLAGS_T>",
 *        "allowed_object_types": [ "<LIST OF ALLOWED OBJECT TYPES>" ],
 *        "default_value": "<DEFAULT ATTR VALUE>"
 *        },
 *        "value": <VALUE of the attribute>
 *    }
 * }
 * ]
 * attributes - Mandatory top-level key where JSON parsing begins
 * attribute_name - Name of one attribute in the list of attributes
 * sai_attr_value_type - Data type of the attribute
 * brief - Optional description of the field
 * sai_attr_flags - Optional Usage flags for the field
 * allowed_object_types - If data type is OID, then this is the list of object types allowed as data
 */
typedef struct _sai_json_t
{
    /** String in JSON format */
    sai_s8_list_t json;
} sai_json_t;

/**

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_JSON */
    sai_json_t json;
	

saimetadatatypes.h
===================

 /**
     * @brief Attribute value is a json.
     */
    SAI_ATTR_VALUE_TYPE_JSON,

```

The PR related to this feature is available at PR#[1585](https://github.com/opencomputeproject/SAI/pull/1585) 


### Bulk API support for Neighbor entries

Added bulk API support for neighbor entries.

```
saineighbor.h
==============

/**
 * @brief Bulk create Neighbor entry
 *
 * @param[in] object_count Number of objects to create
 * @param[in] neighbor_entry List of object to create
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
typedef sai_status_t (*sai_bulk_create_neighbor_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_neighbor_entry_t *neighbor_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk remove Neighbor entry
 *
 * @param[in] object_count Number of objects to remove
 * @param[in] neighbor_entry List of objects to remove
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are removed or
 * #SAI_STATUS_FAILURE when any of the objects fails to remove. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_remove_neighbor_entry_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_neighbor_entry_t *neighbor_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk set attribute on Neighbor entry
 *
 * @param[in] object_count Number of objects to set attribute
 * @param[in] neighbor_entry List of objects to set attribute
 * @param[in] attr_list List of attributes to set on objects, one attribute per object
 * @param[in] mode Bulk operation error handling mode.
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are set or
 * #SAI_STATUS_FAILURE when any of the objects fails to set. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_set_neighbor_entry_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_neighbor_entry_t *neighbor_entry,
        _In_ const sai_attribute_t *attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**
 * @brief Bulk get attribute on Neighbor entry
 *
 * @param[in] object_count Number of objects to get attribute
 * @param[in] neighbor_entry List of objects to get attribute
 * @param[in] attr_count List of attr_count. Caller passes the number
 *    of attribute for each object to get
 * @param[inout] attr_list List of attributes to get on objects, one attribute per object
 * @param[in] mode Bulk operation error handling mode
 * @param[out] object_statuses List of status for every object. Caller needs to
 * allocate the buffer
 *
 * @return #SAI_STATUS_SUCCESS on success when all objects are get or
 * #SAI_STATUS_FAILURE when any of the objects fails to get. When there is
 * failure, Caller is expected to go through the list of returned statuses to
 * find out which fails and which succeeds.
 */
typedef sai_status_t (*sai_bulk_get_neighbor_entry_attribute_fn)(
        _In_ uint32_t object_count,
        _In_ const sai_neighbor_entry_t *neighbor_entry,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses);

/**

    sai_create_neighbor_entry_fn                create_neighbor_entry;
    sai_remove_neighbor_entry_fn                remove_neighbor_entry;
    sai_set_neighbor_entry_attribute_fn         set_neighbor_entry_attribute;
    sai_get_neighbor_entry_attribute_fn         get_neighbor_entry_attribute;
    sai_remove_all_neighbor_entries_fn          remove_all_neighbor_entries;

    sai_bulk_create_neighbor_entry_fn           create_neighbor_entries;
    sai_bulk_remove_neighbor_entry_fn           remove_neighbor_entries;
    sai_bulk_set_neighbor_entry_attribute_fn    set_neighbor_entries_attribute;
    sai_bulk_get_neighbor_entry_attribute_fn    get_neighbor_entries_attribute;
	
```

The PR related to this feature is available at PR#[1504](https://github.com/opencomputeproject/SAI/pull/1504)


### Add switch api for clause 22 mdio access

There are 2 mdio access modes: clause 45 and clause 22. The existing sai switch api "switch_mdio_read/switch_mdio_write" does not distinguish the 2 modes. The new sai switch api "switch_mdio_cl22_ead/switch_mdio_cl22_write" are added for clause 22 mdio access only.

```
saiswitch.h
===========

/**
 * @brief Switch MDIO clause 22 read API
 *
 * Provides clause 22 read access API for devices connected to MDIO from NPU SAI.
 *
 * @objects switch_id SAI_OBJECT_TYPE_SWITCH
 *
 * @param[in] switch_id Switch Id
 * @param[in] device_addr Device address(PHY/lane/port MDIO address)
 * @param[in] start_reg_addr Starting register address to read
 * @param[in] number_of_registers Number of consecutive registers to read
 * @param[out] reg_val Register read values
 */
typedef sai_status_t (*sai_switch_mdio_cl22_read_fn)(        
		_In_ sai_object_id_t switch_id,
        _In_ uint32_t device_addr,
        _In_ uint32_t start_reg_addr,
        _In_ uint32_t number_of_registers,
        _Out_ uint32_t *reg_val);

/**
 * @brief Switch MDIO clause write API
 *
 * Provides clause 22 write access API for devices connected to MDIO from NPU SAI.
 *
 * @objects switch_id SAI_OBJECT_TYPE_SWITCH
 *
 * @param[in] switch_id Switch Id
 * @param[in] device_addr Device address(PHY/lane/port MDIO address)
 * @param[in] start_reg_addr Starting register address to write
 * @param[in] number_of_registers Number of consecutive registers to write
 * @param[in] reg_val Register write values
 */
typedef sai_status_t (*sai_switch_mdio_cl22_write_fn)(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t device_addr,
        _In_ uint32_t start_reg_addr,
        _In_ uint32_t number_of_registers,
        _In_ const uint32_t *reg_val);

/**

```

The PR related to this feature is available at PR#[1507](https://github.com/opencomputeproject/SAI/pull/1507)


### Changes for supporting some PHY Diagnostics

This change includes support for reading the following PHY layer diagnostics in SAI -

Per PMD Lane Rx Signal Detect
Per PMD Lane Rx Lock Status (aka CDR status)
PCS Rx Link Status
Per FEC Lane Alignment Marker Lock
These are some useful diagnostics for debugging a link down or a link flap issue.

All of these diagnostics support a 'current' status and a 'changed' flag and therefore common helper types have been added that are shared by these 4 diagnostics. The current status indicates the status at the time of the read by NOS. The 'changed' flag indicates if the status changed at least once since the last read by NOS.

```
saiport.h
=========

    /**
     * @brief List of port's PMD lanes rx signal detect
     *
     * @type sai_port_lane_latch_status_list_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_RX_SIGNAL_DETECT,

    /**
     * @brief List of port's PMD lanes rx lock status
     *
     * @type sai_port_lane_latch_status_list_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_RX_LOCK_STATUS,

    /**
     * @brief Port's PCS RX Link Status
     *
     * @type sai_latch_status_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_PCS_RX_LINK_STATUS,

    /**
     * @brief List of port's FEC lanes alignment marker lock
     *
     * @type sai_port_lane_latch_status_list_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_FEC_ALIGNMENT_LOCK,

    /**
	
saitypes.h
===========

typedef struct _sai_latch_status_t
{
    /** Current status at the time of read */
    bool current_status;

    /** Indicates that the status changed at least once since the last read */
    bool changed;
} sai_latch_status_t;

typedef struct _sai_port_lane_latch_status_t
{
    uint32_t lane;
    sai_latch_status_t value;
} sai_port_lane_latch_status_t;

typedef struct _sai_port_lane_latch_status_list_t
{
    uint32_t count;
    sai_port_lane_latch_status_t *list;
} sai_port_lane_latch_status_list_t;

/**

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_PORT_LANE_LATCH_STATUS_LIST */
    sai_port_lane_latch_status_list_t portlanelatchstatuslist;

    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_LATCH_STATUS */
    sai_latch_status_t latchstatus;
	

saimetadatatypes.h
===================

    /**
     * @brief Attribute value is a latch's status.
     */
    SAI_ATTR_VALUE_TYPE_LATCH_STATUS,

    /**
     * @brief Attribute value is a list of latch status for all lanes in a port.
     */
    SAI_ATTR_VALUE_TYPE_PORT_LANE_LATCH_STATUS_LIST,
	
```

The PR related to this feature is available at PR#[1527](https://github.com/opencomputeproject/SAI/pull/1527)


### Distributed HW resource management infrastructure

This introduces an infrastructure to be able to query the HW resource for its ingress, egress or both stages i.e. that if physical resource belongs to ingress pipeline, egress pipeline or is a shared resource across ingress/egress stages of pipeline.

Additionally hostif trap group and policer pool object is enhancement with an attribute to specific which stage of the pipeline trap group and/or policer pool belongs.

```

saihostif.h
===========

    /**
     * @brief Hostif trap group object stage
     *
     * @type sai_object_stage_t
     * @flags CREATE_ONLY
     * @default SAI_OBJECT_STAGE_BOTH
     */
    SAI_HOSTIF_TRAP_GROUP_ATTR_OBJECT_STAGE,

    /**

saiobject.h
===========

/**
 * @brief Query the HW stage of an attribute for the specified object type
 *
 * @param[in] switch_id SAI Switch object id
 * @param[in] object_type SAI object type
 * @param[in] attr_count Count of attributes
 * @param[in] attr_list List of attributes
 * @param[out] stage HW stage of the attributes. Length of the array should be attr_count. Caller must allocate the buffer.
  *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
sai_status_t sai_query_object_stage(
        _In_ sai_object_id_t switch_id,
        _In_ sai_object_type_t object_type,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list,
        _Out_ sai_object_stage_t *stage);

/**

saipolicer.h
=============

    /**
     * @brief Policer pool stage
     *
     * @type sai_object_stage_t
     * @flags CREATE_ONLY
     * @default SAI_OBJECT_STAGE_BOTH
     */
    SAI_POLICER_ATTR_OBJECT_STAGE = 0x0000000b,

    /**
	
saistatus.h
===========

/**
 * @brief Object pipeline stage mismatch
 */
#define SAI_STATUS_STAGE_MISMATCH                   SAI_STATUS_CODE(0x00000018L)

/**


saitypes.h
===========

typedef enum _sai_object_stage_t
{
    /** Common stage */
    SAI_OBJECT_STAGE_BOTH,

    /** Ingress stage */
    SAI_OBJECT_STAGE_INGRESS,

    /** Egress stage */
    SAI_OBJECT_STAGE_EGRESS

} sai_object_stage_t;

/**

```

The PR related to this feature is available at PR#[1528](https://github.com/opencomputeproject/SAI/pull/1528)


### Add port FEC histogram counter support

Added port FEC histogram counter support
	

```
saiport.h
=========

    /**
     * @brief Query the maximum number of symbols with errors that can be
     * detected by the current FEC code (per FEC codeword).
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_MAX_FEC_SYMBOL_ERRORS_DETECTABLE,

    /**
	
	    /**
     * @brief Port FEC codeword symbol error counters.
     *
     * This set of counters corresponds to number of symbol errors in each FEC
     * codeword received on the port.
     *
     * The number of symbol errors that may be detected is dependent on the type
     * of FEC in use. For instance, RS-528 FEC supports detection of up to 7
     * symbol errors, while RS-544 FEC supports detection of up to 15 symbol
     * errors. The maximum number of errors that can be detected by the port's
     * current FEC mode may be retrieved via the
     * SAI_PORT_ATTR_MAX_FEC_SYMBOL_ERRORS_DETECTABLE port attribute. If the
     * codeword contains more than SAI_PORT_ATTR_MAX_FEC_SYMBOL_ERRORS_DETECTABLE,
     * the errors are placed in the next higher counter (so for the examples
     * above, SAI_PORT_STAT_IF_IN_FEC_CODEWORD_ERRORS_S8 would be used for
     * greater than 7 symbol errors when RS-528 FEC is used, and
     * SAI_PORT_STAT_IF_IN_FEC_CODEWORD_ERRORS_S16 for greater than 15 symbol
     * errors when using RS-544).
     */

    /** Count of FEC codewords with no symbol errors. */
    SAI_PORT_STAT_IF_IN_FEC_CODEWORD_ERRORS_S0,

    /** Count of FEC codewords with 1 symbol error. */
    SAI_PORT_STAT_IF_IN_FEC_CODEWORD_ERRORS_S1,

    /** Count of FEC codewords with 2 symbol errors. */
    SAI_PORT_STAT_IF_IN_FEC_CODEWORD_ERRORS_S2,

    /** Count of FEC codewords with 3 symbol errors. */
    SAI_PORT_STAT_IF_IN_FEC_CODEWORD_ERRORS_S3,

    /** Count of FEC codewords with 4 symbol errors. */
    SAI_PORT_STAT_IF_IN_FEC_CODEWORD_ERRORS_S4,

    /** Count of FEC codewords with 5 symbol errors. */
    SAI_PORT_STAT_IF_IN_FEC_CODEWORD_ERRORS_S5,

    /** Count of FEC codewords with 6 symbol errors. */
    SAI_PORT_STAT_IF_IN_FEC_CODEWORD_ERRORS_S6,

    /** Count of FEC codewords with 7 symbol errors. */
    SAI_PORT_STAT_IF_IN_FEC_CODEWORD_ERRORS_S7,

    /** Count of FEC codewords with 8 symbol errors. */
    SAI_PORT_STAT_IF_IN_FEC_CODEWORD_ERRORS_S8,

    /** Count of FEC codewords with 9 symbol errors. */
    SAI_PORT_STAT_IF_IN_FEC_CODEWORD_ERRORS_S9,

    /** Count of FEC codewords with 10 symbol errors. */
    SAI_PORT_STAT_IF_IN_FEC_CODEWORD_ERRORS_S10,

    /** Count of FEC codewords with 11 symbol errors. */
    SAI_PORT_STAT_IF_IN_FEC_CODEWORD_ERRORS_S11,

    /** Count of FEC codewords with 12 symbol errors. */
    SAI_PORT_STAT_IF_IN_FEC_CODEWORD_ERRORS_S12,

    /** Count of FEC codewords with 13 symbol errors. */
    SAI_PORT_STAT_IF_IN_FEC_CODEWORD_ERRORS_S13,

    /** Count of FEC codewords with 14 symbol errors. */
    SAI_PORT_STAT_IF_IN_FEC_CODEWORD_ERRORS_S14,

    /** Count of FEC codewords with 15 symbol errors. */
    SAI_PORT_STAT_IF_IN_FEC_CODEWORD_ERRORS_S15,
	
	/** Count of FEC codewords with 16 symbol errors. */
    SAI_PORT_STAT_IF_IN_FEC_CODEWORD_ERRORS_S16,

```

The PR related to this feature is available at PR#[1617](https://github.com/opencomputeproject/SAI/pull/1617)


### Support counters on IP MC route entries

This implements the support counters on IP MC route entries. This is similar to the support added earlier for Route counters.
	
```
saiipmc.h
==========

sai_attribute_t counter_attr;
attr.id = SAI_COUNTER_ATTR_TYPE;
attr.value.s32 = SAI_COUNTER_TYPE_REGULAR;
sai_object_id_t counter_id;
sai_status_t rc = sai_counter_api->create_counter(&counter_id, g_switch_id, 1, &attr);
sai_attribute_t ipmc_attr;
attr.id = SAI_IPMC_ENTRY_ATTR_COUNTER_ID;
attr.value.oid = counter_id;
rc = sai_ipmc_api->set_ipmc_entry_attribute(route_entry, ipmc_attr);
sai_stat_id_t stat_ids[] = { SAI_COUNTER_STAT_PACKETS, SAI_COUNTER_STAT_BYTES };
uint64_t stats[2];
rc = sai_counter_api->sai_get_counter_stats_ext(counter_id, 2, stat_ids, stats);

```

The PR related to this feature is available at PR#[1497](https://github.com/opencomputeproject/SAI/pull/1497)

### Add DONOTDROP packet action

This feature will help resolve the action conflict between ACLs in different priority groups where lower priority group ACL action is DROP and higher priority group ACL action is conflicting with DROP action, say REDIRECT, and requirement is to override the DROP action.

```
saiswitch.h
===========

    SAI_PACKET_ACTION_TRANSIT,

    /** Do not drop the packet. */
    SAI_PACKET_ACTION_DONOTDROP
	
```

The PR related to this feature is available at PR#[1349](https://github.com/opencomputeproject/SAI/pull/1349)


### Add support for link training ability query

Add support for link-training ability query to suppress redundant link-training requests those are doomed to fail

```
saiport.h
==========

/**
     * @brief Query link-training support
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_SUPPORTED_LINK_TRAINING_MODE,
	
	
```

The PR related to this feature is available at PR#[1434](https://github.com/opencomputeproject/SAI/pull/1434)

### Add hostif traps for p4runtime and gNMI protocols

This implements the addition of hostif traps for gNMI and p4runtime network management protocols.

```
saihostif.h
===========

    /**
     * @brief GNMI traffic (TCP dst port == 9339) to local router IP address
     * (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_GNMI = 0x0000400a,

    /**
     * @brief P4RT traffic (TCP dst port == 9559) to local router IP address
     * (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_P4RT = 0x0000400b,
	
	
```

The PR related to this feature is available at PR#[1436](https://github.com/opencomputeproject/SAI/pull/1436)


### Add hostif trap for NTP

Added hostif trap for NTP.

```
saihostif.h
===========

    /**
     * @brief NTPCLIENT traffic (UDP/TCP src port == 123)
     * to local router IP address (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_NTPCLIENT = 0x0000400c,
    /**
     * @brief NTPSERVER traffic (UDP/TCP dst port == 123)
     * to local router IP address (default packet action is drop)
     */
    SAI_HOSTIF_TRAP_TYPE_NTPSERVER = 0x0000400d,

```

The PR related to this feature is available at PR#[1453](https://github.com/opencomputeproject/SAI/pull/1453)


### Tunnel UDP SRC PORT security 

```
saitunnel.h
===========

    /**
     * @brief Drop tunnel packets with not allowed UDP source port
     *
     * Upon enabling this feature, if the tunnel packet ingresses with
     * UDP source port outside of range defined for this tunnel, it
     * will be dropped.
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     * @validonly SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_VXLAN and SAI_TUNNEL_ATTR_VXLAN_UDP_SPORT_MODE == SAI_TUNNEL_VXLAN_UDP_SPORT_MODE_USER_DEFINED
     */
    SAI_TUNNEL_ATTR_VXLAN_UDP_SPORT_SECURITY,
	
```

The PR related to this feature is available at PR#[1455](https://github.com/opencomputeproject/SAI/pull/1455)


### Add bulk create and remove for tunnel

This implements to complete quad bulk api for tunnel

```
sainexthopgroup.h
=================

sai_bulk_object_get_attribute_fn get_next_hop_group_members_attribute;

saitunnel.h
============

	sai_bulk_object_create_fn                    create_tunnels;
    sai_bulk_object_remove_fn                    remove_tunnels;
	
	sai_bulk_object_get_attribute_fn get_tunnels_attribute;

```

The PR related to this feature is available at PR#[1462](https://github.com/opencomputeproject/SAI/pull/1462)


### IPFIX Template Reporting IntervalUpdate saitam.h 

Attribute is introduced to configure IPFIX template reporting interval as per the RFC7011.

```
saitam.h
========

    /**
     * @brief Template report interval in minutes
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 15
     * @validonly SAI_TAM_REPORT_ATTR_TYPE == SAI_TAM_REPORT_TYPE_IPFIX
     */
    SAI_TAM_REPORT_ATTR_TEMPLATE_REPORT_INTERVAL,

    /**
	
```

The PR related to this feature is available at PR#[1496](https://github.com/opencomputeproject/SAI/pull/1496)


### Add fabric port isolation attribute

Added fabric port isolation attribute

```
saiport.h
==========

/**
     * @brief Fabric port isolation setting.
     *
     * true: The link may be enabled in serdes level and the
     * MAC level, but the link partner will not use
     * it for traffic distribution.
     * false: Undo the isolation operation.
     * This attribute is for fabric links only.     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PORT_ATTR_FABRIC_ISOLATE,

    /**
	
```

The PR related to this feature is available at PR#[1498](https://github.com/opencomputeproject/SAI/pull/1498)


