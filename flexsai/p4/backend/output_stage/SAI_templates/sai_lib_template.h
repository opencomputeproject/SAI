/* *
 * @file    sai|filename|.h
 *
 * @brief   This module defines SAI || P4 extension  interface
 */

#if !defined (__SAI|FILENAME|_H_)
#define __SAI|FILENAME|_H_

#include <saitypes.h>

__EXT_API_INITIALIZE__

/**
 * @defgroup SAI|FILENAME| SAI - Extension specific API definitions
 *
 * @{
 */

__PER_TABLE_ACTION_ENUM__
__PER_TABLE_ATTR_ENUM__
__STATS_DEF__
__PER_TABLE_FN_DEF__
typedef struct _sai_|filename|_api_t
{
    __PER_TABLE_API_FN__
} sai_|filename|_api_t;
/**
 * @}
 */
#endif /** __SAI|FILENAME|_H_ */