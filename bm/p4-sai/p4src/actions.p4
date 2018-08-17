#include "defines.p4"
// primitives
action _drop() {
    drop();
}

action _nop() {
	no_op();
}

// ingres L2
action action_set_lag_l2if(in bit is_lag, in bit<6> l2_if) { // , in bit<16> lag_id
	ingress_metadata.is_lag	=	is_lag;
	// ingress_metadata.lag_id =	lag_id;
	ingress_metadata.l2_if 	=	l2_if;
}

action action_set_trap_id(in bit<11> trap_id) {
	ingress_metadata.trap_id = trap_id;	
}

action action_copy_to_cpu() {
	clone_ingress_pkt_to_egress(COPY_TO_CPU_MIRROR_ID, redirect_FL);
}

action action_trap_to_cpu() {
	clone_ingress_pkt_to_egress(COPY_TO_CPU_MIRROR_ID, redirect_FL);
	drop();
}

// ingres L2
action action_set_l2if() { 
	ingress_metadata.l2_if =standard_metadata.ingress_port;
}

action action_set_packet_vid(){
	ingress_metadata.vid = vlan.vid;
}

action action_set_port_configurations(in bit<12> pvid, in bit bind_mode, in bit<32> mtu, in bit drop_tagged, in bit drop_untagged) {
	ingress_metadata.vid = pvid;
	ingress_metadata.bind_mode = bind_mode;
	ingress_metadata.mtu = mtu;
	ingress_metadata.drop_tagged = drop_tagged;
	ingress_metadata.drop_untagged = drop_untagged;
}

action action_set_l2_if_type(in bit<2> l2_if_type, in bit<8> bridge_port){
	// L2_BRIDGE_PORT_WDT
	ingress_metadata.l2_if_type = l2_if_type;
	ingress_metadata.bridge_port = bridge_port; 
}

action action_set_bridge_id(in bit<12> bridge_id){
	ingress_metadata.bridge_id = bridge_id;
}


action action_set_bridge_id_vid(){
	ingress_metadata.bridge_id =ingress_metadata.vid;
}

action action_set_bridge_id_with_vid() {
	ingress_metadata.bridge_id = ingress_metadata.vid;
}

action action_set_mcast_lookup_mode(in bit<2> mcast_mode){
	ingress_metadata.mcast_mode =mcast_mode;
}

action action_set_stp_state(in bit<3> stp_state){//need to enforce STP state  
	ingress_metadata.stp_state = stp_state;
}

action action_set_stp_id(in bit<3> stp_id){
	ingress_metadata.stp_id = stp_id;
}

action action_go_to_in_l3_if_table(){
	no_op();
}
//action action_go_to_fdb_table(){
//	no_op();
//}

//modify_field (ingress_metadata.,);

// L2
action action_learn_mac() {
	ingress_metadata.trap_id = MAC_LEARN_RECEIVER;	 //TODO, should this be configurable to support hostif interface(?)
	clone_ingress_pkt_to_egress(COPY_TO_CPU_MIRROR_ID, redirect_FL);
}

action action_set_egress_br_port(in bit<8> br_port){
	egress_metadata.bridge_port = br_port;
}

action action_set_vlan(in bit<12> vid) {
	ingress_metadata.vid = vid;
}

action action_forward_set_outIfType(in bit<6> out_if,in bit<1> out_if_type){
	egress_metadata.out_if 		  = out_if;
	egress_metadata.out_if_type   = out_if_type;
	standard_metadata.egress_spec = out_if; 
}

action action_set_unknown_unicast(in bit<1> unknown_unicast) {
	ingress_metadata.unknown_unicast = unknown_unicast;
}

//action action_ste_fdb_miss(in bit mc_fdb_miss){
//	ingress_metadata.mc_fdb_miss = mc_fdb_miss;
//}

action action_forward(in bit<6> br_port) {
  //  standard_metadata.egress_spec = port;	//need to map bride port to interface  
    egress_metadata.bridge_port = br_port;
}

action action_forward_mc_set_if_list(in bit<16> mcast_grp, in bit<1> go_to_router){
	// TODO add set egress if list
	modify_field(intrinsic_metadata.mcast_grp, mcast_grp);
	modify_field(ingress_metadata.go_to_router, go_to_router);
}

action action_set_egress_stp_state(in bit<2> stp_state){
	egress_metadata.stp_state = stp_state;
}

action action_forward_vlan_untag(){
	ethernet.etherType = vlan.etherType;
	remove_header(vlan);
}

action action_forward_vlan_tag(in bit<3> pcp, in bit cfi, in bit<12> vid){
	add_header(vlan);
	vlan.pcp = pcp;
	vlan.cfi = cfi;
	vlan.vid = vid;
	vlan.etherType = ethernet.etherType;
	ethernet.etherType = VLAN_TYPE;
	// egress_metadata.tag_mode = tag_mode;
}

action action_set_lag_hash_size(in bit<6> lag_size) {
	modify_field_with_hash_based_offset(egress_metadata.hash_val, 0, lag_hash, lag_size);
}

action action_set_out_port(in bit<6> port){
	standard_metadata.egress_spec 	= port;
}

//action broadcast() {
//    modify_field(egress_metadata.mcast_grp, 1);
//}

action set_egr(in bit<6> egress_spec) {
    modify_field(standard_metadata.egress_spec, egress_spec);
}

// mc
action action_set_mc_fdb_miss() {
	ingress_metadata.mc_fdb_miss=1;
}

action action_cpu_encap() { 
	add_header(cpu_header);
	cpu_header.ingress_port = standard_metadata.ingress_port;
	cpu_header.trap_id = ingress_metadata.trap_id;
	cpu_header.bridge_id = ingress_metadata.bridge_id;
	cpu_header.bridge_port = ingress_metadata.bridge_port;
}