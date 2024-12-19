# SAI 1.15.1 Release Notes

The Switch Abstraction Interface(SAI) defines the APIs to provide a vendor-independent way of controlling forwarding elements, such as a switching ASIC, an NPU or a software switch in a uniform manner. This release document covers the SAI API changes from SAI tag 1.14.0 to SAI tag 1.15.1. The previous release notes corresponding to SAI tag 1.14.0 is available at [SAI 1.14.0 release notes](https://github.com/opencomputeproject/SAI/blob/master/doc/SAI_1.14.0_ReleaseNotes.md) 

This document explains the new SAI features as well as the enhancements and the bug fixes on existing features. 


### List of enhancements added in this release: 

Fix dangling pointer warning for gcc 12 - [PR#1996](https://github.com/opencomputeproject/SAI/pull/1996) <br> 
Add sai_u32_range_t support for SAI struct entries. - [PR#2002](https://github.com/opencomputeproject/SAI/pull/2002) <br> 
Add action type to ENI ether address map entry to be consistent with all other DASH tables. - [PR#2005](https://github.com/opencomputeproject/SAI/pull/2005) <br> 
Update the inbound routing header to use a new tunnel decapsulation action name. - [PR#2006](https://github.com/opencomputeproject/SAI/pull/2006) <br> 
Update SAI API for DASH metering. - [PR#1999](https://github.com/opencomputeproject/SAI/pull/1999) <br> 
Add DASH flow resimulation SAI APIs. - [PR#2004](https://github.com/opencomputeproject/SAI/pull/2004) <br> 
Add basic DASH HA session APIs for concepts, state management and inline sync. - [PR#2007](https://github.com/opencomputeproject/SAI/pull/2007) <br> 
Fix SAI adapter exception due to POE PSE attribute conflicting with variables in sai_adapter.py. - [PR#2011](https://github.com/opencomputeproject/SAI/pull/2011) <br> 
Add sai-thrift support to enable/disable CREDIT WATCHDOG for Voq switch - [PR#2010](https://github.com/opencomputeproject/SAI/pull/2010) <br> 
Use history file on check structs - [PR#2016](https://github.com/opencomputeproject/SAI/pull/2016) <br> 
Allow vendors to add custom range attributes - [PR#2017](https://github.com/opencomputeproject/SAI/pull/2017) <br> 
Add new enum to support 8 lanes breakout mode - [PR#2009](https://github.com/opencomputeproject/SAI/pull/2009) <br> 
Add bulk sync API, flow reconcile API and related counters. - [PR#2014](https://github.com/opencomputeproject/SAI/pull/2014) <br> 
Add Tunnel API - [PR#2025](https://github.com/opencomputeproject/SAI/pull/2025) <br> 
[Add methods to update switch notification pointers - [PR#2029](https://github.com/opencomputeproject/SAI/pull/2029) <br> 
Remove error message on unsupported notification - [PR#2030](https://github.com/opencomputeproject/SAI/pull/2030) <br> 
Use newer action name for IRE meter attributes - [PR#2032](https://github.com/opencomputeproject/SAI/pull/2032) <br> 
Add missing prefixes to PL mapping - [PR#2035](https://github.com/opencomputeproject/SAI/pull/2035) <br> 
Add Routing Group API - [PR#2026](https://github.com/opencomputeproject/SAI/pull/2026) <br> 
SAI proposal for icmp echo offload - [PR#2020](https://github.com/opencomputeproject/SAI/pull/2020) <br> 
Move extensions SAI and API to 0x20000000 range - [PR#2028](https://github.com/opencomputeproject/SAI/pull/2028) <br> 
Update DASH pipeline and HA related counters. - [PR#2051](https://github.com/opencomputeproject/SAI/pull/2051) <br> 
Update DASH SAI API comments - [PR#2050](https://github.com/opencomputeproject/SAI/pull/2050) <br> 
Add port attribute to get the max debug data size. - [PR#2033](https://github.com/opencomputeproject/SAI/pull/2033) <br> 
Add DPU acronym - [PR#2053](https://github.com/opencomputeproject/SAI/pull/2053) <br> 
Convert DASH meter bucket object to table entry - [PR#2056](https://github.com/opencomputeproject/SAI/pull/2056) <br> 
fix markdown format error - [PR#2044](https://github.com/opencomputeproject/SAI/pull/2044) <br> 
Add new Next Hop Type and allow Next Hop to be an IPMC Group Member - [PR#2041](https://github.com/opencomputeproject/SAI/pull/2041) <br> 
SER Support - [PR#1993](https://github.com/opencomputeproject/SAI/pull/1993) <br> 
Add Dash HA DPU scope DPU driven mode attributes - [PR#2055](https://github.com/opencomputeproject/SAI/pull/2055) <br> 
Correct SAI_HA_SCOPE_ATTR_HA_SET_ID type - [PR#2068 ](https://github.com/opencomputeproject/SAI/pull/2068) <br> 
Unreliable LOS - [PR#2063](https://github.com/opencomputeproject/SAI/pull/2063) <br> 
Add DASH Flow API to support connection tracking - [PR#2064](https://github.com/opencomputeproject/SAI/pull/2064) <br> 
Add DASH appliance object APIs - [PR#2073](https://github.com/opencomputeproject/SAI/pull/2073) <br> 
sFlow Truncation support - [PR#2065](https://github.com/opencomputeproject/SAI/pull/2065) <br> 
Include the metadata headers in the generated SWIG bindings - [PR#2069](https://github.com/opencomputeproject/SAI/pull/2069) <br> 
Extended port operational status with various error and fault status - [PR#2060](https://github.com/opencomputeproject/SAI/pull/2060) <br> 
Add ENI mac selection attributes and tunnel learning attributes in DASH. - [PR#2085](https://github.com/opencomputeproject/SAI/pull/2085) <br> 
Use strinct flags on sai_port_error_status_t - [PR#2083](https://github.com/opencomputeproject/SAI/pull/2083) <br> 
SAI Proposal for Counter enhancement. - [PR#1941](https://github.com/opencomputeproject/SAI/pull/1941) <br> 
Hostif trap type for Subnet routes - [PR#2066](https://github.com/opencomputeproject/SAI/pull/2066) <br> 
Add bulk APIs for ingress priority group and queue. - [PR#2076](https://github.com/opencomputeproject/SAI/pull/2076) <br> 
HW Based FRR - [PR#2071](https://github.com/opencomputeproject/SAI/pull/2071) <br> 
Fix for RPC build failure - [PR#2097](https://github.com/opencomputeproject/SAI/pull/2097) <br> 
