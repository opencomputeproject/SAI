#include "defines.p4"

header_type ethernet_t {
    fields {
        bit<48> dstAddr;
        bit<48> srcAddr;
        bit<16> etherType;
    }
}
header_type vlan_t {
    fields {
        bit<3> 	pcp;
	    bit 	cfi;
	    bit<12>	vid;
	    bit<16>	etherType;
    }
    //length  4;
    //max_length 4;
}
header_type ipv4_t {
    fields {
        bit<4> 		version;
        bit<4> 		ihl;
        bit<8> 		diffserv;
        bit<16> 	ipv4_length;
        bit<16> 	id;
        bit<3> 		flags;
        bit<13> 	offset;
        bit<8> 		ttl;
        bit<8> 		protocol;
        bit<16> 	checksum;
        bit<32> 	srcAddr;
        bit<32> 	dstAddr;
    }
    //length ihl * 4;
    //max_length 32;
}
header_type tcp_t {
    fields {
        bit<16> 	srcPort;
        bit<16> 	dstPort;
        bit<32> 	seqNo;
        bit<32> 	ackNo;
        bit<4> 		dataOffset;
        bit<4> 		res;
        bit<8> 		flags;
        bit<16> 	window;
        bit<16> 	checksum;
        bit<16> 	urgentPtr;
    }
}

header_type udp_t {
    fields {
        bit<16> 	srcPort;
        bit<16> 	dstPort;
        bit<16> 	length_;
        bit<16> 	checksum;
    }
}


header_type ingress_metadata_t {
	fields {
		bit<6> 	port;	//PHY_PORT_NUM_WDT
		bit<6> 	l2_if;	//PHY_PORT_NUM_WDT
		bit 	is_tagged;
		bit 	is_lag;
		bit<16> lag_id; // LAG_WDT
		bit     bind_mode; 
		bit<2> 	l2_if_type;
		bit<8> 	bridge_port; //L2_BRIDGE_PORT_WDT
		bit<12> bridge_id;	 //L2_BRIDGE_NUM_WDT
		bit<2> 	stp_state;
		bit<3>	stp_id; // TODO size?
		bit<12> vid;
		bit<1> 	unknown_unicast;
        bit<2>  mcast_mode;
        bit<1>  mc_fdb_miss;
        bit     ipmc;
        bit     isip;
        bit     go_to_router;
        bit<32> mtu;
        bit     drop_tagged;
        bit     drop_untagged;
        bit<11> trap_id;
	}
}



header_type egress_metadata_t {
	fields {
		bit 	out_if_type; 
        bit<6> 	out_if; // PHY_PORT_NUM_WDT TODO remove? same as standard_metadata.egress_spec?
        bit<2> 	stp_state; // same as ingress? duplication?
        bit  	tag_mode;
        bit<6> 	hash_val;// TODO for egress lag table, when it is set?
        bit<4> 	mcast_grp;
        bit<8>  bridge_port; //L2_BRIDGE_PORT_WDT 
    }
}

header_type intrinsic_metadata_t {
    fields {
        bit <48> ingress_global_timestamp;
        bit <8> lf_field_list;
        bit <16> mcast_grp;
        bit <16> egress_rid;
        bit <8> resubmit_flag;
        bit <8> recirculate_flag;
    }
}

metadata intrinsic_metadata_t intrinsic_metadata;

header_type cpu_header_t {
    fields {
        bit<16> trap_id;
        bit<16> bridge_id;
        bit<8>  ingress_port;
        bit<8>  bridge_port;
    }
}