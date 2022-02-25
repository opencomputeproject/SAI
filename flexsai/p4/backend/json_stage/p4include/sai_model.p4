/*
Copyright (C) 2010-2017. Mellanox Technologies, Ltd.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/* P4-16 declaration of the P4 v1.0 switch model */

#ifndef _SAI_MODEL_P4_
#define _SAI_MODEL_P4_

#include "core.p4"
#include "sai_metadata.p4"

// Extend the core match_kind here
// match_kind {
// }


// Sai Architecture model.
// M should be a struct of structs
// H should be a struct of headers or stacks
/* Prototypes for all programmable blocks */
/**
* Programmable parser.
* @param b input packet
* @param <H> headers constructed by parser, defined by user
* @param standard_metadata_t fixed, standard metadata for sai
* @param M Flexible, user defined metadata
*/
parser SaiParser<H, M>(packet_in b,
                            out H parsedHeaders,
                            inout M user_metadata,
                            inout standard_metadata_t metadata);

/**
* L2 Match-action pipeline for L2 port.
* Ingress instance will have next block as HW bridge
* Egress instance will have next block as egress port
* @param <H> type of input and output headers
* @param <M> User defined metadata
* @param headers headers received from the parser and sent to the deparser
* @param inCtrl information from architecture, accompanying input packet
* @param outCtrl information from architecture, accompanying output packet
*/
@pipeline
control SaiIngressPort<H, M>(inout H headers,
                                  inout M user_metadata,
                                  inout standard_metadata_t standard_metadata,
                                  inout in_port_metadata_t in_port_metadata);

@pipeline
control SaiEgressPort<H, M>(inout H headers,
                                 inout M user_metadata,
                                 inout standard_metadata_t standard_metadata,
                                 inout out_port_metadata_t out_port_metadata);

@pipeline
control SaiIngressRif<H, M>(inout H headers,
                                inout M user_metadata,
                                inout standard_metadata_t standard_metadata,
                                inout in_rif_metadata_t in_rif_metadata);

@pipeline
control SaiEgressRif<H, M>(inout H headers,
                                inout M user_metadata,
                                inout standard_metadata_t standard_metadata,
                                inout out_rif_metadata_t out_rif_metadata);

/**
* Switch deparser.
* @param <H> type of headers; defined by user
* @param b output packet
*/
control SaiDeparser<H>(inout H headers,
                            packet_out b);

/**
* Top-level package declaration - must be instantiated by user.
* The arguments to the package indicate blocks that
* must be instantiated by the user.
* @param <H> user-defined type of the headers processed.
*/
package SaiSwitch<H, M>(SaiParser<H, M> parse,
                             SaiIngressPort<H, M> ingressPort,
                             SaiEgressPort<H, M> egressPort,
                             SaiIngressRif<H, M> ingressRif,
                             SaiEgressRif<H, M> egressRif,
                             SaiDeparser<H> deparse);


enum HashAlgorithm {
    crc32,
    crc32_custom,
    crc16,
    crc16_custom,
    random,
    identity
}

@name("drop")
extern void drop();
@name("set_bridge_id")
extern void set_bridge_id(sai_object_id_t bridge_id);
@name("vxlan_tunnel_encap")
extern void vxlan_tunnel_encap(sai_object_id_t tunnel_id, sai_ip4_t underlay_dip);
@name("go_to_router")
extern void go_to_router(sai_object_id_t router_id);
@name("go_to_port")
extern void go_to_port(sai_object_id_t port_id);
@name("nop")
extern void nop();



#endif  /* _SAI_MODEL_P4_ */
