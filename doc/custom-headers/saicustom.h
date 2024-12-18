/**
 * Copyright (c) 2024 Microsoft Open Technologies, Inc.
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
 * @file    saicustom.h
 *
 * @brief   This module defines custom of the Switch Abstraction Interface (SAI)
 */

#ifndef __SAICUSTOM_H_
#define __SAICUSTOM_H_

#include <sai.h>
#include <saitypes.h>

/* existing enum custom */
#include "saitypescustom.h"
#include "saiswitchcustom.h"
#include "saiportcustom.h"

/* new custom object type includes */
#include "saicustomone.h"

/**
 * @brief Custom SAI APIs
 *
 * @flags free
 */
typedef enum _sai_api_custom_t
{
    SAI_API_CUSTOM_RANGE_START = SAI_API_CUSTOM_RANGE_BASE,

    SAI_API_ONE = SAI_API_CUSTOM_RANGE_START,

    /* Add new custom APIs above this line */

    SAI_API_CUSTOM_RANGE_END

} sai_api_custom_t;

#endif /* __SAICUSTOM_H_ */
