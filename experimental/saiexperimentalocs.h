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
 * @file    saiexperimentalocs.h
 *
 * @brief   This module defines SAI OCS
 *
 * @warning This module is a SAI experimental module
 */

#if !defined (__SAIEXPERIMENTALOCS_H_)
#define __SAIEXPERIMENTALOCS_H_

#include <saitypesextensions.h>

/**
 * @defgroup SAIEXPERIMENTALOCS SAI - Experimental: OCS specific API definitions
 *
 * @{
 */

/**
 * @brief List of OCS cross connect attributes
 */
typedef enum _sai_ocs_cross_connect_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_OCS_CROSS_CONNECT_ATTR_START,

    /**
     * @brief A side port
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_OCS_PORT
     */
    SAI_OCS_CROSS_CONNECT_ATTR_A_SIDE_PORT_ID = SAI_OCS_CROSS_CONNECT_ATTR_START,

    /**
     * @brief B side port
     *
     * @type sai_object_id_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @objects SAI_OBJECT_TYPE_OCS_PORT
     */
    SAI_OCS_CROSS_CONNECT_ATTR_B_SIDE_PORT_ID,

    /**
     * @brief End of attributes
     */
    SAI_OCS_CROSS_CONNECT_ATTR_END,

} sai_ocs_cross_connect_attr_t;

/**
 * @brief Create cross connect entry.
 *
 * @param[out] ocs_cross_connect_id OCS cross connect id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_ocs_cross_connect_fn)(
        _Out_ sai_object_id_t *ocs_cross_connect_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove cross connect entry
 *
 * @param[in] ocs_cross_connect_id OCS cross connect Id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_ocs_cross_connect_fn)(
        _In_ sai_object_id_t ocs_cross_connect_id);

/**
 * @brief Set cross connect entry attribute
 *
 * @param[in] ocs_cross_connect_id OCS cross connect id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_ocs_cross_connect_attribute_fn)(
        _In_ sai_object_id_t ocs_cross_connect_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get OCS cross connect entry attribute
 *
 * @param[in] ocs_cross_connect_id OCS cross connect id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_ocs_cross_connect_attribute_fn)(
        _In_ sai_object_id_t ocs_cross_connect_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief Attribute data for OCS port override state parameter
 */
typedef enum _sai_ocs_port_override_state_t
{
    /**
     * @brief First element that the beam hits is powered off, for testing
     */
    SAI_OCS_PORT_OVERRIDE_STATE_POWERED_OFF,

    /**
     * @brief Default, state to be determined by presence of cross-connect
     */
    SAI_OCS_PORT_OVERRIDE_STATE_NORMAL,

    /**
     * @brief Regardless of cross-connect presence state, blocks the port
     */
    SAI_OCS_PORT_OVERRIDE_STATE_FORCE_BLOCKED

} sai_ocs_port_override_state_t;

/**
 * @brief Attribute data for OCS port status parameter
 */
typedef enum _sai_ocs_port_status_t
{
    /**
     * @brief Default status for any port which is with no configuration
     */
    SAI_OCS_PORT_STATUS_BLOCKED,

    /**
     * @brief Insertion loss is >0.5dB of target
     */
    SAI_OCS_PORT_STATUS_TUNING,

    /**
     * @brief Insertion loss is within 0.5dB of target
     */
    SAI_OCS_PORT_STATUS_TUNED,

    /**
     * @brief If there is a hardware failure
     */
    SAI_OCS_PORT_STATUS_FAILED
} sai_ocs_port_status_t;

/**
 * @brief List of OCS port attributes
 */
typedef enum _sai_ocs_port_attr_t
{
    /**
     * @brief Start of device port attributes
     */
    SAI_OCS_PORT_ATTR_START,

    /**
     * @brief Unique identifier for a port
     *
     * @type sai_u8_list_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    SAI_OCS_PORT_ATTR_NAME = SAI_OCS_PORT_ATTR_START,

    /**
     * @brief Overrides the status imposed by the programmed cross-connects
     *
     * @type sai_ocs_port_override_state_t
     * @flags CREATE_AND_SET
     */
    SAI_OCS_PORT_ATTR_OVERRIDE_STATE,

    /**
     * @brief Operational status of the port
     *
     * @type sai_ocs_port_status_t
     * @flags READ_ONLY
     */
    SAI_OCS_PORT_ATTR_OPER_STATUS,

    /**
     * @brief Mapping between face plate port and OCS physical element
     *
     * @type sai_u8_list_t
     * @flags READ_ONLY
     */
    SAI_OCS_PORT_ATTR_PHYSICAL_MAPPING,

    /**
     * @brief End of attributes
     */
    SAI_OCS_PORT_ATTR_END,
} sai_ocs_port_attr_t;

/**
 * @brief Defines the operational status of the OCS port
 */
typedef struct _sai_ocs_port_status_notification_t
{
    /**
     * @brief OCS Port id.
     *
     * @objects SAI_OBJECT_TYPE_OCS_PORT
     */
    sai_object_id_t port_id;

    /** OCS port operational status */
    sai_ocs_port_status_t port_state;
} sai_ocs_port_status_notification_t;

/**
 * @brief Create port entry.
 *
 * @param[out] ocs_port_id OCS port id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_ocs_port_fn)(
        _Out_ sai_object_id_t *ocs_port_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove port entry
 *
 * @param[in] ocs_port_id OCS port Id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_ocs_port_fn)(
        _In_ sai_object_id_t ocs_port_id);

/**
 * @brief Set port entry attribute
 *
 * @param[in] ocs_port_id OCS port id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_ocs_port_attribute_fn)(
        _In_ sai_object_id_t ocs_port_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get OCS port entry attribute
 *
 * @param[in] ocs_port_id OCS port id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_ocs_port_attribute_fn)(
        _In_ sai_object_id_t ocs_port_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief OCS port state change notification
 *
 * Passed as a parameter into sai_initialize_switch()
 *
 * @count data[count]
 *
 * @param[in] count Number of notifications
 * @param[in] data Array of port operational status
 */
typedef void (*sai_ocs_port_state_change_notification_fn)(
        _In_ uint32_t count,
        _In_ const sai_ocs_port_status_notification_t *data);

/**
 * @brief List of OCS cross connect factory data attributes. Inventory data for all possible cross-connects,
 * factory insertion loss measurements
 */
typedef enum _sai_ocs_cross_connect_factory_data_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_OCS_CROSS_CONNECT_FACTORY_DATA_ATTR_START,

    /**
     * @brief Name of A side port in a cross connection
     *
     * @type sai_u8_list_t
     * @flags READ_ONLY
     */
    SAI_OCS_CROSS_CONNECT_FACTORY_DATA_ATTR_A_SIDE_PORT_NAME = SAI_OCS_CROSS_CONNECT_FACTORY_DATA_ATTR_START,

    /**
     * @brief Name of B side port in a cross connection.
     *
     * @type sai_u8_list_t
     * @flags READ_ONLY
     */
    SAI_OCS_CROSS_CONNECT_FACTORY_DATA_ATTR_B_SIDE_PORT_NAME,

    /**
     * @brief Center frequency at which insertion loss is measured, e.g., 229.1 (O-band) 193.5, use int32 for representation of decimal value
     *
     * List of center frequency values. Use int32 for representation of decimal value with 3 fraction digits
     *
     * @type sai_s32_list_t
     * @flags READ_ONLY
     */
    SAI_OCS_CROSS_CONNECT_FACTORY_DATA_ATTR_FREQUENCY_THZ,

    /**
     * @brief Internal temperature sensor that correlates with the ambient temperature at which
     * the insertion loss is measured in Celsius, e.g., 25.00
     *
     * List of temperatures. Use int32 for representation of decimal value with 2 fraction digits
     *
     * @type sai_s32_list_t
     * @flags READ_ONLY
     */
    SAI_OCS_CROSS_CONNECT_FACTORY_DATA_ATTR_MEASURED_TEMPERATURE,

    /**
     * @brief Factory insertion loss in dB, e.g. 1.23
     *
     * List of insertion loss values. Use int32 for representation of decimal value with 2 fraction digits
     *
     * @type sai_s32_list_t
     * @flags READ_ONLY
     */
    SAI_OCS_CROSS_CONNECT_FACTORY_DATA_ATTR_INSERTION_LOSS_DB,

    /**
     * @brief Factory insertion loss in dB, e.g. 1.23
     *
     * List of insertion loss accuracy values. Use int32 for representation of decimal value with 2 fraction digits
     *
     * @type sai_s32_list_t
     * @flags READ_ONLY
     */
    SAI_OCS_CROSS_CONNECT_FACTORY_DATA_ATTR_INSERTION_LOSS_ACCURACY_DB,

    /**
     * @brief End of attributes
     */
    SAI_OCS_CROSS_CONNECT_FACTORY_DATA_ATTR_END,
} sai_ocs_cross_connect_factory_data_attr_t;

/**
 * @brief Create OCS cross connect factory data entry.
 *
 * @param[out] ocs_cross_connect_factory_data_id OCS cross connect factory data id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_ocs_cross_connect_factory_data_fn)(
        _Out_ sai_object_id_t *ocs_cross_connect_factory_data_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove OCS cross connect factory data entry
 *
 * @param[in] ocs_cross_connect_factory_data_id OCS cross connect Id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_ocs_cross_connect_factory_data_fn)(
        _In_ sai_object_id_t ocs_cross_connect_factory_data_id);

/**
 * @brief Set OCS cross connect factory data entry attribute
 *
 * @param[in] ocs_cross_connect_factory_data_id OCS cross connect id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_ocs_cross_connect_factory_data_attribute_fn)(
        _In_ sai_object_id_t ocs_cross_connect_factory_data_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get OCS cross connect factory data attribute
 *
 * @param[in] ocs_cross_connect_factory_data_id OCS cross connect id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_ocs_cross_connect_factory_data_attribute_fn)(
        _In_ sai_object_id_t ocs_cross_connect_factory_data_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief OCS methods table retrieved with sai_api_query()
 */
typedef struct _sai_ocs_api_t
{
    sai_create_ocs_port_fn                                     create_ocs_port;
    sai_remove_ocs_port_fn                                     remove_ocs_port;
    sai_set_ocs_port_attribute_fn                              set_ocs_port_attribute;
    sai_get_ocs_port_attribute_fn                              get_ocs_port_attribute;
    sai_bulk_object_create_fn                                  create_ocs_ports;
    sai_bulk_object_remove_fn                                  remove_ocs_ports;
    sai_bulk_object_set_attribute_fn                           set_ocs_ports_attribute;
    sai_bulk_object_get_attribute_fn                           get_ocs_ports_attribute;
    sai_create_ocs_cross_connect_fn                            create_ocs_cross_connect;
    sai_remove_ocs_cross_connect_fn                            remove_ocs_cross_connect;
    sai_set_ocs_cross_connect_attribute_fn                     set_ocs_cross_connect_attribute;
    sai_get_ocs_cross_connect_attribute_fn                     get_ocs_cross_connect_attribute;
    sai_bulk_object_create_fn                                  create_ocs_cross_connects;
    sai_bulk_object_remove_fn                                  remove_ocs_cross_connects;
    sai_bulk_object_set_attribute_fn                           set_ocs_cross_connects_attribute;
    sai_bulk_object_get_attribute_fn                           get_ocs_cross_connects_attribute;
    sai_create_ocs_cross_connect_factory_data_fn               create_ocs_cross_connect_factory_data;
    sai_remove_ocs_cross_connect_factory_data_fn               remove_ocs_cross_connect_factory_data;
    sai_set_ocs_cross_connect_factory_data_attribute_fn        set_ocs_cross_connect_factory_data_attribute;
    sai_get_ocs_cross_connect_factory_data_attribute_fn        get_ocs_cross_connect_factory_data_attribute;
    sai_bulk_object_create_fn                                  create_ocs_cross_connect_factory_datas;
    sai_bulk_object_remove_fn                                  remove_ocs_cross_connect_factory_datas;
    sai_bulk_object_set_attribute_fn                           set_ocs_cross_connect_factory_datas_attribute;
    sai_bulk_object_get_attribute_fn                           get_ocs_cross_connect_factory_datas_attribute;

} sai_ocs_api_t;

/**
 * @}
 */
#endif /** __SAIEXPERIMENTALOCS_H_ */
