# SAI 1.19.2 Release Notes

The Switch Abstraction Interface (SAI) defines the APIs to provide a vendor-independent way of controlling forwarding elements, such as a switching ASIC, an NPU or a software switch in a uniform manner. This release document covers the changes after SAI tag v1.18.1 through SAI tag v1.19.2. The previous release notes are available at [SAI 1.18.1 release notes](./SAI_1.18.1_ReleaseNotes.md).

This document lists new features, enhancements, bug fixes and corresponding documentation introduced in SAI v1.19.2.

### Features:

- [Credit Based Flow Control as defined in UEC 1.0](https://github.com/opencomputeproject/SAI/pull/2263)
- [Packet Drop Monitoring](https://github.com/opencomputeproject/SAI/pull/2328)
- [Support for Multiple Firmwares](https://github.com/opencomputeproject/SAI/pull/2331)
- [Feature: PTP Peer-to-Peer (P2P) Path Delay Support](https://github.com/opencomputeproject/SAI/pull/2324)
- [SAI API Performance Monitoring](https://github.com/opencomputeproject/SAI/pull/2287)
- [TRIM to Sender and TC-DSCP Map](https://github.com/opencomputeproject/SAI/pull/2349)
- [EVPN MultiHoming FastReRoute for L3VNI-routed traffic](https://github.com/opencomputeproject/SAI/pull/2332)
- [SAI EVPN VXLAN multihoming split-horizon for P2MP tunnels](https://github.com/opencomputeproject/SAI/pull/2286)
- [HW FRR switchover notification support for protection groups](https://github.com/opencomputeproject/SAI/pull/2269)
- [Passive Optical Network (PON)](https://github.com/opencomputeproject/SAI/pull/2350)
- [OTN attenuator and amplifier](https://github.com/opencomputeproject/SAI/pull/2217)
- [Optical circuit switch](https://github.com/opencomputeproject/SAI/pull/2229)
- [APSU SAI port attributes](https://github.com/opencomputeproject/SAI/pull/2303)
- [VLAN-aware MLD Snooping support](https://github.com/opencomputeproject/SAI/pull/2326)
- [Link Down Debounce](https://github.com/opencomputeproject/SAI/pull/2284)

### Enhancements:

- [Add policer operational attributes and MEF policing modes](https://github.com/opencomputeproject/SAI/pull/2323)
- [Add User defined trap types for route,ipmc,l2mc,mysid](https://github.com/opencomputeproject/SAI/pull/2344)
- [Add ACL Mirror Copy Type and Mirror Session ID match fields](https://github.com/opencomputeproject/SAI/pull/2274)
- [[ACL] Add support for REDIRECT_ORIGINAL_PACKET action](https://github.com/opencomputeproject/SAI/pull/2318)
- [Add port MRU attribute](https://github.com/opencomputeproject/SAI/pull/2353)
- [ACL match route destination](https://github.com/opencomputeproject/SAI/pull/2329)
- [NHG Unequal Cost Members](https://github.com/opencomputeproject/SAI/pull/2330)
- [MACSEC: Add additional IEEE 802.1ae stats for SA](https://github.com/opencomputeproject/SAI/pull/2275)
- [Allow IP and MAC header attributes on SFLOW sessions](https://github.com/opencomputeproject/SAI/pull/2366)
- [Support for ADMIN state for BFD session](https://github.com/opencomputeproject/SAI/pull/2267)
- [Add label attribute for ACL entry](https://github.com/opencomputeproject/SAI/pull/2322)
- [saiport: Add counters for 1519_TO_MAX_OCTETS packet size range](https://github.com/opencomputeproject/SAI/pull/2272)
- [Added support for supported loopback mode for port object](https://github.com/opencomputeproject/SAI/pull/2325)
- [[debugcounter] Add support for SAI_IN_DROP_REASON_EGRESS_LINK_DOWN](https://github.com/opencomputeproject/SAI/pull/2333)
- [Add DASH ENI HA operational flow owner attribute](https://github.com/opencomputeproject/SAI/pull/2281)
- [[debug_counter] Add support for SAI_IN_DROP_REASON_LPM_ANY](https://github.com/opencomputeproject/SAI/pull/2354)
- [Count BFD session state changes from UP to DOWN](https://github.com/opencomputeproject/SAI/pull/2268)
- [Increasing the data type from 16 bit to 32 bit for native receiver total credits](https://github.com/opencomputeproject/SAI/pull/2358)
- [Add wide label attribute for objects with a label attribute](https://github.com/opencomputeproject/SAI/pull/2327)
- [Add sample packet and sample mirror attributes to LAG](https://github.com/opencomputeproject/SAI/pull/2369)
- [SAI HW Protection amendments and clarifications](https://github.com/opencomputeproject/SAI/pull/2348)
- [Added new attribute SAI_MIRROR_SESSION_ATTR_GUARANTEE_RATE](https://github.com/opencomputeproject/SAI/pull/2276)
- [Added separate TC to Queue map for Multicast Flows](https://github.com/opencomputeproject/SAI/pull/2340)
- [Port storm control enhancemnets](https://github.com/opencomputeproject/SAI/pull/2258)

### Infra changes and bug fixes:

- [Fix gensairpc.pl crash on Doxygen 1.9.8+ by reusing NeedsTwoPassProcessing](https://github.com/opencomputeproject/SAI/pull/2282)
- [sai_test: add opt-in SIMULATE_SONIC to emulate SONiC teamd/IntfMgr netdevs for standalone benches](https://github.com/opencomputeproject/SAI/pull/2316)
- [Fix SAI_STATUS_IS_* attribute-range helpers for negative status codes](https://github.com/opencomputeproject/SAI/pull/2308)
- [[sai_test]: Isolate simulated IPv6 startup control traffic-](https://github.com/opencomputeproject/SAI/pull/2321)
- [sai_test config: drop redundant bridge_id, env-gated port bring-up, fix v4/v6 NHG port-list aliasing](https://github.com/opencomputeproject/SAI/pull/2301)
- [saiserver: default switch_id on neighbor/route entries when omitted](https://github.com/opencomputeproject/SAI/pull/2300)
- [saithriftv2: link VPP libraries for platform=vpp + bookworm/py3 build fixes](https://github.com/opencomputeproject/SAI/pull/2299)
- [Fix enum re-ordering in metadata header files](https://github.com/opencomputeproject/SAI/pull/2359)
- [fix NHG attribute comment](https://github.com/opencomputeproject/SAI/pull/2317)
- [Update Observation Domain ID description in TAM streaming proposal](https://github.com/opencomputeproject/SAI/pull/2296)
- [Fix sai_adapter.py generation for objects without stat enums](https://github.com/opencomputeproject/SAI/pull/2370)
- [docs: fix typo strucutre -> structure](https://github.com/opencomputeproject/SAI/pull/2341)
- [SAI Hostif - SAI_HOSTIF_TABLE_ENTRY_ATTR_OBJ_ID meta comment update](https://github.com/opencomputeproject/SAI/pull/2345)
- [[doc]: Clarify HFT enterprise number layout](https://github.com/opencomputeproject/SAI/pull/2346)
- [Makefile change to validate the generated python files](https://github.com/opencomputeproject/SAI/pull/2371)
- [Move health_event.md to doc directory](https://github.com/opencomputeproject/SAI/pull/2347)
