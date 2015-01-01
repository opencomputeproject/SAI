/*
* Copyright (c) 2014 Microsoft Open Technologies, Inc. 
*   
*    Licensed under the Apache License, Version 2.0 (the "License"); you may 
*    not use this file except in compliance with the License. You may obtain 
*    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
*
*    THIS CODE IS PROVIDED ON AN  *AS IS* BASIS, WITHOUT WARRANTIES OR 
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
* Module Name:
*
*    saiqos.h
*
* Abstract:
*
*    This module defines SAI QOS API
*
*/

#if !defined (__SAIQOS_H_)
#define __SAIQOS_H_

#include <saitypes.h>

typedef enum _sai_cos_port_trust_t {

    /* Do not trust packet fields for setting CoS */
    SAI_COS_PORT_TRUST_NONE,

    /* Trust packets 802.1p field for setting CoS */
    SAI_COS_PORT_TRUST_DOT1P,

    /* Trust packets DSCP field for setting CoS */
    SAI_COS_PORT_TRUST_DSCP

} sai_cos_port_trust_t;


typedef enum _sai_cos_sched_t {

    /* Strict Scheduling */
    SAI_COS_SCHEDULING_STRICT,

    /* Deficit Weighted Round-Robin Scheduling */
    SAI_COS_SCHEDULING_DWRR

} sai_cos_sched_t;

/*
*  Attribute Id in sai_set_cos_attribute() and 
*  sai_get_cos_attribute() calls
*/
typedef enum _sai_cos_attr_t
{
    /* READ-ONLY */

    /* READ-WRITE */

    /** Number of traffic classes */
    SAI_COS_ATTR_NUMBER_OF_COS_CLASSES,

    /** Port trust mode [sai_cos_port_trust_t] */
    SAI_COS_ATTR_PORT_TRUST,

    /** Scheduling algorithm [sai_cos_sched_t] */
    SAI_COS_ATTR_SCHEDULING_ALGORITHM,

    /** Scheduling algorithm weight */
    SAI_COS_ATTR_SCHEDULING_WEIGHT,

    /** Bandwidth limit [bit/sec] */
    SAI_COS_ATTR_BANDWIDTH_LIMIT,

    /** Buffer limit [bytes] */
    SAI_COS_ATTR_BUFFER_LIMIT,

    /* -- */

    /* Custom range base value */
    SAI_COS_ATTR_CUSTOM_RANGE_BASE  = 0x10000000

} sai_cos_attr_t;

/*
*   Qos map type selector
*/
typedef enum _sai_qos_map_t {

    /* Port to CoS Mapping */
    SAI_QOS_MAP_PORT_TO_COS,

    /* Dot1P to CoS Mapping */
    SAI_QOS_MAP_DOT1P_TO_COS,

    /* DSCP to CoS Mapping */
    SAI_QOS_MAP_DSCP_TO_COS

} sai_qos_map_t;


/*
* Routine Description:
*   Set "class of service" attribute value for the port
*
* Arguments:
*    [in] port_id - port id
*    [in] cos_value - "class of service" value
*    [in] attr - cos attribute.
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_set_cos_attribute_fn)(
    _In_ sai_port_id_t port_id, 
    _In_ sai_cos_t cos_value, 
    _In_ const sai_attribute_t *attr
    );

/*
* Routine Description:
*   Get "class of service" attribute value for the port
*
* Arguments:
*    [in] port_id - port id
*    [in] cos_value - "class of service" value
*    [in] attr_count - number of attributes
*    [inout] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_get_cos_attribute_fn)(
    _In_ sai_port_id_t port_id, 
    _In_ sai_cos_t cos_value, 
    _In_ int attr_count,
    _Inout_ sai_attribute_t *attr_list
    );

/*
* Routine Description:
*   Set "class of service" mapping for the port. In order for the mapping 
*   to be active, the SAI_COS_ATTR_PORT_TRUST must be set.
*
* Arguments:
*    [in] port_id - port id
*    [in] qos_map_selector - qos mapping type
*    [in] value_to_map - value, depends on qos mapping type (not used/801p/dscp)
*    [in] cos_value - the mapped cos value.
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_set_cos_mapping_fn)(
    _In_ sai_port_id_t port_id, 
    _In_ sai_qos_map_t qos_map_selector,
    _In_ uint32_t value_to_map,
    _In_ sai_cos_t cos_value
    );

/*
* Routine Description:
*   Get "class of service" mapping for the port. In order for the mapping 
*   to be active, the SAI_COS_ATTR_PORT_TRUST must be set.
*
* Arguments:
*    [in] port_id - port id
*    [in] qos_map_selector - qos mapping type
*    [in] value_to_map - value, depends on qos mapping type (not used/801p/dscp)
*    [in] cos_value - the mapped cos value.
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
typedef sai_status_t (*sai_get_cos_mapping_fn)(
    _In_ sai_port_id_t port_id, 
    _In_ sai_qos_map_t qos_map_selector,
    _In_ uint32_t value_to_map,
    _Out_ sai_cos_t* cos_value
    );

/*
* Port methods table retrieved with sai_api_query()
*/
typedef struct _sai_qos_api_t
{
    sai_set_cos_attribute_fn        set_cos_attribute;
    sai_get_cos_attribute_fn        get_cos_attribute;
    sai_set_cos_mapping_fn          set_cos_mapping;
    sai_get_cos_mapping_fn          get_cos_mapping;

} sai_qos_api_t;


#endif // __SAIQOS_H_

