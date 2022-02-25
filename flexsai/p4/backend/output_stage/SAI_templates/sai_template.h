/* *
 * @file    sai_ext.h
 *
 * @brief   This module defines SAI || P4 extension  interface
 */
#if !defined (__SAI_EXT_H_)
#define __SAI_EXT_H_

#include <sai.h>
#include "sai_ext_|filename|.h"

/**
 * @brief Defined API sets have assigned ID's.
 *
 * If specific API method table changes in any way (method signature, number of
 * methods), a new ID needs to be created (e.g. VLAN2) and old API still may
 * need to be supported for compatibility with older adapter hosts.
 */
typedef enum _sai_ext_api_t
{
    SAI_EXT_API_UNSPECIFIED      =  0, /**< unspecified API */
    SAI_EXT_API_|FILENAME|       =  1, /**< sai_ext_|filename|_api_t */
    SAI_EXT_API_MAX              =  2, /**< total number of apis */
} sai_ext_api_t;

/**
 * @brief Sai extensions initialization call.
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
sai_status_t sai_ext_api_initialize();

/**
 * @brief Retrieve a pointer to the C-style method table for desired SAI 
 * extensions functionality as specified by the given sai_ext_api_id.
 *
 * @param[in] sai_ext_api_id SAI EXT API ID
 * @param[out] api_method_table Caller allocated method table The table must
 * remain valid until the sai_api_uninitialize() is called
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
sai_status_t sai_ext_api_query(
        _In_ sai_ext_api_t sai_ext_api_id,
        _Out_ void** api_method_table);

/**
 * @brief Uninitialize sai extensions
 * retrieved via sai_ext_api_query() cannot be used after this call.
 *
 * @return #SAI_STATUS_SUCCESS on success Failure status code on error
 */
sai_status_t sai_ext_api_uninitialize();

#endif /** __SAI_EXT_H_ */
