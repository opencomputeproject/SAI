# ECMP Count Capability and Configuration


This document talks about the 
- Capability query of MAX ECMP Count supported by silicon
- Configuration
- SAI adapter expectation

## Problem Description
There is a mechanism to set the HW default of max ecmp member count using a Key-Value string SAI_KEY_NUM_ECMP_MEMBERS during the system boot time. This KV string can be set only once and either is based on prior knowledge of HW capability or assumed default. SAI adapter consumes this Key-Value string and sets the ecmp member count in HW. Set value can be queried by a read only attribute SAI_SWITCH_ATTR_ECMP_MEMBERS.

## Spec Enhancement
Two new Switch attributes are introduced to address the abovementioned issue.

> SAI_SWITCH_ATTR_MAX_ECMP_MEMBER_COUNT (Read Only)
> This is a capability query during switch init to find out device specific max number of ecmp members supported.

> SAI_SWITCH_ATTR_ECMP_MEMBER_COUNT (read/write)
> This attribute is set based on the query for MAX_ECMP_MEMBER_COUNT and can be changed dynamically.

## SAI Adapter Requirements
Typical Workflow:

Switch object create
Switch get SAI_SWITCH_ATTR_MAX_ECMP_MEMBER_COUNT
Switch set SAI_SWITCH_ATTR_ECMP_MEMBER_COUNT (either before configuring ecmp groups or during system runtime)

Following are the possible scenarios for SAI adapter to handle
1. HW does not support the attribute SAI_SWITCH_ATTR_ECMP_MEMBER_COUNT: SAI adapter must return error SAI_STATUS_NOT_SUPPORTED.
2. HW supports dynamic modification of ecmp count only and only if there are no ecmp groups configured
    a. If there are no ECMP groups configured: SAI adapter must handle the update and return SAI_STATUS_SUCCESS
    b. If there are ECMP groups configured: SAI adapter must return SAI_STATUS_INSUFFICIENT_RESOURCES
3. HW supports dynamic modification of ecmp count irrespestive of ecmp group configuration)
    a. If ecmp count is increased: SAI adapter must handle the update and return SAI_STATUS_SUCCESS
    b. If ecmp count is decreased: SAI adapter can not handle the update as it doesn't know which members to purge. In this case SAI adaper must return SAI_STATUS_NOT_IMPLEMENTED

## ECMP Type, Member Count and Next Hop Group Member Count
ECMP member count is a global configuration and controls the allocation of HW memory for carving out chunks of ECMP group with member count. This is used for all ECMP types. If there is a need for differentiating a specific ECMP type for max and configured scale as a separate HW resource, we should add a ecmp type specific attribute later on.

HW may support variable size member count configuration per nexthop group using the attribute SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_COUNT. In this case SAI adapter must return attribute as not supported SAI_STATUS_NOT_SUPPORTED.

