Switch Abstraction Interface Change Proposal

==============================

Title       | Critical Resource Monitor
------------|----------------
Authors     | Microsoft
Status      | In review
Type        | Standards track
Created     | 05/17/2017
SAI-Version | 1.2
----------

## Overview


SAI manages ASIC resources. It is important for the user to query the current resources usage in the ASIC for different types of SAI objects.

When every SAI objects use its own resource pool, it is easy to know the maximum number of objects which can be stored in the ASIC and its current usage. However, different SAI objects may share a same resource pool. For example, the IPv4 and IPv6 routes can share a same resource pool. In this case, the maximum number of IPv4 routes is not very helpful when you have inserted both IPv4 and IPv6 entries into the ASIC, and you want to know how many more IPv4 you can add. This is because the number of IPv4 you can add in maximum depends on the number of IPv6 routes you have already added.

Consider ACL entries as an example, SAI allows user to create different types of ACL table. For each ACL table may have its own maximum capacity of ACL entries. Therefore, user may want to query the ACL entry resource usage per ACL table. However, since different ACL tables may use same resource pool to store the ACL entries, the maximum ACL entries allowed by an ACL table can also change depending on the usage of other ACL tables.

This proposal introduces AVAILABLE\_ENTRY attribute for certain objects to allow users to query the available entries for those objects and this attribute defines the free entries the user can add based on current resource usage.

## Spec

Please refer to the header file changes.
