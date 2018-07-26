// Parser Defintion file.
// Defines how stream of bytes
// that enters the switch,
// get parsed into meaningful packets.


// Structure of parsed headers
struct Headers_t
{
    Ethernet_h ethernet;
    Vlan_h     vlan;

    IP_h       ip;
    Mpls_h     mpls;
 //   Grh_h      grh;
    Fcoe_h     fcoe;
    Arp_h      arp;
    Ptp_h      ptp;
    Control_h  ctl;
    Raw_h      raw;

    Ah_h       ah;
    Esp_h      esp;
    Icmp_v4_h  icmp4;
    Icmp_v6_h  icmp6;
    Bth_h      bth;
    Gre_h      gre;
    Tcp_h      tcp;
    Udp_h      udp;

    Ptp_h      ptp_udp;
    Vxlan_h    vxlan;
    Vxlan_gpe_h vxlangpe;
    // Deth_h     deth;
    // Nd_h       nd;
}

// Parser section
// This describes the default parser graph state machine 
parser Parser(packet_in p,
                 out Headers_t headers,
                 inout user_metadata_t user_meta,
                 inout standard_metadata_t standard_metadata)
{
    state start
    {
        p.extract(headers.ethernet);
        transition select(headers.ethernet.ether_type)
        {
            TYPE_VLAN : parse_vlan;
            TYPE_IPV4 : parse_ipv4;    // 16w0x800
            TYPE_IPV6 : parse_ipv6;    // 16w0x86DD
            TYPE_ARP  : parse_arp;
            TYPE_PTP  : parse_ptp;
            TYPE_MPLS : parse_mpls;
 //         TYPE_GRH  : parse_grh;
            TYPE_FCOE : parse_fcoe;
            TYPE_CONTROL : parse_control;

            // other parser states go here
            // default : reject;
            default : parse_raw;
        }
    }

    state parse_vlan {
        p.extract(headers.vlan);
        transition select(headers.vlan.ether_type) {
            TYPE_IPV4 : parse_ipv4;
            TYPE_IPV6 : parse_ipv6;    // 16w0x86DD
            TYPE_ARP  : parse_arp;
            TYPE_PTP  : parse_ptp;
            TYPE_MPLS : parse_mpls;
 //         TYPE_GRH  : parse_grh;
            TYPE_FCOE : parse_fcoe;
            TYPE_CONTROL : parse_control;
            default: accept;
        }
    }

    state parse_ipv4
    {
        p.extract(headers.ip.v4);
        // verify(headers.ip.v4.version == 4w4, error.IPv4IncorrectVersion);
        // verify(headers.ip.v4.ihl == 4w5, error.IPv4OptionsNotSupported);
        transition select(headers.ip.v4.protocol)
        {
            TCP_PROTOCOL : parse_tcp;
            UDP_PROTOCOL : parse_udp;
            // custom parser states go here

            // no default rule: all other packets rejected
            default : accept;
        }
    }

    state parse_ipv6
    {
        p.extract(headers.ip.v6);
        transition accept;
    }

    state parse_arp
    {
        //p.extract(headers.arp);
        transition accept;
    }

    state parse_tcp
    {
        p.extract(headers.tcp);
        transition accept;
    }

    state parse_udp
    {
        p.extract(headers.udp);
        transition select(headers.udp.dst_port)
        {
            VXLAN_PORT : parse_vxlan;
            default: accept;
        }
    }

    // Built in states
    state parse_roce
    {
        p.extract(headers.bth);
        transition accept;
    }
    
    state parse_vxlan { transition accept; }
    state parse_ptp { transition accept; }
    state parse_mpls { transition accept; }
    state parse_grh { transition accept; }
    state parse_fcoe { transition accept; }
    state parse_control { transition accept; }
    state parse_raw { transition accept; }
}


control Deparser(inout Headers_t headers, packet_out b) {
    apply {
        b.emit(headers.ethernet);
        b.emit(headers.vlan);
        b.emit(headers.ip.v4);
        b.emit(headers.udp);
        b.emit(headers.tcp);
    }
}




