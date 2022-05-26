# SAI Vlan Test plan
- [SAI Vlan Test plan](#sai-vlan-test-plan)
  - [Overriew](#overriew)
    - [Testbed](#testbed)
  - [Scope](#scope)
  - [Basic Configurations and SAI APIs](#basic-configurations-and-sai-apis)
    - [Basic VLAN configuration](#basic-vlan-configuration)
    - [Create VLAN and VLAN member](#create-vlan-and-vlan-member)
    - [Basic Forwarding Table](#basic-forwarding-table)
    - [Create FDB Entry](#create-fdb-entry)
    - [Packet example](#packet-example)
  - [Tests](#tests)
    - [Test Case: VLAN interface (RIF/SVI)](#test-case-vlan-interface-rifsvi)
      - [Test the configuration](#test-the-configuration)
      - [Test description](#test-description)
      - [Route entry](#route-entry)
      - [VLAN interface input packet](#vlan-interface-input-packet)
      - [VLAN interface output packet](#vlan-interface-output-packet)
      - [Test case](#test-case)
    - [Test Case: VLAN interface (RIF/SVI)](#test-case-vlan-interface-rifsvi-1)
      - [Route entry](#route-entry-1)
      - [VLAN interface to Port](#vlan-interface-to-port)
    - [Test suite: Tagging and trunk/access](#test-suite-tagging-and-trunkaccess)
    - [Composit scenario: ARP Flooding and learn](#composit-scenario-arp-flooding-and-learn)
    - [Test case: Test Frame Filtering](#test-case-test-frame-filtering)
  - [SAI APIs operations](#sai-apis-operations)
    - [Test case: Vlan member list.](#test-case-vlan-member-list)
    - [Test case: Test VLAN related counters.](#test-case-test-vlan-related-counters)
## Overriew
The purpose of this test plan is to test the VLAN function from SAI.

### Testbed
Those tests will be run on the testbed structure, the components are:
* PTF - running in a server that can connect to the target DUT
* SAI server - running on a dut
*P.S. All the tests are target on T0 scenario.*
## Scope
The test will include three parts
1. Vlan functionalities
   - Flooding
   - Forwarding
   - Access
   - Untagging(802.1Q)
   - ARP 
2. SAI APIs operations
   - Vlan Counters
   - Vlan and member list operations
3. Composit scenario
   - VLAN Interface (RIF/SVI)

## Basic Configurations and SAI APIs
During testing, we need to use SAI APIs for testing. By using the SAI-PTF structure, we can invoke the SAI with RPC APIs remotely, the sample code is below.

*P.S. All the tests are target on T0 scenario.*
### Basic VLAN configuration
|VLAN ID|Ports|Tag mode|HostIf
|-|-|-|-|
|1000|Port1-25|Untag|Ethernet4-96|

|VLAN connection|Ports|Tag mode|
|-|-|-|
|1000|Port1-25|Untag|

### Create VLAN and VLAN member
- Create Vlan 1000
   ```Python
   sai_thrift_create_vlan(self.client, vlan_id=1000)
   ```
- Create Native vlan 1000
   ```python
   sai_thrift_set_port_attribute(self.client, port_id, port_vlan_id=1000)
   ```
- Create Vlan member and VLAN member with different mode
   Untag mode and access port
   ```python
    ------------------------
    |VLAN ID|Ports|Tag mode|
    |-------|-----|--------|
    | 1000  |Port1| Untag  |
    ------------------------

   sai_thrift_create_vlan_member(
                    self.client,
                    vlan_id=1000,
                    bridge_port_id=port_bridge_port1,
                    vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)

   ```

### Basic Forwarding Table
For testing, we can use basic FDB APIs to Add the FDB entry into CAM. The Rule as below
|Name|MAC|PORT|VLAN|
|-|-|-|-|
|fdb1-25  |mac1-00:11:11:11:11:11 - mac25-03:55:55:55:55:55|Port1-25|1000|
|fdb26-32|mac17-02:66:66:66:66:66 - mac32-03:22:22:22:22:22|Port26-32||


### Create FDB Entry

- Add static FDB entry in the FDB table
   
  In the sample code below, we can see how to create FDB entry.

  This FDB entry is a static entry, it will ``forward`` packet when the packet with a mac1 on port1.
   ```python
    ------------------------------------------
    | MAC   |Ports| VLAN     | Type | Action |
    |-------|-----|----------|----------------
    | mac1  |Port1| vlan_oid |STATIC| FORWARD|
    ------------------------------------------

   sai_thrift_fdb_entry_t(switch_id=self.switch_id, mac_address=mac1, bv_id=self.vlan_oid)
   sai_thrift_create_fdb_entry(
                self.client,
                fdb_entry,
                type=SAI_FDB_ENTRY_TYPE_STATIC,
                bridge_port_id=Port1,
                packet_action=SAI_PACKET_ACTION_FORWARD)
   ```

- FDB operations
  Get FDB entries
  ```python
  sai_thrift_get_switch_attribute(
            self.client,
            available_fdb_entry=True)
  ```
- Clear FDB entries
  Clear the learned FDB entries with the VLAN id object id
  ```python
  sai_thrift_flush_fdb_entries(
                    self.client,
                    bv_id=self.vlan_oid,
                    entry_type=SAI_FDB_ENTRY_TYPE_DYNAMIC)
  ```

### Packet example
- Tagged packet with VLAN id 

   When a packet sent to a access port or send out from a access port, we will get a packet with the VLAN id.
   *In T0, we don't need to test tagged packet.*
   ```python
    tagged_packet(eth_dst='00:11:11:11:11:11',
                eth_src='00:22:22:22:22:22',
                vlan_vid=1000,
                ip_dst='172.16.0.1',
                ip_ttl=64)
   ```
- Untagged packet
  ```Python
    simple_tcp_packet(eth_dst='00:11:11:11:11:11',
                      eth_src='00:22:22:22:22:22',
                      ip_dst='172.16.0.1',
                      ip_ttl=64)
  ```
  
## Tests

### Test Case: VLAN interface (RIF/SVI) 
**Testing Objective**

When a switch needs to forward in layer3, it needs a VLAN interface in layer3(SVI) for packet routing.

In this test case, we will test VLAN interface and VLAN function.

#### Test the configuration 
For VLAN interface in layer3(SVI), it can be configured from a config_db.json. This json file, can be generated from a xml config as below. Base on this configuration we can see what the actual configuration will be.

#### Test description
```
Test example:
Vlan to Port:
  pkt(Untag) -> |DUT|Port3(VLAN1000:Access) -> RIF(VLAN_IF->MAC:IP1) ||| vlan1000 || -> Port(Access) -> ...
```

#### Route entry

|DestIp|Next Hop |Next Hop ip|Next Hop Mac|
|-|-|-|-|
|10.10.0.1|vlanInterface(VLAN100)|192.168.0.1(SVI_IP)|PORTMAC|

- Create Router for a VLAN interface (not used in currernt test, just for explain the route creation.)
  ```Python
  self.vlan100_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_VLAN,
            virtual_router_id=self.default_vrf,
            vlan_id=self.vlan100)

  self.nhop1 = sai_thrift_create_next_hop(
        self.client,
        ip=sai_ipaddress(next_hop_ip), #SVI_IP
        router_interface_id=self.vlan100_rif,
        type=SAI_NEXT_HOP_TYPE_IP)
  self.neighbor_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=self.vlan100_rif, 
            ip_address=sai_ipaddress(next_hop_ip))#SVI_IP
  #Nhop to vlan interface
  sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry1, 
            dst_mac_address=self.portmac) #Dest port mac, i.e port15:mac15 in vlan100
  #Route to vlan interface
  self.route_entry1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, 
            destination=sai_ipprefix(pkt_dest_ip+'/32')) # Packet dest IP
  sai_thrift_create_route_entry(
            self.client, self.route_entry1, next_hop_id=self.nhop1)
  ```

#### VLAN interface input packet
  ```Python
  simple_tcp_packet(
            eth_dst=SVI_MAC, #Forwarding in vlan
            eth_src=SRC_MAC,
            ip_dst=DEST_IP,
            ip_src=SRC_IP)
  ```

#### VLAN interface output packet
  ```Python
  simple_tcp_packet(
            eth_dst=PORT_MAC, 
            eth_src=SVI_MAC, #Switch MAC
            ip_dst=DEST_IP,
            ip_src=SRC_IP)
  ```

**Precondition/Setup:**
- Create VLAN as the basic configuration.
- Create an FDB table as a basic configuration.
- Create VLAN Interface for ``VLAN1000`` with ``SVI_IP``
- Create route ``DEST_IP`` and next hop with ``SVI_IP`` and ``PORTAMC`` to ``PORT15``.

#### Test case
|  Goal |Steps/Cases | Expect  |
|-|-|-|
| Forwarding packet on ``VLAN_INTERFACE`` .| Send ``Untagged`` packet with dest ``SVI_MAC`` on port1. |  ``Untagged`` packet received on ``PORT15``.|




### Test Case: VLAN interface (RIF/SVI) 
**Testing Objective**

Test the configuration as below

```xml
 <DeviceL3Info Hostname="AMS08-0101-0522-08T0">
    <VlanInterface Name="s1h" AttachTo="Ethernet6/1;Ethernet7/1;Ethernet8/1" vlanid="608" tag="608" Subnets="10.95.88.208/28" Type="AzureCompute" />
    <IPInterface AttachTo="s1h" Prefix="10.95.88.209/28" />
    <IPInterface AttachTo="s1h" Prefix="2603:10a0:31c:8144::1/64" />
  </DeviceL3Info>
```
Convert to config_db.json
```json
    "VLAN": {
        "Vlan608": {
            "members": [
                "Ethernet6/1",
                "Ethernet7/1",
                "Ethernet8/1",
				],
            "vlanid": "608"
        }
    },
    "VLAN_INTERFACE": {
        "Vlan608": {},
        "Vlan608|10.95.88.209/28": {},
        "Vlan608|2603:10a0:31c:8144::1/64": {}
    },
    "VLAN_MEMBER": {
        "Vlan608|Ethernet6/1": {
            "tagging_mode": "untagged"
        },
        "Vlan608|Ethernet7/1": {
            "tagging_mode": "untagged"
        },

        "Vlan608|Ethernet8/1": {
            "tagging_mode": "untagged"
        },
    }
```

**Testing Description**

When a switch needs to forward in layer3, it needs a VLAN interface in layer3(SVI) for packet routing or Trunk connection to other devices/servers.
```
Test example:
Vlan to Port:
  pkt(Untag) -> |DUT|Port3(VLAN10:Trunk) -> RIF(VLAN_IF->MAC:IP1) ||| vlan100 || -> Port(Access) -> ...
```

Need the APIs as below

- Create Router for a VLAN interface (not used in currernt test, just for explain the route creation.)
  ```Python
  self.vlan100_rif = sai_thrift_create_router_interface(
            self.client,
            type=SAI_ROUTER_INTERFACE_TYPE_VLAN,
            virtual_router_id=self.default_vrf,
            vlan_id=self.vlan100)

  self.nhop1 = sai_thrift_create_next_hop(
        self.client,
        ip=sai_ipaddress(next_hop_ip), #SVI_IP
        router_interface_id=self.vlan100_rif,
        type=SAI_NEXT_HOP_TYPE_IP)
  self.neighbor_entry1 = sai_thrift_neighbor_entry_t(
            rif_id=self.vlan100_rif, 
            ip_address=sai_ipaddress(next_hop_ip))#SVI_IP
  #Nhop to vlan interface
  sai_thrift_create_neighbor_entry(
            self.client, self.neighbor_entry1, 
            dst_mac_address=self.portmac) #Dest port mac, i.e port15:mac15 in vlan100
  #Route to vlan interface
  self.route_entry1 = sai_thrift_route_entry_t(
            vr_id=self.default_vrf, 
            destination=sai_ipprefix(pkt_dest_ip+'/32')) # Packet dest IP
  sai_thrift_create_route_entry(
            self.client, self.route_entry1, next_hop_id=self.nhop1)
  ```

#### Route entry
|DestIp|Next Hop |Next Hop ip|Next Hop Mac|
|-|-|-|-|
|10.10.0.1|vlanInterface(VLAN100)|10.10.10.10(SVI_IP)|PORTMAC|

#### VLAN interface to Port 
VLAN interface input packet
  ```Python
  simple_tcp_packet(
            eth_dst=SVI_MAC, #Forwarding in vlan
            eth_src=SRC_MAC,
            ip_dst=DEST_IP,
            ip_src=SRC_IP)
  ```

VLAN interface output packet
  ```Python
  simple_tcp_packet(
            eth_dst=PORT_MAC, 
            eth_src=SVI_MAC, #Switch MAC
            ip_dst=DEST_IP,
            ip_src=SRC_IP)
  ```

**Precondition/Setup:**
- Create VLAN as the basic configuration.
- Create an FDB table as a basic configuration.
- Create VLAN Interface for ``VLAN100`` with ``SVI_IP``
- Create route DESTIP:IP1 to ``PORT15``

Below is the test for checking this.
|  Goal |Steps/Cases | Expect  |
|-|-|-|
| Forwarding packet on ``VLAN_INTERFACE`` .| Send ``Untagged`` packet with dest ``SVI_MAC`` on port1. |  ``Untagged`` packet received on ``PORT15``.|


### Test suite: Tagging and trunk/access

This is a basic funcation testing. This test suite will cover the basic VLAN function around untag and access port.

*p.s. This test will not check function with native VLAN scenario (it will be in other tests). Please make sure the native VLAN will not impact the result.*

**Testing Objective**

With a untagged packet, on the access port, when ingress and egress happen, the behavior as below
| Port mode | packet tag mode | Direction | Action                                   |
| :---------|--------------- | :-------- | :--------------------------------------- |
| Access|Untag| Ingress   | Accept the packet.       |
|       |Untag | Egress    | Untag the packet. |


**Testing Description**
```
Test example:
                                     
1. pkt(Untag:10)  -> (access:10)|DUT|DMAC-> |(Acess:10)->pkt(Untag)  
```

Below is the test for checking this.
Precondition/Setup:
- Create VLAN as the basic configuration.
- Create an FDB table as a basic configuration.

Cases:
| Goal| Cases | Expect  |
|-|-|-|
|``Untag``:Acess -> Acess port.| Send VLAN100 ``Untagged`` packet with dest mac5 on port6. |  ``Untag`` packet received on port4.|

### Composit scenario: ARP Flooding and learn
- ARP request
  ```Python
  simple_arp_packet(
            eth_dst='FF:FF:FF:FF:FF:FF', # boardcast
            eth_src='00:11:11:11:11:11',
            arp_op=1,  # ARP request
            ip_tgt='10.10.10.2',
            ip_snd='10.10.10.1',
            hw_snd='00:11:11:11:11:11',
            hw_tgt="00:00:00:00:00:00")
  ```
- ARP response
  ```Python
  Untagged:

  simple_arp_packet(
            eth_dst='00:11:11:11:11:11', 
            eth_src='00:22:22:22:22:22',
            arp_op=2,  # ARP response
            ip_tgt='10.10.10.1',
            ip_snd='10.10.10.2',
            hw_snd='00:22:22:22:22:22',
            hw_tgt="00:11:11:11:11:11")
  Tagged:

  simple_arp_packet(
            eth_dst='00:11:11:11:11:11', 
            eth_src='00:22:22:22:22:22',
            arp_op=2,  # ARP response
            ip_tgt='10.10.10.1',
            ip_snd='10.10.10.2',
            vlan_vid=1000,
            hw_snd='00:22:22:22:22:22',
            hw_tgt="00:11:11:11:11:11")
  ```

In the ARP scenario, the mac learning process is:
1. Send an ARP request, with the source MAC, dest IP, and src IP, for broadcast, the DST MAC is ff:ff:ff:ff:ff:ff
2. After the encountered device get the ARP packet and sends out a response, the packet will be filled with a source mac, this is the target device mac for the dest IP.
3. Switch received that response, cause the switch learned a MAC on step 1 on the sending port, then it can make a unicast here.

**p.s. The test will be ended here, will not test the send from the port in step 1 again, it is already covered in other tests.**

**Testing Description**

Testing ARP scernairo
```
Test example:
1. ARP Request                                  
                                                                
  ARP Req pkt(Untag:DMAC) ->  (access:10)|DUT| -> Flooding -> VLAN:10 -> pkt(Untag):VLAN10:Access

2. ARP Response 
  pkt(access:10)|DUT| <- Forward(FDB:MAC=DMAC) <- VLAN:10:PORT <- RP Resp pkt(Untag:10:DMAC)

```
Precondition/Setup:
- Create VLAN as the basic configuration.

| Goal | Steps | Expect  |
|-|-|-|
| ARP Request flooding to all ports. | Send ``Untagged`` ARP packet with dest ``mac4`` on ``port1``. |``Untagged`` Packets from ``Access``, ``Untagged`` Packet from ``Access``|
| Mac learned from ARP request and ARP response.| Send a ``Untagged`` ARP response packet with src:mac4, dest:mac1 to port4 and check the FDB entries.|Unicast happened.|
| FDB entries added.| Call API to check the FDB entries.|New mac entries in CAM.|


### Test case: Test Frame Filtering
**Testing Objective**

Drop packet when packet's dest mac is port mac in MAC table.

**Testing Description**

Drop packet when packet's dest mac is port mac in MAC table.

```
Test example:                                
                                                                | SRCPORT:MAC1 = DMAC|
  pkt(Untag:DMAC):vlan:10 -> SRCPORT(Access:10)  -> |DUT| -> |FDB|                    | -> X 
                                                                | MAC2               |

```
Precondition/Setup:
- Create VLAN as the basic configuration.
- Create an FDB table as a basic configuration.
- Add a non-existing ``MacX`` to port1

| Goal |Steps |  Expect  |
|-|-|-|
| Filter frame. | Send VLAN100 ``Untagged`` packet with dest ``MacX`` on ``port1``. |Packet dropped|
| Filter frame. | Send VLAN100 ``Untagged`` packet with dest ``MAC1`` on ``port1``. |Packet dropped|


## SAI APIs operations

### Test case: Vlan member list.
**Testing Objective**

Test VLAN and member list.

Sample APIs

Get VLAN member list
```Python
 vlan_member_list = sai_thrift_object_list_t(count=100)
        mbr_list = sai_thrift_get_vlan_attribute(
            self.client, self.vlan10, member_list=vlan_member_list)
```

nagtive test
```python
    vlan_attr = sai_thrift_get_vlan_attribute(
        self.client, vlan_oid=1000, learn_disable=True)

    incorrect_member = sai_thrift_create_vlan_member(
        self.client,
        vlan_id=1000,
        bridge_port_id=self.port27_bp,
        vlan_tagging_mode=SAI_VLAN_TAGGING_MODE_UNTAGGED)
```

Precondition/Setup:
- Create VLAN as the basic configuration.
- Create an FDB table as a basic configuration.


| Steps/Cases | Goal | Expect  |
|-|-|-|
| VLAN member list and member count are right. |Use the SAI API to check the VLAN member |  Vlan member list.|
| VLAN and its member removed.|Remove the VLAN member from VLAN 10 and remove VLAN. |  VLAN and its member removed.|
| Error when creating member to un-exist VLAN. |Use the SAI API to create a VLAN member on VLAN 10 |  Vlan attribute is 0.|

### Test case: Test VLAN related counters.
**Testing Objective**

For VLAN-related counters, SAI should be able to get the counter and clear them.

Below is the sample API to operate those data

Check counters
```Python
        stats = sai_thrift_get_vlan_stats(self.client, self.vlan10)
        in_bytes = stats["SAI_VLAN_STAT_IN_OCTETS"]
        out_bytes = stats["SAI_VLAN_STAT_OUT_OCTETS"]
        in_packets = stats["SAI_VLAN_STAT_IN_PACKETS"]
        in_ucast_packets = stats["SAI_VLAN_STAT_IN_UCAST_PKTS"]
        out_packets = stats["SAI_VLAN_STAT_OUT_PACKETS"]
        out_ucast_packets = stats["SAI_VLAN_STAT_OUT_UCAST_PKTS"]

```
Clear counters
```Python
sai_thrift_clear_vlan_stats(self.client, self.vlan10)
```

Precondition/Setup:
- Create VLAN as the basic configuration.
- Create an FDB table as a basic configuration.
- Create VLAN Interface for ``VLAN10`` with ``IP1``
- Create route DESTIP:IP1 to ``PORT20``

| Goal |Steps/Cases |  Expect  |
|-|-|-|
| Forwarding, ``VLAN10`` ``Untagged`` packet on ``Native`` from a ``VLAN10`` ``Access``.|Send ``Untagged`` packet with dest ``mac3`` on ``port5``. |  ``Untagged`` packet received on port3.|
| Counter Changed accordingly.|Use the SAI API to check the counters | Counter increased, bytes counter: OCTETS increased, other counters + 1|
|  Counter reset.|Use the SAI API to clear the counters.| Related counter is reset to zero. |
