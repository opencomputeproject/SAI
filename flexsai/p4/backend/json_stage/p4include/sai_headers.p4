/*
 *  Copyright (C) 2017. Mellanox Technologies, Ltd. ALL RIGHTS RESERVED.
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
 */

#ifndef _SAI_HEADERS_
#define _SAI_HEADERS_

/* IP protocol numbers in the Protocol field of the IPv4 header
* and the Next Header field of IPv6 header
*/
const bit<8>  IPV6_HBH_OPTION = 0x00;
const bit<8>  ICMP_PROTOCOL = 0x01;
const bit<8>  IGMP_PROTOCOL = 0x02;
const bit<8>  TCP_PROTOCOL = 0x06;
const bit<8>  UDP_PROTOCOL = 0x11;
const bit<8>  GRE_PROTOCOL = 0x2F;
const bit<8>  ESP_PROTOCOL = 0x32;
const bit<8>  AH_PROTOCOL = 0x33;
const bit<8>  ICMP6_PROTOCOL = 0x3A;


/* Ethertype */
const bit<16> TYPE_VLAN = 0x8100;
const bit<16> TYPE_IPV4 = 16w0x0800;
const bit<16> TYPE_IPV6 = 16w0x86DD;
const bit<16> TYPE_ARP = 0x0806;
const bit<16> TYPE_CONTROL = 0x0808;
const bit<16> TYPE_MPLS = 0x8848;
const bit<16> TYPE_PTP = 0x88F7;
const bit<16> TYPE_FCOE = 0x8906;
const bit<16> TYPE_ROCE = 0x8915;  // v1

/* UDP ports */
// const bit<16> ROCE_PORT = 4791;   // v2
const bit<16> VXLAN_PORT = 4789;
// const bit<16> GENEVE_PORT = 6081;
// const bit<16> PTP_EVENT_PORT = 319;
// const bit<16> PTP_GEN_PORT = 320;
// const bit<5>  IPV4_OPTION_MRI = 31;

#define MAX_HOPS 9

@ethernetaddress typedef bit<48> EthernetAddress;
@ipv4address     typedef bit<32>     IPv4Address;
@ipv6address     typedef bit<128>     IPv6Address;

/******/
/* L2 */ 
/******/
// standard Ethernet header
header Ethernet_h
{
    EthernetAddress dst_addr;
    EthernetAddress src_addr;
    bit<16> ether_type;
}

header Vlan_h 
{
    bit<3>  pcp;   /* also called pri */
	bit     dei;   /* also called cfi */
	bit<12> vlan_id;   /* vlan ID */
	bit<16> ether_type;
}

/******/
/* L3 */ 
/******/
// IPv4 header without options
header IPv4_h {
    bit<4>       version;
    bit<4>       ihl;
    bit<6>       diffserv;
    bit<2>       ecn;
    bit<16>      total_len;
    bit<16>      identification;
    bit<3>       flags;
    bit<13>      frag_offset;
    bit<8>       ttl;
    bit<8>       protocol;
    bit<16>      hdr_checksum;
    IPv4Address  src_addr;
    IPv4Address  dst_addr;
}

header IPv6_h {
    bit<4>       version;
    bit<8>       traffic_class;
    bit<20>      flow_label;
    bit<16>      payload_len;
    bit<8>       next_hdr;   // same as ipv4 protocol
    bit<8>       hop_limit;
    IPv6Address       src_addr;
    IPv6Address       dst_addr;
}

// Only one option is allowed
header_union IP_h {
  IPv4_h v4;
  IPv6_h v6;
}

header Mpls_h {
    bit<20> label;
    bit<3> tc;
    bit<1> bos;
    bit<8> ttl;
}
header Grh_h {
    // TODO
}
header Fcoe_h {
    // TODO
}

header Arp_h {
    bit<16> hw_type;
    bit<16> proto_type;
    bit<8> hw_addr_len;
    bit<8> proto_addr_len;
    bit<16> opcode;
    // TODO
}
header Ptp_h {
    bit<4> msg_type;
    bit<16> seq_id;
    bit<8> domain_num;
    bit<4> ver;
    // TODO
}
header Control_h {
    bit<16> opcode;
    bit<16> pause_time;
    bit<16> class_enable;
    bit<128> pfc_pause_time;
    // TODO
}
header Raw_h {
    bit<192> eth_payload;
    bit<6> payload_valid;
    // TODO
}

/******/
/* L4 */ 
/******/
header Icmp_h {
    bit<8> icmp_type;
    bit<8> icmp_code;
    bit<16> checksum;
    bit<16> identifier;
    bit<16> sequence_number;
    bit<64> timestamp;
}

header Tcp_h {
    bit<16> src_port;
    bit<16> dst_port;
    bit<32> seq_no;
    bit<32> ack_no;
    bit<4>  data_offset;
    bit<4>  res;
    bit<8>  flags;
    bit<16> window;
    bit<16> checksum;
    bit<16> urgent_ptr;
}

header Udp_h {
    bit<16> src_port;
    bit<16> dst_port;
    bit<16> length;
    bit<16> checksum;
}

header Ah_h {
    bit<8> nextProtocol;
    bit<32> spi;
}

header Esp_h  {
    bit<32> spi;
}

header Icmp_v4_h {
    bit<8> type;
    bit<8> code;
    //bit<16> checksum;
}

header Icmp_v6_h {
    bit<8> type;
    bit<8> code;
    //bit<16> checksum;
}

header Bth_h {
    bit<8>      opcode;
    bit         solicited_event;
    bit         mitigation_state;
    bit<2>      pad_cnt;
    bit<4>      transport_hdr_ver;
    bit<16>     parition_key;
    bit         fecn;
    bit         becn;
    bit<6>      reserved1;
    bit<24>     dest_queue_pair;
    bit         ack_req;
    bit<7>      reserved2;
    bit<24>     pkt_seq_num;
    bit<16>     zero_padding;
    bit<4>      icrc;
}

header Gre_h     {
    bit    C;
    bit    R;
    bit    K;
    bit    S;
    bit    s;
    bit<3> recur;
    bit<5> flags;
    bit<3> ver;
    // rest are options based on flags
}

/******/
/* Application or L7 */
/******/

header Vxlan_h {
    bit<4>   reserved1;
    bit      isValid;
    bit<3>   reserved2;
    bit<24>  reserved3;
    bit<24>  vni;
    bit<8>   reserved4;
}

header Vxlan_gpe_h{
    bit<4>   reserved1;
    bit      isValid;
    bit<3>   reserved2;
    bit<16>  reserved3;
    bit<8>   next_proto;
    bit<24>  vni;
    bit<8>   reserved4;
}

header Rtp_h {
    bit<2> ver;
    bit<1> pad;
    bit<1> ext;
    bit<4> csrc_cnt;
    bit<1> marker;
    bit<7> payload_type;
    bit<16> seq;
    bit<32> timestamp;
    bit<32> ssrc_id;
}

#endif
