/*
 *  Copyright (c) 2016 Dell Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License. You may obtain
 *  a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *  THIS CODE IS PROVIDED ON AN  *AS IS* BASIS, WITHOUT WARRANTIES OR
 *  CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 *  LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 *  FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
 *
 *  See the Apache Version 2.0 License for specific language governing
 *  permissions and limitations under the License.
 *
 *  @file saiobject.h
 *
 *  @brief This module defines SAI APIs for bulk retrieval for each object-type
 *************************************************************************/

#if !defined (__SAIOBJECT_H_)
#define __SAIOBJECT_H_

#include <saitypes.h>
#include <saifdb.h>
#include <saineighbor.h>
#include <sairoute.h>
/** \defgroup SAIOBJECT SAI - Object API definitions.
 *
 *  \{
 */

/**
 * @brief Structure for bulk retrieval of objectids, attribute
 * and values for each object-type. Key will be used in case
 * of object-types not having object-ids.
 */
typedef struct _sai_object_key_t
{
    union
    {
        sai_object_id_t           object_id;
        sai_vlan_id_t             vlan_id;
        sai_fdb_entry_t           fdb_entry;
        sai_neighbor_entry_t      neighbor_entry;
        sai_unicast_route_entry_t route_entry;

    } key;
} sai_object_key_t;

/**
 * Routine Description:
 *   @brief Get maximum number of attributes for an object type
 * Arguments:
 *  [in] object_type - SAI object type
 *  [inout] count - maximum number of attribute for an object type
 *
 * Return Values:
 *  SAI_STATUS_SUCCESS on success
 *  Failure status code on error
 */
sai_status_t sai_get_maximum_attribute_count(_In_ sai_object_type_t object_type,
                                             _Inout_ uint32_t *count);

/**
 * Routine Description:
 *   @brief Get the number of objects present in SAI
 * Arguments:
 *  [in] object_type - SAI object type
 *  [inout] count - number of objects in SAI
 *
 * Return Values:
 *  SAI_STATUS_SUCCESS on success
 *  Failure status code on error
 */
sai_status_t sai_get_object_count(_In_ sai_object_type_t object_type,
                                  _Inout_ uint32_t *count);

/**
 * Routine Description:
 *   @brief Get the list of object keys present in SAI
 * Arguments:
 *  [in] object_type - SAI object type
 *  [in] count - number of objects in SAI
 *  [in] object_list - List of SAI objects or keys
 *
 * Return Values:
 *  SAI_STATUS_SUCCESS on success
 *  Failure status code on error
 */
sai_status_t sai_get_object_key(_In_ sai_object_type_t object_type,
                                _In_ uint32_t object_count,
                                _Inout_ sai_object_key_t *object_list);

/*
 * Routine Description:
 *   @brief Get the bulk list of valid attributes for a given list of
 *   object keys.Only valid attributes for an objects are returned.
 *
 * Arguments:
 *  [in] object_type - sai object type
 *  [in] object_count - number of objects
 *  [in] object_key - List of object keys
 *  [inout] attr_count - List of attr_count. Caller passes the number
 *          of attribute allocated in. Callee returns with the actual
 *          number of attributes filled in. If the count is less than
 *          needed, callee fills with the needed count and do not fill
 *          the attributes. Callee also set the corresponding status to
 *          SAI_STATUS_BUFFER_OVERFLOW.
 *
 *  [inout] attrs - list of attributes for every object. Caller is
 *          responsible for allocating and freeing buffer for the attributes.
 *          For list based attribute, e.g., s32list, oidlist, callee should
 *          assume the caller has not allocate the memory for the list and
 *          should only to fill the count but not list. Then, caller
 *          can use corresponding get_attribute to get the list.
 *
 *  [inout] object_statuses - status for each object. If the object does
 *          not exist, callee sets the correpsonding status to SAI_STATUS_INVALID_OBJECT_ID.
 *          If the allocated attribute count is not large enough,
 *          set the status to SAI_STATUS_BUFFER_OVERFLOW.
 *
 * Return Values:
 *  SAI_STATUS_SUCCESS on success
 *  Failure status code on error
 **/

sai_status_t sai_bulk_get_attribute(_In_ sai_object_type_t object_type,
                                    _In_ uint32_t object_count,
                                    _In_ sai_object_key_t *object_key,
                                    _Inout_ uint32_t *attr_count,
                                    _Inout_ sai_attribute_t **attrs,
                                    _Inout_ sai_status_t *object_statuses);

/**
 * \}
 */
#endif // __SAIOBJECT_H_

