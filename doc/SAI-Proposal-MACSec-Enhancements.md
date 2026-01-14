
-------------------------------------------------------------------------------
 Title       | MACSec Secure Policy
-------------|-----------------------------------------------------------------
 Authors     | Ruthrapathy Shanmuganandam (Cisco Systems Inc.)
 Status      | In review
 Type        | Standards track
 Created     | 2025-09-30
 SAI-Version | 1.18
-------------------------------------------------------------------------------

# Introduction

This proposal introduces the following enhancements for MACsec configuration:

- **MACSec secure policy** Configuration of policies that define the behavior of MACSec protection on a given link when a MACSec Key Agreement (MKA) session is not established.

- **Confidentiality Offset** Configuration of Confidentiality Offset to allow bytes of the ethernet frame to be unencrypted.

- **Tag Control Information** Configuration to set End Station (ES) and Single Copy Broadcast (SCB) bits in Tag Control Information (TCI).

# MACSec Secure Policies

## Overview

Media Access Control Security (MACSec, IEEE 802.1AE) provides hop-by-hop security at Layer 2, ensuring data confidentiality, integrity, and origin authenticity on direct Ethernet links. The type of secure policy chosen dictates how the interface handles non-MACSec or unauthenticated traffic, balancing strict security requirements against operational resilience.

## Nomenclature

In deploying MACSec, organizations must decide how strictly security should be enforced on each port or link. The two standard operational modes are

- **Should Secure (Fail-Open):** Secure traffic is prioritized if MACSec Key Agreement(MKA) succeeds; but allow cleartext traffic if no secure channel is established

- **Must Secure (Fail-Closed):** Only frames successfully encapsulated and authenticated with the MACSec Security Association Key (SAK) are forwarded; drop all traffic if a secure channel cannot be established.

### Must Secure (Fail-Closed)

Must Secure is the most stringent secure policy.

- The policy ensures only Ethernet frames that are successfully encapsulated and authenticated with the MACSec Security Association Key (SAK) to be forwarded, thus ensuring that no unprotected data flows over the secured link.

- If any issues are encountered during MKA negotiation (scenarios such as mismatches in key or configuration), it results in an immediate and complete connectivity loss.

- If MKA session remains down, only EAPoL (Extensible Authentication Protocol over LAN) packets are exchanged bidirectionally to attempt session establishment. All other traffic is dropped.

- If the peer does not support MACSec at all (no MKA capability), all traffic is dropped to maintain security.

### Should Secure (Fail-Open)

Should Secure is a less strict policy than Must Secure.

- This policy prioritizes service availability over link-layer confidentiality when the secure channel cannot be established.

- In case of MKA negotiation failure, the link reverts to an unsecured, clear-text state.

- The network continues to function, but the traffic on that specific link remains unencrypted.

- If the peer does not support MACSec, traffic passes unencrypted to maintain connectivity over availability.

## SAI Attribute Enhancement

The below MACSec port attribute is newly introduced to allow configuration of the MACSec secure policy. This attribute controls how the switch’s MACsec security engine enforces link protection. When set, the attribute instructs the hardware to apply the corresponding policy on the specified port.

```c
typedef enum _sai_macsec_port_attr_t
{
    ...
    /**
     * @brief Secure policy configuration for MACSec port
     *
     * Attribute to set the type of secure policy for a MACSec port
     *
     * @type sai_macsec_port_secure_policy_t
     * @flags CREATE_AND_SET
     * @default SAI_MACSEC_PORT_SECURE_POLICY_MUST_SECURE
     */
    SAI_MACSEC_PORT_ATTR_SECURE_POLICY,
    ...
} sai_macsec_port_attr_t;
```

The Secure Policy is defined to take values of the below enumeration.

```c
/**
 * @brief Attribute Data for MACSec Secure Policy
 */
typedef enum _sai_macsec_port_secure_policy_t
{
    /**
     * @brief Must Secure Policy: Traffic will need to be dropped till
     * the encryption keys are in place.
     */
    SAI_MACSEC_PORT_SECURE_POLICY_MUST_SECURE,

    /**
     * @brief Should Secure Policy: Traffic is exchanged in clear
     * till the encryption keys are in place.
     */
    SAI_MACSEC_PORT_SECURE_POLICY_SHOULD_SECURE,

} sai_macsec_port_secure_policy_t;
```

## API Workflow

- **Step 1** Create Switch.

- **Step 2** Create MACSec object.

- **Step 3** Set Secure Policy as part of Create MACSec Port.

```c
    sai_attribute_t                 attr;
    std::vector<sai_attribute_t>    attr_list;
    sai_object_id_t                 macsec_port_id;

    attr_list.clear();

    /* Populate other port attributes */

    /* If should secure, set secure policy */
    if (should_secure) {
        attr.id = SAI_MACSEC_PORT_ATTR_SECURE_POLICY;
        attr.value.u32 = SAI_MACSEC_PORT_SECURE_POLICY_SHOULD_SECURE;
    }
    attr_list.push_back(attr);

    sai_create_macsec_port_fn(&macsec_port_id,
                              switch_id,
                              attr_list.size(),
                              attr_list.data());
```

# MACSec Confidentiality Offset

## Overview

The IEEE 802.1AE (MACSec) standard allows for the configuration of a Confidentiality Offset (CO), which defines the number of bytes of the packet payload, following the MACSec Security Tag (SecTag), should remain unencrypted (sent in the clear). The rest of the payload is encrypted. Even though these bytes in a MACSec protected Ethernet frame  are not encrypted,  they are still integrity-protected (authenticated).

Confidentiality offset can take values in the range of 0 - 63; Typical values includes:

- 0 bytes: Full confidentiality; entire payload remains encrypted.

- 30 bytes: Partial confidentiality; The first 30 bytes after SecTAG remain unencrypted.

- 50 bytes: Partial confidentiality; The first 50 bytes after SecTAG remain unencrypted.

The need for a configurable confidentiality offset primarily stems from requirements to leave certain protocol headers unencrypted since intermediate devices may require visibility into the packet in time-sensitive networks.

## SAI Attribute Enhancement

The below MACSec Secure Channel (SC) attribute is newly introduced to allow configuration of the confidentiality offset. This attribute controls the number of bytes of the payload to be left unencrypted.

```c
    /**
     * @brief Confidentiality Offset for this Secure Channel
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_MACSEC_SC_ATTR_CONFIDENTIALITY_OFFSET,
```

## API Workflow

During creation of Secure Channel, confidentiality offset can be configured as below:

```c
    sai_attribute_t                 attr;
    std::vector<sai_attribute_t>    attr_list;
    sai_object_id_t                 macsec_sc_id;

    attr_list.clear();

    /* Populate other SC attributes */

    /* Set conf offset */
    attr.id = SAI_MACSEC_SC_ATTR_CONFIDENTIALITY_OFFSET;
    attr.value.u32 = conf_offset;
    attr_list.push_back(attr);

    sai_create_macsec_sc_fn(&macsec_sc_id,
                            switch_id,
                            attr_list.size(),
                            attr_list.data());
```

# Tag Control Information: ES and SCB

## Overview

The MACSec Security Tag (SecTag) contains the Tag Control Information (TCI) field, which holds critical flags defined by IEEE 802.1AE. Two specific flags, the End Station (ES) bit and the Single Copy Broadcast (SCB) bit, are essential for identifying the nature of the Secure Channel (SC) and its use case.

## Tag Control Information

The Tag Control Information (TCI) is an 8-bit field within the MACSec Security Tag (SecTag) that contains control information for the MACSec frame. The TCI field structure is defined as follows:

```
TCI Octet Structure (8 bits):
┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
│  V  │ ES  │ SC  │ SCB │  E  │  C  │   AN      │
│  =0 │     │     │     │     │     │           │
├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
│  8  │  7  │  6  │  5  │  4  │  3  │  2  │  1  │
└─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
                    Bit Position
```

**Field Descriptions:**

- **V (Version, Bit 8)**: Always set to 0 for current MACSec version
- **ES (End Station, Bit 7)**: Indicates if the transmitting device is an end station
- **SC (Secure Channel, Bit 6)**: Indicates presence of Secure Channel Identifier
- **SCB (Single Copy Broadcast, Bit 5)**: Controls broadcast frame handling
- **E (Encryption, Bit 4)**: Indicates if the payload is encrypted
- **C (Changed Text, Bit 3)**: Indicates if the frame length has changed
- **AN (Association Number, Bits 2-1)**: Identifies the Security Association used for frame protection

Currently SAI_MACSEC_SC_ATTR_MACSEC_EXPLICIT_SCI_ENABLE attribute is used to configure the Secure Channel(SC) bit in the TCI field. This proposal introduces attributes to configure ES and SCB bits.

### End Station (ES)

The ES bit helps the receiving MACsec entity understand the role of the sender in the network topology. Setting this bit allows receivers to identify traffic originating directly from an endpoint, distinguishing it from traffic that has passed through intermediate switches or other devices.

### Single Copy Broadcast (SCB)

The SCB is used to indicate if the ethernet frame belongs to a broadcast/multicast domain (hence its not re-encrypted by intermediate relays) vs a fully protected unicast domain (hence decrypted and re-encrypted hop-by-hop). When the bit is set, it indicates that the frame is associated with an SC that supports the Ethernet Passive Optical Network (EPON) Single Copy Broadcast capability, which is typically point-to-multipoint in nature.

## SAI Attribute Enhancement

The below MACSec Secure Channel (SC) attributes are newly introduced to allow configuration of the ES and SCB bits in the TCI. Both the attributes are configurable only when creating a Secure Channel in the Transmit (Egress) direction.

```c
    /**
     * @brief End Station bit in the TCI field of SecTAG
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     * @validonly SAI_MACSEC_SC_ATTR_MACSEC_DIRECTION == SAI_MACSEC_DIRECTION_EGRESS
     */
    SAI_MACSEC_SC_ATTR_USE_ES,

    /**
     * @brief Single Copy Broadcast bit in the TCI field of SecTAG
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     * @validonly SAI_MACSEC_SC_ATTR_MACSEC_DIRECTION == SAI_MACSEC_DIRECTION_EGRESS
     */
    SAI_MACSEC_SC_ATTR_USE_SCB,
```

## API Workflow

During creation of Secure Channel, the TCI bits for End Station (ES) and Single Copy Broadcast (SCB) can be configured as below:

```c
    sai_attribute_t                 attr;
    std::vector<sai_attribute_t>    attr_list;
    sai_object_id_t                 macsec_sc_id;

    attr_list.clear();

    attr.id = SAI_MACSEC_SC_ATTR_MACSEC_DIRECTION;
    attr.value.s32 = SAI_MACSEC_DIRECTION_EGRESS;
    attr_list.push_back(attr);

    /* Populate other SC attributes */

    /* Populate TCI bits */
    if (use_es) {
        attr.id = SAI_MACSEC_SC_ATTR_USE_ES;
        attr.value.booldata = true;
        attr_list.push_back(attr);
    }

    if (use_scb) {
        attr.id = SAI_MACSEC_SC_ATTR_USE_SCB;
        attr.value.booldata = true;
        attr_list.push_back(attr);
    }

    sai_create_macsec_sc_fn(&macsec_port_id,
                            switch_id,
                            attr_list.size(),
                            attr_list.data());
```
# References

- IEEE 802.1AE (MACSec) Standard

- IEEE 802.1X-2010 (MKA)
