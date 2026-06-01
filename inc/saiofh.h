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
 * @file    saiofh.h
 *
 * @brief   This module defines SAI Optimized Forwarding Header
 */

#if !defined (__SAIOFH_H_)
#define __SAIOFH_H_

#include <saitypes.h>

/**
 * @defgroup SAIOFH SAI - OFH specific API definitions
 *
 * @{
 */

/**
 * @brief OFH types
 */
typedef enum _sai_ofh_type_t
{
    /**
     * @brief ESUN
     */
    SAI_OFH_TYPE_ESUN,

    /**
     * @brief AFH
     */
    SAI_OFH_TYPE_AFH,

    /**
     * @brief UFH
     */
    SAI_OFH_TYPE_UFH,
} sai_ofh_type_t;

/**
 * @brief OFH sub type
 */
typedef enum _sai_ofh_sub_type_t
{
    /**
     * @brief No Sub Type
     */
    SAI_OFH_SUB_TYPE_NONE,

    /**
     * @brief Type 1
     */
    SAI_OFH_SUB_TYPE_1,

    /**
     * @brief Type 2
     */
    SAI_OFH_SUB_TYPE_2,
} sai_ofh_sub_type_t;

/**
 * @brief OFH Version
 */
typedef enum _sai_ofh_ver_t
{
    /**
     * @brief Version 1
     */
    SAI_OFH_VER_1,

    /**
     * @brief Version 2
     */
    SAI_OFH_VER_2,
} sai_ofh_ver_t;

/**
 * @brief Attribute Id for SAI OFH object
 */
typedef enum _sai_ofh_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_OFH_ATTR_START,

    /**
     * @brief Type of OFH header
     *
     * @type sai_ofh_type_t
     * @flags CREATE_ONLY
     * @default SAI_OFH_TYPE_ESUN
     */
    SAI_OFH_ATTR_TYPE = SAI_OFH_ATTR_START,

    /**
     * @brief Sub type of OFH header
     *
     * @type sai_ofh_sub_type_t
     * @flags CREATE_ONLY
     * @default SAI_OFH_SUB_TYPE_NONE
     */
    SAI_OFH_ATTR_SUB_TYPE,

    /**
     * @brief OFH Version number
     *
     * @type sai_ofh_ver_t
     * @flags CREATE_AND_SET
     * @default SAI_OFH_VER_1
     * @validonly SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_AFH or SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_ESUN
     */
    SAI_OFH_ATTR_VER,

    /**
     * @brief OFH ether type identifier
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     * @validonly SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_AFH or SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_ESUN
     */
    SAI_OFH_ATTR_ETHERTYPE,

    /**
     * @brief OFH Version Field Bit Offset
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_AFH or SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_ESUN
     */
    SAI_OFH_ATTR_VER_OFFSET,

    /**
     * @brief OFH Version Field Width in Bits.
     * Value of 0 bits means that field is not configured.
     * ESUN Version: 2 bits
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_AFH or SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_ESUN
     */
    SAI_OFH_ATTR_VER_WIDTH,

    /**
     * @brief OFH Flow Label Present Field Bit Offset
     * This bit specifies the presence of flow label
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_AFH or SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_ESUN
     */
    SAI_OFH_ATTR_F_OFFSET,

    /**
     * @brief OFH Flow Label Present Field width in Bits.
     * Value of 0 bits means that field is not configured.
     * ESUN:1 bit AFH:1 bit
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_AFH or SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_ESUN
     */
    SAI_OFH_ATTR_F_WIDTH,

    /**
     * @brief OFH COS Field Bit Offset
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_AFH or SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_ESUN
     */
    SAI_OFH_ATTR_COS_OFFSET,

    /**
     * @brief OFH COS Field Width in Bits
     * Number of bits are 6. Value of 0 bits means that field is not configured
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 6
     * @validonly SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_AFH
     */
    SAI_OFH_ATTR_COS_WIDTH,

    /**
     * @brief OFH ECN Field Bit Offset
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_AFH or SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_ESUN
     */
    SAI_OFH_ATTR_ECN_OFFSET,

    /**
     * @brief OFH ECN Field Width in Bits
     * Value of 0 bits means that field is not configured
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_AFH or SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_ESUN
     */
    SAI_OFH_ATTR_ECN_WIDTH,

    /**
     * @brief OFH Flow Label Field Bit Offset
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_AFH or SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_ESUN
     */
    SAI_OFH_ATTR_FLOW_LABEL_OFFSET,

    /**
     * @brief OFH Flow Label Field Width in Bits
     * Value of 0 bits means that field is not configured
     * ESUN: 16 bits
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_AFH or SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_ESUN
     */
    SAI_OFH_ATTR_FLOW_LABEL_WIDTH,

    /**
     * @brief OFH TTL Field Bit Offset
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_AFH or SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_ESUN
     */
    SAI_OFH_ATTR_TTL_OFFSET,

    /**
     * @brief OFH TTL Field Width in Bits
     * Value of 0 bits means that field is not configured
     * ESUN: 4 bits
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_AFH or SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_ESUN
     */
    SAI_OFH_ATTR_TTL_WIDTH,

    /**
     * @brief OFH User Defined Field Bit Offset
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_ESUN
     */
    SAI_OFH_ATTR_UD_OFFSET,

    /**
     * @brief OFH User Define Field Width in Bits
     * Value of 0 bits means that field is not configured
     * ESUN: 2bit user defined
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_ESUN
     */
    SAI_OFH_ATTR_UD_WIDTH,

    /**
     * @brief OFH AR Field Bit Offset
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_AFH
     */
    SAI_OFH_ATTR_AR_OFFSET,

    /**
     * @brief OFH AR Field Width in Bits
     * Number of bits are 1. Value of 0 bits means that field is not configured
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_AFH
     */
    SAI_OFH_ATTR_AR_WIDTH,

    /**
     * @brief OFH Congestion Notification Message Eligible Field Bit Offset
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_AFH
     */
    SAI_OFH_ATTR_C_OFFSET,

    /**
     * @brief OFH Congestion Notification Message Eligible Field Width in Bits
     * Number of bits are 1. Value of 0 bits means that field is not configured
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_AFH
     */
    SAI_OFH_ATTR_C_WIDTH,

    /**
     * @brief OFH Congestion Notification Message Field Bit Offset
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_AFH
     */
    SAI_OFH_ATTR_CNM_OFFSET,

    /**
     * @brief OFH Congestion Notification Message Field Width in bits
     * Number of bits are 1. Value of 0 bits means that field is not configured
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_AFH
     */
    SAI_OFH_ATTR_CNM_WIDTH,

    /**
     * @brief OFH header size
     * Size includes all the fields including the reserved fields
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_OFH_ATTR_HDR_SIZE,

    /**
     * @brief Reserved 1 Field Bit Offset
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_ESUN
     */
    SAI_OFH_ATTR_RSVD1_OFFSET,

    /**
     * @brief Reserved 1 Field Width in Bits
     * Value of 0 bits means that field is not configured
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     * @validonly SAI_OFH_ATTR_TYPE == SAI_OFH_TYPE_ESUN
     */
    SAI_OFH_ATTR_RSVD1_WIDTH,

    /**
     * @brief End of attributes
     */
    SAI_OFH_ATTR_END,

    /** Custom range base value */
    SAI_OFH_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_OFH_ATTR_CUSTOM_RANGE_END

} sai_ofh_attr_t;

/**
 * @brief Create and return a OFH object
 *
 * @param[out] ofh_id OFH object
 * @param[in] switch_id Switch object id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_create_ofh_fn)(
        _Out_ sai_object_id_t *ofh_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

/**
 * @brief Deletes a specified OFH object.
 *
 * @param[in] ofh_id OFH object to be removed.
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_remove_ofh_fn)(
        _In_ sai_object_id_t ofh_id);

/**
 * @brief Set OFH attribute value(s).
 *
 * @param[in] ofh_id TAM id
 * @param[in] attr Attribute to set
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_set_ofh_attribute_fn)(
        _In_ sai_object_id_t ofh_id,
        _In_ const sai_attribute_t *attr);

/**
 * @brief Get values for specified OFH attributes.
 *
 * @param[in] ofh_id OFH object id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_get_ofh_attribute_fn)(
        _In_ sai_object_id_t ofh_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

/**
 * @brief OFH methods table retrieved with sai_api_query()
 */
typedef struct _sai_ofh_api_t
{
    /**
     * @brief SAI OFH API set
     */
    sai_create_ofh_fn                   create_ofh;
    sai_remove_ofh_fn                   remove_ofh;
    sai_set_ofh_attribute_fn            set_ofh_attribute;
    sai_get_ofh_attribute_fn            get_ofh_attribute;
} sai_ofh_api_t;

/**
 * @}
 */
#endif /** __SAIOFH_H_ */
