// This is P4 sample source for sai
// Fill in these files with your P4 code


// includes
#include "headers.p4"
#include "parser.p4"
#include "tables.p4"
#include "actions.p4"
#include "defines.p4"
#include "field_lists.p4"

// headers
header   ethernet_t 	  ethernet;
header   vlan_t 		  vlan;
header   ipv4_t 		  ipv4;
header   tcp_t 			  tcp;
header   udp_t			  udp;
header   cpu_header_t     cpu_header;  

// metadata
metadata 	ingress_metadata_t 	 ingress_metadata;
metadata 	egress_metadata_t 	 egress_metadata;

control ingress {
	// phy
	control_ingress_port();	//bridging
    if((ingress_metadata.l2_if_type == L2_1Q_BRIDGE) or (ingress_metadata.l2_if_type == L2_1D_BRIDGE)) {
    	control_bridge();
	}

	// router
	if ((ingress_metadata.l2_if_type == L2_ROUTER_TYPE) or (ingress_metadata.go_to_router == 1)) { 
		control_router_flow();
	}

	//todo: bridge after router
}

control control_bridge { 
	if(ingress_metadata.l2_if_type == L2_1D_BRIDGE){
		control_1d_bridge_flow();
	} else{
		control_1q_bridge_flow();
	}

	// control_learn_fdb();
	if((ethernet.dstAddr&0x010000000000)==0x0){   //unicast 
		control_unicast_fdb();
	} else if(ethernet.dstAddr==0xffffffffffff){  //broadcast
		control_bc_fdb();
	} else { //multicast
	    control_mc_fdb();
	}
}

control control_ingress_port{
	apply(table_ingress_lag); //TODO: rename table?
	apply(table_port_configurations);
	// apply(table_accepted_frame_type);
	if (ingress_metadata.is_tagged==1) { 
	    // apply(table_port_PVID);
	// } else {
		apply(table_port_set_packet_vid_internal);
		apply(table_drop_tagged_internal);
	} else {
		apply(table_drop_untagged_internal);
	}
	// apply(table_port_mode);
	apply(table_l2_trap);
	apply(table_trap_id); //TODO: move this
	// apply(table_check_port_mtu; //TODO
	//apply(table_ingress_acl); // TODO
	if(ingress_metadata.bind_mode == PORT_MODE_PORT) 
	    apply(table_port_ingress_interface_type);
	else
	    apply(table_subport_ingress_interface_type);
}

control control_1d_bridge_flow{
	apply(table_bridge_id_1d);
	apply(table_vbridge_STP);
}

control control_1q_bridge_flow{
	apply(table_bridge_id_1q);
 	apply(table_ingress_vlan_filtering);
 	apply(table_xSTP_instance);
 	apply(table_xSTP);
}

control control_router_flow{
	// TODO
}

// control control_learn_fdb{
//     apply(table_learn_fdb);
// }

control control_unicast_fdb{
	apply(table_learn_fdb); //TODO: is this only relevant for unicast?
	apply(table_l3_interface){//should be for unicast only TDB
		miss{ 
				apply(table_fdb) {
					miss { 
						apply(table_flood);
					}
				}
			}
	 }
}
					
control control_bc_fdb{
	apply(table_broadcast); 
}	

control control_mc_fdb{ 
      apply(table_mc_lookup_mode);
	//non ip multicast 
	 if((ingress_metadata.isip==0) or (ingress_metadata.mcast_mode==MAC_BASE_MC_LOOKUP))//non ip or multicast mode == FDB
	         apply(table_mc_fdb);
     else if((ingress_metadata.isip==1) and (ingress_metadata.mcast_mode==SG_IP_BASE_MC_LOOKUP))
		    apply(table_mc_l2_sg_g); 
	//TBD add * G table 		     
    //FDB miss flow 
	 if(ingress_metadata.mc_fdb_miss==1)
	 {
		//non ip
		if(ingress_metadata.isip==1)
		    apply(table_unknown_multicast_ipv4);
		 else 
		    apply(table_unknown_multicast_nonip);
		   	    
	 }
}


control egress{
	if(ingress_metadata.l2_if_type == L2_1D_BRIDGE){
		apply(table_egress_vbridge_STP);
	}
	if(ingress_metadata.l2_if_type == L2_1Q_BRIDGE){
		apply(table_egress_xSTP);
		apply(table_egress_vlan_filtering);
	}

	apply(table_egress_br_port_to_if);
	if(ingress_metadata.l2_if_type == L2_1D_BRIDGE){
		apply(table_egress_set_vlan);
	}
	apply(table_egress_vlan_tag);

	if (egress_metadata.out_if_type == OUT_IF_IS_LAG) { 
		apply(table_lag_hash);
		apply(table_egress_lag);
	}
	else if(egress_metadata.out_if == OUT_IF_IS_ROUTER){
		control_1q_egress_uni_router();
	}
	//apply(egress_acl); // TODO
	//if((egress_metadata.stp_state == STP_FORWARDING) and (egress_metadata.tag_mode == TAG) ){
		// TODO: go to egress
	//}
	apply(table_egress_clone_internal);
}

control control_1q_egress_uni_router {

}

