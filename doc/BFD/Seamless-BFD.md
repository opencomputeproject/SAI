# Seamless BFD (S-BFD)

Title       | Seamless BFD (S-BFD)
------------|----------------
Authors     | Jason Bos
Status      | In review
Type        | Standards track
Created     | 10/10/2025
SAI-Version | 1.16

Seamless Bidirectional Forwarding Detection (S-BFD) provides a simplified mechanism for continuity testing and validation of forwarding paths. Unlike traditional BFD, S-BFD does not require session establishment or negotiation - instead, it uses a reflector model where one side (the initiator) can immediately begin sending BFD packets to the other side (the reflector) without coordinated session state.

S-BFD is useful for:
- Rapid path validation without session overhead
- Monitoring with minimal state on reflectors
- Validation of return paths in asymmetric routing scenarios
- Scaling to large numbers of monitored paths

SAI supports S-BFD through two session types:

```
typedef enum _sai_bfd_session_type_t
{
    ...
    /** S-BFD Reflector */
    SAI_BFD_SESSION_TYPE_REFLECTOR,

    /** S-BFD Initiator */
    SAI_BFD_SESSION_TYPE_INITIATOR,
    ...
} sai_bfd_session_type_t;
```

### S-BFD Reflector

The S-BFD reflector operates in a stateless manner. When configured, the reflector:
- Listens for incoming S-BFD control packets
- Reflects packets back to the initiator
- Does not maintain per-session state
- Uses the local discriminator to identify the reflector endpoint

Key attributes for a reflector session:
- `SAI_BFD_SESSION_ATTR_TYPE`: Set to `SAI_BFD_SESSION_TYPE_REFLECTOR`
- `SAI_BFD_SESSION_ATTR_LOCAL_DISCRIMINATOR`: Identifies the reflector (must be well-known)
- `SAI_BFD_SESSION_ATTR_SRC_IP_ADDRESS`: Source IP for reflected packets

### S-BFD Initiator

The S-BFD initiator actively monitors paths by sending S-BFD control packets. The initiator:
- Sends S-BFD packets with the reflector's discriminator as the destination
- Monitors returned packets to verify path continuity
- Maintains session state and detects failures based on timeout
- Can dynamically adjust monitoring parameters

## Usage Example: S-BFD Reflector

The following example demonstrates how to configure an S-BFD reflector on a network device. The reflector uses a well-known discriminator (4) that initiators will use to send S-BFD packets.

```c
/*****************************************************
 * Create an S-BFD Reflector Session
 * 
 * This reflector will:
 * - Listen on discriminator 4
 * - Reflect S-BFD packets back to any initiator
 * - Operate statelessly
 *****************************************************/

sai_object_id_t sbfd_reflector_session;
std::vector<sai_attribute_t> attrs;
sai_attribute_t attr;

// Set session type to reflector
attr.id = SAI_BFD_SESSION_ATTR_TYPE;
attr.value.s32 = SAI_BFD_SESSION_TYPE_REFLECTOR;
attrs.push_back(attr);

// Set local discriminator (well-known value for this reflector)
attr.id = SAI_BFD_SESSION_ATTR_LOCAL_DISCRIMINATOR;
attr.value.u32 = 4;
attrs.push_back(attr);

// Set hardware lookup valid to true for packet processing
attr.id = SAI_BFD_SESSION_ATTR_HW_LOOKUP_VALID;
attr.value.booldata = true;
attrs.push_back(attr);

// Set virtual router for routing lookup
attr.id = SAI_BFD_SESSION_ATTR_VIRTUAL_ROUTER;
attr.value.oid = vrf_id;  // Virtual router object
attrs.push_back(attr);

// Set source IP address (reflector's IP)
attr.id = SAI_BFD_SESSION_ATTR_SRC_IP_ADDRESS;
attr.value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
inet_pton(AF_INET, "192.0.2.1", &attr.value.ipaddr.addr.ip4);
attrs.push_back(attr);

// Set IP header version
attr.id = SAI_BFD_SESSION_ATTR_IPHDR_VERSION;
attr.value.u8 = 4;
attrs.push_back(attr);

// Set encapsulation type (no encapsulation for basic S-BFD)
attr.id = SAI_BFD_SESSION_ATTR_BFD_ENCAPSULATION_TYPE;
attr.value.s32 = SAI_BFD_ENCAPSULATION_TYPE_NONE;
attrs.push_back(attr);

// Optional: Set TTL for reflected packets
attr.id = SAI_BFD_SESSION_ATTR_TTL;
attr.value.u8 = 255;
attrs.push_back(attr);

// Optional: Set TOS/DSCP for reflected packets
attr.id = SAI_BFD_SESSION_ATTR_TOS;
attr.value.u8 = 0xC0;  // CS6 for network control
attrs.push_back(attr);

// Create the reflector session
sai_bfd_api->create_bfd_session(
    &sbfd_reflector_session,
    g_switch_id,
    attrs.size(),
    attrs.data());

```

## Usage Example: S-BFD Initiator

The following example demonstrates how to configure an S-BFD initiator that monitors path connectivity to the reflector configured in the previous example.

```c
/*****************************************************
 * Create an S-BFD Initiator Session
 * 
 * This initiator will:
 * - Send S-BFD packets to reflector at 192.0.2.1
 * - Use remote discriminator 4 (reflector's discriminator)
 * - Monitor path with 50ms intervals
 * - Detect failure after 3 missed packets (150ms)
 *****************************************************/

sai_object_id_t sbfd_initiator_session;
std::vector<sai_attribute_t> attrs;
sai_attribute_t attr;

// Set session type to initiator
attr.id = SAI_BFD_SESSION_ATTR_TYPE;
attr.value.s32 = SAI_BFD_SESSION_TYPE_INITIATOR;
attrs.push_back(attr);

// Set local discriminator (unique on this initiator)
attr.id = SAI_BFD_SESSION_ATTR_LOCAL_DISCRIMINATOR;
attr.value.u32 = 99;
attrs.push_back(attr);

// Set remote discriminator (reflector's well-known discriminator)
attr.id = SAI_BFD_SESSION_ATTR_REMOTE_DISCRIMINATOR;
attr.value.u32 = 4;
attrs.push_back(attr);

// Set hardware lookup valid to true
attr.id = SAI_BFD_SESSION_ATTR_HW_LOOKUP_VALID;
attr.value.booldata = true;
attrs.push_back(attr);

// Set virtual router for routing lookup
attr.id = SAI_BFD_SESSION_ATTR_VIRTUAL_ROUTER;
attr.value.oid = vrf_id;  // Virtual router object
attrs.push_back(attr);

// Set source IP address (initiator's IP)
attr.id = SAI_BFD_SESSION_ATTR_SRC_IP_ADDRESS;
attr.value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
inet_pton(AF_INET, "198.51.100.1", &attr.value.ipaddr.addr.ip4);
attrs.push_back(attr);

// Set destination IP address (reflector's IP)
attr.id = SAI_BFD_SESSION_ATTR_DST_IP_ADDRESS;
attr.value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
inet_pton(AF_INET, "192.0.2.1", &attr.value.ipaddr.addr.ip4);
attrs.push_back(attr);

// Set UDP source port (dynamic port for this session)
attr.id = SAI_BFD_SESSION_ATTR_UDP_SRC_PORT;
attr.value.u32 = 49152;
attrs.push_back(attr);

// Set minimum transmit interval (50000 microseconds = 50 ms)
attr.id = SAI_BFD_SESSION_ATTR_MIN_TX;
attr.value.u32 = 50000;
attrs.push_back(attr);

// Set minimum receive interval (50000 microseconds = 50 ms)
attr.id = SAI_BFD_SESSION_ATTR_MIN_RX;
attr.value.u32 = 50000;
attrs.push_back(attr);

// Set detection multiplier (3 missed packets = failure)
attr.id = SAI_BFD_SESSION_ATTR_MULTIPLIER;
attr.value.u8 = 3;
attrs.push_back(attr);

// Set IP header version
attr.id = SAI_BFD_SESSION_ATTR_IPHDR_VERSION;
attr.value.u8 = 4;
attrs.push_back(attr);

// Set encapsulation type
attr.id = SAI_BFD_SESSION_ATTR_BFD_ENCAPSULATION_TYPE;
attr.value.s32 = SAI_BFD_ENCAPSULATION_TYPE_NONE;
attrs.push_back(attr);

// Optional: Set TTL
attr.id = SAI_BFD_SESSION_ATTR_TTL;
attr.value.u8 = 255;
attrs.push_back(attr);

// Optional: Set TOS/DSCP
attr.id = SAI_BFD_SESSION_ATTR_TOS;
attr.value.u8 = 0xC0;  // CS6 for network control
attrs.push_back(attr);

// Optional: Enable multi-hop for non-adjacent reflector
attr.id = SAI_BFD_SESSION_ATTR_MULTIHOP;
attr.value.booldata = true;
attrs.push_back(attr);

// Create the initiator session
sai_bfd_api->create_bfd_session(
    &sbfd_initiator_session,
    g_switch_id,
    attrs.size(),
    attrs.data());
```

## Usage Example: S-BFD Initiator with SRv6 Encapsulation

The following example demonstrates how to configure an S-BFD initiator with SRv6 encapsulation for monitoring paths in an SRv6 network.

```c
/*****************************************************
 * Create an S-BFD Initiator Session with SRv6 Encapsulation
 * 
 * This initiator will:
 * - Send S-BFD packets encapsulated in SRv6
 * - Use SRv6 segment list for path steering
 * - Monitor path with 50ms intervals
 * - Detect failure after 3 missed packets (150ms)
 *****************************************************/

sai_object_id_t sbfd_srv6_initiator_session;
std::vector<sai_attribute_t> attrs;
sai_attribute_t attr;

// Set session type to initiator
attr.id = SAI_BFD_SESSION_ATTR_TYPE;
attr.value.s32 = SAI_BFD_SESSION_TYPE_INITIATOR;
attrs.push_back(attr);

// Set local discriminator (unique on this initiator)
attr.id = SAI_BFD_SESSION_ATTR_LOCAL_DISCRIMINATOR;
attr.value.u32 = 99;
attrs.push_back(attr);

// Set remote discriminator (reflector's well-known discriminator)
attr.id = SAI_BFD_SESSION_ATTR_REMOTE_DISCRIMINATOR;
attr.value.u32 = 4;
attrs.push_back(attr);

// Set hardware lookup valid to true
attr.id = SAI_BFD_SESSION_ATTR_HW_LOOKUP_VALID;
attr.value.booldata = true;
attrs.push_back(attr);

// Set virtual router for routing lookup
attr.id = SAI_BFD_SESSION_ATTR_VIRTUAL_ROUTER;
attr.value.oid = vrf_id;  // Virtual router object
attrs.push_back(attr);

// Set source IP address for inner BFD packet (initiator's IP)
attr.id = SAI_BFD_SESSION_ATTR_SRC_IP_ADDRESS;
attr.value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV6;
inet_pton(AF_INET6, "2001:db8:1::1", &attr.value.ipaddr.addr.ip6);
attrs.push_back(attr);

// Set destination IP address for inner BFD packet (reflector's IP)
attr.id = SAI_BFD_SESSION_ATTR_DST_IP_ADDRESS;
attr.value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV6;
inet_pton(AF_INET6, "2001:db8:2::1", &attr.value.ipaddr.addr.ip6);
attrs.push_back(attr);

// Set UDP source port (dynamic port for this session)
attr.id = SAI_BFD_SESSION_ATTR_UDP_SRC_PORT;
attr.value.u32 = 49153;
attrs.push_back(attr);

// Set minimum transmit interval (50000 microseconds = 50 ms)
attr.id = SAI_BFD_SESSION_ATTR_MIN_TX;
attr.value.u32 = 50000;
attrs.push_back(attr);

// Set minimum receive interval (50000 microseconds = 50 ms)
attr.id = SAI_BFD_SESSION_ATTR_MIN_RX;
attr.value.u32 = 50000;
attrs.push_back(attr);

// Set detection multiplier (3 missed packets = failure)
attr.id = SAI_BFD_SESSION_ATTR_MULTIPLIER;
attr.value.u8 = 3;
attrs.push_back(attr);

// Set IP header version for inner packet
attr.id = SAI_BFD_SESSION_ATTR_IPHDR_VERSION;
attr.value.u8 = 6;
attrs.push_back(attr);

// Set encapsulation type to SRv6
attr.id = SAI_BFD_SESSION_ATTR_BFD_ENCAPSULATION_TYPE;
attr.value.s32 = SAI_BFD_ENCAPSULATION_TYPE_SRV6;
attrs.push_back(attr);

// Set tunnel source IP (outer IPv6 header source)
attr.id = SAI_BFD_SESSION_ATTR_TUNNEL_SRC_IP_ADDRESS;
attr.value.ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV6;
inet_pton(AF_INET6, "2001:db8:100::1", &attr.value.ipaddr.addr.ip6);
attrs.push_back(attr);

// Set SRv6 SID list for segment routing
// Assumes srv6_sidlist_id was created previously with desired segment list
attr.id = SAI_BFD_SESSION_ATTR_SRV6_SIDLIST_ID;
attr.value.oid = srv6_sidlist_id;  // SRv6 SID list object
attrs.push_back(attr);

// Create the SRv6-encapsulated initiator session
sai_bfd_api->create_bfd_session(
    &sbfd_srv6_initiator_session,
    g_switch_id,
    attrs.size(),
    attrs.data());