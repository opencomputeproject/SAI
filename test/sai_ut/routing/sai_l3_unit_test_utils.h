/************************************************************************
* Copyright (c) 2015 Dell Inc. 
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
*
* Module Name:
*
*    sai_l3_unit_test_utils.h
*     
* Abstract:
*
*    This contains the base class definition having the utility and helper 
*    functions for testing the SAI L3 functionalities.
*
*************************************************************************/

#ifndef __SAI_L3_UNIT_TEST_H__
#define __SAI_L3_UNIT_TEST_H__

#include "gtest/gtest.h"

extern "C" {
#include "saitypes.h"
#include "saistatus.h"
#include "saiswitch.h"
#include "saivlan.h"
#include "sairouter.h"
#include "sairouterintf.h"
#include "saineighbor.h"
#include "sainexthop.h"
#include "sainexthopgroup.h"
#include "sairoute.h"
}

class saiL3Test : public ::testing::Test
{
    public:
        /* Method to setup the L3 API table pointers. */
        static void SetUpL3ApiQuery();

        /* Methods for VIRTUAL ROUTER functionality SAI API testing. */
        static sai_status_t sai_test_vrf_create (
                            sai_object_id_t *p_vr_id,
                            unsigned int attr_count, ...);
        static sai_status_t sai_test_vrf_remove (sai_object_id_t vr_id);
        static sai_status_t sai_test_vrf_attr_set (
                            sai_object_id_t vr_id,
                            unsigned int attr_id, unsigned long val,
                            const char *mac_str);
        static sai_status_t sai_test_vrf_attr_get (
                            sai_object_id_t vrf_id,
                            sai_attribute_t *p_attr_list,
                            unsigned int attr_count, ...);

        /* Methods for ROUTER INTERFACE functionality SAI API testing. */
        static sai_status_t sai_test_rif_create (
                            sai_object_id_t *p_rif_id,
                            unsigned int attr_count, ...);
        static sai_status_t sai_test_rif_remove (
                            sai_object_id_t rif_id);
        static sai_status_t sai_test_rif_attr_set (
                            sai_object_id_t rif_id,
                            unsigned int attr_id, unsigned long value,
                            const char *mac_str);
        static sai_status_t sai_test_rif_attr_get (
                            sai_object_id_t rif_id,
                            sai_attribute_t *p_attr_list,
                            unsigned int attr_count, ...);

        /* Methods for ROUTE functionality SAI API testing. */
        static sai_status_t sai_test_route_create (sai_object_id_t vrf,
                            unsigned int ip_family, const char *ip_str,
                            unsigned int prefix_len, unsigned int attr_count,
                            ...);
        static sai_status_t sai_test_route_remove (sai_object_id_t vrf,
                            unsigned int ip_family, const char *ip_str,
                            unsigned int prefix_len);
        static sai_status_t sai_test_route_attr_set (
                            sai_object_id_t vrf, unsigned int ip_family,
                            const char *ip_str, unsigned int prefix_len,
                            unsigned int attr_id, unsigned long value);
        static sai_status_t sai_test_route_attr_get (
                            sai_object_id_t vrf,unsigned int ip_family,
                            const char *ip_str, unsigned int prefix_len,
                            sai_attribute_t *p_attr_list,
                            unsigned int attr_count, ...);

        /* Methods for NEXT-HOP functionality SAI API testing. */
        static sai_status_t sai_test_nexthop_create (sai_object_id_t *p_nh_id,
                                                     unsigned int attr_count,
                                                     ...);
        static sai_status_t sai_test_nexthop_remove (sai_object_id_t nh_id);
        static sai_status_t sai_test_nexthop_attr_get (
                                                     sai_object_id_t nh_id,
                                                     sai_attribute_t *p_list,
                                                     unsigned int attr_count,
                                                     ...);

         /* Methods for NEIGHBOR functionality SAI API testing. */
        static sai_status_t sai_test_neighbor_fdb_entry_create (
                                                         const char *p_mac_str,
                                                         unsigned int vlan_id,
                                                         unsigned int port);
        static sai_status_t sai_test_neighbor_fdb_entry_remove (
                                                         const char *p_mac_str,
                                                         unsigned int vlan_id);
        static sai_status_t sai_test_neighbor_create (
                            sai_object_id_t rif_id,
                            sai_ip_addr_family_t ip_family, const char *ip_str,
                            unsigned int attr_count, ...);
        static sai_status_t sai_test_neighbor_remove (
                            sai_object_id_t rif_id,
                            sai_ip_addr_family_t ip_family, const char *ip_str);
        static sai_status_t sai_test_neighbor_attr_set (
                            sai_object_id_t rif_id,
                            sai_ip_addr_family_t ip_family, const char *ip_str,
                            sai_attr_id_t attr_id, unsigned int int_attr_value,
                            const char *p_str_attr_value);
        static sai_status_t sai_test_neighbor_attr_get (
                            sai_object_id_t rif_id,
                            sai_ip_addr_family_t ip_family, const char *ip_str,
                            sai_attribute_t *p_attr_list,
                            unsigned int attr_count, ...);

        /* Methods for NEXT-HOP-GROUP functionality SAI API testing. */
        static sai_status_t sai_test_nh_group_create (
                                           sai_object_id_t *p_group_id,
                                           sai_object_id_t *p_nh_list,
                                           unsigned int attr_count, ...);
        static sai_status_t sai_test_add_nh_to_group (
                                              sai_object_id_t group_id,
                                              unsigned int nh_count,
                                              sai_object_id_t *p_nh_list);
        static sai_status_t sai_test_remove_nh_from_group (
                                              sai_object_id_t group_id,
                                              unsigned int nh_count,
                                              sai_object_id_t *p_nh_list);
        static sai_status_t sai_test_nh_group_remove (sai_object_id_t group_id);
        static sai_status_t sai_test_nh_group_attr_get (
                                              sai_object_id_t group_id,
                                              sai_attribute_t *p_attr_list,
                                              unsigned int attr_count, ...);

        /* Util for converting IP address bytes to string */
        static const char* sai_test_ip_addr_to_str (
                                              const sai_ip_address_t *p_ip_addr,
                                              char *p_buf, size_t len);

        /* Util for converting MAC addr string to bytes */
        static void sai_test_router_mac_str_to_bytes_get (const char *mac_str,
                                                          uint8_t *mac);

        /* Util for converting to attribute index based status code */
        static inline sai_status_t sai_test_invalid_attr_status_code (
                                                       sai_status_t status,
                                                       unsigned int attr_index)
        {
            return (status + SAI_STATUS_CODE (attr_index));
        }

    protected:
        static void SetUpTestCase();

        /* Methods for retrieving SAI port id for the routing test cases */
        static sai_object_id_t sai_l3_port_id_get (uint32_t port_index);
        static sai_object_id_t sai_l3_invalid_port_id_get ();

        /* Methods for ROUTER MAC set/get */
        static sai_status_t sai_test_router_mac_get (sai_mac_t *p_mac);
        static sai_status_t sai_test_router_mac_set (const char *mac_str);
        static sai_status_t sai_test_router_mac_init (const char *mac_str);

        /* Methods for VLAN Create/Remove for the routing test cases */
        static sai_status_t sai_test_vlan_create (unsigned int vlan_id);
        static sai_status_t sai_test_vlan_remove (unsigned int vlan_id);

        /* Method for retrieving the switch API table pointer */
        static inline sai_switch_api_t* switch_api_tbl_get (void)
        {
            return p_sai_switch_api_tbl;
        }

        static const unsigned int default_rif_attr_count      = 3;
        static const unsigned int default_nh_attr_count       = 3;
        static const unsigned int default_neighbor_attr_count = 1;
        static const unsigned int default_nh_group_attr_count = 2;
        static const char        *router_mac;

        static const unsigned int SAI_TEST_MAX_PORTS      = 256;
        static const unsigned int SAI_TEST_DEFAULT_VLAN   = 1;
        static const unsigned int SAI_TEST_V4_ADMIN_STATE_DFLT = true;
        static const unsigned int SAI_TEST_V6_ADMIN_STATE_DFLT = true;

    private:
        static sai_switch_api_t           *p_sai_switch_api_tbl;
        static sai_vlan_api_t             *p_sai_vlan_api_tbl;
        static sai_virtual_router_api_t   *p_sai_vrf_api_tbl;
        static sai_router_interface_api_t *p_sai_rif_api_tbl;
        static sai_neighbor_api_t         *p_sai_nbr_api_tbl;
        static sai_next_hop_api_t         *p_sai_nh_api_tbl;
        static sai_next_hop_group_api_t   *p_sai_nh_grp_api_tbl;
        static sai_route_api_t            *p_sai_route_api_tbl;
        static sai_fdb_api_t              *p_sai_fdb_api_table;
        static unsigned int                port_count;
        static sai_object_id_t             port_list[SAI_TEST_MAX_PORTS];
};

#endif /* __SAI_L3_UNIT_TEST_H__ */
