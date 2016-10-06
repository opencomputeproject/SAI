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
 * @file    saistatus.h
 *
 * @brief   This module defines SAI Status codes
 */

#if !defined (__SAISTATUS_H_)
#define __SAISTATUS_H_

/**
 * @defgroup SAISTATUS SAI - Status codes definitions
 *
 * @{
 */

#ifdef _WIN32

#define SAI_STATUS_CODE(_S_)    ((sai_status_t)(_S_))

#else

/**
 * @brief Any mapping schema from _S_ to error codes convenient on *nix can be used.
 */
#define SAI_STATUS_CODE(_S_)    (-(_S_))

#endif

/**
 * @brief Status success
 */
#define SAI_STATUS_SUCCESS                                          0x00000000L

/**
 * @brief General failure
 */
#define SAI_STATUS_FAILURE                          SAI_STATUS_CODE(0x00000001L)

/**
 * @brief The request is not supported
 */
#define SAI_STATUS_NOT_SUPPORTED                    SAI_STATUS_CODE(0x00000002L)

/**
 * @brief Not enough memory to complete the operation
 */
#define SAI_STATUS_NO_MEMORY                        SAI_STATUS_CODE(0x00000003L)

/**
 * @brief Insufficient system resources exist to complete the operation
 */
#define SAI_STATUS_INSUFFICIENT_RESOURCES           SAI_STATUS_CODE(0x00000004L)

/**
 * @brief An invalid parameter was passed to a function
 */
#define SAI_STATUS_INVALID_PARAMETER                SAI_STATUS_CODE(0x00000005L)

/**
 * @brief An item already exists. For example, create a route which already
 * exists.
 */
#define SAI_STATUS_ITEM_ALREADY_EXISTS              SAI_STATUS_CODE(0x00000006L)

/**
 * @brief An item was not found. For example, remove a route which does not
 * exist.
 */
#define SAI_STATUS_ITEM_NOT_FOUND                   SAI_STATUS_CODE(0x00000007L)

/**
 * @brief The data was too large to fit into the specified buffer.
 */
#define SAI_STATUS_BUFFER_OVERFLOW                  SAI_STATUS_CODE(0x00000008L)

/**
 * @brief Invalid port number
 */
#define SAI_STATUS_INVALID_PORT_NUMBER              SAI_STATUS_CODE(0x00000009L)

/**
 * @brief Invalid port member
 */
#define SAI_STATUS_INVALID_PORT_MEMBER              SAI_STATUS_CODE(0x0000000AL)

/**
 * @brief Invalid VLAN id
 */
#define SAI_STATUS_INVALID_VLAN_ID                  SAI_STATUS_CODE(0x0000000BL)

/**
 * @brief Object is uninitialized
 */
#define SAI_STATUS_UNINITIALIZED                    SAI_STATUS_CODE(0x0000000CL)

/**
 * @brief Table is full
 */
#define SAI_STATUS_TABLE_FULL                       SAI_STATUS_CODE(0x0000000DL)

/**
 * @brief Mandatory attribute is missing
 */
#define SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING      SAI_STATUS_CODE(0x0000000EL)

/**
 * @brief Function is not implemented
 */
#define SAI_STATUS_NOT_IMPLEMENTED                  SAI_STATUS_CODE(0x0000000FL)

/**
 * @brief Address not found
 */
#define SAI_STATUS_ADDR_NOT_FOUND                   SAI_STATUS_CODE(0x00000010L)

/**
 * @brief SAI Object is in use
 */
#define SAI_STATUS_OBJECT_IN_USE                    SAI_STATUS_CODE(0x00000011L)

/**
 * @brief Invalid SAI Object type passed to a function. If the object id is
 * passed as an attribute value in list, then #SAI_STATUS_INVALID_ATTR_VALUE_0
 * status code should be used
 */
#define SAI_STATUS_INVALID_OBJECT_TYPE              SAI_STATUS_CODE(0x00000012L)

/**
 * @brief Invalid SAI Object ID passed to a function. Return
 * #SAI_STATUS_INVALID_OBJECT_TYPE when the object type is invalid, otherwise
 * return this #SAI_STATUS_INVALID_OBJECT_ID. This can happen if the object id
 * is already deleted.
 */
#define SAI_STATUS_INVALID_OBJECT_ID                SAI_STATUS_CODE(0x00000013L)

/**
 * @brief The NV storage used is either invalid or corrupt. (rv for initialize_switch())
 */
#define SAI_STATUS_INVALID_NV_STORAGE               SAI_STATUS_CODE(0x00000014L)

/**
 * @brief The NV storage is full. (rv for shutdown_switch())
 */
#define SAI_STATUS_NV_STORAGE_FULL                  SAI_STATUS_CODE(0x00000015L)

/**
 * @brief Version mismatch in case of Inservice upgrade (rv for initialize_switch())
 */
#define SAI_STATUS_SW_UPGRADE_VERSION_MISMATCH      SAI_STATUS_CODE(0x00000016L)

/**
 * @brief Attribute is invalid (range from 0x00010000L to 0x0001FFFFL).
 *
 * Return this when the property attached to the attribute does not match
 * the API call. For example:
 * - When a READ_ONLY attribute is passed set_attribute or create call
 * - When a CREATE_ONLY attribute is passed to set_attribute call
 *
 * The offset from #SAI_STATUS_INVALID_ATTRIBUTE_0 is the array index of
 * the attribute list passed to the SAI API call.
 */
#define SAI_STATUS_INVALID_ATTRIBUTE_0              SAI_STATUS_CODE(0x00010000L)

/**
 * @brief End of invalid attribute range
 */
#define SAI_STATUS_INVALID_ATTRIBUTE_MAX            SAI_STATUS_CODE(0x0001FFFFL)

/**
 * @brief Invalid attribute value (range from 0x00020000L to 0x0002FFFFL)
 */
#define SAI_STATUS_INVALID_ATTR_VALUE_0             SAI_STATUS_CODE(0x00020000L)
/**
 * @brief End of invalid attribute value range
 */
#define SAI_STATUS_INVALID_ATTR_VALUE_MAX           SAI_STATUS_CODE(0x0002FFFFL)

/**
 * @brief Attribute is not implmented (range from 0x00030000L to 0x003FFFFL)
 *
 * Return this when the attribute is supported but not implemented on
 * the platform.
 */
#define SAI_STATUS_ATTR_NOT_IMPLEMENTED_0           SAI_STATUS_CODE(0x00030000L)

/**
 * @brief End of not implemented range
 */
#define SAI_STATUS_ATTR_NOT_IMPLEMENTED_MAX         SAI_STATUS_CODE(0x0003FFFFL)

/**
 * @brief Attribute is unknown (range from 0x00040000L to 0x004FFFFL)
 *
 * Return this when the attribute is undefined, e.g., the attribute id
 * is out of range.
 */
#define SAI_STATUS_UNKNOWN_ATTRIBUTE_0              SAI_STATUS_CODE(0x00040000L)

/**
 * @brief End of unknown attribute range
 */
#define SAI_STATUS_UNKNOWN_ATTRIBUTE_MAX            SAI_STATUS_CODE(0x0004FFFFL)

/**
 * @brief Attribute is not supported (range from 0x00050000L to 0x005FFFFL)
 *
 * Return this when the platform does not has the capability to support
 * this attribute.
 */
#define SAI_STATUS_ATTR_NOT_SUPPORTED_0             SAI_STATUS_CODE(0x00050000L)

/**
 * @brief End of attribute not supported range
 */
#define SAI_STATUS_ATTR_NOT_SUPPORTED_MAX           SAI_STATUS_CODE(0x0005FFFFL)

/*
 * Macros to check if attribute related error is within the specified range
 */

/**
 * @brief Is invalid attribute helper
 */
#define SAI_STATUS_IS_INVALID_ATTRIBUTE(x)      ((x) & ~0xFFFF == SAI_STATUS_INVALID_ATTRIBUTE_0)

/**
 * @brief Is invalid attribute value helper
 */
#define SAI_STATUS_IS_INVALID_ATTR_VALUE(x)     ((x) & ~0xFFFF == SAI_STATUS_INVALID_ATTR_VALUE_0)

/**
 * @brief Is attribute not implemented helper
 */
#define SAI_STATUS_IS_ATTR_NOT_IMPLEMENTED(x)   ((x) & ~0xFFFF == SAI_STATUS_ATTR_NOT_IMPLEMENTED_0)

/**
 * @brief Is unknown attribute helper
 */
#define SAI_STATUS_IS_UNKNOWN_ATTRIBUTE(x)      ((x) & ~0xFFFF == SAI_STATUS_INVALID_ATTRIBUTE_0)

/**
 * @brief Is attribute not supported helper
 */
#define SAI_STATUS_IS_ATTR_NOT_SUPPORTED(x)     ((x) & ~0xFFFF == SAI_STATUS_ATTR_NOT_SUPPORTED_0)

/**
 * @}
 */
#endif /** __SAISTATUS_H_ */
