Switch Abstraction Interface Change Proposal

==============================

Title       | Host Interface MACsec Offload
------------|------------------------------
Authors     | Microsoft
Status      | In review
Type        | Standards track
Created     | 06/29/2020
SAI-Version | 
----------

## Overview

Host interfaces of MACsec offload are designed for synchronization parameters of MACsec to [MACsec offload driver](https://lore.kernel.org/patchwork/patch/1034965/).

In the real switch with MACsec ASIC, the switch will leverage the MACsec offload driver and SAI MACsec API to hand over the MACsec functionality of linux MACsec driver to MACsec ASIC. But the MACsec offload driver cannot directly get the next packet numbers(PNs) of Secure Associations(SAs) in MACsec ASIC. So the switch can use this host interface of MACsec offload to synchronize the next PNs for each SAs between the MACsec offload driver with MACsec ASIC.

## Spec

Please refer to the header file changes.