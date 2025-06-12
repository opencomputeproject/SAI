# SAI 1.16.1 Release Notes

The Switch Abstraction Interface(SAI) defines the APIs to provide a vendor-independent way of controlling forwarding elements, such as a switching ASIC, an NPU or a software switch in a uniform manner. This release document covers the SAI API changes from SAI tag 1.15.1 to SAI tag 1.16.1. The previous release notes corresponding to SAI tag 1.15.1 is available at [SAI 1.15.1 release notes](https://github.com/opencomputeproject/SAI/blob/master/doc/SAI_1.15.1_ReleaseNotes.md) 

This document explains the new SAI features as well as the enhancements and the bug fixes on existing features. 


### List of enhancements added in this release: 

[Add word bounceback to aspell ](https://github.com/opencomputeproject/SAI/pull/2090 ) <br>
[Updating saimacsec.h to add macsec_port_id to flow obj ](https://github.com/opencomputeproject/SAI/pull/2003) <br>
[Update dash flow and dash tunnel ](https://github.com/opencomputeproject/SAI/pull/2093) <br>
[Fix missing attr value for object type enum ](https://github.com/opencomputeproject/SAI/pull/2095) <br>
[Mark appliance table local region ID attribute as create-only ](https://github.com/opencomputeproject/SAI/pull/2094) <br>
[Fix for RPC build failure ](https://github.com/opencomputeproject/SAI/pull/2097) <br>
[Next hop group with members.  ](https://github.com/opencomputeproject/SAI/pull/2013) <br>
[Prefix Compression feature addition to SAI ](https://github.com/opencomputeproject/SAI/pull/2045) <br>
[Add label attribute for Policer, RIF, Scheduler and UDF ](https://github.com/opencomputeproject/SAI/pull/2046) <br>
[Add uSID endpoint enums for udx and udt ](https://github.com/opencomputeproject/SAI/pull/2102) <br>
[Add api version on each attribute  ](https://github.com/opencomputeproject/SAI/pull/2100) <br>
[Rename values in saiattrversion.h  ](https://github.com/opencomputeproject/SAI/pull/2109) <br>
[Fix iscustom attribute field ](https://github.com/opencomputeproject/SAI/pull/2110) <br>
[Change the type of action parameter in DASH Flow API  ](https://github.com/opencomputeproject/SAI/pull/2101) <br>
[Make sai switch pointers attr list stable ](https://github.com/opencomputeproject/SAI/pull/2111) <br>
[Add custom range base enums for api and object_type  ](https://github.com/opencomputeproject/SAI/pull/2112) <br>
[Update history files  ](https://github.com/opencomputeproject/SAI/pull/2113) <br>
[Check global functions return type ](https://github.com/opencomputeproject/SAI/pull/2115) <br>
[Introduce new extended port oper status notification ](https://github.com/opencomputeproject/SAI/pull/2087) <br>
[Adding additional Ether pkts stats ranges for Cisco Platforms ](https://github.com/opencomputeproject/SAI/pull/1997) <br>
[SAI Proposal TAM stream telemetry ](https://github.com/opencomputeproject/SAI/pull/2089) <br>
[Unify SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_CUSTOM_RANGE_BASE value ](https://github.com/opencomputeproject/SAI/pull/2114) <br>
[add SRv6 uSID behaviors to the validonly lists ](https://github.com/opencomputeproject/SAI/pull/2120) <br>
[EVPN Multi Home Support ](https://github.com/opencomputeproject/SAI/pull/2084) <br>
[Update check attr condition/validonly ](https://github.com/opencomputeproject/SAI/pull/2126) <br>
[Add HA session attribute Bounceback IP ](https://github.com/opencomputeproject/SAI/pull/2091) <br>
[Allow validonly attr to be conditioned on other validonly ](https://github.com/opencomputeproject/SAI/pull/2131) <br>
[Add packet trimming API ](https://github.com/opencomputeproject/SAI/pull/2077) <br>
[Extend validonly list for SAI_MY_SID_ENTRY_ATTR_TUNNEL_ID in _sai_my_sid_entry_attr_t  ](https://github.com/opencomputeproject/SAI/pull/2130) <br>
[Proposal to support custom vendor headers  ](https://github.com/opencomputeproject/SAI/pull/2122) <br>
[Add ACL actions for setting inner MACs  ](https://github.com/opencomputeproject/SAI/pull/2079) <br>
[Remove mandatory flag on the attribute TAM DSCP ](https://github.com/opencomputeproject/SAI/pull/2133) <br>
[Add occupancy and watermark counters in cells  ](https://github.com/opencomputeproject/SAI/pull/2092) <br>
[Add support for creating/modifying IPMC group with member list. ](https://github.com/opencomputeproject/SAI/pull/2121) <br>
[Add back port_error_status to sai_port_oper_status_notification_t  ](https://github.com/opencomputeproject/SAI/pull/2145) <br>
[Round-robin packet spraying and packet spraying selection per ECMP/LAG ](https://github.com/opencomputeproject/SAI/pull/2078) <br>
[Add a new ENI attribute SAI_ENI_ATTR_FLOW_TABLE_ID ](https://github.com/opencomputeproject/SAI/pull/2149) <br>
[ERSPAN Mirror Session Enhancements  ](https://github.com/opencomputeproject/SAI/pull/2142) <br>
[Add a new attribute SAI_BFD_SESSION_ATTR_NEXT_HOP_ID to saibfd.h ](https://github.com/opencomputeproject/SAI/pull/2127) <br>
[SAI TAM enhancements  ](https://github.com/opencomputeproject/SAI/pull/2141) <br>
[A new LAG member attribute to support weights.  ](https://github.com/opencomputeproject/SAI/pull/2143) <br>
[Add switch-level attribute to control PTP for the entire switch  ](https://github.com/opencomputeproject/SAI/pull/2148) <br>
[Add label attribute used to uniquely identify next-hop-group.](http://github.com/opencomputeproject/SAI/pull/2140)) <br>
[Fix enum initializers list for ancestry check ](https://github.com/opencomputeproject/SAI/pull/2157) <br>
[Hashing Enhancements for Efficient RoCE Traffic Distribution ](https://github.com/opencomputeproject/SAI/pull/2144) <br>
[Add SAI port attribute to fetch PAM4 eye height values ](https://github.com/opencomputeproject/SAI/pull/2154) <br>
[SAI proposal for Synchronous Ethernet ](https://github.com/opencomputeproject/SAI/pull/2146 ) <br>
[Add dash outbound portmap ](https://github.com/opencomputeproject/SAI/pull/2137) <br>
[Allow overriding variable SAI_HEADER_DIR ](https://github.com/opencomputeproject/SAI/pull/2161) <br>
[Add ENI mode VM/FNIC and trusted_vni entry ](https://github.com/opencomputeproject/SAI/pull/2158) <br>
[Add DSCP resolution from TC in packet trim ](https://github.com/opencomputeproject/SAI/pull/2155) <br>
[Add a RIF attribute to specify if the corresponding My MAC entry should not be created ](https://github.com/opencomputeproject/SAI/pull/2021) <br>
