SAI L3 Tunnel Interface API Proposal
=====================

 Title       | SAI L3 Tunnel Interface API Proposal
-------------|----------------------
 Authors     | Broadcom, Microsoft
 Status      | In review
 Type        | Standards track
 Created     | 04/03/2015
 SAI-Version | 0.9.3

---------

# Overview #

L3 tunnels, such as IP-in-IP, GRE, NVGRE, VXLAN, GENEVE, use IP fabrics as a transit network. They use IP encapsulation to deliver packets to the destination in the network through standard switching and routing.

The SAI tunnel API provides separate APIs to manage **L3 tunnel ingress interface** and **L3 tunnel egress interface**. L3 tunnel egress interface is created at the starting endpoint of the tunnel to do the encapsulation, and the L3 tunnel ingress interface is created at the termination endpoint of the tunnel to decapsulate the tunneled packet. (Here egress means it is the outgoing interface of the switch, while ingress means it is the incoming interface of the switch.)

(Current proposal only defines APIs for IP-in-IP tunnel egress interfaces, others are left for the future)

## L3 Tunnel Egress Interface Model ##

Depending on the ASIC pipeline, there are two models for the L3 tunnel egress interface, the **one-pass model** and the **two-pass model**.

For the one-pass model, There are two types of L3 egress interface defined in the SAI, namely **router interface** and **l3 tunnel egress interface**. To note, the router interface object is used for both ingress and egress interface. As shown in Figure 1, the `sai_next_hop` object can point to either a router interface or a tunnel egress interface. A tunnel egress interface must point to a router interface. In this model, the tunnel egress interface is responsible for adding the tunneling header, e.g., IP, GRE, NVGRE. The router interface is responsible for updating the Ethernet source mac and vlan header. The next hop provides the destination mac information. When multiple tunnel egress interfaces point to a same router interface, all these tunnels will share a same source mac address and vlan id.

As specified in earlier spec, a route object can point to either a next hop object or a next hop group object. In case the route points to a next hop group, and the next hop group object points to multiple next hops, and each next hop points to a tunnel egress interface. This will spread the IP packets over a set of L3 tunnels. 

![Relations between SAI objects for L3 Tunnel egress interface](figures/sai_l3_tunnel_egress.png "Figure 1: Relations between SAI objects for L3 Tunnel egress interface (one-pass mode)")

__Figure 1: Relations between SAI objects for L3 Tunnel egress interface (one-pass model)__

For the two-pass model, the tunnel egress provides all the information for the tunneling header. After adding the tunneling header, the ASIC will do a L3 lookup on the outer destination IP to find the next hops. In the model, tunnel egress object will be directly associated with a route or a next hop group.

![Relations between SAI objects for L3 Tunnel egress interface](figures/sai_l3_tunnel_egress_twopass_model.png "Figure 1: Relations between SAI objects for L3 Tunnel egress interface (two-pass mode)")

__Figure 1: Relations between SAI objects for L3 Tunnel egress interface (two-pass model)__

A switch attribute `SAI_SWITCH_ATTR_TUNNEL_EGRESS_MODE` is added to indicate with egress model the SAI supports on a particular platform.

## L3 Tunnel Ingress Interface Model ##

A tunnel Ingress Interface is defined to terminate a tunnel. If an incoming packet matches one of the router interfaces on the switch, a lookup is done in the L3 Tunnel ingress table to match the packet based on the Destionation IP Address in the outer IP header. If a match is found, then incoming packet further validated against the source IP. If source IP address validation is also passed, the Tunnel is terminated and outer IP header is removed. Normal L3 processing is performed on the inside packet. If the source IP address validation is not passed, the packet is then dropped. If destination IP match is not found, the packet is not tunnel terminated and go to normal L3 forwarding.

![Relations between SAI objects for L3 Tunnel ingress interface](figures/sai_l3_tunnel_ingress.png "Figure 1: Relations between SAI objects for L3 Tunnel ingress interface")

__Figure 2: Relations between SAI objects for L3 Tunnel igress interface__

# Specification #

## New Switch Attributes ##

~~~cpp
/*
 * L3 tunnel egress interface mode
 */
typedef enum _sai_switch_attr_tunnel_egress_mode_t
{
    /* no further lookup for the encap header to decide the next hop */
    SAI_SWITCH_ATTR_TUNNEL_EGRESS_MODE_NO_ENCAP_HEADER_LOOKUP,

    /* further lookup for the encap header to decide the next hop */
    SAI_SWITCH_ATTR_TUNNEL_EGRESS_MODE_ECNAP_HEADER_LOOKUP,

} sai_switch_attr_tunnel_egress_mode_t

/* READ-ONLY */
/* Tunnel egress mode supported [sai_switch_attr_tunnel_egress_mode_t] */
SAI_SWITCH_ATTR_TUNNEL_EGRESS_MODE

~~~
## Changes To sai.h ##

A new type **SAI_API_L3_TUNNEL** is added.

~~~cpp
/*
*
* Defined API sets have assigned ID's. If specific api method table changes
* in any way (method signature, number of methods), a new ID needs to be
* created (e.g. VLAN2) and old API still may need to be supported for
* compatibility with older adapter hosts.
*
*/
typedef enum _sai_api_t
{
    SAI_API_UNSPECIFIED      =  0,
    SAI_API_SWITCH           =  1,  /* sai_switch_api_t */
    SAI_API_PORT             =  2,  /* sai_port_api_t */
    SAI_API_FDB              =  3,  /* sai_fdb_api_t */
    SAI_API_VLAN             =  4,  /* sai_vlan_api_t */
    SAI_API_VIRTUAL_ROUTER   =  5,  /* sai_virtual_router_api_t */
    SAI_API_ROUTE            =  6,  /* sai_route_api_t */
    SAI_API_NEXT_HOP         =  7,  /* sai_next_hop_api_t */
    SAI_API_NEXT_HOP_GROUP   =  8,  /* sai_next_hop_group_api_t */
    SAI_API_ROUTER_INTERFACE =  9,  /* sai_router_interface_api_t */
    SAI_API_NEIGHBOR         = 10,  /* sai_neighbor_api_t */
    SAI_API_QOS              = 11,  /* sai_qos_api_t */
    SAI_API_ACL              = 12,  /* sai_acl_api_t */
    SAI_API_HOST_INTERFACE   = 13,  /* sai_host_interface_api_t */
    SAI_API_L3_TUNNEL        = 14,  /* sai_l3_tunnel_api_t */
} sai_api_t;
~~~

## Next Hop Attribute ##
`SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID` is renamed to `SAI_NEXT_HOP_ATTR_L3_EGRESS_INTERFACE_ID`.

~~~cpp
/*
*  Attribute id for next hop
*/
typedef enum _sai_next_hop_attr_t
{
    /* READ-ONLY */

    /* READ-WRITE */

    /* Next hop entry ip address [sai_ip_address_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
    SAI_NEXT_HOP_ATTR_IP,

    /* Next hop entry l3 egress interface id [sai_object_id_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
    SAI_NEXT_HOP_ATTR_L3_EGRESS_INTERFACE_ID,

    /* -- */

    /* Custom range base value */
    SAI_NEXT_HOP_ATTR_CUSTOM_RANGE_BASE  = 0x10000000

} sai_next_hop_attr_t;
~~~

## New Header sail3tunnelintf.h ##

### L3 Tunnel Interface Type ###

*sai_l3_tunnel_interface_type_t* defines the types of the tunnel. More specifically, it specifies the type of the encapsulated header. Currently, IPv4 and IPv6 are supported.

~~~cpp
/*
 * L3 tunnel interface type, i.e., the type of the encapped header
 */
typedef enum _sai_l3_tunnel_interface_type_t
{
    /* Sai l3 tunnel interface IPv4 */
    SAI_L3_TUNNEL_INTERFACE_IPV4,

    /* Sai l3 tunnel interface IPv6 */
    SAI_L3_TUNNEL_INTERFACE_IPV6,

} sai_l3_tunnel_interface_type_t;
~~~

### L3 Tunnel Egress Interface Attribute ###

*sai_l3_tunnel_egress_interface_attr_t* defines the l3 tunnel egress interface attributes.

~~~cpp
/*
 *  Attribute id for l3 tunnel egress interface
 */
typedef enum _sai_l3_tunnel_egress_interface_attr_t
{
    /* READ-ONLY */

    /* L3 tunnel egress interface router interface. [sai_object_id_t] (MANDATORY_ON_CREATE|CREATE_ONLY) 
     * Mandatory only when SAI_SWITCH_ATTR_TUNNEL_EGRESS_MODE == SAI_SWITCH_ATTR_TUNNEL_EGRESS_MODE_ECNAP_HEADER_LOOKUP */
    SAI_L3_TUNNEL_EGRESS_INTERFACE_ATTR_ROUTER_INTF,

    /* READ-WRITE */

    /* L3 tunnel egress interface type [sai_l3_tunnel_egress_interface_type_t] (MANDATORY_ON_CREATE|CREATE_AND_SET) */
    SAI_L3_TUNNEL_EGRESS_INTERFACE_ATTR_TYPE,

    /* L3 tunnel egress interface source IP address. This has to be coherent with the SAI_L3_TUNNEL_EGRESS_INTERFACE_ATTR_TYPE. [sai_ip_address_t] (MANDATORY_ON_CREATE|CREATE_AND_SET) */
    SAI_L3_TUNNEL_EGRESS_INTERFACE_ATTR_SIP,

    /* L3 tunnel egress interface destination IP address. This has to be coherent with the SAI_L3_TUNNEL_EGRESS_INTERFACE_ATTR_TYPE. [sai_ip_address_t] (MANDATORY_ON_CREATE|CREATE_AND_SET) */
    SAI_L3_TUNNEL_EGRESS_INTERFACE_ATTR_DIP,

    /* L3 tunnel egress interface ttl [uint8_t] (CREATE_AND_SET) (default to 64) */
    SAI_L3_TUNNEL_EGRESS_INTERFACE_ATTR_TTL,

    /* L3 tunnel egress interface dscp [uint8_t] (CREATE_AND_SET) (default to 0) */
    SAI_L3_TUNNEL_EGRESS_INTERFACE_ATTR_DSCP,

    /* L3 tunnel egress interface IPv6 flow label [uint32_t] (CREATE_AND_SET) (default to 0) */
    SAI_L3_TUNNEL_EGRESS_INTERFACE_ATTR_IPV6_FLOW_LABEL,

} sai_l3_tunnel_egress_interface_attr_t;
~~~

### L3 Tunnel Ingress Interface Attribute ECN mode ###

*sai_l3_tunnel_ingress_interface_attr_copy_ecn_mod_t* defines how the ECN is copied from outside header to inside header

~~~cpp
/*
 * L3 tunnel ingress interface attribute ecn mode
 */
typedef enum _sai_l3_tunnel_ingress_interface_attr_ecn_mode_t
{
    /* Do not copy ecn fields from outer header to inner header after tunnel decap */
    SAI_L3_TUNNEL_INTERFACE_ATTR_ECN_MODE_NONE,

    /* Copy ecn from outer header to inner header after tunnel decap */
    SAI_L3_TUNNEL_INTERFACE_ATTR_ECN_MODE_SIMPLE,

    /* Set ecn fields of inner header based on RFC 6040 */
    SAI_L3_TUNNEL_INTERFACE_ATTR_ECN_MODE_RFC6040,

} sai_l3_tunnel_ingress_interface_attr_ecn_mode_t;
~~~

### L3 Tunnel Ingress Interface Attribute ###

`sai_l3_tunnel_ingress_interface_attr_t` defines the l3 tunnel ingress interface attributes.

~~~cpp
/*
 *  Attribute id for l3 tunnel ingress interface
 */
typedef enum _sai_l3_tunnel_ingress_interface_attr_t
{
    /* READ-ONLY */

    /* READ-WRITE */
    /* L3 tunnel ingress interface type [sai_l3_tunnel_interface_type_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
    SAI_L3_TUNNEL_INGRESS_INTERFACE_ATTR_TYPE,

    /* L3 tunnel ingress interface destination IP address match. 
     * This has to be coherent with the SAI_L3_TUNNEL_INTERFACE_ATTR_TYPE. 
     * [sai_ip_address_t] (MANDATORY_ON_CREATE|CREATE_AND_SET) */
    SAI_L3_TUNNEL_INGRESS_INTERFACE_ATTR_DIP,

    /* L3 tunnel ingress interface source IP prefix validation. 
     * This has to be coherent with the SAI_L3_TUNNEL_INTERFACE_ATTR_TYPE.
     * Packets failed to pass the source IP prefix validation are dropped
     * [sai_ip_prefix_t] (MANDATORY_ON_CREATE|CREATE_AND_SET) */
    SAI_L3_TUNNEL_INGRESS_INTERFACE_ATTR_SIP_PREFIX,

    /* L3 tunnel ingress interface dscp, whether to copy outer DSCP field to inner header.
     * [bool] (CREATE_AND_SET) (default to false) */
    SAI_L3_TUNNEL_INGRESS_INTERFACE_ATTR_COPY_DSCP,

    /* L3 tunnel ingress interface ECN, whether to copy outer ECN field to inner header ECN (simple) or comply to RFC6040.
     * [sai_l3_tunnel_ingress_interface_attr_copy_ecn_t] (CREATE_AND_SET)  (default to SAI_L3_TUNNEL_INGRESS_INTERFACE_ATTR_COPY_ECN_MODE_NONE) */
    SAI_L3_TUNNEL_INGRESS_INTERFACE_ATTR_COPY_ECN,

    /* L3 tunnel ingress interface virtual router. 
     * Virtual router associated with this tunnel. Post Decap the L3 address lookup will happen in this VRF. 
     * [sai_object_id_t] (CREATE_AND_SET) (default to SAI_SWITCH_ATTR_DEFAULT_VIRTUAL_ROUTER_ID) */
    SAI_L3_TUNNEL_INGRESS_INTERFACE_ATTR_VIRTUAL_ROUTER_ID,
} sai_l3_tunnel_ingress_interface_attr_t;

~~~

### Create L3 Tunnel Interface ###

*sai_create_l3_tunnel_interface_fn* defines the interface to create l3 tunnel interface.

~~~cpp
/*
 * Routine Description:
 *    Create l3 tunnel interface
 *
 * Arguments:
 *    [out] l3_tunnel_interface_id - l3 tunnel interface id
 *    [in] attr_count - number of attributes
 *    [in] attr_list - array of attributes
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
typedef sai_status_t (*sai_create_l3_tunnel_interface_fn)(
    _Out_ sai_object_id_t* l3_tunnel_interface_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list
    );
~~~

### Remove L3 Tunnel Interface ###

*sai_remove_l3_tunnel_interface_fn* defines the interface to remove l3 tunnel interface.

~~~cpp
/*
 * Routine Description:
 *    Remove l3 tunnel interface
 *
 * Arguments:
 *    [in] l3_tunnel_interface_id - l3 tunnel interface id
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
typedef sai_status_t (*sai_remove_l3_tunnel_interface_fn)(
    _In_ sai_object_id_t l3_tunnel_interface_id
    );
~~~

### Set L3 Tunnel Interface Attributes ###

*sai_set_l3_tunnel_interface_attribute_fn* defines the interface to set attributes for the l3 tunnel interface.

~~~cpp
/*
 * Routine Description:
 *    Set l3 tunnel interface attribute
 *
 * Arguments:
 *    [in] sai_object_id_t - l3_tunnel_interface_id
 *    [in] attr - attribute
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
typedef sai_status_t (*sai_set_l3_tunnel_interface_attribute_fn)(
    _In_ sai_object_id_t l3_tunnel_interface_id,
    _In_ const sai_attribute_t *attr
    );
~~~

### Get L3 Tunnel Interface Attributes ###

*sai_get_l3_tunnel_interface_attribute_fn* defines the interface to get attributes for the l3 tunnel interface.

~~~cpp
/*
 * Routine Description:
 *    Get l3 tunnel interface attribute
 *
 * Arguments:
 *    [in] sai_object_id_t - l3_tunnel_interface_id
 *    [in] attr_count - number of attributes
 *    [inout] attr_list - array of attributes
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
typedef sai_status_t (*sai_get_l3_tunnel_interface_attribute_fn)(
    _In_ sai_object_id_t l3_tunnel_interface_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list
    );
~~~

### Tunnel API Table ###

*sai_tunnel_api_t* defines the tunnel API table.

~~~cpp
/*
 *  L3 tunnel methods table retrieved with sai_api_query()
 */
typedef struct _sai_l3_tunnel_api_t
{
    sai_create_l3_tunnel_interface_fn        create_l3_tunnel_egress_interface;
    sai_remove_l3_tunnel_interface_fn        remove_l3_tunnel_egress_interface;
    sai_set_l3_tunnel_interface_attribute_fn set_l3_tunnel_egress_interface_attribute;
    sai_get_l3_tunnel_interface_attribute_fn get_l3_tunnel_egress_interface_attribute;

    sai_create_l3_tunnel_interface_fn        create_l3_tunnel_ingress_interface;
    sai_remove_l3_tunnel_interface_fn        remove_l3_tunnel_ingress_interface;
    sai_set_l3_tunnel_interface_attribute_fn set_l3_tunnel_ingress_interface_attribute;
    sai_get_l3_tunnel_interface_attribute_fn get_l3_tunnel_ingress_interface_attribute;

} sai_tunnel_api_t;
~~~


# Example #

## Get The Tunnel API Table ##

The following code shows how to get the tunnel API table:

~~~cpp
sai_l3_tunnel_api_t* sai_l3_tunnel_api;
if (sai_api_query(SAI_API_L3_TUNNEL, (void**)&sai_l3_tunnel_api) == SAI_STATUS_SUCCESS)
{
    // Succeeded...
}
else
{
    // Failed...
}
~~~

## Create A L3 Tunnel Egress Interface ##

The following code shows how to create a l3 tunnel egress interface:

~~~cpp
sai_attribute_t attr;
attr.id = SAI_SWITCH_ATTR_TUNNEL_EGRESS_MODE;
sai_switch_api->get_switch_attribute(1, &attr);
int egress_mode = attr.value.s32;
int attrnum = 4;

sai_object_id_t l3_tunnel_egress_interface_id;
sai_attribute_t l3_tunnel_egress_interface_attrs[4];
l3_tunnel_egress_interface_attrs[0].id = (sai_attr_id_t)SAI_L3_TUNNEL_EGRESS_INTERFACE_ATTR_TYPE;
l3_tunnel_egress_interface_attrs[0].value.s32 = (sai_int32_t)SAI_L3_TUNNEL_EGRESS_INTERFACE_IPV4; // This is a IPv4 tunnel.
l3_tunnel_egress_interface_attrs[1].id = (sai_attr_id_t)SAI_L3_TUNNEL_EGRESS_INTERFACE_ATTR_SIP;
l3_tunnel_egress_interface_attrs[1].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
l3_tunnel_egress_interface_attrs[1].value.ipaddr.addr.ip4 = ntohl(sip.addr()); // The source IP address of the outer IP header.
l3_tunnel_egress_interface_attrs[2].id = (sai_attr_id_t)SAI_L3_TUNNEL_EGRESS_INTERFACE_ATTR_DIP;
l3_tunnel_egress_interface_attrs[2].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
l3_tunnel_egress_interface_attrs[2].value.ipaddr.addr.ip4 = ntohl(dip.addr()); // The destination IP address of the outer IP header.
if (egress_mode == SAI_SWITCH_ATTR_TUNNEL_EGRESS_MODE_NO_ENCAP_HEADER_LOOKUP)
{
    l3_tunnel_egress_interface_attrs[3].id = (sai_attr_id_t)SAI_L3_TUNNEL_EGRESS_INTERFACE_ATTR_INTF;
    l3_tunnel_egress_interface_attrs[3].value.oid = router_intf; // The router interface to bind.
    attrnum = 3;
}

if (sai_tunnel_api->create_l3_tunnel_egress_interface(&l3_tunnel_egress_interface_id, attrnum, l3_tunnel_egress_interface_attrs) == SAI_STATUS_SUCCESS)
{
    // Succeeded...
}
else
{
    // Failed...
}
~~~

## Remove A L3 Tunnel Egress Interface ##

The following code shows how to remove a l3 tunnel egress interface:

~~~cpp
if (sai_tunnel_api->remove_l3_tunnel_egress_interface(l3_tunnel_egress_interface_id) == SAI_STATUS_SUCCESS)
{
    // Succeeded...
}
else
{
    // Failed...
}
~~~

## Set L3 Tunnel Egress Interface Attributes ##

The following code shows how to set attributes to the tunnel interface:

~~~cpp
sai_attribute_t l3_tunnel_egress_interface_attr;
l3_tunnel_egress_interface_attr.id = (sai_attr_id_t)SAI_L3_TUNNEL_EGRESS_INTERFACE_ATTR_TTL;
l3_tunnel_egress_interface_attr.value.u8 = 128;

if (sai_tunnel_api->set_l3_tunnel_egress_interface_attribute(&tunnel_interface_id, &l3_tunnel_egress_interface_attr) == SAI_STATUS_SUCCESS)
{
    // Succeeded...
}
else
{
    // Failed...
}
~~~

## Get L3 Tunnel Egress Interface Attributes ##

The following code shows how to get attributes to the tunnel interface:

~~~cpp
sai_attribute_t l3_tunnel_egress_interface_attrs[2];
l3_tunnel_egress_interface_attrs[0].id = (sai_attr_id_t)SAI_L3_TUNNEL_EGRESS_INTERFACE_ATTR_TTL;
l3_tunnel_egress_interface_attrs[1].id = (sai_attr_id_t)SAI_L3_TUNNEL_EGRESS_INTERFACE_ATTR_DSCP;

if (sai_tunnel_api->get_l3_tunnel_egress_interface_attribute(&tunnel_interface_id, 2, l3_tunnel_egress_interface_attrs) == SAI_STATUS_SUCCESS)
{
    // Succeeded...
}
else
{
    // Failed...
}
~~~

## Set Up A Route To Forward A Prefix's Traffic To A L3 Tunnel ##

The following code shows how to set up a route to forward a prefix to a l3 tunnel:

~~~cpp
// Step 1: create a l3 tunnel egress interface.
sai_attribute_t attr;
attr.id = SAI_SWITCH_ATTR_TUNNEL_EGRESS_MODE;
sai_switch_api->get_switch_attribute(1, &attr);
int egress_mode = attr.value.s32;
int attrnum = 4;

sai_object_id_t l3_tunnel_egress_interface_id;
sai_attribute_t l3_tunnel_egress_interface_attrs[4];
l3_tunnel_egress_interface_attrs[0].id = (sai_attr_id_t)SAI_L3_TUNNEL_EGRESS_INTERFACE_ATTR_TYPE;
l3_tunnel_egress_interface_attrs[0].value.s32 = (sai_int32_t)SAI_L3_TUNNEL_EGRESS_INTERFACE_IPV4; // This is a IPv4 tunnel.
l3_tunnel_egress_interface_attrs[1].id = (sai_attr_id_t)SAI_L3_TUNNEL_EGRESS_INTERFACE_ATTR_SIP;
l3_tunnel_egress_interface_attrs[1].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4; // The source IP address of the outer IP header.
l3_tunnel_egress_interface_attrs[1].value.ipaddr.addr.ip4 = ntohl(sip.addr()); // The source IP address of the outer IP header.
l3_tunnel_egress_interface_attrs[2].id = (sai_attr_id_t)SAI_L3_TUNNEL_EGRESS_INTERFACE_ATTR_DIP;
l3_tunnel_egress_interface_attrs[2].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4; // The destination IP address of the outer IP header.
l3_tunnel_egress_interface_attrs[2].value.ipaddr = ntohl(dip.addr()); // The destination IP address of the outer IP header.

if (egress_mode == SAI_SWITCH_ATTR_TUNNEL_EGRESS_MODE_NO_ENCAP_HEADER_LOOKUP)
{
    l3_tunnel_egress_interface_attrs[0].id = (sai_attr_id_t)SAI_L3_TUNNEL_EGRESS_INTERFACE_ATTR_INTF;
    l3_tunnel_egress_interface_attrs[0].value.intf = router_intf; // The router interface to bind.
    attrnum = 3;
}

if (sai_tunnel_api->create_l3_tunnel_egress_interface(&l3_tunnel_egress_interface_id, attrnum, l3_tunnel_egress_interface_attrs) != SAI_STATUS_SUCCESS)
{
    // Failed...
    return false;
}

if (egress_mode == SAI_SWITCH_ATTR_TUNNEL_EGRESS_MODE_NO_ENCAP_HEADER_LOOKUP)
{
    // Step 2: create a next hop.
    sai_next_hop_id_t next_hop_id;
    sai_attribute_t next_hop_attrs[2];
    next_hop_attrs[0].id = SAI_NEXT_HOP_ATTR_IP;
    next_hop_attrs[0].value.ipaddr.addr_family= SAI_IP_ADDR_FAMILY_IPV4;
    next_hop_attrs[0].value.ipaddr.addr.ip4 = next_hop_ip.addr(); // The next hop IP address.
    next_hop_attrs[1].id = SAI_NEXT_HOP_ATTR_L3_EGRESS_INTERFACE_ID;
    next_hop_attrs[1].value.oid = l3_tunnel_egress_interface_id; // The l3 tunnel egress interface created in step 1.
    if (sai_next_hop_api->create_next_hop(&next_hop_id, 2, next_hop_attrs) != SAI_STATUS_SUCCESS)
    {
        // Failed...
        return false;
    }
}

// Step 3: create a route entry.
sai_unicast_route_entry_t unicast_route_entry;
unicast_route_entry.vr_id = virtual_router_id;
unicast_route_entry.destination.addr.ip4 = prefixIp; // The target prefix IP.
unicast_route_entry.destination.mask.ip4 = prefixMask; // The target prefix mask.

sai_attribute_t route_attr;
route_attr.id = SAI_ROUTE_ATTR_NEXT_HOP_ID;
if (egress_mode == SAI_SWITCH_ATTR_TUNNEL_EGRESS_MODE_NO_ENCAP_HEADER_LOOKUP)
{
    route_attr.value.oid = next_hop_id; // The next hop id created in step 2.
}
else
{
    route_attr.value.oid = l3_tunnel_egress_interface_id; // The l3 tunnel egress id created in step 1.
}

if (sai_route_api->create_route(&unicast_route_entry, 1, &route_attr) != SAI_STATUS_SUCCESS)
{
    // Failed...
    return false;
}

// Succeeded...
~~~


## Create A L3 Tunnel Ingress Interface ##

The following code shows how to create a l3 tunnel Ingress interface:

~~~cpp
sai_object_id_t l3_tunnel_ingress_interface_id;
sai_attribute_t l3_tunnel_ingress_interface_attrs[3];

l3_tunnel_ingress_interface_attrs[0].id = (sai_attr_id_t)SAI_L3_TUNNEL_INGRESS_INTERFACE_ATTR_TYPE;
l3_tunnel_ingress_interface_attrs[0].value.s32 = (sai_int32_t)SAI_L3_TUNNEL_INTERFACE_IPV4; // This is a IPv4 tunnel.
l3_tunnel_ingress_interface_attrs[1].id = (sai_attr_id_t)SAI_L3_TUNNEL_INGRESS_INTERFACE_ATTR_SIP_PREFIX;
l3_tunnel_ingress_interface_attrs[1].value.ipprefix.addr_family = SAI_IP_ADDR_FAMILY_IPV4; // The source IP address of the outer IP header.
l3_tunnel_ingress_interface_attrs[1].value.ipaddr.addr.ip4 = ntohl(sip.addr()); // The source IP address of the outer IP header.
l3_tunnel_ingress_interface_attrs[1].value.ipaddr.mask.ip4 = inet_aton("255.255.255.255"); // The mask for source IP address of the outer IP header.
l3_tunnel_ingress_interface_attrs[2].id = (sai_attr_id_t)SAI_L3_TUNNEL_INGRESS_INTERFACE_ATTR_DIP;
l3_tunnel_ingress_interface_attrs[2].value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4; // The destination IP address of the outer IP header.
l3_tunnel_ingress_interface_attrs[2].value.ipaddr.addr.ip4 = ntohl(dip.addr()); // The destination IP address of the outer IP header.
if (sai_tunnel_api->create_l3_tunnel_ingress_interface(&l3_tunnel_ingress_interface_id, 3, l3_tunnel_ingress_interface_attrs) == SAI_STATUS_SUCCESS)
{
    // Succeeded...
}
else
{
    // Failed...
}
~~~

## Remove A L3 Tunnel Ingress Interface ##

The following code shows how to remove a l3 tunnel Ingress interface:

~~~cpp
if (sai_tunnel_api->remove_l3_tunnel_ingress_interface(l3_tunnel_ingress_interface_id) == SAI_STATUS_SUCCESS)
{
    // Succeeded...
}
else
{
    // Failed...
}
~~~

## Set L3 Tunnel Ingress Interface Attributes ##

The following code shows how to set attributes to the tunnel interface:

~~~cpp
sai_attribute_t l3_tunnel_ingress_interface_attr;
l3_tunnel_ingress_interface_attr.id = (sai_attr_id_t)SAI_L3_TUNNEL_INGRESS_INTERFACE_ATTR_COPY_DSCP;
l3_tunnel_ingress_interface_attr.value.booldata = true;

if (sai_tunnel_api->set_l3_tunnel_ingress_interface_attribute(&tunnel_interface_id, &l3_tunnel_ingress_interface_attr) == SAI_STATUS_SUCCESS)
{
    // Succeeded...
}
else
{
    // Failed...
}
~~~

## Get L3 Tunnel Ingress Interface Attributes ##

The following code shows how to get attributes to the tunnel interface:

~~~cpp
sai_attribute_t l3_tunnel_ingress_interface_attr;

l3_tunnel_ingress_interface_attr.id = (sai_attr_id_t)SAI_L3_TUNNEL_INGRESS_INTERFACE_ATTR_COPY_DSCP;

if (sai_tunnel_api->get_l3_tunnel_ingress_interface_attribute(&tunnel_interface_id, &l3_tunnel_ingress_interface_attr) == SAI_STATUS_SUCCESS)
{
    // Succeeded...
}
else
{
    // Failed...
}
~~~
