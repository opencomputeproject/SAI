# SAI 1.14.0 Release Notes

The Switch Abstraction Interface(SAI) defines the APIs to provide a vendor-independent way of controlling forwarding elements, such as a switching ASIC, an NPU or a software switch in a uniform manner. This release document covers the SAI API changes from SAI tag 1.13.3 to SAI tag 1.14.0. The previous release notes corresponding to SAI tag 1.13.3 is available at [SAI 1.13.3 release notes](https://github.com/opencomputeproject/SAI/blob/master/doc/SAI_1.13.3_ReleaseNotes.md) 

This document explains the new SAI features as well as the enhancements and the bug fixes on existing features. 


### List of enhancements added in this release: 

Add exception for sai_switch_health_data_t - [1989](https://github.com/opencomputeproject/SAI/pull/1989) <br>
Fix mac default value - [PR#1985](https://github.com/opencomputeproject/SAI/pull/1985) <br>
SAI Proposal for PoE Support - [PR#1977](https://github.com/opencomputeproject/SAI/pull/1977) <br>
Modified the test script to match ACL match fields which should be configured to match l4 src and dst port - [PR#1960](https://github.com/opencomputeproject/SAI/pull/1960) <br>
Modified the saiacl test suite to fix Invalid IP prefix format error while Creating route entry - [PR#1959](https://github.com/opencomputeproject/SAI/pull/1959) <br>
Add attribute to query the capability of Pre-Ingress ACL stage - [PR#1984](https://github.com/opencomputeproject/SAI/pull/1984) <br>
Move the tunnel decapsulation before L2/L3 - [PR#1938](https://github.com/opencomputeproject/SAI/pull/1938) <br>
Reformat SAI-Extensions.md to Markdown, make it readable on e.g. GitHub - [PR#1970](https://github.com/opencomputeproject/SAI/pull/1970) <br>
Fill in Doxygen tags on extension headers - [PR#1971](https://github.com/opencomputeproject/SAI/pull/1971) <br>
ARS Quantization Enhancements - [PR#1988](https://github.com/opencomputeproject/SAI/pull/1988) <br>
Modified the script to pass decap tunnel map object instead of encap tunnel object while creating tunnel decap entry - [PR#1958](https://github.com/opencomputeproject/SAI/pull/1958) <br>
Make TAM report optional - [PR#1957](https://github.com/opencomputeproject/SAI/pull/1957) <br>
Queue delay watermark - [PR#1920](https://github.com/opencomputeproject/SAI/pull/1920) <br>
Tunnel Term Types - fixed the description for types and validity checks for mask attributes - [PR#1848](https://github.com/opencomputeproject/SAI/pull/1848) <br>
Revert "Tunnel Term Types - fixed the description for types and validity checks for mask attributes" - [PR#1987](https://github.com/opencomputeproject/SAI/pull/1987) <br>
TAM Mirror-on-Drop to Local host via Generic Netlink - [PR#1986](https://github.com/opencomputeproject/SAI/pull/1986) <br>
Add DSCP mode to DASH - [PR#1981](https://github.com/opencomputeproject/SAI/pull/1981) <br>
ARS Enhancements - [PR#1978](https://github.com/opencomputeproject/SAI/pull/1978) <br>
Fix saithrift build issue for Bookworm SONiC - [PR#1974](https://github.com/opencomputeproject/SAI/pull/1974) <br>
Remove depreacated bm directory - [PR#1954](https://github.com/opencomputeproject/SAI/pull/1954) <br>
SAI proposal for icmp echo offload - [PR#1943](https://github.com/opencomputeproject/SAI/pull/1943) <br>
Revert "SAI proposal for icmp echo offload" - [PR#1969](https://github.com/opencomputeproject/SAI/pull/1969) <br>
SAI proposal for icmp echo offload - [PR#1943](https://github.com/opencomputeproject/SAI/pull/1943) <br>
Adding DASH SAI APIs for load balancer fast path scenario. - [PR#1966](https://github.com/opencomputeproject/SAI/pull/1966) <br>
Add BW to acronyms - [PR#1968](https://github.com/opencomputeproject/SAI/pull/1968) <br>
Add SAI_ACL_ENTRY_ATTR_FIELD_VRF_ID and SAI_ACL_ENTRY_ATTR_FIELD_IPMC_NPU_META_DST_HIT match qualifiers for acl table - [PR#1952](https://github.com/opencomputeproject/SAI/pull/1952) <br>
Add word of ENI into aspell. - [PR#1962](https://github.com/opencomputeproject/SAI/pull/1962) <br>
Add custom range start end values check - [PR#1945](https://github.com/opencomputeproject/SAI/pull/1945) <br>
Update the Doxyfile for doxygen in Debian Bookworm - [PR#1946](https://github.com/opencomputeproject/SAI/pull/1946) <br>
Add support for port stat extensions - [PR#1947](https://github.com/opencomputeproject/SAI/pull/1947) <br>
Enable sai_uint16_t in ProcessStructValueType Struct Member - [PR#1949](https://github.com/opencomputeproject/SAI/pull/1949) <br>
Cable diagnostics attribute added - [PR#1894](https://github.com/opencomputeproject/SAI/pull/1894) <br>
Add attributes to disable L3 rewrites - [PR#1924](https://github.com/opencomputeproject/SAI/pull/1924) <br>
Add MAC remote loopback to the port loopback enums. - [PR#1934](https://github.com/opencomputeproject/SAI/pull/1934) <br>
[TAM] Granular counter subscription - [PR#1670](https://github.com/opencomputeproject/SAI/pull/1670) <br>
Added saithrift support to return sai_object_id for a given system_port_id and read VOQ counters for system port - [PR#1931](https://github.com/opencomputeproject/SAI/pull/1931) <br>
Update .gitignore to ignore more generated files from DASH that doesn't need to be checked in. - [PR#9133](https://github.com/opencomputeproject/SAI/pull/1933) <br>
Add word of TWAMP for aspell - [PR#1932](https://github.com/opencomputeproject/SAI/pull/1932) <br>
Update message for struct size check - [PR#1929](https://github.com/opencomputeproject/SAI/pull/1929) <br>
Update ancestry file to latest commit - [PR#1927](https://github.com/opencomputeproject/SAI/pull/1927) <br>
Add custom range start/end to sai_object_type_t and sai_api_t as workaround for internal enum size - [PR#1926](https://github.com/opencomputeproject/SAI/pull/1926) <br>
Flush all FDB entries if no attributes specified - [PR#1918](https://github.com/opencomputeproject/SAI/pull/1918) <br>
Add Private Link attributes to DASH - [PR#1907](https://github.com/opencomputeproject/SAI/pull/1907) <br>
Add TX/RX precoding to SERDES attribute list - [PR#1903](https://github.com/opencomputeproject/SAI/pull/1903) <br>
Allow null on SAI_PORT_ATTR_PORT_SERDES_ID - [PR#1914](https://github.com/opencomputeproject/SAI/pull/1914) <br>
Make notifications and pointers enum stable - [PR#1913](https://github.com/opencomputeproject/SAI/pull/1913) <br>
Remove serialize check for static arrays in structs - [PR#1912](https://github.com/opencomputeproject/SAI/pull/1912) <br>
Add option to saidepgraph to show extensions objects - [PR#1911](https://github.com/opencomputeproject/SAI/pull/1911) <br>
Add sanity check for acl mask field - [PR#1910](https://github.com/opencomputeproject/SAI/pull/1910) <br>
Check sai_json_t type size - [PR#1909](https://github.com/opencomputeproject/SAI/pull/1909) <br>
Use libsaimetadata.so instead of linked meta objects - [PR#1901](https://github.com/opencomputeproject/SAI/pull/1901) <br>
Fix tests to include extensions objects - [PR#1900](https://github.com/opencomputeproject/SAI/pull/1900) <br>
Support update of SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_VR_ID - [PR#1855](https://github.com/opencomputeproject/SAI/pull/1855) <br>
Refactor RPC frontend - [PR#1896](https://github.com/opencomputeproject/SAI/pull/1896) <br>
Add metadata query api version - [PR#1898](https://github.com/opencomputeproject/SAI/pull/1898) <br>
Fix spelling - [PR#1895](https://github.com/opencomputeproject/SAI/pull/1895) <br>
Move some warning messages from sai sanity to debug - [PR#1892](https://github.com/opencomputeproject/SAI/pull/1892) <br>
Add relaxed condition type - [PR#1893](https://github.com/opencomputeproject/SAI/pull/1893) <br>
Add bulk-api for router interface (RIF) object - [PR#1891](https://github.com/opencomputeproject/SAI/pull/1891) <br>
Add warning message on bm directory being deprecated - [PR#1889](https://github.com/opencomputeproject/SAI/pull/1889) <br>
Tunnel Term Attributes validation fixed for all MP2P and MP2MP attributes - [PR#1799](https://github.com/opencomputeproject/SAI/pull/1799) <br>
Revert "Tunnel Term Attributes validation fixed for all MP2P and MP2MP attributes" - [PR#1890](https://github.com/opencomputeproject/SAI/pull/1890) <br>
Build saithriftv2 saiserver in az pipeline - [PR#1888](https://github.com/opencomputeproject/SAI/pull/1888) <br>
