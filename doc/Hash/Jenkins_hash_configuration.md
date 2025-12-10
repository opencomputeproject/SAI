# [SAI] Jenkins Hash Algorithm

--------------------------------------------------------------------------------

Title       | Jenkins Hash Algorithm
----------- | ---------------------------------------
Authors     | Mike Beresford, Abhishek Verma (Google)
Status      | In-Review
Type        | Standards track
Created     | 2026-02-05
SAI-Version | 1.18

--------------------------------------------------------------------------------

## 1. Introduction

ECMP or Next hop groups (NHG) and LAG use load balancing techniques to spread
the traffic across the various members. The most common load balancing technique
is to use hash-based member selection. In this technique, flows are identified
by performing a hash on a set of packet fields like the 5-tuple, and then
selecting the group member based on the computed hash value.

This proposal introduces the addition of Jenkins hash algorithm to
_sai_hash_algorithm_t.

## 2. Motivation

Limitation of CRC/XOR: Traditional CRC and XOR algorithms are susceptible to
polarization when traffic passes through multiple next hops.

Jenkins hash function is a non-cryptographic hash function that offers better
avalanche properties compared to CRC and XOR. This results in a more uniform
distribution of flows across members, especially in multi-stage networks (e.g.,
CLOS topologies) where polarization is a concern.

## 3. Behavior

The Jenkins algorithm (specifically `lookup3`) produces a 32-bit hash value for
a given input (e.g., packet 5-tuple). Switching ASICs generally rely on a 16-bit
hash value for load balancing. To accommodate this, the 32-bit Jenkins hash is
split into two 16-bit segments.

Two new enums are introduced to allow selecting either the lower 16 bits
(`_LO`) or the upper 16 bits (`_HI`) of the computed hash. This provides
flexibility, for example allowing different hash values for ECMP and LAG even
if the same flow keys are used.

## 4. SAI Enhancement

New enums added to the `sai_hash_algorithm_t`.

Enum defining the Jenkins hash algorithm:

```c
/**
 * @brief Attribute data for #SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_ALGORITHM
 * and #SAI_SWITCH_ATTR_LAG_DEFAULT_HASH_ALGORITHM
 */
typedef enum _sai_hash_algorithm_t
{
    ...

    /** Round-robin based hash algorithm (per-packet round-robin spraying) */
    SAI_HASH_ALGORITHM_ROUND_ROBIN = 8,

    /** Lower 16 bits of Jenkins hash algorithm */
    SAI_HASH_ALGORITHM_JENKINS_LO = 9,

    /** Upper 16 bits of Jenkins hash algorithm */
    SAI_HASH_ALGORITHM_JENKINS_HI = 10,

} sai_hash_algorithm_t;

```

## 5. API Example(s)

### Query supported hashing algorithms

```c
    sai_s32_list_t enum_values_capability;
    enum_values_capability.count = 0;
    enum_values_capability.list = nullptr;

   // Fetch the number of supported hash algorithms, then allocate a buffer and 
   // fetch the actual list.
    sai_status_t status = sai->queryAttributeEnumValuesCapability(
        switchid,
        SAI_OBJECT_TYPE_SWITCH,
        SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_ALGORITHM,
        &enum_values_capability);

    std::vector<int32_t> list;
    list.resize(enum_values_capability.count);
    enum_values_capability.list = list.data();
    sai_status_t status = sai->queryAttributeEnumValuesCapability(
        switchid,
        SAI_OBJECT_TYPE_SWITCH,
        SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_ALGORITHM,
        &enum_values_capability);

    ASSERT_EQ(status, SAI_STATUS_SUCCESS);

    bool jenkins_hash_lo_supported = false;
    for (uint32_t i = 0; i < enum_values_capability.count; ++i)
    {
        if (enum_values_capability.list[i] == SAI_HASH_ALGORITHM_JENKINS_LO)
        {
            jenkins_hash_lo_supported = true;
        }
    }
```

### Set Jenkins algorithm as the default hash algorithm

```c
    sai_attribute_t attr;
    attr.id = SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_ALGORITHM;
    attr.value.s32 = SAI_HASH_ALGORITHM_JENKINS_LO;
    status = sai->set(SAI_OBJECT_TYPE_SWITCH, switchid, &attr);
    ASSERT_EQ(status, SAI_STATUS_SUCCESS);
```

## 6. References

https://burtleburtle.net/bob/c/lookup3.c

