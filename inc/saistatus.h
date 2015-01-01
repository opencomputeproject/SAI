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
*    saistatus.h
*
* Abstract:
*
*    This module defines SAI status codes
*
*/

#if !defined (__SAISTATUS_H_)
#define __SAISTATUS_H_

#ifdef _WIN32

#define SAI_STATUS_CODE(_S_)    ((sai_status_t)(_S_)) 

#else
//
// Any mapping schema from _S_ to error codes convenient on *nix can be used.
//
#define SAI_STATUS_CODE(_S_)    (-(_S_))
#endif


/* 
*  Status success 
*/
#define SAI_STATUS_SUCCESS                                  0x00000000L

/* 
*  General failure 
*/
#define SAI_STATUS_FAILURE                  SAI_STATUS_CODE(0x00000001L)  

/* 
*  The request is not supported 
*/
#define SAI_STATUS_NOT_SUPPORTED            SAI_STATUS_CODE(0x00000002L)

/* 
*  Not enough memory to complete the operation 
*/
#define SAI_STATUS_NO_MEMORY                SAI_STATUS_CODE(0x00000003L)

/* 
*  Insufficient system resources exist to complete the operation 
*/
#define SAI_STATUS_INSUFFICIENT_RESOURCES   SAI_STATUS_CODE(0x00000004L)

/*  
*  An invalid parameter was passed to a function 
*/
#define SAI_STATUS_INVALID_PARAMETER        SAI_STATUS_CODE(0x00000005L)

/*  
*  An item already exists 
*/
#define SAI_STATUS_ITEM_ALREADY_EXISTS      SAI_STATUS_CODE(0x00000006L)

/*  
*  An item was not found 
*/
#define SAI_STATUS_ITEM_NOT_FOUND           SAI_STATUS_CODE(0x00000007L)

/* 
*  The data was too large to fit into the specified buffer. 
*/ 
#define SAI_STATUS_BUFFER_OVERFLOW          SAI_STATUS_CODE(0x00000008L)

/*  
*  Invalid port number
*/
#define SAI_STATUS_INVALID_PORT_NUMBER      SAI_STATUS_CODE(0x00000009L)

/*  
*  Invalid port member
*/
#define SAI_STATUS_INVALID_PORT_MEMBER      SAI_STATUS_CODE(0x0000000AL)

/*  
*  Invalid VLAN id
*/
#define SAI_STATUS_INVALID_VLAN_ID          SAI_STATUS_CODE(0x0000000BL)

/*  
*  Object is uninitialized 
*/
#define SAI_STATUS_UNINITIALIZED            SAI_STATUS_CODE(0x0000000CL)

/*  
*  Table is full
*/
#define SAI_STATUS_TABLE_FULL               SAI_STATUS_CODE(0x0000000DL)

/*
*  Mandatory attribute is missing 
*/
#define SAI_MANDATORY_ATTRIBUTE_MISSING     SAI_STATUS_CODE(0x0000000EL)

/*  
*  Invalid interface id 
*/
#define SAI_STATUS_INVALID_INTERFACE        SAI_STATUS_CODE(0x0000000FL)

/*  
*  Port is in use 
*/
#define SAI_STATUS_PORT_IN_USE              SAI_STATUS_CODE(0x00000010L)

/*  
*  Invalid switch ID 
*/
#define SAI_STATUS_INVALID_SWITCH_ID        SAI_STATUS_CODE(0x00000011L)

/*  
*  Function is not implemented
*/
#define SAI_STATUS_NOT_IMPLEMENTED          SAI_STATUS_CODE(0x00000012L)

/*  
*  Address not found 
*/
#define SAI_STATUS_ADDR_NOT_FOUND           SAI_STATUS_CODE(0x00000013L)

/*  
*  Invalid virtual router ID 
*/
#define SAI_STATUS_INVALID_VRID             SAI_STATUS_CODE(0x00000014L)

/*  
*  Invalid ACL table ID
*/
#define SAI_STATUS_INVALID_ACL_TABLE_ID     SAI_STATUS_CODE(0x00000015L)

/*  
*  Invalid ACL entry ID
*/
#define SAI_STATUS_INVALID_ACL_ENTRY_ID     SAI_STATUS_CODE(0x00000016L)

/*  
*  Invalid ACL counter ID
*/
#define SAI_STATUS_INVALID_ACL_COUNTER_ID   SAI_STATUS_CODE(0x00000017L)

/*  
*  Attribute is invalid (range from 0x00010000L to 0x0001FFFFL). 
*  The offset from SAI_STATUS_INVLAID_ATTRIBUTE_0 is the array index of
*  the attribute list passed to the SAI API call.
*/
#define SAI_STATUS_INVALID_ATTRIBUTE_0      SAI_STATUS_CODE(0x00010000L)
#define SAI_STATUS_INVALID_ATTRIBUTE_MAX    SAI_STATUS_CODE(0x0001FFFFL)

/*  
*  Invalid attribute value (range from 0x00020000L to 0x0002FFFFL)
*/
#define SAI_STATUS_INVALID_ATTR_VALUE_0     SAI_STATUS_CODE(0x00020000L)
#define SAI_STATUS_INVALID_ATTR_VALUE_MAX   SAI_STATUS_CODE(0x0002FFFFL)

/*  
*  Attribute is not implmented (range from 0x00030000L to 0x003FFFFL)
*/
#define SAI_STATUS_ATTR_NOT_IMPLEMENTED_0   SAI_STATUS_CODE(0x00030000L)
#define SAI_STATUS_ATTR_NOT_IMPLEMENTED_MAX SAI_STATUS_CODE(0x0003FFFFL)

/*  
*  Attribute is unknown (range from 0x00040000L to 0x004FFFFL)
*/
#define SAI_STATUS_UNKNOWN_ATTRIBUTE_0      SAI_STATUS_CODE(0x00040000L)
#define SAI_STATUS_UNKNOWN_ATTRIBUTE_MAX    SAI_STATUS_CODE(0x0004FFFFL)

/*
*  Macros to check if attribute related error is within the specified range
*/
#define SAI_STATUS_IS_INVALID_ATTRIBUTE(x)      ((x) & ~0xFFFF == SAI_STATUS_INVALID_ATTRIBUTE_0)
#define SAI_STATUS_IS_INVALID_ATTR_VALUE(x)     ((x) & ~0xFFFF == SAI_STATUS_INVALID_ATTR_VALUE_0)
#define SAI_STATUS_IS_ATTR_NOT_IMPLEMENTED(x)   ((x) & ~0xFFFF == SAI_STATUS_ATTR_NOT_IMPLEMENTED_0)
#define SAI_STATUS_IS_UNKNOWN_ATTRIBUTE(x)      ((x) & ~0xFFFF == SAI_STATUS_INVALID_ATTRIBUTE_0)

#endif // __SAISTATUS_H_
