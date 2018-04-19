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
 * @file  saiexperimentalfpga.h
 *
 * @brief This module defines an experimental API to access low level FPGA
 *        functionality.
 *
 * @note  Hypothetical functionality, assumes that some NPUs allows user
 *        application to initialize and load and FPGA object
 */

#ifndef __SAIEXPERIMENTALFPGA_H_
#define __SAIEXPERIMENTALFPGA_H_

#include <saitypes.h>

/**
 * @defgroup SAIEXPERIMENTALFPGA SAI - FPGA specific API definitions
 *
 * @{
 */

/**
 * @brief Enum defining FPGA attributes.
 */
typedef enum _sai_fpga_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_FPGA_ATTR_START,

    /**
     * @brief FPGA attribute ID
     *
     * @type sai_uint32_t
     * @flags CREATE_ONLY | MANDATORY_ON_CREATE
     */
    SAI_FPGA_ATTR_ID = SAI_FPGA_ATTR_START,

    /**
     * @brief FPGA image name
     *
     * @type char
     * @flags CREATE_ONLY
     * @default ""
     */
    SAI_FPGA_ATTR_IMG_NAME,

    /**
     * @brief FPGA register ID 1
     *
     * @type sai_uint64_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_FPGA_ATTR_REG_1,

    /**
     * @brief FPGA register ID 2
     *
     * @type sai_uint64_t
     * @flags MANDATORY_ON_CREATE | CREATE_AND_SET
     */
    SAI_FPGA_ATTR_REG_2,

    /**
     * @brief FPGA register ID 3
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_FPGA_ATTR_REG_3,

    /**
     * @brief FPGA register ID 4
     *
     * @type sai_uint64_t
     * @flags READ_ONLY
     */
    SAI_FPGA_ATTR_REG_4,

    /**
     * @brief FPGA register ID 5
     *
     * @type sai_uint16_t
     * @flags READ_ONLY
     * @isvlan false
     */
    SAI_FPGA_ATTR_REG_5,

    SAI_FPGA_ATTR_END,

    /** Custom range base value */
    SAI_FPGA_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_FPGA_ATTR_CUSTOM_RANGE_END

} sai_fpga_attr_t;

/**
 * @brief Used to initialize the FPGA for a given NPU/switch ID
 * Must specify an FPGA id (returned by querying the switch FPGA Ids SAI_SWITCH_ATTR_FPGA_IDS)
 *
 * Optionally, an FPGA image name (a file) can be specify in order to reload the FPGA image
 *
 * @param[out] fpga_id FPGA id
 * @param[in] switch_id Switch id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_fpga_fn)(
        _Out_ sai_object_id_t *fpga_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Remove/Disable FPGA
 *
 * @param[in] fpga_id FPGA id
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_fpga_fn)(
        _In_ sai_object_id_t fpga_id);

/**
 * @brief Set FPGA register value.
 *
 * @param[in] fpga_id FPGA id
 * @param[in] attr Attribute
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_fpga_attribute_fn)(
        _In_ sai_object_id_t fpga_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get FPGA attribute values.
 *
 * @param[in] fpga_id FPGA Identifier
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_fpga_attribute_fn)(
        _In_ sai_object_id_t fpga_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief FPGA methods table retrieved with sai_api_query()
 */
typedef struct _sai_fpga_api_t
{
    sai_create_fpga_fn                create_fpga;
    sai_remove_fpga_fn                remove_fpga;
    sai_set_fpga_attribute_fn         set_fpga_attribute;
    sai_get_fpga_attribute_fn         get_fpga_attribute;

} sai_fpga_api_t;

/**
 * @}
 */

#endif /* __SAIEXPERIMENTALFPGA_H_ */
