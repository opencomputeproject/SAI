import logging
try:
    from meta.sai_adapter import *
except ImportError:
    from sai_thrift.sai_adapter import *

class MockSuccessClient():
    
    def sai_thrift_remove_acl_table(self, var):
        logging.info("sai_thrift_remove_acl_table invoked")
        # e = sai_thrift_exception()
        # e.status = -2
        # raise e

        return 0
    
    def sai_thrift_create_switch(client,
                             init_switch=None,
                             hardware_access_bus=None,
                             platfrom_context=None,
                             register_read=None,
                             register_write=None,
                             switch_id=None,
                             max_system_cores=None,
                             system_port_config_list=None,
                             ingress_acl=None,
                             egress_acl=None,
                             restart_warm=None,
                             warm_recover=None,
                             switching_mode=None,
                             bcast_cpu_flood_enable=None,
                             mcast_cpu_flood_enable=None,
                             src_mac_address=None,
                             max_learned_addresses=None,
                             fdb_aging_time=None,
                             fdb_unicast_miss_packet_action=None,
                             fdb_broadcast_miss_packet_action=None,
                             fdb_multicast_miss_packet_action=None,
                             ecmp_default_hash_algorithm=None,
                             ecmp_default_hash_seed=None,
                             ecmp_default_symmetric_hash=None,
                             ecmp_hash_ipv4=None,
                             ecmp_hash_ipv4_in_ipv4=None,
                             ecmp_hash_ipv6=None,
                             lag_default_hash_algorithm=None,
                             lag_default_hash_seed=None,
                             lag_default_symmetric_hash=None,
                             lag_hash_ipv4=None,
                             lag_hash_ipv4_in_ipv4=None,
                             lag_hash_ipv6=None,
                             counter_refresh_interval=None,
                             qos_default_tc=None,
                             qos_dot1p_to_tc_map=None,
                             qos_dot1p_to_color_map=None,
                             qos_dscp_to_tc_map=None,
                             qos_dscp_to_color_map=None,
                             qos_tc_to_queue_map=None,
                             qos_tc_and_color_to_dot1p_map=None,
                             qos_tc_and_color_to_dscp_map=None,
                             switch_shell_enable=None,
                             switch_profile_id=None,
                             switch_hardware_info=None,
                             firmware_path_name=None,
                             switch_state_change_notify=None,
                             switch_shutdown_request_notify=None,
                             fdb_event_notify=None,
                             port_state_change_notify=None,
                             packet_event_notify=None,
                             fast_api_enable=None,
                             mirror_tc=None,
                             queue_pfc_deadlock_notify=None,
                             pfc_dlr_packet_action=None,
                             pfc_tc_dld_interval=None,
                             pfc_tc_dlr_interval=None,
                             tpid_outer_vlan=None,
                             tpid_inner_vlan=None,
                             crc_check_enable=None,
                             crc_recalculation_enable=None,
                             bfd_session_state_change_notify=None,
                             ecn_ect_threshold_enable=None,
                             vxlan_default_router_mac=None,
                             vxlan_default_port=None,
                             uninit_data_plane_on_removal=None,
                             tam_object_id=None,
                             tam_event_notify=None,
                             pre_shutdown=None,
                             nat_zone_counter_object_id=None,
                             nat_enable=None,
                             firmware_download_broadcast=None,
                             firmware_load_method=None,
                             firmware_load_type=None,
                             firmware_download_execute=None,
                             firmware_broadcast_stop=None,
                             firmware_verify_and_init_switch=None,
                             type=None,
                             macsec_object_id=None,
                             qos_mpls_exp_to_tc_map=None,
                             qos_mpls_exp_to_color_map=None,
                             qos_tc_and_color_to_mpls_exp_map=None,
                             failover_config_mode=None,
                             tunnel_objects_list=None):
        logging.info("sai_thrift_create_switch invoked")
        return 0


    def sai_thrift_get_acl_table_attribute(client, 
                                            oid,
                                            attr_list):
        logging.info("sai_thrift_get_acl_table_attribute invoked")
        attr_list = []
        attribute1 = sai_thrift_attribute_t(id=SAI_ACL_TABLE_ATTR_ACL_STAGE)
        attribute1.value = sai_thrift_attribute_value_t()
        attribute = sai_thrift_attribute_t(id=SAI_ACL_TABLE_ATTR_ACL_STAGE, value=attribute1.value)
        attr_list.append(attribute)


        attr_lists = sai_thrift_attribute_list_t(attr_list=attr_list)
        attr_lists.attr_list = attr_list
        return attr_lists
