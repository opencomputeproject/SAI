#include "defines.p4"

// PORT
table table_ingress_lag {
    reads {
        standard_metadata.ingress_port : exact;
    }
    actions {action_set_lag_l2if;action_set_l2if;}//mattyk update
    size : PHY_PORT_NUM;
}

table table_drop_tagged_internal {
    reads {
        ingress_metadata.drop_tagged : exact;
    }
    actions {_drop;_nop;}
    size: 1;
}
table table_drop_untagged_internal {
    reads {
        ingress_metadata.drop_untagged : exact;
    }
    actions {_drop;_nop;}
    size: 1;
}

table table_l2_trap {
    reads {
        ethernet.dstAddr : exact;
    }
    actions {action_set_trap_id;}
}

table table_trap_id { //TODO: move this?
    reads {
        ingress_metadata.trap_id : exact;
    }
    actions {_drop;_nop;action_copy_to_cpu;action_trap_to_cpu;} 
}

table table_port_configurations {
    reads {
        ingress_metadata.l2_if : exact;
    }
    actions {action_set_port_configurations;}
}

table table_port_set_packet_vid_internal {  
    reads {
        ingress_metadata.is_tagged : exact;
    }
    actions {action_set_packet_vid;}
    size : 1; 
}


table table_port_ingress_interface_type {// should be 
    reads {
        ingress_metadata.l2_if: exact;
    }
    actions {action_set_l2_if_type; _drop;}
}

table table_subport_ingress_interface_type {
    reads {
        ingress_metadata.l2_if : exact;
        ingress_metadata.vid   : exact;
    }
    actions {action_set_l2_if_type; _drop;}
}


//-----------
// ingress 1d bridge
//-----------
table table_bridge_id_1d {
    reads {
        ingress_metadata.bridge_port : exact;
    }
    actions {action_set_bridge_id; _drop;}
}

table table_vbridge_STP {
    reads {
        ingress_metadata.bridge_port : exact;
    }
    actions {action_set_stp_state;}
    //size : 1; TODO
}
//-----------
// ingress 1q bridge
//-----------
table table_bridge_id_1q {
    reads {
        ingress_metadata.vid : exact;
    }
    actions {action_set_bridge_id;action_set_bridge_id_vid;}//why drop 
}

table table_ingress_vlan_filtering{
	reads{
		ingress_metadata.bridge_port : exact;
        ingress_metadata.vid   : exact;
	}
	actions{_drop;_nop;}
}

table table_mc_lookup_mode{
    reads{
        ingress_metadata.vid   : exact;
    }
    actions{action_set_mcast_lookup_mode;}
}

table table_xSTP_instance{
	reads{
		ingress_metadata.vid   : exact;
	}
	actions{action_set_stp_id;}
}

table table_xSTP {
    reads {
        ingress_metadata.bridge_port : exact;
        ingress_metadata.stp_id 	 : exact;
    }
    actions {action_set_stp_state;_drop;}
    //size : 1; TODO
}

//-----------
// fdb
//-----------

table table_learn_fdb {
    reads {
        ethernet.srcAddr 			: exact;
        ingress_metadata.bridge_id 	: exact;
    }
    actions {_nop;action_learn_mac;}
    //size : 1; TODO
}

table table_l3_interface {
    reads {
    	ethernet.dstAddr			: exact;
    	ingress_metadata.bridge_id 	: exact;
    }
    actions {action_set_egress_br_port;}//action_go_to_fdb_table;}
    //size : 1; TODO
}

//---------
// unicast:
//---------
table table_fdb { // TODO ask if can be melded into l3 interface table...
    reads {
    	ethernet.dstAddr		   : exact;
        ingress_metadata.bridge_id : exact;
    }
    actions {action_set_egress_br_port;action_set_unknown_unicast;}
    size : FDB_TABLE_SIZE;
}

//table table_l3_if{ // TODO - definition
//	reads{
//		ethernet.dstAddr		   : exact;
//        ingress_metadata.bridge_id : exact;
//	}
//	actions{action_forward;}//action_go_to_fdb_table;}
//}

//---------
// multicast:
//---------
table table_mc_fdb{
    reads{
        ethernet.dstAddr           : exact;
        ingress_metadata.bridge_id : exact;
    }
    actions{action_forward_mc_set_if_list;action_set_mc_fdb_miss;}
}

table table_mc_l2_sg_g{// IP MC
    reads{
        ingress_metadata.bridge_id  : exact;
        ipv4.srcAddr                : exact;
        ipv4.dstAddr                : exact;
    }
    actions{action_forward_mc_set_if_list;action_set_mc_fdb_miss;}
}

table table_unknown_multicast_nonip{
    reads{
      ingress_metadata.bridge_id : exact;
    }
    actions{action_forward_mc_set_if_list;}
}

table table_unknown_multicast_ipv4{
    reads{
      ingress_metadata.bridge_id : exact;
    }
    actions{action_forward_mc_set_if_list;}
}

//table table_unknown_multicast_ipv6{
//    reads{
//      ingress_metadata.bridge_id : exact;
//    }
//    actions{action_forward_mc_set_if_list;}
//}

table table_broadcast{
    reads{
      ingress_metadata.bridge_id : exact;
    }
    actions{action_forward_mc_set_if_list;}
}

table table_flood {
    reads {
        ingress_metadata.bridge_id : exact;
    }
    actions{action_forward_mc_set_if_list;}
}

//-----------
// egress 1d bridge
//-----------

table table_egress_vbridge_STP {
    reads {
        egress_metadata.bridge_port : exact; 
    }
    actions {action_set_egress_stp_state; _drop;}
    //size : 1; // TODO
}

table table_egress_vlan_tag {
    reads {
        egress_metadata.out_if : exact;
        ingress_metadata.vid : exact;
        vlan : valid;
    }
    actions {action_forward_vlan_tag; action_forward_vlan_untag; _drop;_nop;}
    //size : 1; // TODO
}

//-----------
// egress 1q bridge
//-----------

table table_egress_xSTP{
    reads{
        egress_metadata.bridge_port  : exact;
        ingress_metadata.stp_id : exact;
    }
    actions {action_set_egress_stp_state; _drop;}
}

table table_egress_vlan_filtering {
    reads{
        egress_metadata.bridge_port  : exact;
        ingress_metadata.vid    : exact;
    }
    actions{_drop; _nop; } 
}

// --------------
// egress bridge
// --------------
table table_egress_br_port_to_if {
    reads {
        egress_metadata.bridge_port : exact;
    }
    actions {action_forward_set_outIfType; _drop;}
}

table table_egress_set_vlan {
    reads {
        egress_metadata.bridge_port : exact;
    }
    actions {action_set_vlan;}
}

//-----------
// egress lag/phy
//-----------
table table_lag_hash { //TODO: FW flow to add ports as lag, should edit this table with lag size
    reads {
        egress_metadata.out_if : exact;
    }
    actions {action_set_lag_hash_size;}
}

table table_egress_lag {
    reads {
        egress_metadata.out_if : exact;
        egress_metadata.hash_val : exact;
    }
    actions {action_set_out_port; _drop;}
    //size : 1; // TODO
}

table table_egress_clone_internal {
    reads {
        standard_metadata.instance_type : exact;
    }
    actions {_nop; action_cpu_encap;} 
    // size: 16;
}