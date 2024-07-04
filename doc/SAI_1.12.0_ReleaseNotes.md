# SAI 1.12.0 Release Notes

The Switch Abstraction Interface(SAI) defines the APIs to provide a vendor-independent way of controlling forwarding elements, such as a switching ASIC, an NPU or a software switch in a uniform manner. This release document covers the SAI API changes from SAI tag 1.10.2 to SAI tag 1.11.0. The previous release notes corresponding to SAI tag 1.11.0 is available at [SAI 1.11.0 release notes](https://github.com/opencomputeproject/SAI/blob/master/doc/SAI_1.11.0_ReleaseNotes.md) 

This document explains the new SAI features as well as the enhancements and the bug fixes on existing features. 


### List of enhancements added in this release: 

Add remove route entry, nexthop group test [PR#1624](https://github.com/opencomputeproject/SAI/pull/1624) <br> 
Add ip prefix list type [PR#1629](https://github.com/opencomputeproject/SAI/pull/1629) <br> 
Fix prefix list sanity check [PR#1643](https://github.com/opencomputeproject/SAI/pull/1643) <br> 
Fix a compile issue when build docker-syncd-brcm-dnx-rpc.gz [PR#1649](https://github.com/opencomputeproject/SAI/pull/1649) <br> 
Add ecmp lag hash seed test [PR#1654](https://github.com/opencomputeproject/SAI/pull/1654) <br> 
Remove pdb [PR#1666](https://github.com/opencomputeproject/SAI/pull/1666) <br> 
Hierarchical ecmp [PR#1555](https://github.com/opencomputeproject/SAI/pull/1555) <br> 
Add svi mac move test [PR#1680](https://github.com/opencomputeproject/SAI/pull/1680) <br> 
Update saiport.h to add new interface type for USXGMII [PR#1679](https://github.com/opencomputeproject/SAI/pull/1679) <br> 
Update build status badge [PR#1641](https://github.com/opencomputeproject/SAI/pull/1641) <br> 
Modify case description according to actual test step [PR#1705](https://github.com/opencomputeproject/SAI/pull/1705) <br> 
Enable github code scanning to replace LGTM. [PR#1710](https://github.com/opencomputeproject/SAI/pull/1710) <br> 
Fixed port autoneg status attr description [PR#1664](https://github.com/opencomputeproject/SAI/pull/1664) <br> 
Add DASH API [PR#1590](https://github.com/opencomputeproject/SAI/pull/1590) <br> 
Add basic ipinip decap encap test, refactor svi ipinip test and fix bug [PR#1721](https://github.com/opencomputeproject/SAI/pull/1721) <br> 
Improve enum values integration check [PR#1727](https://github.com/opencomputeproject/SAI/pull/1727) <br> 
Add SAI_UDF_MATCH_ATTR_L4_DST_PORT_TYPE attribute to _sai_udf_match_attr_t [PR#1739](https://github.com/opencomputeproject/SAI/pull/1739) <br> 
Add svitunnel flood, ttlmode test [PR#1751](https://github.com/opencomputeproject/SAI/pull/1751) <br> 
Refactor QoS related PTF test cases [PR#1755](https://github.com/opencomputeproject/SAI/pull/1755) <br> 
Adaptive Routing and Switching [PR#1681](https://github.com/opencomputeproject/SAI/pull/1681) <br> 
Add port attribute to capture debug data. [PR#1324](https://github.com/opencomputeproject/SAI/pull/1324) <br> 
Add tunnel test plan [PR#44](https://github.com/opencomputeproject/SAI/pull/44) & [PR#1776](https://github.com/opencomputeproject/SAI/pull/1776) <br>
Add an ACL stage for exact match [PR#1717](https://github.com/opencomputeproject/SAI/pull/1717) <br> 
Add ACL table attribute to specify Dst IPv6 valid bits [PR#1718](https://github.com/opencomputeproject/SAI/pull/1718) <br> 
Provide the ability to set Hash algorithm, offset for a port that [PR#1775](https://github.com/opencomputeproject/SAI/pull/1775) <br> 
Add ACL table and table group attributes to identify match type [PR#1730](https://github.com/opencomputeproject/SAI/pull/1730) <br> 
Add more tunnel test case [PR#1778](https://github.com/opencomputeproject/SAI/pull/1778) <br> 
WatchDog Timer Configuration for VoQ [PR#1779](https://github.com/opencomputeproject/SAI/pull/1779) <br> 
Fabric Switch Isolation [PR#1780](https://github.com/opencomputeproject/SAI/pull/1780) <br> 
SRv6 VPN enhancements [PR#1744](https://github.com/opencomputeproject/SAI/pull/1744) <br> 
Add SRV6 encap support to BFD [PR#1759](https://github.com/opencomputeproject/SAI/pull/1759) <br> 
Add DASH attributes for ST routing [PR#1729](https://github.com/opencomputeproject/SAI/pull/1729) <br> 
Add modes to update the host interface oper status. [PR#1646](https://github.com/opencomputeproject/SAI/pull/1646) <br> 

### SAI-PTF enhancements <br>

Add decorator for skipping test on specified error [PR#1609](https://github.com/opencomputeproject/SAI/pull/1609) <br> 
Add more ECMP tests [PR#1628](https://github.com/opencomputeproject/SAI/pull/1628) <br> 
Add disable vlan interface ingress from ecmp test, remove pdb from test [PR#1631](https://github.com/opencomputeproject/SAI/pull/1631) <br> 
Enhance the function for getting counters in sai_adapter.py [PR#1626](https://github.com/opencomputeproject/SAI/pull/1626) <br> 
Add uninit API support in SAI-PTF and enhance the platformhelper [PR#1636](https://github.com/opencomputeproject/SAI/pull/1636) <br> 
Enable ptf case on BRCM platform [PR#1632](https://github.com/opencomputeproject/SAI/pull/1632) <br> 
Add/Update ecmp coexist test [PR#1665](https://github.com/opencomputeproject/SAI/pull/1665) <br> 
Disable L3 config for some vlan and fdb tests [PR#1640](https://github.com/opencomputeproject/SAI/pull/1640) <br> 
Ipv4InIpv6 and Ipv4InIpv4 decap test [PR#1621](https://github.com/opencomputeproject/SAI/pull/1621) <br> 
Fix failed cases [PR#1645](https://github.com/opencomputeproject/SAI/pull/1645) <br> 
Fix Failed cases by add dataplane flush with time consuming verify [PR#1648](https://github.com/opencomputeproject/SAI/pull/1648) <br> 
Add tunnel encap test and more route test [PR#1644](https://github.com/opencomputeproject/SAI/pull/1644)  <br> 
Add invocation logger [PR#1651](https://github.com/opencomputeproject/SAI/pull/1651)  <br> 
Add conditional import for unit test [PR#1656](https://github.com/opencomputeproject/SAI/pull/1656)  <br> 
Include sai expermential for generate the rpc headers [PR#1660](https://github.com/opencomputeproject/SAI/pull/1660)  <br> 
Warm reboot structure for SAI-PTF and SAI-T0 [PR#1668](https://github.com/opencomputeproject/SAI/pull/1668) <br> 
Add svi macage, RouteDiffPrefixAddThenDeleteShorter Test [PR#1671](https://github.com/opencomputeproject/SAI/pull/1671) <br> 
Fix issue when transfer make parameter GEN_SAIRPC_OPTS [PR#1676](https://github.com/opencomputeproject/SAI/pull/1676)  <br> 
Add return value in the SAI-PTF log [PR#1682](https://github.com/opencomputeproject/SAI/pull/1682)  <br> 
Add to config file for setup port [PR#1713](https://github.com/opencomputeproject/SAI/pull/1713)  <br> 
API Logger - reformat dict in return value [PR#1690](https://github.com/opencomputeproject/SAI/pull/1690) & [PR#1715](https://github.com/opencomputeproject/SAI/pull/1715)  <br> 
Update SAI-PTF doc with latest download links and run case parameters [PR#1714](https://github.com/opencomputeproject/SAI/pull/1714)  <br> 
Split L3 subtests into individual tests [PR#1677](https://github.com/opencomputeproject/SAI/pull/1677)  <br> 
Add svi l3 test [PR#1692](https://github.com/opencomputeproject/SAI/pull/1692)  <br> 
Reformat ptf cases with configer [PR#1712](https://github.com/opencomputeproject/SAI/pull/1712)  <br> 
Remove Draft tag from cases on enabled cases [PR#1724](https://github.com/opencomputeproject/SAI/pull/1724)  <br> 
Support dynamic port init [PR#1725](https://github.com/opencomputeproject/SAI/pull/1725)  <br> 
Init port base on available interface and optimize init process [PR#1726](https://github.com/opencomputeproject/SAI/pull/1726) <br> 
Update PTF submodule to latest master [PR#1720](https://github.com/opencomputeproject/SAI/pull/1720)  <br> 
Read configuration from a json config file [PR#1732](https://github.com/opencomputeproject/SAI/pull/1732)  <br> 
Fix SAI PTF nexthop group test cases [PR#1719](https://github.com/opencomputeproject/SAI/pull/1719)  <br> 
Enhance the check enum lock script [PR#1741](https://github.com/opencomputeproject/SAI/pull/1741) <br> 
Fix two fdb counter assertation failures [PR#1749](https://github.com/opencomputeproject/SAI/pull/1749) <br> 
Add back the skipped cases for reporting the failure [PR#1752](https://github.com/opencomputeproject/SAI/pull/1752)  <br> 
Refactor ptf case by reading configuration from json file [PR#1753](https://github.com/opencomputeproject/SAI/pull/1753) <br> 
Query and clear counter by each id [PR#1754](https://github.com/opencomputeproject/SAI/pull/1754) <br> 
Add draft tag for unchecked test cases [PR#1757](https://github.com/opencomputeproject/SAI/pull/1757)  <br> 
Convert generated enum code from ctypesgen into pythoon enum class [PR#1758](https://github.com/opencomputeproject/SAI/pull/1758) <br> 
Refactor ptf switch cases [PR#1756](https://github.com/opencomputeproject/SAI/pull/1756)  <br> 
Add port test cases to cover AN/LT [PR#1765](https://github.com/opencomputeproject/SAI/pull/1765)  <br> 
Add accepted error code in switch case [PR#1767](https://github.com/opencomputeproject/SAI/pull/1767)  <br> 
Enable Queue tests [PR#1768](https://github.com/opencomputeproject/SAI/pull/1768)  <br> 
Fix a parameter error in queue case [PR#1770](https://github.com/opencomputeproject/SAI/pull/1770)  <br> 
Refactor stats counter in buffer and one switch case [PR#1771](https://github.com/opencomputeproject/SAI/pull/1771)  <br> 
Refactor stats counter related method [PR#1773](https://github.com/opencomputeproject/SAI/pull/1773)  <br> 
Tunnel cases support p2mp [PR#1772](https://github.com/opencomputeproject/SAI/pull/1772)  <br> 
Add a route case to compare the cases structure [PR#1774](https://github.com/opencomputeproject/SAI/pull/1774)  <br> 
API Logger - reformat arg values [PR#1696](https://github.com/opencomputeproject/SAI/pull/1696) & [PR#1700](https://github.com/opencomputeproject/SAI/pull/1700) <br> 
Skip test when hit expected error from sai api [PR#1699](https://github.com/opencomputeproject/SAI/pull/1699)  <br> 
Add uninit method for warm reboot shut down [PR#1704](https://github.com/opencomputeproject/SAI/pull/1704)  <br> 

