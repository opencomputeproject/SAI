//-----------------------------------------
//------- User code example ---------------
//-----------------------------------------

#include <core.p4>
#include <sai_model.p4>
#include <sai_headers.p4>
#include "metadata.p4"
#include "parser.p4"

#define VHOST_TABLE_SIZE 256
#define PORTNUM 32

control control_in_port(inout Headers_t headers, inout user_metadata_t user_meta, inout standard_metadata_t standard_metadata, inout in_port_metadata_t in_port_meta){

    action set_bridge(sai_object_id_t tunnel_id, sai_ip4_t underlay_dip, sai_object_id_t bridge_id){
        set_bridge_id(bridge_id);
    }

    action to_router(sai_object_id_t router_id) {
        go_to_router(router_id);
    }

    action to_port(sai_object_id_t port_id) { 
        go_to_port(port_id);
    }

    table table_ex1{
        key = { 
            in_port_meta.ingress_port :exact;
        }
        actions = {set_bridge;}
        size = PORTNUM;
    }   

    table table_ex2{
        key = { 
            headers.ip.v4.dst_addr :exact;
        }
        actions = {to_router;to_port;}
        size=VHOST_TABLE_SIZE;
    }   


    // pipe
    apply{
        table_ex1.apply();
        table_ex2.apply();
    }
}

control control_out_port(inout Headers_t headers, inout user_metadata_t user_meta, inout standard_metadata_t standard_metadata, inout out_port_metadata_t out_port_meta){
    apply{}
}

control control_in_rif(inout Headers_t headers, inout user_metadata_t user_meta, inout standard_metadata_t standard_metadata, inout in_rif_metadata_t in_rif_meta){
    apply{}
}

control control_out_rif(inout Headers_t headers, inout user_metadata_t user_meta, inout standard_metadata_t standard_metadata, inout out_rif_metadata_t out_rif_meta){
    apply{}
}



SaiSwitch(
    Parser(),
    control_in_port(),
    control_out_port(),
    control_in_rif(),
    control_out_rif(),
    Deparser()
    ) main;
