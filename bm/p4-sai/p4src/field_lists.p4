field_list mac_learn_digest {
    ethernet.srcAddr;
    ingress_metadata.bridge_id; // TODO 
    standard_metadata.ingress_port; // TODO
}

field_list_calculation ipv4_checksum {
    input {
        ipv4_checksum_list;
    }
    algorithm : csum16;
    output_width : 16;
}

field_list ipv4_checksum_list {
        ipv4.version;
        ipv4.ihl;
        ipv4.diffserv;
        ipv4.ipv4_length;
        ipv4.id;
        ipv4.flags;
        ipv4.offset;
        ipv4.ttl;
        ipv4.protocol;
        ipv4.srcAddr;
        ipv4.dstAddr;
}

field_list lag_hash_fieldlist { 
    ethernet.srcAddr;
    ipv4.id;
}

field_list_calculation lag_hash {
    input {
        lag_hash_fieldlist;
    }
    algorithm : xor8; //TODO: change to lag_hash
    output_width : 1; //TODO: LOG2(NUM_OF_PORTS)
}

field_list redirect_FL {
    standard_metadata;
    ingress_metadata.trap_id;
    ingress_metadata.bridge_id;
    ingress_metadata.bridge_port;
}