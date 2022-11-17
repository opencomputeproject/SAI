# SAI PTF TESTPLAN

SAI PTF TESTPLAN contains all test cases covered in ptf/ directory divided by functionality.

## ACL

| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
|  acl.1 | Create ACL table and ACL entries and attach table to port/LAG/RIF/VLAN | saiacl.SrcIpAclTest, saiacl.DstIpAclTest, saiacl.MACSrcAclTest, saiacl.L3L4PortTest, saiacl.L3AclRangeTest, saiacl.TCPFlagsACLTest, saiacl.IPv6NextHdrTest, saiacl.IPAclFragmentTest, saiacl.IngressL3AclDscp, saiacl.AclRedirectPortAndLagTest, saiacl.AclLagTest, saiacl.AclPreIngressTest, saiacl.L3AclCounterTest, saiacl.AclGroupTest |
|  acl.2 | Create ACL table, attach table to port, and add entries to ACL | saiacl.SrcIpAclTest, saiacl.DstIpAclTest, saiacl.MACSrcAclTest, saiacl.L3L4PortTest, saiacl.L3AclRangeTest, saiacl.TCPFlagsACLTest, saiacl.IPv6NextHdrTest, saiacl.IPAclFragmentTest, saiacl.IngressL3AclDscp, saiacl.AclRedirectPortAndLagTest, saiacl.AclLagTest, saiacl.AclPreIngressTest, saiacl.L3AclCounterTest, saiacl.AclGroupTest |
|  acl.3 | Add ACL table group, v4 or v6 permit/deny/redirect ACL and v4/v6 mirror ACLs and attach to port/LAG/RIF/VLAN | saiacl.ACLGroupSeveralMembersTest |
|  acl.4 | Add ACL table group, v4 or v6 permit/deny/redirect ACL tables attach to port/LAG/RIF/VLAN and add Mirror ACLs | saiacl.MultAclTableGroupBindTest |
|  acl.5 | ACL field validation - src IP field | saiacl.SrcIpAclTest |
|  acl.6 | ACL field validation - dst IP field | saiacl.DstIpAclTest |
|  acl.7 | ACL field validation - src MAC field | saiacl.MACSrcAclTest |
|  acl.8 | ACL field validation - L4 dst port and L4 src port fields | saiacl.L3L4PortTest |
|  acl.9 | ACL field validation - TCP flag field | saiacl.TCPFlagsACLTest |
|  acl.10 | ACL field validation - IPv6 next header field | saiacl.IPv6NextHdrTest |
|  acl.11 | ACL field validation - IP fragmentation field | saiacl.IPAclFragmentTest |
|  acl.12 | ACL field validation - DSCP field | saiacl.IngressL3AclDscp |
|  acl.13 | ACL action validation - permit | saiacl.AclPreIngressTest, saiacl.IPv6NextHdrTest |
|  acl.14 | ACL action validation - deny | saiacl.AclGroupTest, saiacl.SrcIpAclTest, saiacl.DstIpAclTest, saiacl.MACSrcAclTest, saiacl.L3L4PortTest, saiacl.L3AclRangeTest, saiacl.ACLGroupSeveralMembersTest, saiacl.MultAclTableGroupBindTest, saiacl.SonicACLTest, saiacl.TCPFlagsACLTest, saiacl.AclTableTypeTest, saiacl.IPv6NextHdrTest, saiacl.IPAclFragmentTest, saiacl.L3AclCounterTest, saiacl.VlanAclTest, saiacl.IngressL3AclDscp |
|  acl.15 | ACL action validation - mirror | saiacl.AclTableTypeTest, saiacl.MultAclTableGroupBindTest |
|  acl.16 | ACL action validation - redirect to port and LAG | saiacl.AclRedirectPortAndLagTest |
|  acl.17 | Matching on ACL range | saiacl.L3AclRangeTest |
|  acl.18 | Test pre-ingress matching and VRF assignment | saiacl.AclPreIngressTest |
|  acl.19 | Verify ACL counters | saiacl.L3AclCounterTest |
|  acl.20 | Verify ACL with LAG | saiacl.AclLagTest |
|  acl.21 | Attach ACL table group to set of combination of bind points (Port+LAG, Port+RIF, LAG+RIF etc.) | saiacl.AclGroupTest |

## Bridge Port

| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| bridgeport.1 | Verify sample bridge port attributes getting and setting | saibridgeport.BridgePortAttributeTest |
| bridgeport.2 | Verify creation of bridge port of type Port | saibridgeport.BridgePortCreationTest.bpTypePortCreationTest |
| bridgeport.3 | Verify packet is dropped on port when no bridge_port is created on that port | saibridgeport.BridgePortCreationTest.noBpDropTest |
| bridgeport.4 | Verify FDB is being flushed on port if bridge port admin_state is being set to DOWN | saibridgeport.BridgePortStateTest.bpStateDownFlushTest |
| bridgeport.5 | Verify MAC address in not being learned when bridge port admin_state is DOWN | saibridgeport.BridgePortStateTest.bpStateDownNoLearnTest |
| bridgeport.6 | Verify MAC address in not being learned on port bridge port when FDB_LEARNING_MODE is disabled | saifdb.FdbNoLearnTest.bpPortNoLearnTest |
| bridgeport.7 | Verify MAC address in not being learned on LAG bridge port when FDB_LEARNING_MODE is disabled | saifdb.FdbNoLearnTest.bpLagNoLearnTest |
| bridgeport.8 | Verify MAC address in not being learned on a VLAN member if bridge port is not created on that port | saifdb.FdbNoLearnTest.noBpNoLearnTest |
| bridgeport.9 | Verify MAC address in not being learned on a VLAN member after bridge port is removed on that port | saifdb.FdbNoLearnTest.removedBpNoLearnTest |

## Buffer

| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| buffer.1 | Verify Buffer Pool Statistics GET SAI_BUFFER_POOL_STAT_CURR_OCCUPANCY_BYTES. | saibuffer.BufferStatistics |
| buffer.2 | Verify Buffer Pool Statistics GET SAI_BUFFER_POOL_STAT_WATERMARK_BYTES. | saibuffer.BufferStatistics |
| buffer.3 | Verify Buffer Pool Statistics CLEAR SAI_BUFFER_POOL_STAT_WATERMARK_BYTES. | saibuffer.BufferStatistics |
| buffer.4 | Verify Ingress Buffer Pool Creation Count = SAI_SWITCH_ATTR_INGRESS_BUFFER_POOL_NUM. Verify Pools creation with traffic and stats. | saibuffer.BufferPoolNumber |
| buffer.5 | Verify Egress Buffer Pool Creation Count = SAI_SWITCH_ATTR_EGRESS_BUFFER_POOL_NUM. Verify Pools creation with traffic and stats. | saibuffer.BufferPoolNumber |
| buffer.6 | Verify ingress_priority_group creation and corresponding gets for SAI_PORT_ATTR_NUMBER_OF_INGRESS_PRIORITY_GROUPS. | saiqosmap.QosTcToPriorityGroupTest |
| buffer.7 | Verify ingress_priority_group creation and corresponding gets for SAI_PORT_ATTR_INGRESS_PRIORITY_GROUP_LIST. | saiqosmap.QosTcToPriorityGroupTest |
| buffer.8 | Verify traffic forwarding for created ingress_priority_groups with no buffer profile. | saibuffer.Forwarding |
| buffer.9 | Verify traffic forwarding for created ingress_priority_groups with Buffer Profile 1. | saibuffer.Forwarding |
| buffer.10 | Verify traffic forwarding for created ingress_priority_groups with Buffer Profile 2 (Buffer profile update case). | saibuffer.Forwarding |
| buffer.11 | Verify traffic forwarding for created ingress_priority_groups again with no Buffer Profile. | saibuffer.Forwarding |
| buffer.12 | Verify ingress_priority_group stats GET SAI_INGRESS_PRIORITY_GROUP_STAT_PACKETS. | saibuffer.BufferStatistics |
| buffer.13 | Verify ingress_priority_group stats GET SAI_INGRESS_PRIORITY_GROUP_STAT_BYTES. | saibuffer.BufferStatistics |
| buffer.14 | Verify ingress_priority_group stats GET SAI_INGRESS_PRIORITY_GROUP_STAT_CURR_OCCUPANCY_BYTES. | saibuffer.BufferStatistics |
| buffer.15 | Verify ingress_priority_group stats GET SAI_INGRESS_PRIORITY_GROUP_STAT_WATERMARK_BYTES. | saibuffer.BufferStatistics |
| buffer.16 | Verify ingress_priority_group stats GET SAI_INGRESS_PRIORITY_GROUP_STAT_SHARED_CURR_OCCUPANCY_BYTES. | saibuffer.BufferStatistics |
| buffer.17 | Verify ingress_priority_group stats GET SAI_INGRESS_PRIORITY_GROUP_STAT_DROPPED_PACKETS. | saibuffer.BufferStatistics |
| buffer.18 | Verify ingress_priority_group stats CLEAR SAI_INGRESS_PRIORITY_GROUP_STAT_PACKETS. | saibuffer.BufferStatistics |
| buffer.19 | Verify ingress_priority_group stats CLEAR SAI_INGRESS_PRIORITY_GROUP_STAT_BYTES. | saibuffer.BufferStatistics |
| buffer.20 | Verify ingress_priority_group stats CLEAR SAI_INGRESS_PRIORITY_GROUP_STAT_WATERMARK_BYTES. | saibuffer.BufferStatistics |
| buffer.21 | Verify ingress_priority_group stats CLEAR SAI_INGRESS_PRIORITY_GROUP_STAT_DROPPED_PACKETS. | saibuffer.BufferStatistics |
| buffer.22 | For ingress PPG verify SAI_BUFFER_PROFILE_ATTR_RESERVED_BUFFER_SIZE with traffic. | saibuffer.BufferStatistics |
| buffer.23 | For ingress queue verify SAI_BUFFER_PROFILE_ATTR_RESERVED_BUFFER_SIZE with traffic. | saibuffer.BufferStatistics |
| buffer.24 | For egress PPG verify SAI_BUFFER_PROFILE_ATTR_RESERVED_BUFFER_SIZE with traffic. | saibuffer.BufferStatistics |
| buffer.25 | For egress queue verify SAI_BUFFER_PROFILE_ATTR_RESERVED_BUFFER_SIZE with traffic. | saibuffer.BufferStatistics |

## Debug Counter

| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| dbgc.1 |  Verify Port Debug Counter Remove drop reasons | saidebugcounters.PortDebugCounterRemoveDropReason |
| dbgc.2 |  Verify Debug Counter Remove drop reasons | saidebugcounters.SwitchDebugCounterRemoveDropReason |
| dbgc.3 |  Verify Port Debug Counter add drop reasons | saidebugcounters.PortDebugCounterAddDropReason |
| dbgc.4 |  Verify Switch Debug Counter add drop reasons | saidebugcounters.SwitchDebugCounterAddDropReason |
| dbgc.5 |  Verify Port Debug Counter for SMAC Multicast | saidebugcounters.PortDropMCSMAC |
| dbgc.6 |  Verify Switch Debug Counter for SMAC Multicast | saidebugcounters.SwitchDropMCSMAC |
| dbgc.7 |  Verify Port Debug Counter for L2 any drop reason | saidebugcounters.PortDropL2Any |
| dbgc.8 |  Verify Switch Debug Counter for L2 any drop reason | saidebugcounters.SwitchDropL2Any |
| dbgc.9 |  Verify Port Debug Counter for SMAC equals DMAC drop reason | saidebugcounters.PortDropSMACequalsDMAC |
| dbgc.10 |  Verify Switch Debug Counter for SMAC equals DMAC drop reason | saidebugcounters.SwitchDropSMACequalsDMAC |
| dbgc.11 |  Verify Port Debug Counter for Ingress VLAN filter drop reason | saidebugcounters.PortDropIngressVLANFilter |
| dbgc.12 |  Verify Switch Debug Counter for Ingress VLAN filter drop reason | saidebugcounters.SwitchDropIngressVLANFilter |
| dbgc.13 |  Verify Port Debug Counter for source IP Multicast drop reason | saidebugcounters.PortDropSIPMCTest |
| dbgc.14 |  Verify Switch Debug Counter for source IP Multicast drop reason | saidebugcounters.PortDropSIPMCTest |
| dbgc.15 |  Verify Port Debug Counter for TTL zero  drop reason | saidebugcounters.PortDropReasonTTLTest |
| dbgc.16 |  Verify Switch Debug Counter for TTL zero  drop reason | saidebugcounters.SwitchDropReasonTTLTest |
| dbgc.17 |  Verify Port Debug Counter for SIP Class E drop reason | saidebugcounters.PortDropSIPClassETest |
| dbgc.18 |  Verify Switch Debug Counter for SIP Class E drop reason | saidebugcounters.SwitchDropSIPClassETest |
| dbgc.19 |  Verify Port Debug Counter for unicast DIP with MC DMAC drop reason | saidebugcounters.PortDropUCDIPMCDMACTest |
| dbgc.20 |  Verify Port Debug Counter for IP header error drop reason | saidebugcounters.PortDropReasonIPHeaderErrorTest |
| dbgc.21 |  Verify Switch Debug Counter for IP header error drop reason | saidebugcounters.SwitchDropReasonIPHeaderErrorTest |
| dbgc.22 |  Verify Port multi Debug Counter | saidebugcounters.PortMultiDebugCounters |
| dbgc.23 |  Verify Switch multi Debug Counter | saidebugcounters.SwitchMultiDebugCounters |
| dbgc.24 |  Verify Port Debug Counter for DIP Link Local drop reason | saidebugcounters.PortDropDIPLinkLocalTest |
| dbgc.25 |  Verify Port Debug Counter for SIP Link Local drop reason | saidebugcounters.PortDropSIPLinkLocalTest |
| dbgc.26 |  Verify Port Debug Counter for SIP Unspecified drop reason | saidebugcounters.PortDropSIPUnspecifiedTest |
| dbgc.27 |  Verify Port Debug Counter for IPv4 disabled RIF drop reason | saidebugcounters.PortDropIPv4RIFDisabled |
| dbgc.28 |  Verify Port Debug Counter for IPv4 disabled RIF and L3 any drop reasons | saidebugcounters.PortDropIPv4L3AnyRIFDisabled |
| dbgc.29 |  Verify Port Debug Counter for IPv6 disabled RIF drop reason | saidebugcounters.PortDropIPv6RIFDisabled |
| dbgc.30 |  Verify Switch Debug Counter for DIP loopback drop reasons | saidebugcounters.SwitchDropDIPLoopback |
| dbgc.31 |  Verify Switch Debug Counter for SIP loopback drop reasons | saidebugcounters.SwitchDropSIPLoopback |
| dbgc.32 |  Verify Port Debug Counter for IPv4 route miss drop reasons | saidebugcounters.PortDropIPv4Miss |
| dbgc.33 |  Verify Port Debug Counter for IPv6 route miss drop reasons | saidebugcounters.PortDropIPv6Miss |
| dbgc.34 |  Verify Port Debug Counter for blackhole route drop reasons | saidebugcounters.PortDropBlackHoleRoute
| dbgc.35 |  Verify Port Debug Counter for L3 any drop reasons | saidebugcounters.PortDropL3AnyTest
| dbgc.36 |  Verify Port Debug Counter for ACL any drop reason | saidebugcounters.PortDropAclAnyTest
| dbgc.37 |  Verify SAI query Debug Counter enum values capabilities | saidebugcounters.GetDebugCounterEnumValuesCapabilities
| dbgc.38 |  Verify SAI query Debug Counter availability | saidebugcounters.GetDebugCounterAvailability

## FDB

| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| fdb.1 | Verify basic FDB attributes getting and setting | saifdb.FdbAttributeTest |
| fdb.2 | Verify MAC not learned when VLAN learning disabled on port | saifdb.FdbNoLearnTest.vlanPortNoLearnTest |
| fdb.3 | Verify MAC not learned when VLAN learning disabled on LAG | saifdb.FdbNoLearnTest.vlanLagNoLearnTest |
| fdb.4 | Verify static MAC creation | saifdb.FdbStaticMacTest |
| fdb.5 | Verify static MAC move from port->port | saifdb.FdbMacMoveTest.staticMacMoveTest |
| fdb.6 | Verify static MAC move from LAG->LAG | saifdb.FdbMacMoveTest.staticMacMoveTest |
| fdb.7 | Verify static MAC move from port->LAG | saifdb.FdbMacMoveTest.staticMacMoveTest |
| fdb.8 | Verify static MAC move from LAG->port | saifdb.FdbMacMoveTest.staticMacMoveTest |
| fdb.9 | Verify MAC learning on untagged port | saifdb.FdbLearnTest.dynamicMacLearnTest |
| fdb.10 | Verify MAC learning on tagged port | saifdb.FdbLearnTest.dynamicMacLearnTest |
| fdb.11 | Verify MAC learning on untagged LAG all members | saifdb.FdbLearnTest.dynamicMacLearnTest |
| fdb.12 | Verify MAC learning on tagged LAG all members | saifdb.FdbLearnTest.dynamicMacLearnTest |
| fdb.13 | Verify MAC learning on new VLAN member | saifdb.FdbLearnTest.dynamicMacLearnTest |
| fdb.14 | Verify MAC learning on new LAG member | saifdb.FdbLearnTest.dynamicMacLearnTest |
| fdb.15 | Verify dynamic MAC move from port -> port with static FDB entry previously installed | saifdb.FdbMacMoveTest.dynamicMacMoveTest |
| fdb.16 | Verify dynamic MAC move from LAG -> LAG with static FDB entry previously installed | saifdb.FdbMacMoveTest.dynamicMacMoveTest |
| fdb.17 | Verify dynamic MAC move from port -> LAG with static FDB entry previously installed | saifdb.FdbMacMoveTest.dynamicMacMoveTest |
| fdb.18 | Verify dynamic MAC move from LAG -> port with static FDB entry previously installed | saifdb.FdbMacMoveTest.dynamicMacMoveTest |
| fdb.19 | Verify MAC move from port -> port with FDB entry learned dynamically | saifdb.FdbMacMoveTest.dynamicMacMoveTest |
| fdb.20 | Verify MAC move from LAG -> LAG with FDB entry learned dynamically | saifdb.FdbMacMoveTest.dynamicMacMoveTest |
| fdb.21 | Verify MAC move from port -> LAG with FDB entry learned dynamically | saifdb.FdbMacMoveTest.dynamicMacMoveTest |
| fdb.22 | Verify MAC move from LAG -> port with FDB entry learned dynamically | saifdb.FdbMacMoveTest.dynamicMacMoveTest |
| fdb.23 | Verify MAC aging on port | saifdb.FdbAgeTest.macAgingOnPortTest |
| fdb.24 | Verify MAC aging on LAG | saifdb.FdbAgeTest.macAgingOnLagTest |
| fdb.25 | Verify MAC aging after moving | saifdb.FdbAgeTest.macAgingAfterMoveTest |
| fdb.26 | Verify MAC move after aging | saifdb.FdbAgeTest.macMoveAfterAgingTest |
| fdb.27 | Verify MAC not learned for any dropped packets (test for different L2 drops) | saifdb.FdbLearnTest.macLearnErrorTest |
| fdb.28 | Verify MAC not learned for invalid VLAN tagged packet | saifdb.FdbLearnTest.macLearnErrorTest |
| fdb.29 | Verify MAC not moved for learned MAC if static MAC already present | saifdb.FdbLearnTest.macLearnErrorTest |
| fdb.30 | Verify FDB attributes values after learn | saifdb.FdbEventTest.macLearnEventTest |
| fdb.31 | Verify FDB attributes values after age | saifdb.FdbEventTest.macAgeEvenTest |
| fdb.32 | Verify FDB attributes values after move | saifdb.FdbEventTest.macMoveEventTest |
| fdb.33 | Verify FDB attributes values after flush | saifdb.FdbEventTest.macFlushEventTest |
| fdb.34 | Verify FDB attributes values after delete | saifdb.FdbEventTest.macDeleteEventTest |
| fdb.35 | Verify FDB static MACs flush per VLAN | saifdb.FdbFlushTest.flushStaticPerVlanTest |
| fdb.36 | Verify FDB dynamic MACs flush per VLAN | saifdb.FdbFlushTest.flushDynamicPerVlanTest |
| fdb.37 | Verify FDB all MACs flush per VLAN | saifdb.FdbFlushTest.flushAllPerVlanTest |
| fdb.38 | Verify FDB static MACs flush per bridge port | saifdb.FdbFlushTest.flushStaticPerPortTest |
| fdb.39 | Verify FDB dynamic MACs flush per bridge port | saifdb.FdbFlushTest.flushDynamicPerPortTest	 |
| fdb.40 | Verify FDB all MACs flush per bridge port | saifdb.FdbFlushTest.flushAllPerPortTest |
| fdb.41 | Verify FDB static MACs flush per bridge LAG port | saifdb.FdbFlushTest.flushStaticPerLagTest |
| fdb.42 | Verify FDB dynamic MACs flush per bridge LAG port | saifdb.FdbFlushTest.flushDynamicPerLagTest |
| fdb.43 | Verify FDB all MACs flush per bridge LAG port | saifdb.FdbFlushTest.flushAllPerLagTest |
| fdb.44 | Verify FDB static MACs flush per VLAN/bridge port (for trunk port) | saifdb.FdbFlushTest.flushStaticPerVlanAndPortTest |
| fdb.45 | Verify FDB dynamic MACs flush per VLAN/bridge port (for trunk port) | saifdb.FdbFlushTest.flushDynamicPerVlanAndPortTest |
| fdb.46 | Verify FDB all MACs flush per VLAN/bridge port (for trunk port) | saifdb.FdbFlushTest.flushAllPerVlanAndPortTest |
| fdb.47 | Verify FDB flush for all static MACs | saifdb.FdbFlushTest.flushAllStaticTest |
| fdb.48 | Verify FDB flush for all dynamic MACs | saifdb.FdbFlushTest.flushAllDynamicTest |
| fdb.49 | Verify FDB flush for all static and dynamic MACs | saifdb.FdbFlushTest.flushAllMacsTest |
| fdb.50 | Verify MAC not learned after VLAN member removed on a port | saifdb.FdbLearnTest.macLearnErrorTest |
| fdb.51 | Verify MAC not learned after LAG member removed on a LAG | saifdb.FdbLearnTest.macLearnErrorTest |
| fdb.52 | Verify FDB miss - unicast with action drop | saifdb.FdbMissTest.unicastMissDropActionTest |
| fdb.53 | Verify FDB miss - unicast with action copy to CPU | saifdb.FdbMissTest.unicastMissCopyActionTest |
| fdb.54 | Verify FDB miss - unicast with action trap | saifdb.FdbMissTest.unicastMissTrapActionTest |
| fdb.55 | Verify FDB miss - multicast with action drop | saifdb.FdbMissTest.multicastDropActionTest |
| fdb.56 | Verify FDB miss - multicast with action copy to CPU | saifdb.FdbMissTest.multicastCopyActionTest |
| fdb.57 | Verify FDB miss - multicast with action trap | saifdb.FdbMissTest.multicastTrapActionTest |
| fdb.58 | Verify if LLDP packets are redirected to CPU regardless of multicast packet action | saifdb.FdbMissTest |
| fdb.59 | Verify FDB miss - broadcast with action drop | saifdb.FdbMissTest.broadcastDropActionTest |
| fdb.60 | Verify FDB miss - broadcast with action copy to CPU | saifdb.FdbMissTest.broadcastCopyActionTest |
| fdb.61 | Verify FDB miss - broadcast with action trap | saifdb.FdbMissTest.broadcastTrapActionTest |
| fdb.62 | Verify if ARP packets are redirected to CPU regardless of broadcast packet action | saifdb.FdbMissTest |
| fdb.63 | Verify frame dropped when destination MAC is equal to ingress port MAC | saifdb.FdbStaticMacTest.selfForwardingTest |
| fdb.64 | Verify packets forwarding on a port with static MAC entry installed | saifdb.FdbStaticMacTest.saifdb.FdbStaticMacTest |

## Hash

| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| hash.1 | L2 Lag basic test with varying seed values | saihash.L2LagHashSeedTest |
| hash.2 | L3 Lag basic test with varying seed values | saihash.L3LagIPv4HashSeedTest |
| hash.3 | IPv4 ECMP seed test with varying values | saihash.L3EcmpIPv4HashSeedTest |
| hash.4 | IPv6 ECMP seed test with varying values | saihash.L3EcmpIPv6HashSeedTest |
| hash.5 | Traffic distribution using src MAC selection for LAG hash | saihash.L2LagHashSrcMACTest |
| hash.6 | Traffic distribution using dst MAC selection for LAG hash | saihash.L2LagHashDstMACTest |
| hash.7 | L3 IPv4 traffic distribution using src port selection for LAG hash | saihash.L3LagIPv4SrcPortHashTest |
| hash.8 | L3 IPv4 traffic distribution using dst Port selection for IPv4 LAG hash | saihash.L3LagIPv4DstPortHashTest |
| hash.9 | L3 IPv4 traffic distribution using src IP selection for IPv4 LAG hash | saihash.L3LagIPv4SrcIPHashTest |
| hash.10 | L3 IPv4 traffic distribution using dst IP selection for IPv4 LAG hash | saihash.L3LagIPv4DstIPHashTest |
| hash.11 | L3 IPv4 traffic distribution using using all the field selected for IPv4 LAG hash | saihash.L3LagIPv4HashTest |
| hash.12 | Traffic distribution using src IP selection for ECMP IPv4 hash | saihash.EcmpIPv4SrcIPHashTest |
| hash.13 | Traffic distribution using dst IP selection for ECMP IPv4 hash | saihash.EcmpIPv4DstIPHashTest |
| hash.14 | Traffic distribution using using all the field selected for ECMP IPv4 hash | saihash.EcmpIPv4HashTest |
| hash.15 | Traffic distribution using src IP selection for ECMP IPv6 hash | saihash.EcmpIPv6SrcIPHashTest |
| hash.16 | Traffic distribution using dst IP selection for ECMP IPv6 hash | saihash.EcmpIPv6DstIPHashTest |
| hash.17 | Traffic distribution using all the field selected for ECMP IPv6 hash | saihash.EcmpIPv6HashTest |
| hash.18 | Traffic distribution using src MAC selection for Non-IP hash | saihash.NonIPSrcMacHashTest |
| hash.19 | Traffic distribution using dst MAC selection for Non-IP hash | saihash.NonIPDstMacHashTest |
| hash.20 | ECMP IPv4 hash defined for single IPv4 hash field with traffic with this field NOT being modified . No LB | saihash.EcmpIPv4BasicHashNoLBTest |
| hash.21 | Same as above but for LAG. No load balancing | all tests with hash_field config |
| hash.22 | IPv4 create/modify/remove ECMP hash fields | saihash.EcmpIPv4HashSaveRestoreTest |
| hash.23 | IPv6 create/modify/remove ECMP hash fields | saihash.EcmpIPv6HashSaveRestoreTest |
| hash.24 | Modify switch LAG hash fields | saihash.LagHashSaveRestoreTest |
| hash.25 | Modify switch LAG IPv4 hash fields | saihash.LagIPv4HashSaveRestoreTest |
| hash.26 | Modify switch LAG IPv6  hash fields | saihash.LagIPv6HashSaveRestoreTest |
| hash.27 | Verify IPv6 hash does not impact IPv4 hashing | saihash.EcmpIPv4vsIPv6HashTest |
| hash.28 | Verify IPv4 hash does not impact IPv6 hashing | saihash.EcmpIPv6vsIPv4HashTest |
| hash.29 | L3 IPv6 Lag basic test with varying seed values | saihash.L3LagIPv6HashSeedTest |
| hash.30 | L3 IPv6 traffic distribution using src port selection for LAG hash | saihash.L3LagIPv6SrcPortHashTest |
| hash.31 | L3 IPv6 traffic distribution using dst port selection for LAG hash | saihash.L3LagIPv6DstPortHashTest |
| hash.32 | L3 IPv6 traffic distribution using src IP selection for LAG hash | saihash.L3LagIPv6SrcIPHashTest |
| hash.33 | L3 IPv6 traffic distribution using dst IP selection for LAG hash | saihash.L3LagIPv6DstIPHashTest |
| hash.34 | L3 IPv6 traffic distribution using using all the field selected for LAG hash | saihash.L3LagIPv6HashTest |
| hash.35 | L3 IPv4 traffic distribution using dst port selection for ECMP hash | saihash.EcmpIPv4DstPortHashTest |
| hash.36 | L3 IPv4 traffic distribution using src port selection for ECMP hash | saihash.EcmpIPv4SrcPortHashTest |
| hash.37 | L3 IPv6 traffic distribution using dst port selection for ECMP hash | saihash.EcmpIPv6DstPortHashTest |
| hash.38 | L3 IPv6 traffic distribution using src port selection for ECMP hash | saihash.EcmpIPv6SrcPortHashTest |
| hash.39 | L2 Lag basic test with varying hash fields values | saihash.L2LagHashTest |
| hash.40 | L2 Lag hashing on Ether Type | saihash.L2LagHashEtherTypeTest |

## Hostif

| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| hostif.1 | Verify host interface creation and packet Rx for hostif type = netdev, host interface object type Port | saihostif.HostifCreationTest.portNetdevHostifCreationTest |
| hostif.2 | Verify host interface creation and packet Rx for hostif type = netdev, host interface object type LAG | saihostif.HostifCreationTest.lagNetdevHostifCreationTest |
| hostif.3 | Verify host interface creation and packet Rx for hostif type = netdev, host interface object type VLAN (L3 SVI) | saihostif.HostifCreationTest.vlanSviNetdevHostifCreationTest |
| hostif.4 | Verify hostif received packet VLAN tag is stripped when hostif VLAN tag type is STRIP for hostif type = netdev | saihostif.HosifTaggingTest/hostifStripTagTest |
| hostif.5 | Verify hostif received packet VLAN tag is not stripped when hostif VLAN tag type is KEEP for hostif type = netdev | saihostif.HosifTaggingTest/hostifKeepTagTest |
| hostif.6 | Verify hostif received packet VLAN tag is same as in original received packet when hostif VLAN tag type is ORIGINAL for hostif type = netdev | saihostif.HosifTaggingTest/hostifOriginalTagTest |
| hostif.7 | Verify hostif interface table match for entry type Wildcard and channel type = Callback with LLDP packet | saihostif.HostifTableMatchTest.wildcardEntryCbChannelLldp |
| hostif.8 | Verify hostif interface table match for entry type Wildcard and channel type = Callback with LACP packet | saihostif.HostifTableMatchTest.wildcardEntryCbChannelLacp |
| hostif.9 | Verify hostif interface table match for entry type Wildcard and channel type = Callback with STP packet | saihostif.HostifTableMatchTest.wildcardEntryCbChannelStp |
| hostif.10 | Verify hostif trap packet action DROP | saihostif.HostifTrapActionTest.dropTrapActionTest  |
| hostif.11 | Verify hostif trap packet action FORWARD | saihostif.HostifTrapActionTest.forwardTrapActionTest |
| hostif.12 | Verify hostif trap packet action COPY | saihostif.HostifTrapActionTest.copyTrapActionTest |
| hostif.13 | Verify hostif trap packet action TRAP | saihostif.HostifTrapActionTest.trapTrapActionTest |
| hostif.14 | Verify hostif trap packet action LOG | saihostif.HostifTrapActionTest.logTrapActionTest |
| hostif.15 | Verify hostif trap packet action DENY | saihostif.HostifTrapActionTest.denyTrapActionTest |
| hostif.16 | Verify hostif trap packet action TRANSIT | saihostif.HostifTrapActionTest.transitTrapActionTest |
| hostif.17 | Verify host interface Rx/Tx path wich ARP packet | saihostif.HostifTxTest.arpRxTxTest |
| hostif.18 | Verify hostif Tx via host interface type netdev, objec type port | saihostif.HostifTxTest.portHostifTxTest |
| hostif.19 | Verify hostif Tx via host interface type netdev, objec type LAG | saihostif.HostifTxTest.lagHostifTxTest |
| hostif.20 | Verify hostif trap type TTL error | saihostif.HostifTrapTypesTest.ttlErrorTrapTest |
| hostif.21 | Verify hostif trap type ARP request | saihostif.HostifTrapTypesTest.arpTrapTest |
| hostif.22 | Verify hostif trap type ARP response | saihostif.HostifTrapTypesTest.arpTrapTest |
| hostif.23 | Verify hostif trap type BGP | saihostif.HostifTrapTypesTest.bgpTrapTest |
| hostif.24 | Verify hostif trap type DHCP | saihostif.HostifTrapTypesTest.dhcpTrapTest |
| hostif.25 | Verify hostif trap type IP2ME | saihostif.HostifTrapTypesTest.ip2meTrapTest |
| hostif.26 | Verify hostif trap type LACP | saihostif.HostifTrapTypesTest.lacpTrapTest |
| hostif.27 | Verify hostif trap type LLDP | saihostif.HostifTrapTypesTest.lldpTrapTest |
| hostif.28 | Verify hostif trap type OSPF | saihostif.HostifTrapTypesTest.ospfTrapTest |
| hostif.29 | Verify hostif trap type IGMP | saihostif.HostifTrapTypesTest.igmpTrapTest |
| hostif.30 | Verify hostif trap type STP | saihostif.HostifTrapTypesTest.stpTrapTest |
| hostif.31 | Verify hostif trap type PIM | saihostif.HostifTrapTypesTest.pimTrapTest |
| hostif.32 | Verify hostif trap type UDLD | saihostif.HostifTrapTypesTest.udldTrapTest |
| hostif.33 | Verify hostif trap type IPv6 neighbor discovery | saihostif.HostifTrapTypesTest.icmpV6TrapTest |
| hostif.34 | Verify hostif trap type BGPV6 | saihostif.HostifTrapTypesTest.bgpTrapTest |
| hostif.35 | Verify hostif trap type BFD_RX | saihostif.HostifTrapTypesTest.bfdRxTrapTest |
| hostif.36 | Verify hostif trap type PTP | saihostif.HostifTrapTypesTest.ptpTrapTest |
| hostif.37 | Verify hostif trap type MPLS router alert | saihostif.HostifTrapTypesTest.mplsRouterAlertTrapTest |
| hostif.38 | Verify hostif trap type MPLS TTL error | saihostif.HostifTrapTypesTest.mplsTtlErrorTrapTest |
| hostif.39 | Verify getting of trap priority hostif attribute | saihostif.HostifTrapAttributeGetterTest.trapPriorityTest |
| hostif.40 | Verify getting of trap type hostif attribute | saihostif.HostifTrapAttributeGetterTest.trapTypeTest |
| hostif.41 | Verify getting of trap group hostif attribute | saihostif.HostifTrapAttributeGetterTest.trapGroupTest |
| hostif.42 | Verify getting of trap action hostif attribute | saihostif.HostifTrapAttributeGetterTest.trapActionTest |
| hostif.43 | Verify hostif user defined trap - the traffic matching ACL rule associated with user defined trap | saihostif.HostifUserDefinedTrapTest.aclIpSrcNetdevTrapTest |
| hostif.43 | Verify hostif priority handling | saihostif.HostifTrapActionTest.hostifPriorityTest |

## Isolation Group

| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| isolationgroup.1 | Verify isolation group CRUD operations | saiisolationgroup.PortIsolationTest.attributeTest |
| isolationgroup.2 | Verify forwarding between ports with isolation groups attached | saiisolationgroup.PortIsolationTest.forwardingTest |

## Lag

| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| lag.1 | Verify basic load balancing of L2 traffic | sailag.LAGL2LoadBalancing |
| lag.2 | Verify basic load balancing of L3 traffic | sailag.LAGL3LoadBalancing |
| lag.3 | Verify load balancing after add/delete LAG member | sailag.LAGL3LoadBalancing.l3LoadBalancingRemovedMembersTest |
| lag.4 | Verify load balancing after activate/deactivate LAG member | sailag.LAGL3LoadBalancing.l3LoadBalancingDisableMembersTest |
| lag.5 | Verify no traffic if all members inactive | sailag.LAGDisableEgressLagMember |
| lag.6 | Verify traffic with 1 active LAG member after disabling other members | sailag.LAGDisableEgressLagMember |
| lag.7 | Verify LAG ingress/egress when LAG is tagged on multiple VLAN | sailag.LAGDisableEgressLagMember.multipleVlanTest |
| lag.8 | Verify PV miss when LAG is tagged on multiple VLAN | sailag.LAGDisableEgressLagMember.multipleVlanTest |
| lag.9 | Verify LAG member list using SAI_LAG_ATTR_PORT_LIST | sailag.LAGAttrPortList |
| lag.10 | Verify no flooding on deactivated LAG member | sailag.LAGDisableEgressLagMember.lagMemberActivateFloodTest |
| lag.11 | Verify no bridging on deactivated LAG member | sailag.LAGDisableEgressLagMember.lagMemberActivateBridgeTest |
| lag.12 | Verify no egress traffic on LAG member when egress_disable set | sailag.LAGDisableEgressLagMember |
| lag.13 | Verify no ingress traffic on LAG member when ingress_disable set | sailag.LAGDisableIngressLagMember |
| lag.14 | Create/Remove Lag/Lag member | sailag.LAGCreateLagMember |
| lag.15 | L2 verify Load Balancing with LAG HASH modifications | saihash.L2LagHashTest, saihash.L2LagHashEtherTypeTest, saihash.L2LagHashSrcMACTest, saihash.L2LagHashDstMACTest |

## Mirror

| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| mirror.1 | Verify port ingress local mirror, monitor = Port. | saimirror.localIngressPortMirroringTest |
| mirror.2 | Verify port egress local mirror, monitor = Port. | saimirror.localEgressPortMirroringTest |
| mirror.3 | Verify port ingress local mirror, monitor = LAG. Start with empty LAG, verify no mirrored packets. | saimirror.localIngressLagMirroringTest |
| mirror.4 | Verify port ingress local mirror, monitor = LAG. Add two ports to LAG, verify mirrored packet. | saimirror.localIngressLagMirroringTest |
| mirror.5 | Verify port ingress local mirror, monitor = LAG. Remove the port on which previous mirrored packet received, verify packet mirrored to other LAG port. | saimirror.localIngressLagMirroringTest |
| mirror.6 | Verify port ingress local mirror, monitor = LAG. Remove the last LAG member, verify no mirrored packet. | saimirror.localIngressLagMirroringTest |
| mirror.7 | Verify port ingress local mirror, monitor = LAG. Update monitor LAG1 -> LAG2, verify mirrored packet. | saimirror.localIngressLagMirroringTest |
| mirror.8 | Verify port ingress local mirror, monitor = LAG. Update monitor LAG2 -> Port, verify mirrored packet. | saimirror.localIngressLagMirroringTest |
| mirror.9 | Verify port ingress local mirror, monitor = LAG. Update monitor Port -> LAG1 , verify mirrored packet. | saimirror.localIngressLagMirroringTest |
| mirror.10 | Verify port egress local mirror, monitor = LAG. Start with empty LAG, verify no mirrored packets. | saimirror.localEgressLagMirroringTest |
| mirror.11 | Verify port egress local mirror, monitor = LAG. Add two ports to LAG, verify mirrored packet. | saimirror.localEgressLagMirroringTest |
| mirror.12 | Verify port egress local mirror, monitor = LAG. Remove the port on which previous mirrored packet received, verify packet mirrored to other LAG port. | saimirror.localEgressLagMirroringTest |
| mirror.13 | Verify port egress local mirror, monitor = LAG. Remove the last LAG member, verify no mirrored packet. | saimirror.localEgressLagMirroringTest |
| mirror.14 | Verify port egress local mirror, monitor = LAG. Update monitor LAG1 -> LAG2, verify mirrored packet. | saimirror.localEgressLagMirroringTest |
| mirror.15 | Verify port egress local mirror, monitor = LAG. Update monitor LAG2 -> Port, verify mirrored packet. | saimirror.localEgressLagMirroringTest |
| mirror.16 | Verify port egress local mirror, monitor = LAG. Update monitor Port -> LAG1 , verify mirrored packet. | saimirror.localEgressLagMirroringTest |
| mirror.17 | Verify same mirror session applied together: port ingress and port egress. | saimirror.ingressEgressMirrorSessionTest |
| mirror.18 | Verify packet with destination egress port p2, dropped in ingress port p1 is not mirrored by egress mirror. | saimirror.egressMirrorDropOnIngress |
| mirror.19 | Verify packet dropped in ingress is mirrored with ingress mirror. | saimirror.mirrorDroppedPacketIngressTest |
| mirror.20 | Verify packet dropped in egress is mirrored with egress mirror. | saimirror.mirrorDroppedPacketEgressTest |
| mirror.21 | Verify span mirror session TC (queue ID). | saimirror.mirrorSessionTrafficClassTest |
| mirror.22 | Verify span ingress mirror meter. | saimirror.ingressMirrorPolicingTest |
| mirror.23 | Verify ACL ingress local mirror, monitor = LAG. | saimirror.localAclIngressLagMirroringTest |
| mirror.24 | Verify ACL Egress Local Mirror, monitor = LAG. | saimirror.localAclEgressLagMirroringTest |
| mirror.25 | Verify same mirror session applied together to: ACL ingress, ACL egress. | saimirror.aclIngressEgressMirrorSessionTest |
| mirror.26 | Verify packet dropped with ingress ACL is mirrored with ingress ACL mirror | saimirror.aclMirrorDroppedPacketIngressTest |
| mirror.27 | Verify packet dropped with egress ACL is mirrored with egress ACL mirror | saimirror.aclMirrorDroppedPacketEgressTest |
| mirror.28 | Verify egress ACL ERSPAN, GRE protocol type = 0x22eb. | saimirror.erspanAclEgressGreProto0x22ebTest |
| mirror.29 | Verify ERSPAN, monitor = Port. | saimirror.erspanPortMirroringTest |
| mirror.30 | Verify ERSPAN, monitor = LAG. | saimirror.erspanLagMirroringTest |
| mirror.31 | Verify ERSPAN mirror session TC (queue ID). | saimirror.erspanMirrorSessionTrafficClassTest |
| mirror.32 | Verify ERSPAN ingress mirror meter. | saimirror.erspanIngressMirrorPolicingTest |
| mirror.33 | Verify ERSPAN egress mirror meter. | saimirror.erspanEgressMirrorPolicingTest |
| mirror.34 | Verify ERSPAN TPID create and set. | saimirror.erspanPortMirroringTest |
| mirror.35 | Verify ERSPAN VLAN ID create and set. | saimirror.erspanVlanPortMirroringTest |
| mirror.36 | Verify ERSPAN VLAN priority create and set. | saimirror.erspanPortMirroringTest |
| mirror.37 | Verify ERSPAN CFI create and set. | saimirror.erspanVlanPortMirroringTest |
| mirror.38 | Verify ERSPAN mirror session source IP v4 create and set. | saimirror.erspanPortMirroringTest |
| mirror.39 | Verify ERSPAN mirror session destination IP v4 create and set. | saimirror.erspanPortMirroringTest |
| mirror.40 | Verify ERSPAN mirror session source MAC create and set. | saimirror.erspanPortMirroringTest |
| mirror.41 | Verify ERSPAN mirror session destination MAC create and set. | saimirror.erspanPortMirroringTest |

## MPLS

| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| mpls.1 | Verify hostif trap type MPLS_TTL_ERROR | saihostif.HostifTrapTypesTest.mplsTtlErrorTest |
| mpls.2 | Verify MPLS packet is trapped to CPU when packet_action is set to SAI_PACKET_ACTION_TRAP with SAI_IN_DROP_REASON_MPLS_MISS drop reason | saimpls.MplsDropTrapTest.mplsImplicitNullLabelTrapDrop |
| mpls.3 | Verify hostif trap type MPLS_ROUTER_ALERT | saihostif.HostifTrapTypesTest.mplsRouterAlertTest |
| mpls.4 | Verify if packet with unknown label is dropped and associated debug counter is hit | saimpls.MplsDropTrapTest.mplsLabelLookupMissTest |
| mpls.5 | Verify MPLS labels are added to packet in ingress LER for IPv4 | saimpls.MplsIpv4Test.mplsIngressLERTest |
| mpls.6 | Verify MPLS label is popped in Egress LER and packet is forwarded based in IP lookup for IPv4 | saimpls.MplsIpv4Test.mplsEgressLERTermTest |
| mpls.7 | Verify MPLS label is popped in Egress LER and packet is forwarded based on IP lookup after changing VRF on MPLS RIF for IPv4 | saimpls.MplsIpv4Test.mplsEgressLERTermUpdateMplsRifVrfTest |
| mpls.8 | Verify MPLS null label is popped in Egress LER and packet is forwarded based on IP lookup for IPv4 | saimpls.MplsIpv4Test.mplsEgressLERNullTermTest |
| mpls.9 | Verify PHP pops label and forwards packet based on IP lookup for IPv4 | saimpls.MplsIpv4Test.mplsEgressPhpTest |
| mpls.10 | Verify PHP swaps label with explicit null and forwards packet based on IP lookup for IPv4 | saimpls.MplsIpv4Test.mplsEgressPhpSwapNullTest |
| mpls.11 | Verify MPLS label is swapped with another label, explicit null, swap only top label in transit switch for IPv4 | saimpls.MplsIpv4Test.mplsTransitSwapTest |
| mpls.12 | Verify ECMP group with MPLS transit nexthops for IPv4 | saimpls.MplsIpv4Test.mplsTransitSwapEcmpHashTest |
| mpls.13 | Verify MPLS label is pushed on stash in transit LSR for IPv4 | saimpls.MplsIpv4Test.mplsTransitPushTest |
| mpls.14 | Verify MPLS labels are added to packet in ingress LER for IPv6 | saimpls.MplsIpv6Test.mplsIngressLERTest |
| mpls.15 | Verify MPLS label is popped in Egress LER and packet is forwarded based in IP lookup for IPv6 | saimpls.MplsIpv6Test.mplsEgressLERTermTest |
| mpls.16 | Verify MPLS null label is popped in Egress LER and packet is forwarded based on IP lookup for IPv6 | saimpls.MplsIpv6Test.mplsEgressLERNullTermTest |
| mpls.17 | Verify PHP pops label and forwards packet based on IP lookup for IPv6 | saimpls.MplsIpv6Test.mplsEgressPhpTest |
| mpls.18 | Verify PHP swaps label with explicit null and forwards packet based on IP lookup for IPv6 | saimpls.MplsIpv6Test.mplsEgressPhpSwapNullTest |
| mpls.19 | Verify MPLS label is swapped with another label, explicit null, swap only top label in transit switch for IPv6 | saimpls.MplsIpv6Test.mplsTransitSwapTest |
| mpls.20 | Verify ECMP group with MPLS transit nexthops for IPv6 | saimpls.MplsIpv6Test.mplsTransitSwapEcmpHashTest |
| mpls.21 | Verify MPLS label is pushed on stash in transit LSR for IPv6 | saimpls.MplsIpv6Test.mplsTransitPushTest |

## NAT

| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| nat.1 | Verify that source NAT translation doesn't happen if ACL is configures to disable translation | sainat.NatTranslationTest.srcNatAclTranslationDisableTest |
| nat.2 | Verify that destination NAT translation doesn't happen if ACL is configures to disable translation | sainat.NatTranslationTest.dstNatAclTranslationDisableTest |
| nat.3 | Configure basic source NAT entry and check translation | sainat.NatTest.srcNatTest |
| nat.4 | Configure basic destination NAT entry and check translation | sainat.NatTest.dstNatTest |
| nat.5 | Configure NAT Zones on RIF and verify if SNAT happens for packets from Zone 0 and DNAT happens for packets from Zone 1 | sainat.NatTest |
| nat.6 | Verify SNAT miss packet is forwarded to CPU. | sainat.NatTest.natTrapTest |
| nat.7 | Query and validate Hit Bit of a NAT entry | sainat.NatTest |
| nat.8 | Clear and verify Hit Bit of a NAT entry | sainat.NatTest |
| nat.9 | Query and verify NAT entry statistic | sainat.NatTest |
| nat.10 | Clear and verify NAT entry statistics | sainat.NatTest |

## Neighbor

| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| neighbor.1 | Verify packet DMAC updated with neighbor dst MAC for dst IP after routing for L3 RIF | sairif.L3InterfaceTest |
| neighbor.2 | Verify packet DMAC updated with neighbor dst MAC for dst IP after routing for SVI | sairif.L3SVITest |
| neighbor.3 | Verify packet DMAC is correct if neighbor created after nexthop for L3 RIF | sairif.L3InterfaceTest |
| neighbor.4 | Verify packet DMAC is correct if neighbor created before nexthop for L3 RIF | sairif.L3InterfaceTest |
| neighbor.5 | Verify packet DMAC is correct if neighbor created after nexthop SVI, MAC already learned | sairif.L3SVITest |
| neighbor.6 | Verify packet DMAC is correct if neighbor created before nexthop SVI, MAC already learned | sairif.L3SVITest |
| neighbor.7 | Verify egress port for MAC learned after neighbor created for SVI | sairif.L3SVITest.sviRouteDynamicMacTest |
| neighbor.8 | Verify egress port for MAC learned before neighbor created for SVI | sairif.L3SVITest.sviRouteDynamicMacTest |
| neighbor.9 | Verify correct DMAC is set in packet after neighbor dst MAC is updated | sairif.L3SVITest.sviIpv4ArpMoveTest |
| neighbor.10 | Verify egress port after MAC move | sairif.L3SVITest.sviRouteDynamicMacMoveTest |
| neighbor.11 | Verify packet flooded on egress VLAN when neighbor present but no MAC learned | sairif.L3SVITest.sviIPv4HostPortRoutedFloodTest |
| neighbor.12 | Verify host route is not created when no_host_route=True for IPv4 neighbor | saineighbor.noHostRouteIpv4NeighborTest |
| neighbor.13 | Verify host route is created when no_host_route=False for IPv4 neighbor | saineighbor.addHostRouteIpv4NeighborTest |
| neighbor.14 | Verify host route is not created when no_host_route=True for IPv6 neighbor | saineighbor.NeighborAttrTest.noHostRouteIpv6NeighborTest |
| neighbor.15 | Verify host route is created when no_host_route=False for IPv6 neighbor | saineighbor.NeighborAttrTest.addHostRouteIpv6NeighborTest |
| neighbor.16 | Verify host route is not created for IPv6 link local address irrespectively of no_host_route value | saineighbor.NeighborAttrTest.noHostRouteIpv6LinkLocalNeighborTest |

## Nexthop

| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| nexthop.1 | Verify traffic with route pointing to nexthop with RIF as SAI_ROUTER_INTERFACE_TYPE_PORT and nexthop type as SAI_NEXT_HOP_TYPE_IP | sairif.L3InterfaceTest |
| nexthop.2 | Verify traffic with route pointing to nexthop with RIF as SAI_ROUTER_INTERFACE_TYPE_VLAN and nexthop type as SAI_NEXT_HOP_TYPE_IP | sairif.L3SVITest |
| nexthop.3 | Verify traffic with route pointing to nexthop with RIF as SAI_ROUTER_INTERFACE_TYPE_SUB_PORT and nexthop type as SAI_NEXT_HOP_TYPE_IP | sairif.L3SubPortTest |
| nexthop.4 | Repeat nexthop.1 to nexthop.3 tests for nexthop type as SAI_NEXT_HOP_TYPE_TUNNEL_ENCAP | sainhop.NhopTunnelEncapDecapTest |
| nexthop.5 | Verify traffic after overriding tunnel VNI of tunnel mapper using nexthop attribute SAI_NEXT_HOP_ATTR_TUNNEL_VNI | sainhop.NhopTunnelVniTest |
| nexthop.6 | Add multiple routes pointing to nexthop and delete the nexthop, check for failures | sainhop.L3NexthopTest.removeNexthopTest |
| nexthop.7 | Verify traffic for nexthop with LAG RIF when LAG members are deleted, packets rehash and forwarded | sailag.LagL3Nhop |
| nexthop.8 | Verify traffic for nexthop with LAG RIF when LAG members are added, packets rehash and forwarded | sailag.LagL3Nhop |
| nexthop.9 | Verify no traffic for v4 route pointing to nexthop when RIF V4 is disabled | sairif.L3InterfaceTest.ipv4DisableTest |
| nexthop.10 | Verify no traffic for v6 route pointing to nexthop when RIF V6 is disabled | sairif.ipv6DisabledTest |
| nexthop.11 | Verify correct traffic with host route pointing to CPU interface as SAI_ROUTE_ENTRY_ATTR_NEXT_HOP_ID | sainhop.L3NexthopTest.cpuNexthopTest |
| nexthop.12 | Verify correct traffic with LPM route pointing to CPU interface as SAI_ROUTE_ENTRY_ATTR_NEXT_HOP_ID | sainhop.L3NexthopTest.cpuNexthopTest |
| nexthop.13 | Verify nexthop.11 and nexthop.12 for both V4 and V6 routes | sainhop.L3NexthopTest.cpuNexthopTest |
| nexthop.14 | Route pointing to nexthop with neighbor deleted, post route flood | sairif.L3SVITest.sviIv4HostPortRoutedFloodTest, sairif.L3SVITest.sviIpv6HostPortRoutedFloodTest |
| nexthop.15 | Verify traffic for SVI nexthop with static MAC move to different interface | sairif.L3SVITest.sviIpv4HostStaticMacMoveTest, sairif.L3SVITest.sviipv6HostStaticMacMoveTest |
| nexthop.16 | Verify traffic for SVI nexthop with dynamic MAC move to different interface | sairif.L3SVITest.sviIpv4RouteDynamicMacMoveTest, sairif.L3SVITest.sviIpv6DynamicMacMoveTest |
| nexthop.17 | Verify traffic for nexthop resolution with ARP move | sairif.L3SVITest.sviIpv4ArpMoveTest, sairif.L3SVITest.sviIpv6IcmpMoveTest |
| nexthop.18 | Verify traffic when nexthop with static MAC moves to LAG | sairif.L3SVITest.sviIpv4LagHostStaticMacMoveTest, sairif.L3SVITest.sviIpv6LagHostStaticMacMoveTest |
| nexthop.19 | Verify traffic when nexthop with dynamic MAC moves to LAG | sairif.L3SVITest.sviIpv4LagHostDynamicMacMoveTest, sairif.L3SVITest.sviIpv6LagHostDynamicMacMoveTest |
| nexthop.20 | Verify traffic for nexthop to RIF with MTU less than packet size | sairif.L3nterface.ipv4MtuTest, sairif.L3Interface.ipv6MtuTest |
| nexthop.21 | Verify traffic for nexthop to LAG RIF with MTU less than packet size | sairif.L3Interface.ipv4MtuTest, sairif.L3Interface.ipv6MtuTest |
| nexthop.22 | Verify nexthop.14 to nexthop.21 for both V4 and V6 routes | nexthop.14 - nexthop.21 test names |

## Nexthop Group

| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| nexthopgrp.1 | Verify IPv4 ECMP with all members with RIF as port | sainexthopgroup.L3IPv4EcmpHost.l3IPv4EcmpHostTest |
| nexthopgrp.2 | Verify IPv6 ECMP with all members with RIF as port | sainexthopgroup.L3IPv6EcmpHost.l3IPv6EcmpHostTest |
| nexthopgrp.3 | Verify IPv4 ECMP with all members with RIF as LAG | sainexthopgroup.L3IPv4EcmpLagTest.l3IPv4EcmpHostTwoLagsTest |
| nexthopgrp.4 | Verify IPv6 ECMP with all members with RIF as LAG | sainexthopgroup.L3IPv6EcmpLagTest.l3IPv6EcmpHostTwoLagsTest |
| nexthopgrp.5 | Verify IPv4 ECMP with members combination of port and LAG RIFs | sainexthopgroup.L3IPv4EcmpLagTest.L3IPv4EcmpHostPortLag |
| nexthopgrp.6 | Verify IPv6 ECMP with members combination of port and LAG RIFs | sainexthopgroup.L3IPv6EcmpLagTest.L3IPv6EcmpHostPortLag |
| nexthopgrp.7 | Verify IPv4 ECMP with SVI RIF as member | sainexthopgroup.L3IPv4SVIEcmpTest.l3IPv4EcmpSVIHostTest |
| nexthopgrp.8 | Verify IPv6 ECMP with SVI RIF as member | sainexthopgroup.L3IPv6SVIEcmpTest.l3IPv6EcmpSVIHostTest |
| nexthopgrp.9 | Verify IPv4 ECMP with Port, LAG and SVI RIFs as nexthop members | sainexthopgroup.L3IPv4SVIEcmpTest.L3IPv4EcmpSVILagHostTest |
| nexthopgrp.10 | Verify IPv6 ECMP with Port, LAG and SVI RIFs as nexthop members | sainexthopgroup.L3IPv6SVIEcmpTest.L3IPv6EcmpSVIPortLagHostTest |
| nexthopgrp.11 | Verify IPv4 ECMP rebalance with adding a new nexthop member | sainexthopgroup.L3IPv4EcmpLpmTest.L3Ipv4EcmpLpmAddRemoveNhopTest |
| nexthopgrp.12 | Verify IPv6 ECMP rebalance with adding a new nexthop member | sainexthopgroup.L3IPv6EcmpLagTest.L3Ipv6EcmpAddRemoveNhopTest |
| nexthopgrp.13 | Verify IPv4 ECMP rebalance with removal of a nexthop member | sainexthopgroup.L3IPv4EcmpLpmTest.L3Ipv4EcmpLpmAddRemoveNhopTest |
| nexthopgrp.14 | Verify IPv6 ECMP rebalance with removal of a nexthop member | sainexthopgroup.L3IPv6EcmpLagTest.L3Ipv6EcmpAddRemoveNhopTest |
| nexthopgrp.15 | Verify IPv4 ECMP with LAG RIF and some LAG member in disable state. Disable LAG members using SAI_LAG_MEMBER_ATTR_EGRESS_DISABLE | sainexthopgroup.L3IPv4EcmpLagTest.L3IPv4EcmpHostTwoLagsDisabledLagMembersTest |
| nexthopgrp.16 | Verify IPv6 ECMP with LAG RIF and some LAG member in disable state. Disable LAG members using SAI_LAG_MEMBER_ATTR_EGRESS_DISABLE | sainexthopgroup.L3IPv6EcmpLagTest.l3IPv6EcmpHostTwoLagsDisabledLagMembersTest |
| nexthopgrp.17 | Verify IPv4 ECMP load balance to check fair share on all members | sainexthopgroup.L3IPv4EcmpLagTest.L3IPv4EcmpHostPortLag |
| nexthopgrp.18 | Verify IPv6 ECMP load balance to check fair share on all members | sainexthopgroup.L3IPv6EcmpLagTest.L3IPv6EcmpHostPortLag |
| nexthopgrp.19 | Verify Multiple IPv4 ECMP with shared nexthop members | sainexthopgroup.L3IPv4EcmpLagTest.l3IPv4EcmpHostPortLagSharedMembersTest |
| nexthopgrp.20 | Verify Multiple IPv6 ECMP with shared nexthop members | sainexthopgroup.L3IPv6EcmpLagTest.l3IPv6EcmpHostPortLagSharedMembersTest |
| nexthopgrp.21 | Create hash object with all 5-tuple and set to switch using SAI_SWITCH_ATTR_ECMP_HASH_IPV4 attribute | sainexthopgroup.L3IPv4EcmpLagTest.l3IPv4EcmpHashPortLagTest |
| nexthopgrp.22 | Create hash object with all 5-tuple and set to switch using SAI_SWITCH_ATTR_ECMP_HASH_IPV6 attribute | sainexthopgroup.L3IPv6EcmpLagTest.l3IPv6EcmpHashPortLagTest |
| nexthopgrp.23 | IPv4: Create hash object with one in 5-tuple and send multiple packets. Validate that there is no load balancing. Validate with various fields in ECMP hash. | sainexthopgroup.L3IPv4EcmpLagTest.l3IPv4EcmpHashPortLagTest |
| nexthopgrp.24 | IPv6: Create hash object with one in 5-tuple and send multiple packets. Validate that there is no load balancing. Validate with various fields in ECMP hash. | sainexthopgroup.L3IPv6EcmpLagTest.l3IPv6EcmpHashPortLagTest |
| nexthopgrp.25 | Verify ECMP seed attribute changing using SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_SEED and check for rebalancing | sainexthopgroup.L3IPv4EcmpLagTest.l3IPv4EcmpHashSeedPortLag |
| nexthopgrp.26 | Verify ECMP tunnel nexthop | saitunnel.TunnelNhopResolutionTestIpv4Underlay.underlayEcmpTunnelTest, saitunnel.TunnelNhopResolutionTestIpv6Underlay.underlayEcmpTunnelTest |
| nexthopgrp.27 | Verify SAI switch ECMP attributes - SAI_SWITCH_ATTR_NUMBER_OF_ECMP_GROUPS, SAI_SWITCH_ATTR_AVAILABLE_NEXT_HOP_GROUP_MEMBER_ENTRY, SAI_SWITCH_ATTR_AVAILABLE_NEXT_HOP_GROUP_ENTRY | sainexthopgroup.L3IPv4EcmpHost.l3SaiNhgSetGetTest |
| nexthopgrp.28 | Validate get and set attributes of both nexthop_group and nexthop_group_member objects. | sainexthopgroup.L3IPv4EcmpHost.l3SaiNhgSetGetTest |

## Policer

| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| policer.1 | Verify policer creation for various meter types. | saipolicer.policerCreate |
| policer.2 | Verify policer creation for various supported color sources. | saipolicer.policerCreate |
| policer.3 | Verify policer creation for various modes. | saipolicer.policerCreate |
| policer.4 | Verify policer bind to hostif_trap_group with existing policer. | saipolicer.policerOverwriteTrapGroup |
| policer.5 | Verify policer bind to hostif_trap_group with no existing policer. | saipolicer.noPolicerTrapGroup |
| policer.6 | Verify policer can be bound to >1 hostif_trap_group. | saipolicer.Overflow1Policer2TrapGroups, saipolicer.Underflow1Policer2TrapGroups |
| policer.7 | Verify policer is being applied for all hostif_trap object if bound to >1 hostif_trap_group. | saipolicer.Overflow1Policer2TrapGroups, saipolicer.Underflow1Policer2TrapGroups |
| policer.8 | Verify policer unbind from one hostif_trap_group does not affect other. | saipolicer.Overflow1Policer2TrapGroups, saipolicer.Underflow1Policer2TrapGroups |
| policer.9 | Verify policer bind to ACL entry. | saipolicer.BindPolicerToAclEntry |
| policer.10 | Verify policer bind to ingress mirror session object. | saimirror.ingressMirrorPolicingTest |
| policer.11 | Verify policer bind to egress mirror session object. | saimirror.egressMirrorPolicingTest |
| policer.12 | Verify policer bind to port. | saipolicer.BindPolicerToPort |
| policer.13 | Verify policer works for SAI_PORT_ATTR_BROADCAST_STORM_CONTROL_POLICER_ID. | saipolicer.StormControlTests |
| policer.14 | Verify policer works for SAI_PORT_ATTR_MULTICAST_STORM_CONTROL_POLICER_ID. | saipolicer.StormControlTests |
| policer.15 | Verify policer works for SAI_PORT_ATTR_FLOOD_STORM_CONTROL_POLICER_ID. | saipolicer.StormControlTests |
| policer.16 | Verify counters per color. | saipolicer.VerifyColors |

## Port
| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| port.1 | Test switch attribute port list | saiport.PortAttributeTest.switchAttributePortListTest |
| port.2 | Verify the creation of sample packet | saiport.PortAttributeTest.portAttributeIngressSamplePacket |
| port.3 | Verify the creation of buffer profile list | saiport.PortAttributeTest.portBufferProfileList |
| port.4 | Test port attributes | saiport.PortAttributeTest.portAttributeTest |
| port.5 | Test list of port attributes | saiport.ListPortAttributesTest |
| port.6 | Test port QoS attributes | saiport.PortQOSAttributeTest |
| port.7 | Test port QoS DSCP to TC map attribute | saiport.PortQosMapAttributeTest.portQosDscpToTcMapAttributeTest |
| port.8 | Test port QoS dot1p to color map attribute | saiport.PortQosMapAttributeTest.portQosDot1pToColorMapAttributeTest |
| port.9 | Test port QoS dot1p to TC map attribute | saiport.PortQosMapAttributeTest.portQosDot1pToTcMapAttributeTest |
| port.10 | Test port QoS DSCP to color map attribute | saiport.PortQosMapAttributeTest.portQosDscpToColorMapAttributeTest |
| port.11 | Test port QoS TC to queue map attribute | saiport.PortQosMapAttributeTest.portQosTcToQueueMapAttributeTest
| port.12 | Test port QoS TC to priority group map attribute | saiport.PortQosMapAttributeTest.portQosTcToPriorityGroupMapAttributeTest
| port.13 | Test port QoS TC and color to DSCP map attribute | saiport.PortQosMapAttributeTest.portQosTcAndColorToDscpMapAttributeTest
| port.14 | Test port QoS TC and color to dot1p map attribute | saiport.PortQosMapAttributeTest.portQosTcAndColorToDot1pMapAttributeTest
| port.15 | Test port QoS PFC priority to queue map attribute | saiport.PortQosMapAttributeTest.portQosPfcPriorityToQueueMapAttributeTest
| port.16 | Test port QoS PFC priority to priority group map attribute | saiport.PortQosMapAttributeTest.portQosPfcPriorityToPriorityGroupMapAttributeTest
| port.17 | Test port FEC mode attribute | saiport.PortFecModeAttributeTest |
| port.18 | Test port speed attribute | saiport.PortSpeedAttributeTest|
| port.19 | Test auto negation attribute | saiport.PortAutoNegAttributeTest |
| port.20 | Test port PortEgressMirrorSessionTest attributes | saiport.PortEgressMirrorSessionTest |
| port.21 | Test port PortIngressMirrorSessionTest attributes | saiport.PortIngressMirrorSessionTest |
| port.22 | Test single port ingress ACL table binding | saiport.SinglePortIngressAclTableBindingTest |
| port.23 | Test single port ingress ACL group binding | saiport.SinglePortIngressAclGroupBindingTest |
| port.24 | Test port ingress ACL table binding | saiport.PortIngressAclTableBindingTest |
| port.25 | Test port ingress ACL table add remove binding | saiport.PortIngressAclTableAddRemoveBindingTest |
| port.26 | Test port ingress ACL group binding | saiport.PortIngressAclGroupBindingTest |
| port.27 | Test port ingress ACL group add remove binding | saiport.PortIngressAclGroupAddRemoveBindingTest |
| port.28 | Test single port egress ACL table binding | saiport.SinglePortEgressAclTableBindingTest |
| port.29 | Test single port egress ACL group binding | saiport.SinglePortEgressAclGroupBindingTest |
| port.30 | Test port egress ACL table binding | saiport.PortEgressAclTableBindingTest |
| port.31 | Test port egress ACL table add remove binding | saiport.PortEgressAclTableAddRemoveBindingTest |
| port.32 | Test port egress ACL group binding | saiport.PortEgressAclGroupBindingTest |
| port.33 | Test port egress ACL group add remove binding | saiport.PortEgressAclGroupAddRemoveBindingTest |

## QoS Map
| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| qosmap.1 | Test L3 IPv4 DSCP to TC default mapping for ingress port | saiqosmap.L3QosDscpToTcTest.l3IPv4QosMapDscpToTcDefaultMappingTest |
| qosmap.2 | Test L3 IPv6 DSCP to TC default mapping ingress port | saiqosmap.L3QosDscpToTcTest.l3IPv6QosMapDscpToTcDefaultMappingTest |
| qosmap.3 | Test L3 IPv4 multiple DSCP to single TC mapping for ingress port | saiqosmap.L3QosDscpToTcTest.l3IPv4QosMapMultipleDscpToSingleTcMappingTest |
| qosmap.4 | Test L3 IPv6 multiple DSCP to single TC mapping for ingress port | saiqosmap.L3QosDscpToTcTest.l3IPv6QosMapMultipleDscpToSingleTcMappingTest |
| qosmap.5 | Test L3 IPv4 one DSCP to one TC mapping for ingress ports | saiqosmap.L3QosDscpToTcTest.l3IPv4QosMapOneDscpToOneTcMappingTest |
| qosmap.6 | Test L3 IPv6 one DSCP to single TC mapping for ingress port | saiqosmap.L3QosDscpToTcTest.l3IPv6QosMapOneDscpToOneTcMappingTest |
| qosmap.7 | Test L3 IPv4 same DSCP to TC mapping for various ingress ports | saiqosmap.L3QosDscpToTcTest.l3IPv4QosSameDscpToTcMappingManyPortsTest |
| qosmap.8 | Test L3 IPv6 same DSCP to TC mapping for various ingress port | saiqosmap.L3QosDscpToTcTest.l3IPv6QosSameDscpToTcMappingManyPortsTest |
| qosmap.9 | Test L3 IPv4 DSCP to TC mapping for various ingress ports | saiqosmap.L3QosDscpToTcTest.l3IPv4QosVariousDscpToTcMappingManyPortsTest |
| qosmap.10 | Test L3 IPv6 various DSCP to TC mapping for various ingress port | saiqosmap.L3QosDscpToTcTest.l3IPv6QosVariousDscpToTcMappingManyPortsTest |
| qosmap.11 | Test L3 IPv4 DSCP to color default mapping for ingress port | saiqosmap.QosDscpToColorTest.l3IPv4QosMapDscpToColorDefaultMappingTest |
| qosmap.12 | Test L3 IPv6 DSCP to color default mapping for ingress port | saiqosmap.QosDscpToColorTest.l3IPv6QosMapDscpToColorDefaultMappingTest |
| qosmap.13 | Test L3 IPv4 many DSCP to same color mapping for ingress port | saiqosmap.QosDscpToColorTest.l3IPv4QosMapManyDscpToSameColorTest |
| qosmap.14 | Test L3 IPv6 many DSCP to same color mapping for ingress port | saiqosmap.QosDscpToColorTest.l3IPv6QosMapManyDscpToSameColorTest |
| qosmap.15 | Test L3 IPv4 many DSCP to color mapping for various ingress port | saiqosmap.QosDscpToColorTest.l3IPv4QosMapSameDscpToColorManyIngressPortsTest |
| qosmap.16 | Test L3 IPv6 same DSCP to color mapping for various ingress port | saiqosmap.QosDscpToColorTest.l3IPv6QosMapSameDscpToColorManyIngressPortsTest |
| qosmap.17 | Test L3 IPv4 various DSCP to color mapping for various ingress port | saiqosmap.QosDscpToColorTest.l3IPv4QosMapVariousDscpToColorManyIngressPortsTest |
| qosmap.18 | Test L3 IPv6 various DSCP to color mapping for various ingress port | saiqosmap.QosDscpToColorTest.l3IPv6QosMapVariousDscpToColorManyIngressPortsTest |
| qosmap.19 | Test default no PFC to Priority Group mapping | saiqosmap.QosTcToPriorityGroupTestPFC |
| qosmap.20 | Test multiple PFC to the same Priority Group mapping | saiqosmap.QosTcToPriorityGroupTestPFC |
| qosmap.21 | Test one to one PFC to Priority Group mapping | saiqosmap.QosTcToPriorityGroupTestPFC |
| qosmap.22 | Test the same PFC to Priority Group mapping on many ingress ports | saiqosmap.QosTcToPriorityGroupTestPFC |
| qosmap.23 | Test various PFC to Priority Group mapping on many ingress ports | saiqosmap.QosTcToPriorityGroupTestPFC |
| qosmap.24 | Test no PFC to Priority Group mapping | saiqosmap.QosTcToPriorityGroupTestPFC |
| qosmap.25 | Test default no TC to Priority Group mapping | saiqosmap.QosTcToPriorityGroupTestTC |
| qosmap.26 | Test multiple TC to the same Priority Group mapping | saiqosmap.QosTcToPriorityGroupTestTC |
| qosmap.27 | Test one to one TC to Priority Group mapping | saiqosmap.QosTcToPriorityGroupTestTC |
| qosmap.28 | Test the same TC to Priority Group mapping on different ingress ports | saiqosmap.QosTcToPriorityGroupTestTC |
| qosmap.29 | Test various TC to Priority Group mapping on different ingress ports | saiqosmap.QosTcToPriorityGroupTestTC |
| qosmap.30 | Test no PFC to Priority Group mapping | saiqosmap.QosTcToPriorityGroupTestTC |
| qosmap.31 | Verify no DSCP overriding of original packet in default L3 IPv4 no TC + color to DSCP mapping | saiqosmap.QosTcAndColorToDscpTest.l3IPv4QosMapTcColorToDscpDefaultMappingTest |
| qosmap.32 | Verify no DSCP overriding of original packet in default L3 IPv6 no TC + color to DSCP mapping | saiqosmap.QosTcAndColorToDscpTest.l3IPv6QosMapTcColorToDscpDefaultMappingTest |
| qosmap.33 | Verify no DSCP overriding of original packet in QoS L3 IPv4 TC + color to DSCP mapping | saiqosmap.QosTcAndColorToDscpTest.l3IPv4QosMapTcColorToDscpMappingTest |
| qosmap.34 | Verify no DSCP overriding of original packet in QoS L3 IPv6 TC + color to DSCP mapping | saiqosmap.QosTcAndColorToDscpTest.l3IPv6QosMapTcColorToDscpMappingTest |
| qosmap.35 | Verify no DSCP overriding of original packet in QoS L3 IPv4 TC + color to DSCP mapping defined on various egress ports | saiqosmap.QosTcAndColorToDscpTest.l3IPv4QosMapTcColorToDscpManyIngressPortsTest |
| qosmap.36 | Verify no DSCP overriding of original packet in QoS L3 IPv6 TC + color to DSCP mapping defined on various egress ports | saiqosmap.QosTcAndColorToDscpTest.l3IPv6QosMapTcColorToDscpManyIngressPortsTest |
| qosmap.37 | Verify no DSCP overriding of original packet in QoS L3 IPv4 TC + color to DSCP mapping defined on various ingress ports | saiqosmap.QosTcAndColorToDscpTest.l3IPv4QosMapVariousTcColorToDscpManyIngressPortsTest |
| qosmap.38 | Verify no DSCP overriding of original packet in QoS L3 IPv6 TC + color to DSCP mapping defined on various ingress ports | saiqosmap.QosTcAndColorToDscpTest.l3IPv6QosMapVariousTcColorToDscpManyIngressPortsTest |
| qosmap.39 | Verify default PCP to one color mapping on L2 traffic | saiqosmap.L2QosMapPcpToColorTest.l2QosMapPCPToColorDefaultMappingTest |
| qosmap.40 | Verify many PCP to one color mapping on L2 traffic | saiqosmap.L2QosMapPcpToColorTest.l2QosMapMultiPCPToOneColorMappingTest |
| qosmap.41 | Verify same PCP to one color mapping on L2 traffic | saiqosmap.L2QosMapPcpToColorTest.l2QosMapSamePCPToColorMultiIngresssPortTest |
| qosmap.42 | Verify many PCP to one color mapping on L2 traffic on various ingress ports | saiqosmap.L2QosMapPcpToColorTest.l2QosMapDifferentPCPToColorMultiIngresssPortTest |
| qosmap.43 | Verify default L2 PCP to TC mapping | saiqosmap.L2QosMapPcpToTcTest.l2QosMapPCPToTcDefaultMappingTest |
| qosmap.44 | Verify L2 traffic multiple PCP to TC mapping | saiqosmap.L2QosMapPcpToTcTest.l2QosMapMultiplePCPToOneTcMappingTest |
| qosmap.45 | Verify L2 traffic one to one PCP to TC mapping | saiqosmap.L2QosMapPcpToTcTest.l2QosMapOneToOnePCPToTcMappingTest |
| qosmap.46 | Verify L2 traffic same PCP to many TC mapping for many ingress ports | saiqosmap.L2QosMapPcpToTcTest.l2QosMapSamePcpToTcManyIngressPortsTest |
| qosmap.47 | Verify L2 traffic multiple PCP to various TC mapping for many ingress ports | saiqosmap.L2QosMapPcpToTcTest.l2QosMapVariousPcpToTcManyIngressPortsTest |
| qosmap.48 | Verify creation and update of the QoS TC to queue map object | saiqosmap.QosMapCreateModifyTest.qosMapTcToQueueModifyTest |
| qosmap.49 | Verify creation and update of the QoS TC + color to DSCP map object | saiqosmap.QosMapCreateModifyTest.qosMapTcColorToDscpCreateModifyTest |
| qosmap.50 | Verify creation and update of the QoS DSCP to TC map object | saiqosmap.QosMapCreateModifyTest.qosMapDscpToTcCreateModifyTest |
| qosmap.51 | Verify creation and update of the QoS DSCP to color map object | saiqosmap.QosMapCreateModifyTest.qosMapDscpToColorCreateModifyTest |
| qosmap.52 | Verify creation and update of the QoS priority to queue map object | saiqosmap.QosMapCreateModifyTest.qosMapPrioToQueueModifyTest |
| qosmap.53 | Verify creation and update of the QoS dot1p to TC map object | saiqosmap.QosMapCreateModifyTest.qosMapDot1ToTcCreateModifyTest |
| qosmap.54 | Verify creation and update of the QoS dot1p to color map object | saiqosmap.QosMapCreateModifyTest.qosMapDot1ToColorCreateModifyTest |
| qosmap.55 | Verify creation and update of the QoS TC + color to dot1p map object | saiqosmap.QosMapCreateModifyTest.qosMapTcColorToDot1pCreateModifyTest |
| qosmap.56 | Verify creation and update of the QoS PFC priority to Priority Group map object | saiqosmap.QosMapCreateModifyTest.qosMapPfcPrioToPrioGroupCreateModifyTest |
| qosmap.57 | Verify creation and update of the QoS TC to Priority Group map object | saiqosmap.QosMapCreateModifyTest.qosMapTcPgCreateModifyTest |

## Queue

| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| queue.1 | Query queue handles for the port. Query for SAI_QUEUE_ATTR_PORT and SAI_QUEUE_ATTR_INDEX attributes and validate. | saiqueue.portQueueQueryTest |
| queue.2 | Configure DSCP-->TC and TC-->Queue map table. Modify the queue index in the map table to reflect the correct queue. | saiqosmap.L3IPv4QosMapMultipleDscpToSingleTcMappingTest |
| queue.3 | Configure PCP-->TC and TC-->Queue map table. Modify the queue index in the map table to reflect the correct queue. | saiqosmap.L2QosMapOneToOnePCPToTcMappingTest |
| queue.4 | Configure PFC priority to Queue map table. Modify the queue index in the map table to reflect the correct queue. | saiqueue.pfcPriorityQueueTest |
| queue.5 | Query SAI_QUEUE_ATTR_PARENT_SCHEDULER_NODE attribute. | saiqueue.portQueueQueryTest |
| queue.6 | Assign queue WRED profile and validate WRED functionality. | saiqueue.wredQueueTest |
| queue.7 | Assign queue buffer profile and validate the buffer profile parameters. | saiqueue.bufferQueueTest |
| queue.8 | Remove queue buffer profile and the queue should be assigned to the default buffer profile. | saiqueue.bufferQueueTest |
| queue.9 | Attach Scheduler profile to the queue and validate Queue priority, Weight, min/max rate. | saiqueue.schedulerQueueTest |
| queue.10 | Modify the attached scheduler profile parameters and validate those parameters. | saiqueue.schedulerQueueTest |
| queue.11 | Try above tests on CPU port queue object. | saiqueue.cpuPortQueueObjectTest |
| queue.12 | Verify a queue creation. | saiqueue.queueCreateTest |

## Route

| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| route.1 | Verify IPv4 host route | sairif.L3Interface.ipv4FibTest |
| route.2 | Verify IPv4 LPM route | sairif.L3Interface.ipv4FibLPMTest |
| route.3 | Verify IPv6 host route | sairif.L3Interface.ipv6FibTest |
| route.4 | Verify IPv6 LPM route | sairif.L3Interface.ipv6FibLPMTest |
| route.5 | Verify multiple routes to the same nexthop | sairoute.L3RouteTest.multipleRoutesTest |
| route.6 | Verify CPU route for IP2ME addresses | saihostif.HostifTrapTypesTest |
| route.7 | Verify drop route | sairoute.L3RouteTest.dropRouteTest |
| route.8 | Verify route nexthop update to different nexthop | sairoute.L3RouteTest.routeUpdateTest |
| route.9 | Verify route update from CPU to regular nexthop | sairoute.L3RouteTest.routeUpdateTest |
| route.10 | Verify route update from regular nexthop to CPU | sairoute.L3RouteTest.routeUpdateTest |
| route.11 | Verify route update from drop to regular nexthop | sairoute.L3RouteTest.routeUpdateTest |
| route.12 | Verify route update from regular to nexthop drop | sairoute.L3RouteTest.routeUpdateTest |
| route.13 | Verify route from ingress L3 intf VRF to RIF on different VRF | saivrf.VrfForwarding.innerVrfFwdL3NhopTest |
| route.14 | Verify route from ingress SVI interface VRF to RIF on different VRF | saivrf.VrfForwarding.innerVrfFwdSviNhopTest |
| route.15 | Verify route to ECMP | sairif.L3SubPortTest.subPortECMPTest |
| route.16 | Verify packet dropped when route to ingress RIF | sairoute.L3RouteTest.routeIngressRifTest |
| route.17 | Verify drop when route to empty ECMP group | sairoute.L3RouteTest.emptyECMPGroupTest |
| route.18 | Verify if packet is gleaned to CPU when nexthop id is RIF for case without a neighbor | sairoute.L3RouteTest.routeNbrColision |
| route.19 | Verify packet forwarded to RIF when nexthop id is RIF and RIF has a neighbor | sairoute.L3RouteTest.routeNbrColision |
| route.20 | Verify packet forwarded to CPU when nexthop id is CPU port ID | sairoute.L3RouteTest.cpuForwardTest |
| route.21 | Verify packet is routed to SVI, flooded if no neighbor found | sairoute.L3RouteTest.sviNeighborTest |
| route.22 | Verify packet is routed to SVI, forwarded to correct neighbor | sairoute.L3RouteTest.sviNeighborTest |
| route.23 | Verify routing between VRFs | saivrf.VrfForwardingTest.interVrfFwdL3NhopTest |
| route.24 | Verify routing between VRFs with SVI | saivrf.VrfForwardingTest.interVrfFwdSviNhopTest |
| route.25 | Verify direct broadcast routing | sairif.L3DirBcastRouteTest |

## RIF

| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| | L3 Interface | |
| rif.1 | Verify IPv4 packets are dropped when admin_v4_state is False | sairif.L3Interface.ipv4DisableTest |
| rif.2 | Verify IPv6 packets are dropped when admin_v6_state is False | sairif.L3Interface.ipv6DisableTest |
| rif.3 | Verify packet forwarded correctly after MAC address update | sairif.L3Interface.macUpdateTest |
| rif.4 | Verify packet dropped for old MAC address after update | sairif.L3Interface.macUpdateTest |
| rif.5 | Verify basic forwarding for IPv4 host | sairif.L3Interface.ipv4FibTest |
| rif.6 | Verify basic forwarding for IPv6 host | sairif.L3Interface.ipv6FibTest |
| rif.7 | Verify basic forwarding for IPv4 LPM | sairif.L3Interface.ipv4FibLPMTest |
| rif.8 | Verify basic forwarding for IPv6 LPM | sairif.L3Interface.ipv6FibLPMTest |
| rif.9 | Verify IPv4 packet forwarded with packet size less than MTU | sairif.L3Interface.ipv4MtuTest |
| rif.10 | Verify IPv4 packet forwarded with packet size equal to MTU | sairif.L3Interface.ipv4MtuTest |
| rif.11 | Verify IPv4 packet dropped with packet size greater than MTU | sairif.L3Interface.ipv4MtuTest |
| rif.12 | Verify IPv4 packet with packet size greater than MTU punted to CPU if trap present | sairif.L3MtuTrapTest.ipv4MtuTest |
| rif.13 | Verify IPv6 packet forwarded with packet size less than MTU | sairif.L3Interface.ipv6MtuTest |
| rif.14 | Verify IPV6 packet forwarded with packet size equal to MTU | sairif.L3Interface.ipv6MtuTest |
| rif.15 | Verify IPv6 packet dropped with packet size greater than MTU | sairif.L3Interface.ipv6MtuTest |
| rif.16 | Verify IPv6 packet with packet size greater than MTU punted to CPU if trap present | sairif.L3MtuTrapTest.ipv6MtuTest |
| rif.17 | Verify packet forward after MTU change for IPv4 | sairif.L3Interface.ipv4MtuTest |
| rif.18 | Verify packet forward after MTU change for IPv6 | sairif.L3Interface.ipv6MtuTest |
| rif.19 | Verify same MTU value shared between RIF | sairif.L3Interface.rifSharedMtuTest |
| rif.20 | Verify MTU check works after deleting another RIF with the same MTU value | sairif.L3Interface.rifSharedMtuTest |
| rif.21 | Verify MTU value works after adding another RIF with the same MTU value | sairif.L3Interface.rifSharedMtuTest |
| rif.22 | Verify basic forwarding on RIF using LAG IPv4 | sairif.L3Interface.ipv4FibLagTest |
| rif.23 | Verify basic forwarding on RIF using LAG with new LAG member IPv4 | sairif.L3Interface.ipv4FibLagTest |
| rif.24 | Verify packet dropped on ingress port after being removed from LAG IPv4 | sairif.L3Interface.ipv4FibLagTest |
| rif.25 | Verify basic forwarding on RIF using LAG IPv6 | sairif.L3Interface.ipv6FibLagTest |
| rif.26 | Verify basic forwarding on RIF using LAG with new LAG member IPv6 | sairif.L3Interface.ipv6FibLagTest |
| rif.27 | Verify packet dropped on ingress port after being removed from LAG IPv6 | sairif.L3Interface.ipv6FibLagTest |
| rif.28 | Verify create fails when TYPE is PORT and port_id is 0 | sairif.L3Interface.negativeRifTest |
| rif.29 | Verify ingress ACL table bind to RIF | sairif.L3Interface.ipv4IngressAclTest |
| rif.30 | Verify egress ACL table bind to RIF | sairif.L3Interface.ipv4EgressAclTest |
| rif.31 | Verify ingress ACL group bind to RIF | sairif.L3Interface.ipv4IngressAclTest |
| rif.32 | Verify egress ACL group bind to RIF | sairif.L3Interface.ipv6EgressAclTest |
| rif.33 | Verify IPv4 multicast packets are dropped when V4_MCAST_ENABLE is False | sairif.L3Interface.mcastDisableTest |
| rif.34 | Verify IPv6 multicast packets are dropped when V6_MCAST_ENABLE is False | sairif.L3Interface.mcastDisableTest |
| rif.35 | Verify multiple loopback RIF on same VRF is allowed | sairif.L3Interface.loopbackRifTest |
| rif.36 | Verify Ingress RIF stats for unicast packets | sairif.L3Interface.rifStatsTest |
| rif.37 | Verify Ingress RIF stats for multicast packets | sairif.L3Interface.rifStatsTest |
| rif.38 | Verify Egress RIF stats for unicast packets | sairif.L3Interdface.rifStatsTest |
| rif.39 | Verify Egress RIF stats for multicast packets | sairif.L3Interface.rifStatsTest |
| rif.40 | Verify MYIP works for subnet routes | sairif.L3Interface.rifMyIPTest |
| rif.41 | Verify duplicate L3 RIF creation fails | sairif.L3Interface.duplicatePortRifCreationTest |
| rif.42 | Verify RIF can be created or updated with custom RMAC | sarif.L3Interface.rifCreateOrUpdateRmacTest |
| | Sub-port Interface | |
| rif.43 | Verify packet routed with valid VLAN on sub-port | sairif.L3SubPortTest.rifToSubPortTest |
| rif.44 | Verify packet dropped when invalid VLAN tag on port | sairif.L3SubPortTest.pvMissTest |
| rif.45 | Verify up to 256 sub-ports can be created per port or LAG | sairif.L3SubPortTest.subPortNoTest |
| rif.46 | Verify multiple sub-ports with same VLAN ID and different ports | sairif.L3SubPortTest.setUp |
| rif.47 | Verify packet not flooded on tagged VLAN when no route hit | sairif.L3SubPortTest.noFloodTest |
| rif.48 | Verify routing between sub-ports | sairif.L3SubPortTest.subPortToSubPortTest |
| rif.49 | Verify routing between sub-ports on the same physical port or LAG | sairif.L3SubPortTest.subPortToSubPortTest |
| rif.50 | Verify routing between SVI and sub-port | sairif.L3SubPortTest.subPortToRifTest |
| rif.51 | Verify routing between L3 RIF and sub-port | sairif.L3SubPortTest.rifToSubPortTest |
| rif.52 | Verify load-balancing when sub-port is part of ECMP | sairif.L3SubPortTest.subPortECMPTest |
| rif.53 | Verify tunnel encap-decap over sub-port| sairif.TunnelL3SubPortTest.subPortTunnelTest |
| rif.54 | Verify IP2ME is working for sub-port | sairif.L3SubPortTest.subPortMyIPTest |
| rif.55 | Verify admin status DOWN disabled packet forwarding on ingress | sairif.L3SubPortTest.subPortAdminV*StatusTest |
| rif.56 | Verify hostif creation with CPU Rx/Tx | saihostif multiple tests |
| rif.57 | Create a VLAN RIF and sub-port RIF with the same VLAN number and make sure two separate RIFs are created. Now delete VLAN and make sure sub-port RIF is not impacted. Repeat but this time delete the sub-port RIF.| sairif.L3SubPortTest.vlanConflictTest |
| rif.58 | Verify ingress ACL table is bound correctly to sub-port | sairif.L3SubPortTest.subPortIngressAclTest |
| rif.59 | Verify egress ACL table is bound correctly to sub-port | sairif.L3SubPortTest.subPortEgressAclTest |
| rif.60 | Verify ingress ACL group is bound correctly to sub-port | sairif.L3SubPortTest.subPortIngressAclTest |
| rif.61 | Verify egress ACL group is bound correctly to sub-port | sairif.L3SubPortTest.subPortEgressAclTest |
| rif.62 | Verify QoS group setting inherited from parent port or LAG | sairif.L3SubPortTest.subPortQosGroupTest |
| rif.63 | Verify IPv4 packets are dropped when admin_v4_state is False | sairif.L3SubPortTest.subPortAdminV4StatusTest |
| rif.64 | Verify IPv6 packets are dropped when admin_v6_state is False| sairif.L3SubPortTest.subPortAdminV6StatusTest |
| rif.65 | Verify IPv4 packet forwarded with packet size less than MTU | sairif.L3SubPortTest.subPotyV4MtuTest |
| rif.66 | Verify IPv4 packet forwarded with packet size equal to MTU | sairif.L3SubPortTest.subPortV4MtuTest |
| rif.67 | Verify IPv4 packet dropped with packet size greater than MTU | sairif.L3SubPortTest.subPortV4MtuTest |
| rif.68 | Verify IPv4 packet with packet size greater than MTU punted to CPU if trap present | sairif.L3MtuTrapTest.subPortIpv4MtuTrapTest |
| rif.69 | Verify IPv6 packet forwarded with packet size less than MTU | sairif.L3SubPortTest.subPortV6MtuTest |
| rif.70 | Verify IPv6 packet forwarded with packet size equal to MTU | sairif.L3SubPortTest.subPortV6MtuTest |
| rif.71 | Verify IPv6 packet dropped with packet size greater than MTU | sairif.L3SubPortTest.subPortV6MtuTest |
| rif.72 | Verify IPv6 packet with packet size greater than MTU punted to CPU if trap present | sairif.L3MtuTrapTest.subPortIpv6MtuTrapTest |
| rif.73 | Verify packet forwarded after MTU change for IPv4 | sairif.L3SubPortTest.subPortV4MtuTest |
| rif.74 | Verify packet forwarded after MTU change for IPv6 | sairif.L3SubPortTest.subPortV6MtuTest |
| rif.75 | Verify ingress sub-port stats for unicast packets | sairif.L3SubPortTest.subPortStatsTest |
| rif.76 | Verify egress sub-port stats for unicast packets | sairif.L3SubPortTest.subPortStatsTest |
| rif.77 | Verify MYIP works for subnet routes | sairif.L3SubPortTest.subPortMyIPTest |
| | VLAN Interface | |
| rif.78 | Verify IPv4 packets are dropped when admin_v4_state is False | sairif.L3SviTest.sviRifIpv4DisableTest |
| rif.79 | Verify IPv6 packets are dropped when admin_v6_state is False | sairif.L3SviTest.sviRifIpv6DisableTest |
| rif.80 | Verify L2 bridging on SVI port for RMAC miss | sairif.L3SviTest.sviBridgingTest |
| rif.81 | Verify L2 flooding on SVI for broadcast packet| sairif.L3SviTest.sviBridgingTest |
| rif.82 | Verify routing between SVIs | sairif.L3SviTest.sviToSviRoutingTest |
| rif.83 | Verify routing after nexthop resolved via static MAC entry | sairif.L3SviTest.sviHostTest.sviHostTest |
| rif.84 | Verify routing after nexthop resolved via static MAC entry on tagged member | sairif.L3SviTest.sviHostVlanTaggingTest |
| rif.85 | Verify port routed flood when static MAC entry is missing | sairif.L3SviTest.svIPv4HostPostRoutedFloodTest, sairif.L3SviTest.sviIPv6HostPostRoutedFloodTest |
| rif.86 | Verify routing after nexthop resolved via static MAC move | sairif.L3SviTest.sviIPv4HostStaticMacMoveTest, sairif.L3SviTest.sviIPv6HostStaticMacMoveTest |
| rif.87 | Verify routing after nexthop resolved via dynamically learned MAC entry | sairif.L3SviTest.sviRouteDynamicMacTest |
| rif.88 | Verify routing after nexthop resolved when dynamically learned MAC entry is moved | sairif.L3SviTest.sviRouteDynamicMacMoveTest |
| rif.89 | Verify routing after nexthop resolved via dynamically after neighbor is moved | sairif.L3SviTest.sviIPv4ArpMoveTest, sairif.L3SviTest.sviIPv6IcmpMoveTest |
| rif.90 | Verify routing after nexthop resolved via static MAC entry on LAG | sairif.L3SviTest.sviLagHostTest |
| rif.91 | Verify routing after nexthop resolved via static MAC move for LAG | sairif.L3SviTest.sviIPv4LagHostStaticMacMoveTest, sairif.L3SviTest.sviIPv6LagHostStaticMacMoveTest |
| rif.92 | Verify routing after nexthop resolved via dynamically learned MAC entry for LAG | sairif.L3SviTest.sviLagHostDynamicMacTest |
| rif.93 | Verify routing after nexthop resolved when dynamically learned MAC entry is moved for LAG| sairif.L3SviTest.sviIPv4LagHostDynamicMacMoveTest, sairif.L3SviTest.sviIPv6LagHostDynamicMacMoveTest |
| rif.94 | Verify IPv4 packet forwarded with packet size less than MTU | sairif.L3SviTest.sviIPv4MtuTest |
| rif.95 | Verify IPv4 packet forwarded with packet size equal to MTU | sairif.L3SviTest.sviIPv4MtuTest |
| rif.96 | Verify IPv4 packet dropped with packet size greater than MTU | sairif.L3SviTest.sviIPv4MtuTest |
| rif.97 | Verify IPv4 packet with packet size greater than MTU punted to CPU if trap present| sairif.L3MtuTrapTest.sviIpv4MtuTrapTest |
| rif.98 | Verify IPv6 packet forwarded with packet size less than MTU | sairif.L3SviTest.sviIPv6MtuTest |
| rif.99 | Verify IPv6 packet forwarded with packet size equal to MTU | sairif.L3SviTest.sviIPv6MtuTest |
| rif.100 | Verify IPv6 packet dropped with packet size greater than MTU | sairif.L3SviTest.sviIPv6MtuTest |
| rif.101 | Verify IPv6 packet with packet size greater than MTU punted to CPU if trap present | sairif.L3MtuTrapTest.svi.Ipv6MtuTrapTest |
| rif.102 | Verify packet forward after MTU change for IPv4 | sairif.L3SviTest.sviIPv4MtuTest |
| rif.103 | Verify packet forward after MTU change for IPv6 | sairif.L3SviTest.sviIPv6MtuTest |
| rif.104 | Verify ingress sub-port stats for unicast packets | sairif.L3SviTest.sviStatsTest |
| rif.105 | Verify egress sub-port stats for unicast packets | sairif.L3SviTest.sviStatsTest |
| rif.106 | Verify ingress sub-port stats for broadcast packets | sairif.L3SviTest.sviStatsTest |
| rif.107 | Verify egress sub-port stats for broadcast packets | sairif.L3SviTest.sviStatsTest |
| rif.108 | Verify packet not routed if tagged packet ingresses on untagged SVI | sairif.L3SviTest.sviTaggingTest |
| rif.109 | Verify packet dropped if unknown VLAN tagged packet on tagged SVI | sairif.L3SviTest.sviTaggingTest |
| rif.110 | Verify IP2ME on untagged RIF port | sairif.L3SviTest.sviMyIPTest |
| rif.111 | Verify IP2ME on tagged RIF port | sairif.L3SviTest.sviMyIPTest |
| rif.112 | Verify ARP reply from Linux interface on untagged RIF | sairif.L3SviTest.sviArpReplyTest |
| rif.113 | Verify ARP reply from Linux interface on tagged RIF | sairif.L3SviTest.sviArpReplyTest |
| rif.114 | Verify MYIP works for subnet routes | sairif.L3SviTest.sviMyIPTest |
| rif.115 | Verify create fails when TYPE is VLAN and VLAN_id is 0 | sairif.L3SviTest.incorrectVlanIdTest |
| rif.116 | Verify duplicate VLAN interface creation fails | sairif.L3SviTest.duplicateVlanRifCreationTest |
| | Common tests | |
| rif.117 | Verify MTU trap packet statistics | sairif.L3MtuTrapTest.mtuPacketStatsTest |
| rif.119 | Verify get of non existent RIF fails | sairif.L3Interface.negativeRifTest |
| rif.120 | Verify removal of non existent RIF fails | sairif.L3Interface.negativeRifTest |
| rif.121 | Verify set of non existent RIF fails | sairif.L3Interface.negativeRifTest |
| rif.122 | Verify create fails with invalid vrf_id | sairif.L3Interface.negativeRifTest |

## Scheduler

| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| scheduler.1 | Create scheduler with SAI_SCHEDULING_TYPE_DWRR and attach to queue. Set and verify SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT. Attach scheduler to a Queue by SAI queue attribute SAI_QUEUE_ATTR_SCHEDULER_PROFILE_ID | saischeduler.SchedulerParamsTest.schedulerWeightTest |
| scheduler.2 | Create scheduler with SAI_SCHEDULING_TYPE_STRICT and attach to queue | saischeduler.SchedulerParamsTest.schedulerStrictPriorityTest |
| scheduler.3 | Create scheduler with SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE and attach to queue | saischeduler.SchedulerParamsTest.schedulerMinBwidthRateTest |
| scheduler.4 | Create scheduler with SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE and attach to queue | saischeduler.SchedulerParamsTest.schedulerMaxBwidthRateTest |
| scheduler.5 | Create scheduler with SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE and attach to queue | saischeduler.SchedulerParamsTest.schedulerMinBwidthBurstRateTest |
| scheduler.6 | Create scheduler with SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE and attach to queue | saischeduler.SchedulerParamsTest.schedulerMaxBwidthBurstRateTest |
| scheduler.7 | Modify different scheduler parameters and validate. | saischeduler.SchedulerParamsTest |
| scheduler.8 | Verify attaching scheduler with SAI_SCHEDULING_TYPE_DWRR to a scheduler group | saischeduler.SchedulerGroupAttachTest.schedulerWeightGroupAttachTest |
| scheduler.9 | Verify attaching scheduler with SAI_SCHEDULING_TYPE_STRICT to a scheduler group | saischeduler.SchedulerGroupAttachTest.schedulerStrictPriorityGroupAttachTest |
| scheduler.10 | Verify attaching scheduler with SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE to a scheduler group | saischeduler.SchedulerGroupAttachTest.schedulerMinBwidthRateGroupAttachTest |
| scheduler.11 | Verify attaching scheduler with SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE to a scheduler group | saischeduler.SchedulerGroupAttachTest.schedulerMaxBwidthRateGroupAttachTest |
| scheduler.12 | Verify attaching scheduler with SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE to a scheduler group | saischeduler.SchedulerGroupAttachTest.schedulerMinBwidthBurstRateGroupAttachTest |
| scheduler.13 | Verify attaching scheduler with SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE to a scheduler group | saischeduler.SchedulerGroupAttachTest.schedulerMaxBwidthBurstRateGroupAttachTest |
| scheduler.14 | Attach scheduler with SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE to a port using SAI port attribute - SAI_PORT_ATTR_QOS_SCHEDULER_PROFILE_ID | saischeduler.SchedulerPortAttachTest.schedulerMaxBwidthRatePortAttachTest |
| scheduler.15 | Attach scheduler with SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE to a port using SAI port attribute - SAI_PORT_ATTR_QOS_SCHEDULER_PROFILE_ID | saischeduler.SchedulerPortAttachTest.schedulerMaxBwidthBurstRatePortAttachTest |

## Scheduler Group

| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| schedulergroup.1 | Query and verify the number of scheduler groups per port using PORT attribute - SAI_PORT_ATTR_QOS_NUMBER_OF_SCHEDULER_GROUPS | saischedulergroup.SchGroupParamsTest |
| schedulergroup.2 | Query and verify the list of scheduler groups per port using PORT Attribute - SAI_PORT_ATTR_QOS_SCHEDULER_GROUP_LIST | saischedulergroup.SchGroupParamsTest |
| schedulergroup.3 | Query and verify the SAI_SCHEDULER_GROUP_ATTR_PORT_ID of scheduler group and match with the port object. | saischedulergroup.SchGroupParamsTest |
| schedulergroup.4 | Query and verify the scheduler profile attached to this scheduler group using attribute - SAI_SCHEDULER_GROUP_ATTR_CHILD_LIST | saischedulergroup.SchGroupParamsTest |
| schedulergroup.5 | Query and verify the queue count associated with this scheduler group - SAI_SCHEDULER_GROUP_ATTR_CHILD_COUNT | saischedulergroup.SchGroupParamsTest |
| schedulergroup.6 | Attach the scheduler profile to scheduler group using - SAI_SCHEDULER_GROUP_ATTR_SCHEDULER_PROFILE_ID | saischedulergroup.SchGroupParamsTest |
| schedulergroup.7 | Modify the parameters of the scheduler profile and verify if those attributes are updated per port or queue associated to the scheduler group. | saischedulergroup.SchGroupParamsTest |
| schedulergroup.8 | Verify if scheduler group creation fails without mandatory parameters | saischedulergroup.SchGroupCreateFailTest |

## Segment Routing v6

| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| srv6.1 | Verify SRv6 source encapsulation with one SID | saisrv6.Srv6SrcEncapTest.sourceEncapOneSidTest |
| srv6.2 | Verify SRv6 source encapsulation with two SIDs | saisrv6.Srv6SrcEncapTest.sourceEncapTwoSidTest |
| srv6.3 | Verify SRv6 source encapsulation with three SIDs | saisrv6.Srv6SrcEncapTest.sourceEncapThreeSidTest |
| srv6.4 | Verify SRv6 source encapsulation with ECMP SID | saisrv6.Srv6SrcEncapTest.sourceEncapEcmpSidTest |
| srv6.5 | Verify SRv6 insert headend operation with a single SID | saisrv6.Srv6SrcEncapTest.insertOneSidTest |
| srv6.6 | Verify SRv6 insert headend operation with two SIDs | saisrv6.Srv6SrcEncapTest.insertTwoSidTest |
| srv6.7 | Verify getting and setting SRv6 sidlist members | saisrv6.Srv6SrcEncapTest.getSetSidlistTest |
| srv6.8 | Verify SRv6 End endpoint behavior | saisrv6.Srv6MySidTest.mySidEndTest |
| srv6.9 | Verify SRv6 End endpoint behavior with PSP flavor | saisrv6.Srv6MySidTest.mySidEndTest |
| srv6.10 | Verify SRv6 End endpoint behavior with USD flavor | saisrv6.Srv6MySidTest.mySidEndTest |
| srv6.11 | Verify SRv6 End.T endpoint behavior | saisrv6.Srv6MySidTest.mySidEndTTest |
| srv6.12 | Verify SRv6 End.DT46 endpoint behavior | saisrv6.Srv6MySidTest.mySidEndDT46Test |
| srv6.13 | Verify SRv6 End.DT6 endpoint behavior | saisrv6.Srv6MySidTest.mySidEndDT4Test |
| srv6.14 | Verify SRv6 End.DT4 endpoint behavior | saisrv6.Srv6MySidTest.mySidEndDT6Test |
| srv6.15 | Verify SRv6 End.X endpoint behavior | saisrv6.Srv6MySidTest.mySidXConnectTest |
| srv6.16 | Verify SRv6 re-encapsulation with End.DT4 behavior | saisrv6.Srv6MySidTest.mySidEndDT4ReEncapTest |
| srv6.17 | Verify SRv6 re-encapsulation with End.DT6 behavior | saisrv6.Srv6MySidTest.mySidEndDT6ReEncapTest |
| srv6.18 | Verify SRv6 re-encapsulation with End.DT46 behavior | saisrv6.Srv6MySidTest.mySidEndDT46ReEncapTest |
| srv6.19 | Verify SRv6 End.B6.Encaps.Red endpoint behavior | saisrv6.Srv6MySidTest.mySidB6EncapTest |
| srv6.20 | Verify SRv6 End.B6.Insert.Red endpoint behavior | saisrv6.Srv6MySidTest.mySidB6InsertTest |
| srv6.21 | Verify getting and setting my SID entry attributes | saisrv6.Srv6MySidTest.getSetMySidEntryTest |
| srv6.22 | Verify statistics of a counter attached to my_sid object. Also verify getting counter_id of my_sid object and statistics clearing | saisrv6.Srv6MySidTest.mySidCounterTest |
| srv6.23 | Verify SRv6 End.uN uSID behavior | saisrv6.Srv6MySidUsidTest.mySidEndUNTest |
| srv6.24 | Verify SRv6 End.uN uSID behavior with PSP flavor | saisrv6.Srv6MySidUsidTest.mySidEndUNPSPTest |
| srv6.25 | Verify SRv6 End.uN uSID behavior with USD flavor | saisrv6.Srv6MySidUsidTest.mySidEndUNUSDTest |
| srv6.26 | Verify SRv6 End.uN uSID behavior with USD flavor and no SRH | saisrv6.Srv6MySidUsidTest.mySidEndUNUSDNoSRHTest |
| srv6.27 | Verify SRv6 End.uA uSID behavior | saisrv6.Srv6MySidUsidTest.mySidEndUATest |
| srv6.28 | Verify SRv6 End.uA uSID behavior with PSP flavor | saisrv6.Srv6MySidUsidTest.mySidEndUAPSPTest |
| srv6.29 | Verify SRv6 End.uA uSID behavior with USD flavor | saisrv6.Srv6MySidUsidTest.mySidEndUAUSDTest |
| srv6.30 | Verify SRv6 End.uA uSID behavior with USD flavor and no SRH | saisrv6.Srv6MySidUsidTest.mySidEndUAUSDNoSRHTest |
| srv6.31 | Verify if packets are dropped when my SID packet action is SAI_PACKET_ACTION_DROP | saisrv6.Srv6MySidDropTest.packetActionDropTest |
| srv6.32 | Verify if packets with SL!=0 are dropped for End.D* endpoints | saisrv6.Srv6MySidDropTest.nonZeroSlEndDTxDropTest |
| srv6.33 | Verify if maximum number of MY SID entries may be created | saisrv6.MySidObjectsAvailibilityTest |

## Switch

| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| switch.1 | Verify get for SAI_SWITCH_ATTR_NUMBER_OF_ACTIVE_PORTS | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.2 | Verify get for SAI_SWITCH_ATTR_MAX_NUMBER_OF_SUPPORTED_PORTS | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.3 | Verify get for SAI_SWITCH_ATTR_PORT_LIST | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.4 | Verify get for SAI_SWITCH_ATTR_PORT_MAX_MTU | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.5 | Verify get for SAI_SWITCH_ATTR_CPU_PORT | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.6 | Verify get for SAI_SWITCH_ATTR_MAX_VIRTUAL_ROUTES | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.7 | Verify get for SAI_SWITCH_ATTR_FDB_TABLE_SIZE | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.8 | Verify get for SAI_SWITCH_ATTR_L3_NEIGHBOR_TABLE_SIZE | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.9 | Verify get for SAI_SWITCH_ATTR_L3_ROUTE_TABLE_SIZE | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.10 | Verify get for SAI_SWITCH_ATTR_LAG_MEMBERS | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.11 | Verify get for SAI_SWITCH_ATTR_NUMBER_OF_LAGS | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.12 | Verify get for SAI_SWITCH_ATTR_ECMP_MEMBERS | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.13 | Verify get for SAI_SWITCH_ATTR_NUMBER_OF_ECMP_GROUPS | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.14 | Verify get for SAI_SWITCH_ATTR_NUMBER_OF_UNICAST_QUEUES | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.15 | Verify get for SAI_SWITCH_ATTR_NUMBER_OF_MULTICAST_QUEUES | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.16 | Verify get for SAI_SWITCH_ATTR_NUMBER_OF_QUEUES | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.17 | Verify get for SAI_SWITCH_ATTR_NUMBER_OF_CPU_QUEUES | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.18 | Verify get for SAI_SWITCH_ATTR_ACL_TABLE_MINIMUM_PRIORITY | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.19 | Verify get for SAI_SWITCH_ATTR_ACL_TABLE_MAXIMUM_PRIORITY | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.20 | Verify get for SAI_SWITCH_ATTR_ACL_ENTRY_MINIMUM_PRIORITY | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.21 | Verify get for SAI_SWITCH_ATTR_ACL_ENTRY_MAXIMUM_PRIORITY | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.22 | Verify get for SAI_SWITCH_ATTR_DEFAULT_VLAN_ID | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.23 | Verify get for SAI_SWITCH_ATTR_DEFAULT_STP_INST_ID | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.24 | Verify get for SAI_SWITCH_ATTR_MAX_STP_INSTANCE | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.25 | Verify get for SAI_SWITCH_ATTR_DEFAULT_VIRTUAL_ROUTER_ID | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.26 | Verify get for SAI_SWITCH_ATTR_DEFAULT_1Q_BRIDGE_ID | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.27 | Verify get for SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_TRAFFIC_CLASSES | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.28 | Verify get for SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_SCHEDULER_GROUP_HIERARCHY_LEVELS | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.29 | Verify get for SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_SCHEDULER_GROUPS_PER_HIERARCHY_LEVEL | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.30 | Verify get for SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_CHILDS_PER_SCHEDULER_GROUP | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.31 | Verify get for SAI_SWITCH_ATTR_TOTAL_BUFFER_SIZE | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.32 | Verify get for SAI_SWITCH_ATTR_INGRESS_BUFFER_POOL_NUM | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.33 | Verify get for SAI_SWITCH_ATTR_EGRESS_BUFFER_POOL_NUM | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.34 | Verify get for SAI_SWITCH_ATTR_ECMP_HASH | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.35 | Verify get for SAI_SWITCH_ATTR_LAG_HASH | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.36 | Verify get for SAI_SWITCH_ATTR_MAX_ACL_ACTION_COUNT | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.37 | Verify get for SAI_SWITCH_ATTR_MAX_ACL_RANGE_COUNT | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.38 | Verify get for SAI_SWITCH_ATTR_ACL_CAPABILITY | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.39 | Verify get for SAI_SWITCH_ATTR_MAX_MIRROR_SESSION | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.40 | Verify get for SAI_SWITCH_ATTR_DEFAULT_TRAP_GROUP | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.41 | Verify get for SAI_SWITCH_ATTR_ACL_STAGE_INGRESS | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.42 | Verify get for SAI_SWITCH_ATTR_ACL_STAGE_EGRESS | saiswitch.SwitchAttrTest.readOnlyAttributesTest |
| switch.43 | Verify available is 0 after MAX is reached for SAI_SWITCH_ATTR_AVAILABLE_IPV4_ROUTE_ENTRY | saiswitch.SwitchAttrTest.availableIPv4RouteEntryTest |
| switch.44 | Verify available is 0 after MAX is reached for SAI_SWITCH_ATTR_AVAILABLE_IPV6_ROUTE_ENTRY | saiswitch.SwitchAttrTest.availableIPv6RouteEntryTest |
| switch.45 | Verify available is 0 after MAX is reached for SAI_SWITCH_ATTR_AVAILABLE_IPV4_NEXTHOP_ENTRY | saiswitch.SwitchAttrTest.availableIPv4NexthopEntryTest |
| switch.46 | Verify available is 0 after MAX is reached for SAI_SWITCH_ATTR_AVAILABLE_IPV6_NEXTHOP_ENTRY | saiswitch.SwitchAttrTest.availableIPv6NexthopEntryTest |
| switch.47 | Verify available is 0 after MAX is reached for SAI_SWITCH_ATTR_AVAILABLE_IPV4_NEIGHBOR_ENTRY | saiswitch.SwitchAttrTest.availableIPv4NeighborEntryTest |
| switch.48 | Verify available is 0 after MAX is reached for SAI_SWITCH_ATTR_AVAILABLE_IPV6_NEIGHBOR_ENTRY | saiswitch.SwitchAttrTest.availableIPv6NeighborEntryTest |
| switch.49 | Verify available is 0 after MAX is reached for SAI_SWITCH_ATTR_AVAILABLE_NEXT_HOP_GROUP_ENTRY | saiswitch.SwitchAttrTest.availableNexthopGroupEntryTest |
| switch.50 | Verify available is 0 after MAX is reached for SAI_SWITCH_ATTR_AVAILABLE_NEXT_HOP_GROUP_MEMBER_ENTRY | saiswitch.SwitchAttrTest.availableNexthopGroupMemberEntryTest |
| switch.51 | Verify available is 0 after MAX is reached for SAI_SWITCH_ATTR_AVAILABLE_FDB_ENTRY | saiswitch.SwitchAttrTest.availableFdbEntryTest |
| switch.52 | Verify available is 0 after MAX is reached for SAI_SWITCH_ATTR_AVAILABLE_SNAT_ENTRY | saiswitch.SwitchAttrTest.availableSnatEntryTest |
| switch.53 | Verify available is 0 after MAX is reached for SAI_SWITCH_ATTR_AVAILABLE_DNAT_ENTRY | saiswitch.SwitchAttrTest.availableDnatEntryTest |
| switch.54 | Verify available is 0 after MAX is reached for SAI_SWITCH_ATTR_AVAILABLE_ACL_ENTRY | saiswitch.SwitchAttrTest.availableAclEntryTest |
| switch.55 | Verify traffic with SAI_SWITCH_ATTR_SRC_MAC_ADDRESS | set in sai_base_test, verified in sairif (multiple test cases) |
| switch.56 | Verify traffic with SAI_SWITCH_ATTR_FDB_AGING_TIME | saifdb.FdbAgeTest |
| switch.57 | Verify traffic with SAI_SWITCH_ATTR_FDB_UNICAST_MISS_PACKET_ACTION | saifdb.FdbMissTest.unicast*ActionTest |
| switch.58 | Verify traffic with SAI_SWITCH_ATTR_FDB_BROADCAST_MISS_PACKET_ACTION | saifdb.FdbMissTest.broadcast*ActionTest |
| switch.59 | Verify traffic with SAI_SWITCH_ATTR_FDB_MULTICAST_MISS_PACKET_ACTION | saifdb.FdbMissTest.multicast*ActionTest |
| switch.60 | Verify traffic with SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_SEED | saihash.L3EcmpIPv4HashSeedTest, saihash.L3EcmpIPv6HashSeedTest |
| switch.61 | Verify traffic with SAI_SWITCH_ATTR_ECMP_HASH_IPV4 | saihash.EcmpIPv4SrcIPHashTest |
| switch.62 | Verify traffic with SAI_SWITCH_ATTR_ECMP_HASH_IPV6 | saihash.EcmpIPv6SrcIPHashTest |
| switch.63 | Verify traffic with SAI_SWITCH_ATTR_LAG_DEFAULT_HASH_SEED | saihash.L3LagIPv4HashSeedTest |
| switch.64 | Verify traffic with SAI_SWITCH_ATTR_LAG_HASH_IPV4 | saihash.L3LagIPv4HashTest |
| switch.65 | Verify traffic with SAI_SWITCH_ATTR_LAG_HASH_IPV6 | saihash.L3LagIPv6HashTest |
| switch.66 | Verify traffic with SAI_SWITCH_ATTR_COUNTER_REFRESH_INTERVAL | saiswitch.SwitchAttrTest.refreshIntervalTest |
| switch.67 | Verify traffic with SAI_SWITCH_ATTR_VXLAN_DEFAULT_ROUTER_MAC | saiswitch.SwitchVxlanTest |
| switch.68 | Verify traffic with SAI_SWITCH_ATTR_VXLAN_DEAFAULT_PORT | saiswitch.SwitchVxlanTest |
| switch.69 | Verify packet forwarding with dest_mac = SAI_SWITCH_ATTR_SRC_MAC_ADDRESS | sairif.L3InterfaceTest.macUpdateTest |

## Tunnel

| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| | IPv4 underlay | |
| tunnel.1 | Verify tunneled packet with malformed inner ethernet header is dropped - all 0's inner src MAC | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelInvalidInnerSmacTest |
| tunnel.2 | Verify tunneled packet with malformed inner ethernet header is dropped - broadcast inner src MAC | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelInvalidInnerSmacTest |
| tunnel.3 | Verify tunneled packet with malformed inner ethernet header is dropped - multicast inner src MAC | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelInvalidInnerSmacTest |
| tunnel.4 | Verify tunneled packet with malformed inner ethernet header is dropped - all 0's inner dst MAC | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelInvalidInnerDmacTest |
| tunnel.5 | Verify tunnel Encap of IPv4 packet (packet having tunnel nexthop) with invalid (overlay) IP headers is dropped - Overlay packet Invalid IP version - valid IPv4 Packet with incorrect version = 6 | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelEncapInvalidIpVersionTest |
| tunnel.6 | Verify tunnel Encap of IPv4 packet (packet having tunnel nexthop) with invalid (overlay) IP headers is dropped - Overlay packet Invalid IHL values 0-4 | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelEncapInvalidIhlTest |
| tunnel.7 | Verify tunnel Encap of IPv4 packet (packet having tunnel nexthop) with invalid (overlay) IP headers is dropped - Overlay packet TTL = 0 | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelEncapInvalidTtlTest |
| tunnel.8 | Verify tunnel Encap of IPv4 packet (packet having tunnel nexthop) with invalid (overlay) IP headers is dropped - Overlay packet Invalid IPv4 checksum | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelEncapInvalidChksumTest |
| tunnel.9 | Verify tunnel Encap of IPv4 packet (packet having tunnel nexthop) with invalid (overlay) IP headers is dropped - Overlay packet Source IP address = loopback IP | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelEncapInvalidSrcIpTest |
| tunnel.10 | Verify tunnel Encap of IPv4 packet (packet having tunnel nexthop) with invalid (overlay) IP headers is dropped - Overlay packet Source IP address = multicast IP | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelEncapInvalidSrcIpTest |
| tunnel.11 | Verify Underlay MTU exceeded on IPv4 encap drop (overlay_pkt_size < overlay_mtu < underlay_mtu < underlay_pkt_size) | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelEncapInvalidIpv4MtuTest |
| tunnel.12 | Verify Tunnel Encap of IPv6 packet (packet having tunnel nexthop) with invalid IP headers (overlay) is dropped - Overlay packet Invalid IP version, Valid IPv6 Packet with incorrect version = 4 | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelEncapInvalidIpVersionTest |
| tunnel.13 | Verify Tunnel Encap of IPv6 packet (packet having tunnel nexthop) with invalid IP headers (overlay) is dropped - Overlay packet Hop Limit = 0 | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelEncapInvalidHlimTest |
| tunnel.14 | Verify Tunnel Encap of IPv6 packet (packet having tunnel nexthop) with invalid IP headers (overlay) is dropped - Overlay packet Source IP address = multicast IP | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelEncapInvalidSrcIpv6Test |
| tunnel.15 | Verify Underlay MTU exceeded on IPv6 encap drop (overlay_pkt_size < overlay_mtu < underlay_mtu < underlay_pkt_size) | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelEncapInvalidIpv6MtuTest |
| tunnel.16 | Verify Tunnel Decap Invalid Inner IPv4 packet is dropped - Inner DMAC not equal to RMAC | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelDecapInvalidInnerDmacIpv4Test |
| tunnel.17 | Verify Tunnel Decap Invalid Inner IPv4 packet is dropped - Inner Valid IPv4 Packet with Invalid IP version, version=6 | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelDecapInvalidInnerIpVersionTest |
| tunnel.18 | Verify Tunnel Decap Invalid Inner IPv4 packet is dropped - Inner IHL is invalid, IHL values 0-4 | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelDecapInvalidInnerIhlTest |
| tunnel.19 | Verify Tunnel Decap Invalid Inner IPv4 packet is dropped - Inner TTL=0 | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelDecapInvalidInnerTtlTest |
| tunnel.20 | Verify Tunnel Decap Invalid Inner IPv4 packet is dropped - Inner Checksum is invalid | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelDecapInvalidInnerChksumTest |
| tunnel.21 | Verify Tunnel Decap Invalid Inner IPv4 packet is dropped - Invalid Inner Source IP: Loopback and Multicast | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelDecapInvalidInnerSrcIpv4Test |
| tunnel.22 | Verify Tunnel Decap with Invalid Inner IPv6 packet is dropped - Inner DMAC not equal to RMAC | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelDecapInvalidInnerDmacTest |
| tunnel.23 | Verify Tunnel Decap with Invalid Inner IPv6 packet is dropped - Inner Valid IPv4 Packet with Invalid IP version, version=6 | saitunnel.TunnelMalformedPacketsTestIpv4Underlay |
| tunnel.24 | Verify Tunnel Decap with Invalid Inner IPv6 packet is dropped - Hop limit=0 | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelDecapInvalidInnerHlimTest |
| tunnel.25 | Verify Tunnel Decap with Invalid Inner IPv6 packet is dropped - Invalid Inner Source IP: Loopback and Multicast | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelDecapInvalidInnerSrcIpTest |
| tunnel.26 | Verify Tunnel Decap with Invalid Outer IPv4 packet is dropped - Outer Valid IPv4 Packet with Invalid Version, version=6 | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelDecapInvalidOuterIpVersionTest |
| tunnel.27 | Verify Tunnel Decap with Invalid Outer IPv4 packet is dropped - Outer IHL is invalid , IHL values 0-4 | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelDecapInvalidOuterIhlTest |
| tunnel.28 | Verify Tunnel Decap with Invalid Outer IPv4 packet is dropped - Outer TTL=0 | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelDecapInvalidOuterTtlTest |
| tunnel.29 | Verify Tunnel Decap with Invalid Outer IPv4 packet is dropped - Outer Checksum is invalid | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelDecapInvalidOuterChksumTest |
| tunnel.30 | Verify Tunnel Decap with Invalid Outer IPv4 packet is dropped -  Invalid Source IP: Loopback IP or Multicast IP address | saitunnel.TunnelMalformedPacketsTestIpv4Underlay.tunnelDecapInvalidOuterSrcIpTest |
| tunnel.31 | Verify Underlay Nexthop Resolution for Tunnels for different sequences of objects creation when underlay RIF is L3 interface (for both IPv4 and IPv6 overlay) | saitunnel.TunnelNhopResolutionIpv4UnderlayTest.tunnelL3IntfTest |
| tunnel.32 | Verify Underlay Nexthop Resolution for Tunnels for different sequences of objects creation when underlay RIF is L3 LAG interface (for both IPv4 and IPv6 overlay) | saitunnel.TunnelNhopResolutionIpv4UnderlayTest.tunnelL3LagIntfTest |
| tunnel.33 | Verify Underlay Nexthop Resolution for Tunnels for different sequences of objects creation when underlay RIF is L3 Subport interface (for both IPv4 and IPv6 overlay) | saitunnel.TunnelNhopResolutionIpv4UnderlayTest.tunnelL3SubPortIntfTest |
| tunnel.34 | Verify Underlay Nexthop Resolution for Tunnels for different sequences of objects creation when underlay RIF is an SVI (for both IPv4 and IPv6 overlay) | saitunnel.TunnelNhopResolutionIpv4UnderlayTest.tunnelSviIntfTest |
| tunnel.35 | Verify (Underlay) Nexthop Resolution with Multi Tunnels for different sequences of objects creation | saitunnel.TunnelNhopResolutionIpv4UnderlayTest.multiTunnelNhopTest |
| tunnel.36 | Verify Tunnel Underlay Nexthop Resolution with Underlay ECMP | saitunnel.TunnelNhopResolutionIpv4UnderlayTest/underlayEcmpTunnelTest |
| tunnel.37 | Verify Overlay Tunnel ECMP Nexthop Resolution | saitunnel.TunnelNhopResolutionIpv4UnderlayTest.ecmpTunnelTest |
| tunnel.38 | Verify Tunnel Encap Decap for following Overlay/Underlay Combination: Overlay:(L3, L3 Subport , [SVI: Tagged & Untagged, Port & LAG], L3 LAG) <-> Underlay:(L3, L3 Subport, [SVI: Tagged & Untagged, Port & LAG], L3 LAG) | saitunnel.VxLanRifsConfigTunnelIpv4UnderlayOportsTest.portsToPortsConfigTest |
| tunnel.39 | Verify Tunnel Encap Decap for following Overlay/Underlay Combination: Overlay:(L3, L3 Subport, [SVI Tagged & SVI Untagged], L3 LAG) <-> Underlay:(ECMP(L3, L3 Subport, [SVI: Tagged & Untagged, Port & Lag], L3 LAG) | saitunnel.VxLanRifsConfigTunnelIpv4UnderlayOportsTest.portsToEcmpConfigTest |
| tunnel.40 | Verify Tunnel Encap Decap for following Overlay/Underlay Combination: Overlay:(L3, L3 Subport, [SVI Tagged & SVI Untagged], L3 LAG) <-> Underlay ECMP of L3 + L3 subport + SVI (tagged & untagged, Port & LAG) + L3 Lag | saitunnel.VxLanRifsConfigTunnelIpv4UnderlayOportsTest.portsToPortsEcmpConfigTest |
| tunnel.41 | Verify Tunnel Encap Decap for following Overlay/Underlay Combination: Overlay ECMP Tunnel Nexthops <-> Underlay:(L3, L3 Subport, [SVI Tagged & SVI Untagged], L3 LAG) | saitunnel.VxLanRifsConfigTunnelIpv4UnderlayOecmpTest.ecmpToPortsConfigTest |
| tunnel.42 | Verify Tunnel Encap Decap for following Overlay/Underlay Combination: Overlay ECMP Tunnel Nexthop <-> Underlay:(ECMP(L3, L3 Subport, [SVI Tagged & SVI Untagged], L3 LAG) | saitunnel.VxLanRifsConfigTunnelIpv4UnderlayOecmpTest.ecmpToEcmpConfigTest |
| tunnel.43 | Verify Tunnel Encap Decap for following Overlay/Underlay Combination: Overlay ECMP Tunnel Nexthop <-> Underlay ECMP of L3 + L3 subport + SVI (tagged & untagged) + L3 Lag | saitunnel.VxLanRifsConfigTunnelIpv4UnderlayOecmpTest.ecmpToPortsEcmpConfigTest |
| tunnel.44 | Verify Multi Tunnel Forwarding with single overlay VRF, and multiple underlay VRFs | saitunnel.SingleOvrfMultiTunnelIpv4UnderlayTest |
| tunnel.45 | Verify Multi Tunnel Forwarding with multiple overlay VRF, and multiple underlay VRFs | saitunnel.MultipleOvrfMultiTunnelIpv4UnderlayTest |
| tunnel.46 | Verify Tunnel Encap/Decap for multi VRF<->VNI mapping, with different overlay interface types (for both: IPv4 and IPv6 overlay) | saitunnel.MultipleMapperEntriesIpv4UnderlayTest |
| tunnel.47 | Verify tunnel encap/decap by dynamically adding and removing tunnel mapper entries (type = SAI_TUNNEL_MAP_TYPE_VNI_TO_VIRTUAL_ROUTER_ID and  SAI_TUNNEL_MAP_TYPE_VIRTUAL_ROUTER_ID_TO_VNI) from tunnel map object associated to tunnel object | saitunnel.MultipleMapperEntriesIpv4UnderlayTest.addDelMapperEntriesTest |
| tunnel.48 | Verify tunnel encap/decap for P2MP VxLAN Tunnels | saitunnel.VxLanP2MpTnnnelIpv4UnderlayTest |
| tunnel.49 | Verify tunnel encap/decap for P2MP IPinIP Tunnels V4 Underlay | saitunnel.IpInIpP2MpTnnnelIpv4UnderlayTest |
| tunnel.50 | For P2P VxLAN tunnel - Verify tunnel encap/decap | saitunnel.VxLanTunnelIpv4UnderlayTest.encapDecapTest |
| tunnel.51 | For P2P VxLAN tunnel - Verify tunnel de-encapsulation occurs only for tunneled packets with src IP =  SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_SRC_IP | saitunnel.VxLanTunnelIpv4UnderlayTest.tunnelTermSrcIpTest |
| | IPv6 underlay | |
| tunnel.52 | Verify tunneled packet with malformed inner ethernet header is dropped - all 0's inner src MAC | saitunnel.TunnelMalformedPacketsTestIpv6Underlay.tunnelInvalidInnerSmacTest |
| tunnel.53 | Verify tunneled packet with malformed inner ethernet header is dropped - broadcast inner src MAC | saitunnel.TunnelMalformedPacketsTestIpv6Underlay.tunnelInvalidInnerSmacTest |
| tunnel.54 | Verify tunneled packet with malformed inner ethernet header is dropped - multicast inner src MAC | saitunnel.TunnelMalformedPacketsTestIpv6Underlay.tunnelInvalidInnerSmacTest |
| tunnel.55 | Verify tunneled packet with malformed inner ethernet header is dropped - all 0's inner dst MAC | saitunnel.TunnelMalformedPacketsTestIpv6Underlay.tunnelInvalidInnerDmacTest |
| tunnel.56 | Verify tunnel Encap of IPv4 packet (packet having tunnel nexthop) with invalid (overlay) IP headers is dropped - Overlay packet Invalid IP version - valid IPv4 Packet with incorrect version = 6 | saitunnel.TunnelMalformedPacketsTestIpv6Underlay.tunnelEncapInvalidIpVersionTest |
| tunnel.57 | Verify tunnel Encap of IPv4 packet (packet having tunnel nexthop) with invalid (overlay) IP headers is dropped - Overlay packet Invalid IHL values 0-4 | saitunnel.TunnelMalformedPacketsTestIpv6Underlay.tunnelEncapInvalidIhlTest |
| tunnel.58 | Verify tunnel Encap of IPv4 packet (packet having tunnel nexthop) with invalid (overlay) IP headers is dropped - Overlay packet TTL = 0 | saitunnel.TunnelMalformedPacketsTestIpv6Underlay.tunnelEncapInvalidTtlTest |
| tunnel.59 | Verify tunnel Encap of IPv4 packet (packet having tunnel nexthop) with invalid (overlay) IP headers is dropped - Overlay packet Invalid IPv4 checksum | saitunnel.TunnelMalformedPacketsTestIpv6Underlay.tunnelEncapInvalidChksumTest |
| tunnel.60 | Verify tunnel Encap of IPv4 packet (packet having tunnel nexthop) with invalid (overlay) IP headers is dropped - Overlay packet Source IP address = loopback IP | saitunnel.TunnelMalformedPacketsTestIpv46Underlay.tunnelEncapInvalidSrcIpTest |
| tunnel.61 | Verify tunnel Encap of IPv4 packet (packet having tunnel nexthop) with invalid (overlay) IP headers is dropped - Overlay packet Source IP address = multicast IP | saitunnel.TunnelMalformedPacketsTestIp6Underlay.tunnelEncapInvalidSrcIpTest |
| tunnel.62 | Verify Underlay MTU exceeded on encap drop (overlay_pkt_size < overlay_mtu < underlay_mtu < underlay_pkt_size) | saitunnel.TunnelMalformedPacketsTestIpv6Underlay.tunnelEncapInvalidIpv4MtuTest |
| tunnel.63 | Verify Tunnel Encap of IPv6 packet (packet having tunnel nexthop) with invalid IP headers (overlay) is dropped - Overlay packet Invalid IP version, Valid IPv6 Packet with incorrect version = 4 | saitunnel.TunnelMalformedPacketsTestIpv6Underlay.tunnelEncapInvalidIpVersionTest |
| tunnel.64 | Verify Tunnel Encap of IPv6 packet (packet having tunnel nexthop) with invalid IP headers (overlay) is dropped - Overlay packet Hop Limit = 0 | saitunnel.TunnelMalformedPacketsTestIpv6Underlay.tunnelEncapInvalidHlimTest |
| tunnel.65 | Verify Tunnel Encap of IPv6 packet (packet having tunnel nexthop) with invalid IP headers (overlay) is dropped - Overlay packet Source IP address = multicast IP | saitunnel.TunnelMalformedPacketsTestIpv6Underlay.tunnelEncapInvalidSrcIpv6Test |
| tunnel.66 | Verify Underlay MTU exceeded on IPv6 encap drop (overlay_pkt_size < overlay_mtu < underlay_mtu < underlay_pkt_size) | saitunnel.TunnelMalformedPacketsTestIpv6Underlay.tunnelEncapInvalidIpv6MtuTest |
| tunnel.67 | Verify Tunnel Decap Invalid Inner IPv4 packet is dropped - Inner DMAC not equal to RMAC | saitunnel.TunnelMalformedPacketsTestIpv6Underlay.tunnelDecapInvalidInnerDmacIpv4Test |
| tunnel.68 | Verify Tunnel Decap Invalid Inner IPv4 packet is dropped - Inner Valid IPv4 Packet with Invalid IP version, version=6 | saitunnel.TunnelMalformedPacketsTestIpv6Underlay.tunnelDecapInvalidInnerIpVersionTest |
| tunnel.69 | Verify Tunnel Decap Invalid Inner IPv4 packet is dropped - Inner IHL is invalid, IHL values 0-4 | saitunnel.TunnelMalformedPacketsTestIpv6Underlay.tunnelDecapInvalidInnerIhlTest |
| tunnel.70 | Verify Tunnel Decap Invalid Inner IPv4 packet is dropped - Inner TTL=0 | saitunnel.TunnelMalformedPacketsTestIpv6Underlay.tunnelDecapInvalidInnerTtlTest |
| tunnel.71 | Verify Tunnel Decap Invalid Inner IPv4 packet is dropped - Inner Checksum is invalid | saitunnel.TunnelMalformedPacketsTestIpv6Underlay.tunnelDecapInvalidInnerChksumTest |
| tunnel.72 | Verify Tunnel Decap with Invalid Inner IPv6 packet is dropped - Inner DMAC not equal to RMAC | saitunnel.TunnelMalformedPacketsTestIpv6Underlay.tunnelDecapInvalidInnerDmacTest |
| tunnel.73 | Verify Tunnel Decap with Invalid Inner IPv6 packet is dropped - Inner Valid IPv4 Packet with Invalid IP version, version=6 | saitunnel.TunnelMalformedPacketsTestIpv6Underlay |
| tunnel.74 | Verify Tunnel Decap with Invalid Inner IPv6 packet is dropped - Hop limit=0 | saitunnel.TunnelMalformedPacketsTestIpv6Underlay.tunnelDecapInvalidInnerHlimTest |
| tunnel.75 | Verify Tunnel Decap with Invalid Inner IPv6 packet is dropped - Invalid Inner Source IP: Loopback and Multicast | saitunnel.TunnelMalformedPacketsTestIpv6Underlay.tunnelDecapInvalidInnerSrcIpTest |
| tunnel.76 | Verify Tunnel Decap with Invalid Outer IPv6 packet is dropped - Outer Valid IPv6 Packet with Invalid Version, version=4 | saitunnel.TunnelMalformedPacketsTestIpv6Underlay.tunnelDecapInvalidOuterIpVersionTest |
| tunnel.77 | Verify Tunnel Decap with Invalid Outer IPv6 packet is dropped - Outer Hop Limit=0 | saitunnel.TunnelMalformedPacketsTestIpv6Underlay.tunnelDecapInvalidOuterHlimTest |
| tunnel.78 | Verify Tunnel Decap with Invalid Outer IPv6 packet is dropped - Invalid Source IP: Loopback IP or Multicast IP address | saitunnel.TunnelMalformedPacketsTestIpv6Underlay.tunnelDecapInvalidOuterSrcIpv6Test |
| tunnel.79 | Verify Underlay Nexthop Resolution for Tunnels for different sequences of objects creation when underlay RIF is L3 interface (for both IPv4 and IPv6 overlay) | saitunnel.TunnelNhopResolutionTestIpv6Underlay.tunnelL3IntfTest |
| tunnel.80 | Verify Underlay Nexthop Resolution for Tunnels for different sequences of objects creation when underlay RIF is L3 LAG interface (for both IPv4 and IPv6 overlay) | saitunnel.TunnelNhopResolutionTestIpv6Underlay.tunnelL3LagIntfTest |
| tunnel.81 | Verify Underlay Nexthop Resolution for Tunnels for different sequences of objects creation when underlay RIF is L3 Subport interface (for both IPv4 and IPv6 overlay) | saitunnel.TunnelNhopResolutionTestIpv6Underlay.tunnelL3SubPortIntfTest |
| tunnel.82 | Verify Underlay Nexthop Resolution for Tunnels for different sequences of objects creation when underlay RIF is an SVI (for both IPv4 and IPv6 overlay) | saitunnel.TunnelNhopResolutionTestIpv6Underlay.tunnelSviIntfTest |
| tunnel.83 | Verify (Underlay) Nexthop Resolution with Multi Tunnels for different sequences of objects creation | saitunnel.TunnelNhopResolutionTestIpv6Underlay.multiTunnelNhopTest |
| tunnel.84 | Verify Tunnel Underlay Nexthop Resolution with Underlay ECMP | saitunnel.TunnelNhopResolutionTestIpv6Underlay/underlayEcmpTunnelTest |
| tunnel.85 | Verify Overlay Tunnel ECMP Nexthop Resolution | saitunnel.TunnelNhopResolutionTestIpv6Underlay.ecmpTunnelTest |
| tunnel.86 | Verify Tunnel Encap Decap for following Overlay/Underlay Combination: Overlay:(L3, L3 Subport , [SVI: Tagged & Untagged, Port & LAG], L3 LAG) <-> Underlay:(L3, L3 Subport, [SVI: Tagged & Untagged, Port & LAG], L3 LAG) | saitunnel.VxLanRifsConfigTunnelIpv6UnderlayOportsTest.portsToPortsConfigTest |
| tunnel.87 | Verify Tunnel Encap Decap for following Overlay/Underlay Combination: Overlay:(L3, L3 Subport, [SVI Tagged & SVI Untagged], L3 LAG) <-> Underlay:(ECMP(L3, L3 Subport, [SVI: Tagged & Untagged, Port & Lag], L3 LAG) | saitunnel.VxLanRifsConfigTunnelIpv6UnderlayOportsTest.portsToEcmpConfigTest |
| tunnel.88 | Verify Tunnel Encap Decap for following Overlay/Underlay Combination: Overlay:(L3, L3 Subport, [SVI Tagged & SVI Untagged], L3 LAG) <-> Underlay ECMP of L3 + L3 subport + SVI (tagged & untagged, Port & LAG) + L3 Lag | saitunnel.VxLanRifsConfigTunnelIpv6UnderlayOportsTest.portsToPortsEcmpConfigTest |
| tunnel.89 | Verify Tunnel Encap Decap for following Overlay/Underlay Combination: Overlay ECMP Tunnel Nexthops <-> Underlay:(L3, L3 Subport, [SVI Tagged & SVI Untagged], L3 LAG) | saitunnel.VxLanRifsConfigTunnelIpv6UnderlayOecmpTest.ecmpToPortsConfigTest |
| tunnel.90 | Verify Tunnel Encap Decap for following Overlay/Underlay Combination: Overlay ECMP Tunnel Nexthop <-> Underlay:(ECMP(L3, L3 Subport, [SVI Tagged & SVI Untagged], L3 LAG) | saitunnel.VxLanRifsConfigTunnelIpv6UnderlayOecmpTest.ecmpToEcmpConfigTest |
| tunnel.91 | Verify Tunnel Encap Decap for following Overlay/Underlay Combination: Overlay ECMP Tunnel Nexthop <-> Underlay ECMP of L3 + L3 subport + SVI (tagged & untagged) + L3 Lag | saitunnel.VxLanRifsConfigTunnelIpv6UnderlayOecmpTest.ecmpToPortsEcmpConfigTest |
| tunnel.92 | Verify Multi Tunnel Forwarding with single overlay VRF, and multiple underlay VRFs | saitunnel.SingleOvrfMultiTunnelIpv6UnderlayTest |
| tunnel.93 | Verify Multi Tunnel Forwarding with multiple overlay VRF, and multiple underlay VRFs | saitunnel.MultipleOvrfMultiTunnelIpv6UnderlayTest |
| tunnel.94 | Verify Tunnel Encap/Decap for multi VRF<->VNI mapping, with different overlay interface types (for both: IPv4 and IPv6 overlay) | saitunnel.MultipleMapperEntriesIpv6UnderlayTest |
| tunnel.95 | Verify tunnel encap/decap by dynamically adding and removing tunnel mapper entries (type = SAI_TUNNEL_MAP_TYPE_VNI_TO_VIRTUAL_ROUTER_ID and  SAI_TUNNEL_MAP_TYPE_VIRTUAL_ROUTER_ID_TO_VNI) from tunnel map object associated to tunnel object | saitunnel.MultipleMapperEntriesIpv6UnderlayTest.addDelMapperEntriesTest |
| tunnel.96 | Verify tunnel encap/decap for P2MP VxLAN Tunnels | saitunnel.VxLanP2MpTnnnelIpv6UnderlayTest |
| tunnel.97 | Verify tunnel encap/decap for P2MP IPinIP Tunnels V4 Underlay | saitunnel.IpInIpP2MpTnnnelIpv6UnderlayTest |
| tunnel.98 | For P2P VxLAN tunnel - Verify tunnel encap/decap | saitunnel.VxLanTunnelIpv6UnderlayTest.encapDecapTest |
| tunnel.99 | For P2P VxLAN tunnel - Verify tunnel de-encapsulation occurs only for tunneled packets with src IP =  SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_SRC_IP | saitunnel.VxLanTunnelIpv6UnderlayTest.tunnelTermSrcIpTest |
| | General | |
| tunnel.100 | Verify if TTL value is properly set according to SAI_TUNNEL_ATTR_ENCAP_TTL_VAL attribute in VXLAN packets | saitunnel.TunnelAtributesTest.encapTtlPipeModeVxlanTest |
| tunnel.101 | Verify if TTL value is properly set according to SAI_TUNNEL_ATTR_ENCAP_TTL_VAL attribute in IP-in-IP packets | saitunnel.TunnelAtributesTest.encapTtlPipeModeIpInIpTest |
| tunnel.102 | Verify if TTL value is properly set according to SAI_TUNNEL_ATTR_ENCAP_TTL_VAL attribute in IPv6-in-IPv6 packets | saitunnel.TunnelAtributesTest.encapTtlPipeModeIpv6InIpv6Test |
| tunnel.103 | Verify if TTL value is properly set according to the inner packet, when using pipe mode in VXLAN packets | saitunnel.TunnelAtributesTest.decapTtlPipeModeVxlanTest |
| tunnel.104 | Verify if TTL value is properly set according to the inner packet, when using pipe mode in IP-in-IP packets | saitunnel.TunnelAtributesTest.decapTtlPipeModeIpInIpTest |
| tunnel.105 | Verify if TTL value is properly set according to the inner packet, when using pipe mode in IPv6-in-IPv6 packets | saitunnel.TunnelAtributesTest.decapTtlPipeModeIpv6InIpv6Test |
| tunnel.106 | Verify if DSCP value is properly set in VXLAN packets, according to the received DSCP value | saitunnel.TunnelAtributesTest.encapDscpUniformModeVxlanTest |
| tunnel.107 | Verify if DSCP value is properly set in VXLAN packets, according to the received DSCP value | saitunnel.TunnelAtributesTest.encapDscpUniformModeIpv4InIpv4Test |
| tunnel.108 | Verify if DSCP value is properly set in VXLAN packets, according to the received DSCP value | saitunnel.TunnelAtributesTest.encapDscpUniformModeIpv6InIpv6Test |
| tunnel.109 | Verify if DSCP value is properly set according to the outer packet, when using uniform mode in VXLAN packets | saitunnel.TunnelAtributesTest.decapDscpUniformModeVxlanTest |
| tunnel.110 | Verify if DSCP value is properly set according to the outer packet, when using uniform mode in IP-in-IP packets | saitunnel.TunnelAtributesTest.decapDscpUniformModeIpv4InIpv4Test |
| tunnel.111 | Verify if DSCP value is properly set according to the outer packet, when using uniform mode in IPv6-in-IPv6 packets | saitunnel.TunnelAtributesTest.decapDscpUniformModeIpv6InIpv6Test |
| tunnel.112 | Verify if only tunneled packets with destination IP set as SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_DST_IP are de-encapsulated | saitunnel.TunnelAtributesTest.tunnelTermDstIpTest |
| tunnel.113 | Verify if only tunneled packets coming in on VRF set as SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_VR_ID are de-encapsulated | saitunnel.TunnelAtributesTest.tunnelTermVrIdTest |
| tunnel.114 | Verify that tunneled packets are still transmitted when the tunnel is defined with two encap mappers (VRF_TO_VNI and VLAN_TO_VNI) | saitunnel.TunnelAtributesTest.tunnelDualEncapMapperTest |
| tunnel.115 | Verify that tunneled packets are still received when the tunnel is defined with two decap mappers (VNI_TO_VRF and VNI_TO_VLAN) | saitunnel.TunnelAtributesTest.tunnelDualDecapMapperTest |
| tunnel.116 | Verify that tunneled packets are still received when the tunnel is defined with peer mode P2MP | saitunnel.TunnelAtributesTest.tunnelPeerModeTest |
| tunnel.117 | Verify VxLAN L3 EVPN Prefix - check if packets are properly encapsulated and decapsulated after SAI API calls emulating SONiC L3 EVPN operation, specifically after receiving EVPN Type 5 (IP prefix) routes. | saitunnel.VxLanEvpnTest.vxLanL3EvpnPrefixTest |
| tunnel.118 | Verify if TTL value is properly set in VXLAN packets, according to the received TTL value | saitunnel.TunnelTtlModeTest.encapTtlUniformModeVxlanTest |
| tunnel.119 | Verify if TTL value is properly set in IPv4-in-IPv4 packets, according to the received TTL value | saitunnel.TunnelTtlModeTest.encapTtlUniformModeIpv4InIpv4Test |
| tunnel.120 | Verify if TTL value is properly set in IPv4-in-IPv6 packets, according to the received TTL value | saitunnel.TunnelTtlModeTest.encapTtlUniformModeIpv4InIpv6Test |
| tunnel.121 | Verify if TTL value is properly set in IPv6-in-IPv4 packets, according to the received TTL value | saitunnel.TunnelTtlModeTest.encapTtlUniformModeIpv6InIpv4Test |
| tunnel.122 | Verify if TTL value is properly set in IPv6-in-IPv6 packets according to the received TTL value | saitunnel.TunnelTtlModeTest.encapTtlUniformModeIpv6InIpv6Test |
| tunnel.123 | Verify if TTL value is properly set according to the outer packet, when using uniform mode in VXLAN packets | saitunnel.TunnelTtlModeTest.decapTtlUniformModeVxlanTest |
| tunnel.124 | Verify if TTL value is properly set according to the outer packet, when using uniform mode in IPv4-in-IPv4 packets | saitunnel.TunnelTtlModeTest.decapTtlUniformModeIpv4InIpv4Test |
| tunnel.125 | Verify if TTL value is properly set according to the outer packet, when using uniform mode in IPv4-in-IPv6 packets | saitunnel.TunnelTtlModeTest.decapTtlUniformModeIpv4InIpv6Test |
| tunnel.126 | Verify if TTL value is properly set according to the outer packet, when using uniform mode in IPv6-in-IPv4 packets | saitunnel.TunnelTtlModeTest.decapTtlUniformModeIpv6InIpv4Test |
| tunnel.127 | Verify if TTL value is properly set according to the outer packet, when using uniform mode in IPv6-in-IPv6 packets | saitunnel.TunnelTtlModeTest.decapTtlUniformModeIpv6InIpv6Test |
| tunnel.128 | This verifies if DSCP value is properly set according to SAI_TUNNEL_ATTR_ENCAP_DSCP_VAL attribute | saitunnel.TunnelQosModeTest.encapDscpPipeModeVxlanTest |
| tunnel.129 | This verifies if DSCP value is properly set according to SAI_TUNNEL_ATTR_ENCAP_DSCP_VAL attribute in IP-in-IP packets | saitunnel.TunnelQosModeTest.encapDscpPipeModeIpInIpTest |
| tunnel.130 | This verifies if DSCP value is properly set according to SAI_TUNNEL_ATTR_ENCAP_DSCP_VAL attribute in IPv6-in-IPv6 packets | saitunnel.TunnelQosModeTest.encapDscpPipeModeIpv6InIpv6Test |
| tunnel.131 | This verifies if DSCP value is properly set according to the inner packet, when using pipe mode in VXLAN packets | saitunnel.TunnelQosModeTest.decapDscpPipeModeVxlanTest |
| tunnel.132 | This verifies if DSCP value is properly set according to the inner packet, when using pipe mode in IP-in-IP packets | saitunnel.TunnelQosModeTest.decapDscpPipeModeIpInIpTest |
| tunnel.133 | This verifies if DSCP value is properly set according to the inner packet, when using pipe mode in IPv6-in-IPv6 packets | saitunnel.TunnelQosModeTest.decapDscpPipeModeIpv6InIpv6Test |

## VRF

| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| vrf.1 | Verify IPv4 packets are not forwarded with admin_v4_state = false | saivrf.VrfFrowardingTest.vrfStateTest |
| vrf.2 | Verify IPv6 packets are not forwarded with admin_v4_state = false | saivrf.VrfFrowardingTest.vrfStateTest |
| vrf.3 | Verify IPv4 packets are forwarded with admin_v4_state = true | saivrf.VrfFrowardingTest.vrfStateTest |
| vrf.4 | Verify IPv6 packets are forwarded with admin_v4_state = true | saivrf.VrfFrowardingTest.vrfStateTest |
| vrf.5 | Verify multiple RIF creation of type PORT, LAG, VLAN (with tagged and untagged members), SUB_PORT and LOOPBACK for more than two different VRFs, with routes pointing to each RIF type | saivrf.VrfMultipleRifCreationTest.vrfInterfacesTest |
| vrf.6 | Verify inter-VRF forwarding with RIF pointing to regular L3 nexthop | saivrf.VrfForwardingTest.interVrfFwdL3NhopTest |
| vrf.7 | Verify inter-VRF forwarding with RIF pointing to L3 LAG nexthop | saivrf.VrfForwardingTest.interVrfFwdL3LagNhopTest |
| vrf.8 | Verify inter-VRF forwarding with RIF pointing to regular SVI nexthop | saivrf.VrfForwardingTest.interVrfFwdSviNhopTest |
| vrf.9 | Verify inter-VRF forwarding with RIF pointing to regular subport nexthop | saivrf.VrfForwardingTest.interVrfFwdSubportNhopTest |
| vrf.10 | Verify inter-VRF forwarding with RIF pointing to ECMP nexthop | saivrf.VrfForwardingTest.interVrfFwdEcmpNhopTest |
| vrf.11 | Verify inter-VRF isolation with overlapping IPv4 LPM routes | saivrf.VrfIsolationTest.overlappingIPv4LpmTest |
| vrf.12 | Verify inter-VRF isolation with overlapping IPv6 LPM routes | saivrf.VrfIsolationTest.overlappingIPv6LpmTest |
| vrf.13 | Verify inter VRF isolation with overlapping IPv4 host routes | saivrf.VrfIsolationTest.overlappingIPv4HostTest |
| vrf.14 | Verify inter VRF isolation with overlapping IPv6 host routes | saivrf.VrfIsolationTest.overlappingIPv6HostTest |
| vrf.15 | Verify ACL redirect to regular L3 nexthop in different VRF | saivrf.VrfAclRedirectTest.aclFwdL3NhopTest |
| vrf.16 | Verify ACL redirect to L3 LAG nexthop in different VRF | VrfAclRedirsaivrf.VrfAclRedirectTest.aclFwdL3LagNhopTest |
| vrf.17 | Verify ACL redirect to SVI nexthop in different VRF | saivrf.VrfAclRedirectTest.aclFwdSviNhopTest |
| vrf.18 | Verify ACL redirect to subport nexthop in different VRF | saivrf.VrfAclRedirectTest.aclFwdSubportNhopTest |
| vrf.19 | Verify ACL redirect to ECMP nexthop in different VRF | saivrf.VrfAclRedirectTest.aclFwdEcmpNhopTest |
| vrf.20 | Verify the possibility to create the numbers of VRFs equal to SAI_SWITCH_ATTR_MAX_VIRTUAL_ROUTERS | saivrf.VrfScaleTest |
| vrf.21 | Verify maximum number of VRFs with at least one route of each type (IPv4 host, IPv4 LPM, IPv6 host, IPv6 LPM) in each VRF | saivrf.VrfScaleTest |
| vrf.22 | Verify multiple RIF creation of type PORT, LAG, VLAN (with tagged and untagged members), SUB_PORT and LOOPBACK for three different VRFs, with routes pointing to each RIF type | saivrf.VrfMultipleRifCreationTest.vrfInterfacesTest |
| vrf.23 | Verify SMAC configuration on VRF create | saivrf.VrfSMACTest.testSMACCreateSet |
| vrf.24 | Verify SMAC configuration on VRF created with default mac | saivrf.VrfSMACTest.testSMACSet |

## VLAN

| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| vlan.1 | Verify forwarding from untagged to untagged port | saivlan.L2VlanTest.forwardingTest |
| vlan.2 | Verify forwarding from untagged to tagged port | saivlan.L2VlanTest.forwardingTest |
| vlan.3 | Verify forwarding from tagged to tagged port | saivlan.L2VlanTest.forwardingTest |
| vlan.4 | Verify forwarding from tagged to untagged port | saivlan.L2VlanTest.forwardingTest |
| vlan.5 | Verify forwarding of native VLAN on tagged port | saivlan.L2VlanTest.nativeVlanTest |
| vlan.6 | Verify forwarding of native VLAN on tagged LAG | saivlan.L2VlanTest.nativeVlanTest |
| vlan.7 | Verify forwarding of priority tagged packets on port | saivlan.L2VlanTest.priorityTaggingTest |
| vlan.8 | Verify forwarding of priority tagged packets on LAG | saivlan.L2VlanTest.priorityTaggingTest |
| vlan.9 | Verify drops for invalid port-VLAN packet on untagged port | saivlan.L2VlanTest.pvDropTest |
| vlan.10 | Verify drops for invalid port-VLAN packet on untagged LAG all members | saivlan.L2VlanTest.lagPvMissTest |
| vlan.11 | Verify drops for invalid port-VLAN packet on tagged port | saivlan.L2VlanTest.pvDropTest |
| vlan.12 | Verify drops for invalid port-VLAN packet on tagged LAG all members | saivlan.L2VlanTest.lagPvMissTest |
| vlan.13 | Verify flooding on tagged ports with correct tagging | saivlan.L2VlanTest.vlanFloodTest |
| vlan.14 | Verify flooding on tagged LAGs with correct tagging | saivlan.L2VlanTest.vlanFloodTest |
| vlan.15 | Verify flooding on untagged ports with no tagging | saivlan.L2VlanTest.vlanFloodTest |
| vlan.16 | Verify flooding on untagged LAGs with no tagging | saivlan.L2VlanTest.vlanFloodTest |
| vlan.17 | Verify flooding after add tagged physical port VLAN member | saivlan.L2VlanTest.vlanFloodTest |
| vlan.18 | Verify flooding after add untagged physical port VLAN member | saivlan.L2VlanTest.vlanFloodTest |
| vlan.19 | Verify flooding after add tagged LAG VLAN member | saivlan.L2VlanTest.vlanFloodTest |
| vlan.20 | Verify flooding after add untagged LAG VLAN member | saivlan.L2VlanTest.vlanFloodTest |
| vlan.21 | Verify flooding after add tagged LAG member port | saivlan.L2VlanTest.vlanFloodEnhancedTest |
| vlan.22 | Verify flooding after add untagged LAG member port | saivlan.L2VlanTest.vlanFloodEnhancedTest |
| vlan.23 | Verify flooding after remove tagged physical port VLAN member | saivlan.L2VlanTest.vlanFloodTest |
| vlan.24 | Verify flooding after remove untagged physical port VLAN member | saivlan.L2VlanTest.vlanFloodTest |
| vlan.25 | Verify flooding after remove tagged LAG VLAN member | saivlan.L2VlanTest.vlanFloodTest |
| vlan.26 | Verify flooding after remove untagged LAG VLAN member | saivlan.L2VlanTest.vlanFloodTest |
| vlan.27 | Verify flooding after remove tagged LAG member port | saivlan.L2VlanTest.vlanFloodEnhancedTest |
| vlan.28 | Verify flooding after remove untagged LAG member port | saivlan.L2VlanTest.vlanFloodEnhancedTest |
| vlan.29 | Verify ingress port pruning on ports when flooding | saivlan.L2VlanTest.vlanFloodPruneTest |
| vlan.30 | Verify ingress port pruning on LAG when flooding | saivlan.L2VlanTest.vlanFloodPruneTest |
| vlan.31 | Verify ingress Unicast/Multicast/Broadcast statistics for VLAN | saivlan.L2VlanTest.vlanStatsTest |
| vlan.32 | Verify egress Unicast/Multicast/Broadcast statistics for VLAN | saivlan.L2VlanTest.vlanStatsTest |
| vlan.33 | Verify clear statistics for VLAN | saivlan.L2VlanTest.vlanStatsTest.countersClearTest |
| vlan.34 | Verify learning disabled attribute for VLAN | saivlan.L2VlanTest.vlanLearningTest |
| vlan.35 | Verify VLAN member list using SAI_VLAN_ATTR_MEMBER_LIST | saivlan.L2VlanTest.vlanMemberList |
| vlan.36 | Verify flooding for the VLAN which contains ports and LAGs | saivlan.L2VlanTest.vlanFloodEnhancedTest |
| vlan.37 | Verify duplicate VLAN creation fails | saivlan.L2VlanTest.vlanNegativeTest |
| vlan.38 | Verify non-existent VLAN get fails | saivlan.L2VlanTest.vlanNegativeTest |
| vlan.39 | Verify non-existent VLAN remove fails | saivlan.L2VlanTest.vlanNegativeTest |
| vlan.40 | Verify non-existent VLAN set fails | saivlan.L2VlanTest.vlanNegativeTest |
| vlan.41 | Verify VLAN member add fails with invalid VLAN ID | saivlan.L2VlanTest.vlanNegativeTest |
| vlan.42 | Verify VLAN member add fails with invalid port ID | saivlan.L2VlanTest.vlanNegatieTest |
| vlan.43 | Verify packet is dropped when ingress on single VLAN member (no flood and pruned) | saivlan.L2VlanTest.singleVlanMemberTest |
| vlan.44 | Verify flood control attributes for VLAN ('all' and 'none') | saivlan.L2VlanTest.vlanFloodDisableTest |
| vlan.45 | Verify binding VLAN to egress ACL table | saivlan.L2VlanTest.vlanEgressAcl |
| vlan.46 | Verify binding VLAN to ingress ACL table | saivlan.L2VlanTest.vlanIngressAcl |
| vlan.47 | Verify disable learn on VLAN if mac entries in fdb table equals or more than MaxLearnedAddresses attribute | saivlan.L2VlanTest.vlanMacLearnedAddressesTest |

## WRED

| Test case id | Description | Test name |
| ------------ | ----------- | --------- |
| wred.1 | Verify WRED profile bind to a queue with no prior WRED profile | saiwred.WredTest.WredBindProfileTest |
| wred.2 | Verify WRED profile bind to a queue with an existing WRED profile | saiwred.WredTest.WredReplaceProfileTest |
| wred.3 | Verify WRED profile can be bound to multiple queues | saiwred.WredTest.WredBindProfileMultipleQueuesTest |
| wred.4 | Verify non-ECN traffic is unaffected by WRED profile on a queue | saiwred.WredTest.WredIPv4NonECNTest |
