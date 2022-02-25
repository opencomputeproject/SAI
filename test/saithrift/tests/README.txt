List of Tests with brief description
====================================

L2AccessToAccessVlanTest
    Create a VLAN(10) and add two untagged interfaces for port1 and port2 as members
    Setup static FDB entries for port1 and port2
    Send a simple untagged packet on port 2 and verify packet on port 1

L2TrunkToTrunkVlanTest
    Create a VLAN(10) and add two tagged interfaces for port1 and port2 as members
    Setup static FDB entries for port1 and port2
    Send a simple vlan tag (10) packet on port 2 and verify packet on port 1
    [Note: Test packet recieved are checked w/o VLAN tag as the kernel strips the tag before senidn it in. The tag can be validated with PCAP files captured to validate the actual packet format]

L2AccessToTrunkVlanTest
    Create a VLAN(10) and add a tagged interfaces for port1 and  untagged interface for port2 as members
    Setup static FDB entries for port1 and port2
    Send a simple untagged packet on port 2 and verify packet on port 1 (w/ Vlan 10)

L2TrunkToAccessVlanTest
    Create a VLAN(10) and add a tagged interfaces for port2 and  untagged interface for port1 as members
    Setup static FDB entries for port1 and port2
    Send a simple tagged packet on port 2 and verify packet on port 1(untagged)

L2StpTest
    Create a VLAN(10) and add two untagged interfaces for port1 and port2 as members
    Setup static FDB entries for port1 and port2
    Set the STP states for the VLAN in forwarding state on both the ports
    Send a packet on port 2 and verify the packet on port 1
    Set the STP state on port 2 to blocking
    Send a packet on port 2 and verify that the packet is dropped

L3IPv4HostTest
    Create a VRF with IPv4 and IPv6 enabled
    Create two router interfaces in the same VRF
    Create a nhop, route (/32 mask) and neighbor entry for destination
    Send a packet on port 2 to the destination IP address and verify packet on port 1( changed destination and source mac address)

L3IPv4LpmTest
    Create a VRF with IPv4 and IPv6 enabled
    Create two router interfaces in the same VRF
    Create a nhop, route (/24 mask) and neighbor entry for destination
    Send a packet on port 2 to the destination IP address and verify packet on port 1( changed destination and source mac address)

L3IPv6HostTest
    Same as L3IPv4HostTest except the IP destination addresses are IPv6 addresses and the mask is /128. Test packets sent are IPv6 packets

L3IPv6LpmTest
    Same as L3IPv4LpmTest except the IP destination addresses are IPv6 addresses and the mask is /120. Test packets sent are IPv6 packets

L3IPv4EcmpHostTest
    Same as L3IPv4HostTest with the addition of another router interface and corresponding nexthop and neighbor entry. Packets are with different source IP addresses and verified to be recived on the two configured outgoing interfaces.

L3IPv6EcmpHostTest
    Same as L3IPv4EcmpHostTest for the IPv6 packets.

L3IPv4EcmpLpmTest
    Same as L3IPv4EcmpHostTest with a route prefix set

L3IPv6EcmpLpmTest
    Same as L3IPv6EcmpHostTest with a route prefix set

L2FloodTest
    Create a VLAN (10)
    Add three ports as untagged members to the VLAN
    Send packet on each port and verify that it only shows up on the other two ports

L2LagTest
    Create a LAG group with 4 ports 1 through 4
    Setup static FDB entries for port 5 and the LAG with unique MAC addresses
    Send packets (100)a with varying  and check count of packets recieved in ports 1 through 4 - verify that the counts are around divided equally amonst them with a margin of error 10%
    Send packets from each of the members and check they are recieved on port 5 (with port 5's destination MAC)

L3IPv4LagTest
    Same as L3Ipv4HostTest with the additon of adding interfaces on a LAG (with ports 1 and 2 as its members)
    Send packets on port 3 and ensure all are recieved on ports 1 and 2

L3IPv6LagTest
    Same as L3IPv4LagTest for IPv6 hosts

L3EcmpLagTest
    Same as L3IPv4EcmpLpmTest with two of the interfaces on LAG groups

IPAclTest
    Deny packets from a IPv4 Source address

IngressLocalMirrorTest
    Mirror incoming packets froma port to an alternate port. This uses ACL to send to a mirrror port

IngressERSpanMirrorTest
    Span incoming packets from one port onto an ERSPAN port

EgressLocalMirrorTest
    Mirror outgoing packets to a port to an alternate port

EgressERSpanMirrorTest
    Span outgoing packets to a port onto an ERSPAN port
