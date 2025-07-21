/**
 * Copyright (c) 2025 Microsoft Open Technologies, Inc.
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
 * @brief   This module defines SAI Custom interface
 */

#ifndef __SAICUSTOM_H__
#define __SAICUSTOM_H__

#include <sai.h>
#include <saitypes.h>

#include "saiaclcustom.h"
#include "saiarscustom.h"
#include "saiarsprofilecustom.h"
#include "saibfdcustom.h"
#include "saibridgecustom.h"
#include "saibuffercustom.h"
#include "saicountercustom.h"
#include "saidebugcountercustom.h"
#include "saidtelcustom.h"
#include "saifdbcustom.h"
#include "saigenericprogrammablecustom.h"
#include "saihashcustom.h"
#include "saihostifcustom.h"
#include "saiicmpechocustom.h"
#include "saiisolationgroupcustom.h"
#include "saiipmccustom.h"
#include "saiipmcgroupcustom.h"
#include "saiipseccustom.h"
#include "sailagcustom.h"
#include "sail2mccustom.h"
#include "sail2mcgroupcustom.h"
#include "saimacseccustom.h"
#include "saimcastfdbcustom.h"
#include "saimirrorcustom.h"
#include "saimplscustom.h"
#include "saimymaccustom.h"
#include "sainexthopgroupcustom.h"
#include "sainatcustom.h"
#include "saineighborcustom.h"
#include "sainexthopcustom.h"
#include "saipoecustom.h"
#include "saipolicercustom.h"
#include "saiportcustom.h"
#include "saiprefixcompressioncustom.h"
#include "saiqosmapcustom.h"
#include "saiqueuecustom.h"
#include "sairoutecustom.h"
#include "sairouterinterfacecustom.h"
#include "sairpfgroupcustom.h"
#include "saisamplepacketcustom.h"
#include "saischedulercustom.h"
#include "saischedulergroupcustom.h"
#include "saisrv6custom.h"
#include "saistpcustom.h"
#include "saiswitchcustom.h"
#include "saisyncecustom.h"
#include "saisystemportcustom.h"
#include "saitamcustom.h"
#include "saitunnelcustom.h"
#include "saitwampcustom.h"
#include "saiudfcustom.h"
#include "saivirtualroutercustom.h"
#include "saivlancustom.h"
#include "saiwredcustom.h"



/**
 * @brief Custom SAI APIs
 *
 * @flags free
 */
typedef enum _sai_api_custom_t {
    SAI_API_CUSTOM_RANGE_START = SAI_API_CUSTOM_RANGE_BASE,

    /* Add new custom APIs above this line */

    SAI_API_CUSTOM_RANGE_END
} sai_api_custom_t;

#endif /** __SAICUSTOM_H__ */