
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

This proposal enables configuration of different MACSec secure policies, that define the behavior of MACSec protection on a given link when a MACSec Key Agreement (MKA) session is not established.

# Overview

Media Access Control Security (MACSec, IEEE 802.1AE) provides hop-by-hop security at Layer 2, ensuring data confidentiality, integrity, and origin authenticity on direct Ethernet links. The type of secure policy chosen dictates how the interface handles non-MACSec or unauthenticated traffic, balancing strict security requirements against operational resilience.

# Nomenclature

In deploying MACSec, organizations must decide how strictly security should be enforced on each port or link. The two standard operational modes are

- **Should Secure (Fail-Open):** Secure traffic is prioritized if MACSec Key Agreement(MKA) succeeds; but allow cleartext traffic if no secure channel is established

- **Must Secure (Fail-Closed):** Only frames successfully encapsulated and authenticated with the MACSec Security Association Key (SAK) are forwarded; drop all traffic if a secure channel cannot be established.

## Must Secure (Fail-Closed)

Must Secure is the most stringent secure policy.

- The policy ensures only Ethernet frames that are successfully encapsulated and authenticated with the MACSec Security Association Key (SAK) to be forwarded, thus ensuring that no unprotected data flows over the secured link.

- If any issues are encountered during MKA negotiation (scenarios such as mismatches in key or configuration), it results in an immediate and complete connectivity loss.

- If MKA session remains down, only EAPol(Extensible Authentication Protocol over LAN) packets are exchanged.

## Should Secure (Fail-Open)

Should Secure is a less stricter policy than Must Secure.

- This policy prioritizes service availability over link-layer confidentiality when the secure channel cannot be established.

- In case of MKA negotiation failure, the link reverts to an unsecured, clear-text state.

- The network continues to function, but the traffic on that specific link remains unencrypted.

# SAI Attribute Enhancement

The below MACSec port attribute is newly introduced to allow configuration of the MACSec secure policy. This attribute controls how the switch’s MACsec security engine enforces link protection. When set, the attribute instructs the hardware to apply the corresponding policy on the specified port.

```c
typedef enum _sai_macsec_port_attr_t
{
    ...
    /**
     * @brief Secure policy for MACSEC port
     *
     * Attribute to set the type of secure policy for a MACSEC port
     *
     * @type sai_macsec_port_secure_policy_t
     * @flags CREATE_AND_SET
     * @default SAI_MACSEC_PORT_SECURE_POLICY_SHOULD_SECURE
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
     * @brief Should Secure Policy: Traffic is exchanged in clear
     * till the encryption keys are in place.
     */
    SAI_MACSEC_PORT_SECURE_POLICY_SHOULD_SECURE,

    /**
     * @brief Must Secure Policy: Traffic will need to be dropped till
     * the encryption keys are in place.
     */
    SAI_MACSEC_PORT_SECURE_POLICY_MUST_SECURE,

} sai_macsec_port_secure_policy_t;
```

# API Workflow

- **Step 1** Create Switch.

- **Step 2** Create MACSec object.

- **Step 3** Set Secure Policy as part of Create MACSec Port.

```c
    sai_attribute_t                 attr;
    std::vector<sai_attribute_t>    attr_list;
    sai_object_id_t                 macsec_port_id;

    attr_list.clear();

    /* Populate other port attributes */

    /* Set Secure Policy */
    attr.id = SAI_MACSEC_PORT_ATTR_SECURE_POLICY;

    if (must_secure) {
        attr.value.u32 = SAI_MACSEC_PORT_SECURE_POLICY_MUST_SECURE;
    } else {
        attr.value.u32 = SAI_MACSEC_PORT_SECURE_POLICY_SHOULD_SECURE;
    }
    attr_list.push_back(attr);

    sai_create_macsec_port_fn(&macsec_port_id,
                              switch_id,
                              attr_list.size(),
                              attr_list.data());
```

# References

- IEEE 802.1AE (MACSec) Standard

- IEEE 802.1X-2010 (MKA)
