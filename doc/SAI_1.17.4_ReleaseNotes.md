# SAI 1.17.4 Release Notes

The Switch Abstraction Interface(SAI) defines the APIs to provide a vendor-independent way of controlling forwarding elements, such as a switching ASIC, an NPU or a software switch in a uniform manner. This release document covers the SAI API changes from SAI tag 1.16.1 to SAI tag 1.17.4. The previous release notes corresponding to SAI tag 1.16.1 is available at [SAI 1.16.1 release notes](https://github.com/opencomputeproject/SAI/blob/master/doc/SAI_1.16.1_ReleaseNotes.md)

This document explains the new SAI features as well as the enhancements and the bug fixes on existing features.


### List of enhancements added in this release:

[Update workflow to use latest Ubuntu ](https://github.com/opencomputeproject/SAI/pull/2165) <br>
[FDB table size attribute description changes ](https://github.com/opencomputeproject/SAI/pull/2164) <br>
[Add SAI_PORT_SERDES_ATTR_CUSTOM_COLLECTION ](https://github.com/opencomputeproject/SAI/pull/2156) <br>
[MACSec and IPSec FIPS Compliance ](https://github.com/opencomputeproject/SAI/pull/2167) <br>
[[az] Use ubuntu-latests for vmImage ](https://github.com/opencomputeproject/SAI/pull/2172) <br>
[New in drop reasons and new port stat counters to map to in drop reasons ](https://github.com/opencomputeproject/SAI/pull/2170) <br>
[Update README.md ](https://github.com/opencomputeproject/SAI/pull/2173) <br>
[Add switch attributes for CPU port buffer pools ](https://github.com/opencomputeproject/SAI/pull/2171) <br>
[Add SAI_MIRROR_SESSION_ATTR_LABEL to mirror session attributes ](https://github.com/opencomputeproject/SAI/pull/2180) <br>
[Add packet trimming counter ](https://github.com/opencomputeproject/SAI/pull/2177) <br>
[Adding custom range base values ](https://github.com/opencomputeproject/SAI/pull/2179) <br>
[KV for managing the Hierarchical NHG ](https://github.com/opencomputeproject/SAI/pull/2183) <br>
[Update DWRR scheduler weight range ](https://github.com/opencomputeproject/SAI/pull/2188) <br>
[Support for Next Hop Meta Data ](https://github.com/opencomputeproject/SAI/pull/2181) <br>
[Add BFD and ICMP echo session to monitored objects list ](https://github.com/opencomputeproject/SAI/pull/2193) <br>
[Fix bind point of tam ](https://github.com/opencomputeproject/SAI/pull/2182) <br>
[Enable ICMP sessions on LAG port ](https://github.com/opencomputeproject/SAI/pull/2194) <br>
[SAI LPO attributes ](https://github.com/opencomputeproject/SAI/pull/2178) <br>
[Added port serdes attributes for various taps ](https://github.com/opencomputeproject/SAI/pull/2207) <br>
[Packet trimming: Add counter description ](https://github.com/opencomputeproject/SAI/pull/2184) <br>
[Create API for fast linkup configuration ](https://github.com/opencomputeproject/SAI/pull/2203) <br>
[meta/Makefile: use &: for grouped targets ](https://github.com/opencomputeproject/SAI/pull/2200) <br>
[Add SAI_ATTR_PORT_FW_REVISION to saiport ](https://github.com/opencomputeproject/SAI/pull/2197) <br>
[Adding custom range base type for SAI_TAM_TEL_MATH_FUNC_TYPE ](https://github.com/opencomputeproject/SAI/pull/2206) <br>
[Added missing enable=True to acl_action_data_t in IPv6NextHdrTest (saiacl module) ](https://github.com/opencomputeproject/SAI/pull/2202) <br>
[Per Lane PRBS statuses and statistics ](https://github.com/opencomputeproject/SAI/pull/2204) <br>
[Manually override HW switchover for planned operation ](https://github.com/opencomputeproject/SAI/pull/2219) <br>
[[DASH] Add flow bulk get session event data struct ](https://github.com/opencomputeproject/SAI/pull/2175) <br>
[Add flow entry to the bulk session event data ](https://github.com/opencomputeproject/SAI/pull/2237) <br>
[Fix: "INVAILD" typo in enum '_sai_dash_flow_entry_bulk_get_session_filter_key_t' ](https://github.com/opencomputeproject/SAI/pull/2224) <br>
[Replicate VXLAN UDP sport security to switch level ](https://github.com/opencomputeproject/SAI/pull/2195) <br>
[Fix SAI metadata generation in Debian 13 ](https://github.com/opencomputeproject/SAI/pull/2239) <br>
[Detect XML structure instead of relying on Doxygen version ](https://github.com/opencomputeproject/SAI/pull/2240) <br>
[Fix enum sequencing issue ](https://github.com/opencomputeproject/SAI/pull/2243) <br>
