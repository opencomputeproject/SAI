// Parser Defintion file.
// Defines how stream of bytes
// that enters the switch,
// get parsed into meaningfull packets. 

#define TCP_PROTOCOL_NUM	0x06
#define UDP_PROTOCOL_NUM	0x11
#define VLAN_TYPE 		0x8100
#define IPV4_TYPE		0x0800


// parser starts here
// check if needs to add clone_to_cpu encapsulation
parser start {
    return select(current(0, 64)) {
        0 : parse_cpu_header;
        default: parse_ethernet;
    }
}


parser parse_cpu_header {
    extract(cpu_header);
    return parse_ethernet;
}

// ethernet parser - decide next layer according the ethertype
parser parse_ethernet {
    extract(ethernet);
    set_metadata(ingress_metadata.is_tagged,0);
    return select(latest.etherType) {
    	VLAN_TYPE : parse_vlan;
        IPV4_TYPE : parse_ipv4;
        default: ingress;
    }
}

parser parse_vlan {
    extract(vlan);
    set_metadata(ingress_metadata.is_tagged, (bit)(vlan.vid >> 11) | (bit)(vlan.vid >> 10) | (bit)(vlan.vid >> 9) | (bit)(vlan.vid >> 8) | (bit)(vlan.vid >> 7) | (bit)(vlan.vid >> 6) | (bit)(vlan.vid >> 5) | (bit)(vlan.vid >> 4) | (bit)(vlan.vid >> 3) | (bit)(vlan.vid >> 2) | (bit)(vlan.vid >> 1) | (bit)(vlan.vid)); //if vid==0 not tagged. TODO: need to do this better (maybe add parser support for casting boolean)
    return select(latest.etherType) {
        IPV4_TYPE : parse_ipv4;
        default: ingress;
    }
}

// IPv4 parser, decide next layer according to protocol field
parser parse_ipv4 {
    extract(ipv4);
    set_metadata(ingress_metadata.isip,1);
    return post_parse_ipv4 ;
}

parser post_parse_ipv4 {
    return select(ipv4.protocol) {
        TCP_PROTOCOL_NUM 	: parse_tcp;
        UDP_PROTOCOL_NUM	: parse_udp;
        default 			: ingress;
    }
}


// TCP parser, next layer are irrelvant for us, thus not included
// (cosidered payload data).
parser parse_udp {
	extract(udp);
	return ingress;
}

parser parse_tcp {
	extract(tcp);
	return ingress;
}
