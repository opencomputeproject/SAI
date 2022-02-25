import sys
sys.path.append('../sai_thrift_src/gen-py')
sys.path.append('../')
sys.path.append('../../../p4-softswitch/tools/')
from sai_types import *
from cli_driver import SwitchThriftClient
from switch_sai import switch_sai_rpc
from switch_sai.ttypes import *

from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol
from thrift.server import TServer


import socket

def list_to_str(num_list):
  st = ''
  for num in num_list:
    st = st + str(num) + ' '
  return st


def GetNewIndex(num_list):
  return min(set(xrange(len(num_list)+1)) - set(num_list))

def CreateNewItem(obj_list, obj_class, forbidden_list=[]):
    new_id = GetNewIndex(forbidden_list)
    new_obj = obj_class(sai_object_id=new_id)
    obj_list[new_id] = new_obj
    # obj_list.update({new_id: new_obj})
    return new_id, new_obj

class Sai_obj():
    def __init__(self, sai_object_id):
        self.sai_object_id = sai_object_id


class Port_obj(Sai_obj):
    def __init__(self, sai_object_id, l2_if=0, hw_port=0, pvid=1, mtu=1512, drop_tagged=0, drop_untagged=0, bind_mode=SAI_PORT_BIND_MODE_PORT):
        Sai_obj.__init__(self, sai_object_id)
        self.l2_if = l2_if
        self.hw_port = hw_port
        self.pvid = pvid
        self.bind_mode = bind_mode
        self.mtu = mtu
        self.drop_tagged = drop_tagged
        self.drop_untagged = drop_untagged



class Lag_obj(Sai_obj):
    def __init__(self, sai_object_id, l2_if = 0, lag_members=[], port_obj=None):
      Sai_obj.__init__(self, sai_object_id)
      self.l2_if = l2_if
      self.lag_members = lag_members
      self.port_obj = port_obj


class LagMember_obj(Sai_obj):
    def __init__(self, sai_object_id, port_id=0,lag_id=0,hw_port=0):
        Sai_obj.__init__(self, sai_object_id)
        self.port_id = port_id
        self.lag_id = lag_id
        self.hw_port = hw_port


class VlanMember_obj(Sai_obj):
    def __init__(self, sai_object_id, vid=1, bridge_port_id=0,vlan_oid=0,tagging_mode=0):
        Sai_obj.__init__(self, sai_object_id)
        self.vlan_oid = vlan_oid 
        self.vid = vid
        self.bridge_port_id = bridge_port_id
        self.tagging_mode = tagging_mode


class Vlan_obj(Sai_obj):
    def __init__(self, sai_object_id, vid=1, vlan_members=None):
        Sai_obj.__init__(self, sai_object_id)
        self.vid = vid
        self.vlan_members = vlan_members


class BridgePort_obj(Sai_obj):
    def __init__(self, sai_object_id, bridge_port=0, port_id=0, vlan_id=1, br_port_type=SAI_BRIDGE_PORT_TYPE_PORT, bridge_id=1):
        Sai_obj.__init__(self, sai_object_id)
        self.bridge_port = bridge_port
        self.port_id = port_id
        self.vlan_id = vlan_id
        self.br_port_type = br_port_type
        self.bridge_id = bridge_id


class Bridge_obj(Sai_obj):
    def __init__(self, sai_object_id, bridge_id=1, bridge_type=SAI_BRIDGE_TYPE_1Q, bridge_port_list=[]):
        Sai_obj.__init__(self, sai_object_id)
        self.bridge_id = bridge_id
        self.bridge_type = bridge_type
        self.bridge_port_list = bridge_port_list


class SaiHandler():
  def __init__(self):
    self.switch_id = 0
    self.log = {}
    print "connecting to cli thrift"
    self.cli_client = SwitchThriftClient(json='../../../p4-softswitch/targets/P4-SAI/sai.json',default_config='../../../p4-softswitch/targets/P4-SAI/p4src/DefaultConfig.txt')
    self.hw_port_list = [0, 1, 2, 3, 4, 5, 6, 7]
    self.sai_thrift_create_switch([])

  def get_all_oids(self):
    return self.ports.keys() + self.vlans.keys() + self.vlan_members.keys() + self.bridge_ports.keys() + self.bridges.keys() + self.lag_members.keys() + self.lags.keys()

  def get_new_l2_if(self):
    l2_ifs = [x.l2_if for x in self.ports.values() + self.lags.values()]
    return GetNewIndex(l2_ifs)

  def get_new_bridge_id(self):
    bridge_ids = [x.bridge_id for x in self.bridges.values()]
    return GetNewIndex(bridge_ids)
  
  def get_new_bridge_port(self):
    bridge_ports = [x.bridge_port for x in self.bridge_ports.values()]
    return GetNewIndex(bridge_ports)

  # Switch API
  def sai_thrift_create_switch(self, thrift_attr_list):
    # self.cli_client.ReloadDefaultConfig()
    self.ports = {}
    self.vlans = {}
    self.vlan_members = {}
    self.bridge_ports = {}
    self.bridges = {}
    self.lag_members = {}
    self.lags = {}
    self.l2_ifs = []
    bridge_object_id, bridge_obj = CreateNewItem(self.bridges, Bridge_obj, forbidden_list=self.get_all_oids())
    bridge_obj.bridge_id = 1
    for port_num in self.hw_port_list:
      port_id, port_obj = CreateNewItem(self.ports, Port_obj, forbidden_list=self.get_all_oids())
      port_obj.hw_port = port_num
      port_obj.l2_if = port_num
      br_port_id, br_port_obj = CreateNewItem(self.bridge_ports, BridgePort_obj, forbidden_list=self.get_all_oids())
      br_port_obj.port_id = port_id
      br_port_obj.bridge_port = port_num
      bridge_obj.bridge_port_list.append(br_port_id)
    return self.switch_id

  def sai_thrift_get_switch_attribute(self, thrift_attr_list):
    for attr in thrift_attr_list:
      if attr.id == SAI_SWITCH_ATTR_DEFAULT_1Q_BRIDGE_ID:
        attr.value.oid = self.bridges[0].sai_object_id
    for attr in thrift_attr_list:
      if attr.id == SAI_SWITCH_ATTR_PORT_LIST:
        attr.value.objlist = sai_thrift_object_list_t(count=len(self.ports.keys()), object_id_list=self.ports.keys())

    return sai_thrift_attribute_list_t(attr_list=thrift_attr_list, attr_count = len(thrift_attr_list))

  def sai_thrift_get_port_id_by_front_port(self, port_name):
    for port in self.ports.values():
      if port.hw_port == int(port_name):
        return port.sai_object_id
    return -1

  # FDB API
  def sai_thrift_create_fdb_entry(self, thrift_fdb_entry, thrift_attr_list):
    # fdb_entry = sai_thrift_fdb_entry_t(mac_address=mac, vlan_id=vlan_id)
    for attr in thrift_attr_list:
      if attr.id == SAI_FDB_ENTRY_ATTR_TYPE:
        entry_type = attr.value.s32
      elif attr.id == SAI_FDB_ENTRY_ATTR_BRIDGE_PORT_ID:
        bridge_port = self.bridge_ports[attr.value.oid].bridge_port
      elif attr.id == SAI_FDB_ENTRY_ATTR_PACKET_ACTION:
        packet_action = attr.value.s32
    bridge_type = thrift_fdb_entry.bridge_type
    bridge_id = self.bridges[thrift_fdb_entry.bridge_id].bridge_id
    mac = thrift_fdb_entry.mac_address
    vlan_id = thrift_fdb_entry.vlan_id
    match_str = thrift_fdb_entry.mac_address + ' ' + str(bridge_id)
    action_str = str(bridge_port)
    if packet_action == SAI_PACKET_ACTION_FORWARD:
      if entry_type == SAI_FDB_ENTRY_TYPE_STATIC:
        self.cli_client.AddTable('table_fdb', 'action_set_egress_br_port', match_str, action_str)
    return 0

  def sai_thrift_delete_fdb_entry(self, thrift_fdb_entry):
    match_str = thrift_fdb_entry.mac_address + ' ' + str(self.bridges[thrift_fdb_entry.bridge_id].bridge_id)
    self.cli_client.RemoveTableEntry('table_fdb', match_str)
    return 0

  # VLAN API
  def sai_thrift_create_vlan(self, thrift_attr_list):
    for attr in thrift_attr_list:
      if attr.id == SAI_VLAN_ATTR_VLAN_ID:
        vid = attr.value.u16
    if vid in [x.vid for x in self.vlans]:
      print "vlan id %d already exists" % vid
      return SAI_STATUS_ITEM_ALREADY_EXISTS
    else:
      print "vlan id %d created" % vid
      vlan_oid, vlan_obj = CreateNewItem(self.vlans, Vlan_obj, forbidden_list=self.get_all_oids())
      vlan_obj.vid = vid
      vlan_obj.vlan_members = []
      return vlan_oid

  def sai_thrift_delete_vlan(self, vlan_oid):
    self.vlans.pop(vlan_oid, None)
    return 0

  def sai_thrift_remove_vlan_member(self, vlan_member_id):
    vlan_member = self.vlan_members[vlan_member_id]
    bridge_port_id = vlan_member.bridge_port_id
    vid = vlan_member.vid
    self.cli_client.RemoveTableEntry('table_egress_vlan_filtering', list_to_str([bridge_port_id, vid]))
    self.cli_client.RemoveTableEntry('table_ingress_vlan_filtering', list_to_str([bridge_port_id, vid]))
    out_if = self.ports[self.bridge_ports[bridge_port_id].port_id].hw_port
    if vlan_member.tagging_mode == SAI_VLAN_TAGGING_MODE_UNTAGGED:
      self.cli_client.RemoveTableEntry('table_egress_vlan_tag', list_to_str([out_if, vid, 1]))
    else:
      self.cli_client.RemoveTableEntry('table_egress_vlan_tag', list_to_str([out_if, vid, 0]))
    self.vlans[vlan_member.vlan_oid].vlan_members.remove(vlan_member_id)
    self.vlan_members.pop(vlan_member_id, None)
    return 0

  def sai_thrift_create_vlan_member(self, vlan_member_attr_list):
    # SAI_VLAN_TAGGING_MODE_TAGGED
    # SAI_VLAN_TAGGING_MODE_UNTAGGED
    for attr in vlan_member_attr_list:
      if attr.id == SAI_VLAN_MEMBER_ATTR_VLAN_ID:
        vlan_oid = attr.value.oid
      elif attr.id == SAI_VLAN_MEMBER_ATTR_BRIDGE_PORT_ID:
        bridge_port_id = attr.value.oid
      elif attr.id == SAI_VLAN_MEMBER_ATTR_VLAN_TAGGING_MODE:
        tagging_mode = attr.value.s32
    vlan_obj = self.vlans[vlan_oid]
    vlan_id = vlan_obj.vid
    vlan_member_id, vlan_member_obj = CreateNewItem(self.vlan_members, VlanMember_obj, forbidden_list=self.get_all_oids())
    vlan_member_obj.bridge_port_id = bridge_port_id
    vlan_member_obj.vid = vlan_id
    vlan_member_obj.vlan_oid = vlan_oid
    vlan_member_obj.tagging_mode = tagging_mode
    vlan_obj.vlan_members.append(vlan_member_id)

    port_id = self.bridge_ports[bridge_port_id].port_id
    bridge_port = self.bridge_ports[bridge_port_id].bridge_port
    if port_id in self.lags.keys():
      out_if = self.lags[port_id].l2_if
    else:
      out_if = self.ports[port_id].hw_port

    if tagging_mode == SAI_VLAN_TAGGING_MODE_TAGGED:
      vlan_pcp = 0 
      vlan_cfi = 0
      self.cli_client.AddTable('table_egress_vlan_tag','action_forward_vlan_tag',
                               list_to_str([out_if, vlan_id, 0]), list_to_str([vlan_pcp, vlan_cfi, vlan_id]))
    elif tagging_mode == SAI_VLAN_TAGGING_MODE_PRIORITY_TAGGED:
      vlan_pcp = 0 
      vlan_cfi = 0
      self.cli_client.AddTable('table_egress_vlan_tag','action_forward_vlan_tag',
                               list_to_str([out_if, vlan_id, 0]), list_to_str([vlan_pcp, vlan_cfi, 0]))
    else:
      self.cli_client.AddTable('table_egress_vlan_tag','action_forward_vlan_untag',
                              list_to_str([out_if, vlan_id, 1]),'')

    self.cli_client.AddTable('table_egress_vlan_filtering','_nop',
                              list_to_str([bridge_port, vlan_id]),'')
    self.cli_client.AddTable('table_ingress_vlan_filtering','_nop',
                              list_to_str([bridge_port, vlan_id]),'')

    return vlan_member_id

  # Port API
  def config_port(self, port_obj,l2_if):
    self.cli_client.RemoveTableEntry('table_port_configurations', str(l2_if))
    self.cli_client.AddTable('table_port_configurations', 'action_set_port_configurations',str(l2_if), list_to_str([port_obj.pvid, port_obj.bind_mode, port_obj.mtu,
                                                                                                                   port_obj.drop_tagged, port_obj.drop_untagged]))

  def sai_thrift_create_port(self, thrift_attr_list):
    port, port_obj = CreateNewItem(self.ports, Port_obj, forbidden_list=self.get_all_oids())
    for attr in thrift_attr_list:
      if attr.id == SAI_PORT_ATTR_PORT_VLAN_ID:
        vlan_id = attr.value.u16
        port_obj.pvid = vlan_id
      elif attr.id == SAI_PORT_ATTR_BIND_MODE:
        bind_mode = attr.value.s32
        port_obj.bind_mode = bind_mode
      elif attr.id == SAI_PORT_ATTR_HW_LANE_LIST:
        hw_port_list = attr.value.u32list.u32list
      # TODO: Add MTU and drop tagged, untagged
    hw_port = hw_port_list[0]
    port_obj.hw_port = hw_port
    port_obj.l2_if = self.get_new_l2_if()
    self.cli_client.AddTable('table_ingress_lag', 'action_set_lag_l2if', str(port_obj.hw_port), list_to_str([0,port_obj.l2_if]))
    self.config_port(port_obj, port_obj.l2_if)
    return port

  def sai_thrift_remove_port(self, port_id):
    hw_port = self.ports[port_id].hw_port
    l2_if = self.ports[port_id].l2_if
    self.cli_client.RemoveTableEntry('table_ingress_lag', str(hw_port))
    self.cli_client.RemoveTableEntry('table_port_configurations', str(l2_if))
    self.ports.pop(port_id, None)
    return 0

  def sai_thrift_set_port_attribute(self, port, attr):
    if port in self.ports:
      port_obj = self.ports[port]
    else: 
      port_obj = self.lags[port].port_obj

    if attr.id == SAI_PORT_ATTR_PORT_VLAN_ID:
      port_obj.pvid = attr.value.u16
    if attr.id == SAI_PORT_ATTR_BIND_MODE:
      port_obj.bind_mode = attr.value.s32
    if attr.id == SAI_PORT_ATTR_DROP_UNTAGGED:
      port_obj.drop_untagged = int(attr.value.booldata)
    if attr.id == SAI_PORT_ATTR_DROP_TAGGED:
      port_obj.drop_tagged = int(attr.value.booldata)

    self.config_port(port_obj, port_obj.l2_if)
    return 0

  def sai_thrift_get_port_attribute(self, port_id, thrift_attr_list):
    if port_id in self.lags:
      port_obj = self.lags[port_id].port_obj
    else:
      port_obj = self.ports[port_id]
      
    for attr in thrift_attr_list:
      if attr.id == SAI_PORT_ATTR_HW_LANE_LIST:
        hw_port = port_obj.hw_port
        attr.value.u32list = sai_thrift_u32_list_t(u32list=[hw_port], count=1)
    return sai_thrift_attribute_list_t(attr_list=thrift_attr_list, attr_count = len(thrift_attr_list))

  # LAG Api
  def sai_thrift_create_lag(self, thrift_attr_list):
    lag_id, lag_obj = CreateNewItem(self.lags, Lag_obj, forbidden_list=self.get_all_oids())
    lag_obj.l2_if = self.get_new_l2_if()
    return lag_id

  def sai_thrift_remove_lag(self, lag_id):
    lag = self.lags.pop(lag_id, None)
    self.cli_client.RemoveTableEntry('table_port_configurations', str(lag.l2_if))
    self.cli_client.RemoveTableEntry('table_lag_hash',str(lag.l2_if))
    return 0

  def sai_thrift_create_lag_member(self, thrift_attr_list):
    lag_member_id, lag_member_obj = CreateNewItem(self.lag_members, LagMember_obj, forbidden_list=self.get_all_oids())
    for attr in thrift_attr_list:
      if attr.id == SAI_LAG_MEMBER_ATTR_PORT_ID:
        port_id = attr.value.oid
        lag_member_obj.port_id = port_id
        port = self.ports[port_id]
      if attr.id == SAI_LAG_MEMBER_ATTR_LAG_ID:
        lag_id = attr.value.oid
        lag_member_obj.lag_id = lag_id
        lag = self.lags[lag_id]
        lag.lag_members.append(lag_member_id)
    lag.port_obj = port
    lag_member_obj.hw_port = port.hw_port
    l2_if = lag.l2_if
    self.cli_client.RemoveTableEntry('table_ingress_lag', str(lag_member_obj.hw_port))
    self.cli_client.AddTable('table_ingress_lag', 'action_set_lag_l2if', str(lag_member_obj.hw_port), list_to_str([1, lag_id]))
    self.cli_client.AddTable('table_egress_lag', 'action_set_out_port', list_to_str([l2_if, len(self.lags[lag_id].lag_members)-1]), str(lag_member_obj.hw_port))
    self.cli_client.RemoveTableEntry('table_lag_hash',str(l2_if))
    self.cli_client.AddTable('table_lag_hash', 'action_set_lag_hash_size', str(l2_if), str(len(self.lags[lag_id].lag_members)))
    self.config_port(lag.port_obj, lag.l2_if)
    return lag_member_id

  def sai_thrift_remove_lag_member(self, lag_member_id):
    lag_member = self.lag_members.pop(lag_member_id, None)
    if not lag_member:
      return 0
    lag = self.lags[lag_member.lag_id]
    hash_ind = lag.lag_members.index(lag_member_id)
    del lag.lag_members[hash_ind]
    self.cli_client.RemoveTableEntry('table_lag_hash',str(lag.l2_if))
    self.cli_client.RemoveTableEntry('table_ingress_lag', str(lag_member.hw_port))
    self.cli_client.AddTable('table_lag_hash', 'action_set_lag_hash_size', str(lag.l2_if), str(len(lag.lag_members)))
    self.cli_client.RemoveTableEntry('table_egress_lag', list_to_str([lag.l2_if, hash_ind]))
    if hash_ind!=len(lag.lag_members):
      self.cli_client.RemoveTableEntry('table_egress_lag', list_to_str([lag.l2_if, len(lag.lag_members)]))
      last_lag_member_id = lag.lag_members.pop()
      lag.lag_members.insert(hash_ind, last_lag_member_id)
      last_lag_member = self.lag_members[last_lag_member_id]
      self.cli_client.AddTable('table_egress_lag', 'action_set_out_port', list_to_str([lag.l2_if, hash_ind]), str(last_lag_member.hw_port))
    return 0


  # Bridge API
  def sai_thrift_create_bridge(self, thrift_attr_list):
    bridge_id, bridge_obj = CreateNewItem(self.bridges, Bridge_obj, forbidden_list=self.get_all_oids())
    bridge_obj.bridge_id = self.get_new_bridge_id()
    for attr in thrift_attr_list:
      if attr.id == SAI_BRIDGE_ATTR_TYPE:
        bridge_type = attr.value.s32
        bridge_obj.bridge_type = bridge_type
    return bridge_id

  def sai_thrift_remove_bridge(self, bridge_id):
    self.bridges.pop(bridge_id, None)
    return 0

  def sai_thirft_get_bridge_attribute(self, bridge_id, thrift_attr_list):
    for attr in thrift_attr_list:
      if attr.id == SAI_BRIDGE_ATTR_PORT_LIST:
        attr.value.objlist =  sai_thrift_object_list_t(object_id_list = self.bridges[bridge_id].bridge_port_list,count = len(self.bridges[bridge_id].bridge_port_list))
      if attr.id == SAI_BRIDGE_ATTR_TYPE:
        attr.value.s32 = self.bridges[bridge_id].bridge_type
    return sai_thrift_attribute_list_t(attr_list=thrift_attr_list, attr_count = len(thrift_attr_list))

  def sai_thrift_create_bridge_port(self, thrift_attr_list):
    for attr in thrift_attr_list:
      if attr.id == SAI_BRIDGE_PORT_ATTR_VLAN_ID:
        vlan_id = attr.value.u16
      elif attr.id == SAI_BRIDGE_PORT_ATTR_BRIDGE_ID:
        bridge_sai_object_id = attr.value.oid
        bridge_id = self.bridges[bridge_sai_object_id].bridge_id
      elif attr.id == SAI_BRIDGE_PORT_ATTR_TYPE:
        bridge_port_type = attr.value.s32
      elif attr.id == SAI_BRIDGE_PORT_ATTR_PORT_ID:
        port_id = attr.value.oid
    br_port = self.get_new_bridge_port()
    br_port_id, br_port_obj = CreateNewItem(self.bridge_ports, BridgePort_obj, forbidden_list=self.get_all_oids())
    br_port_obj.bridge_port = br_port
    br_port_obj.port_id = port_id
    br_port_obj.vlan_id = vlan_id
    br_port_obj.br_port_type = bridge_port_type
    br_port_obj.bridge_id = bridge_id
    if bridge_port_type == SAI_BRIDGE_PORT_TYPE_SUB_PORT: #.1D
      self.cli_client.AddTable('table_bridge_id_1d', 'action_set_bridge_id', str(br_port), str(bridge_id))
      self.cli_client.AddTable('table_egress_set_vlan', 'action_set_vlan', str(br_port), str(vlan_id))
      l2_if_type = 2
    elif bridge_port_type == SAI_BRIDGE_PORT_TYPE_PORT: #.1Q
      self.cli_client.AddTable('table_bridge_id_1q', 'action_set_bridge_id', str(vlan_id), str(bridge_id))
      l2_if_type = 3 
    if port_id in self.lags.keys(): # LAG
      l2_if = self.lags[port_id].l2_if
      self.cli_client.AddTable('table_egress_br_port_to_if', 'action_forward_set_outIfType', str(br_port), list_to_str([l2_if, 1]))
      bind_mode = self.lags[port_id].port_obj.bind_mode
    else: # port
      l2_if = self.ports[port_id].l2_if
      hw_port = self.ports[port_id].hw_port
      self.cli_client.AddTable('table_egress_br_port_to_if', 'action_forward_set_outIfType', str(br_port), list_to_str([hw_port, 0]))
      bind_mode = self.ports[port_id].bind_mode
    if bind_mode == SAI_PORT_BIND_MODE_SUB_PORT:
      self.cli_client.AddTable('table_subport_ingress_interface_type', 'action_set_l2_if_type', list_to_str([l2_if, vlan_id]), list_to_str([l2_if_type, br_port]))
    else:
      self.cli_client.AddTable('table_port_ingress_interface_type', 'action_set_l2_if_type', str(l2_if), list_to_str([l2_if_type, br_port]))

    return br_port_id

  def config_bridge_port(self, bridge_port_obj):
    br_port = bridge_port_obj.bridge_port
    vlan_id = bridge_port_obj.vlan_id
    bridge_port_type = bridge_port_obj.br_port_type
    bridge_id = bridge_port_obj.bridge_id
    if bridge_port_type == SAI_BRIDGE_PORT_TYPE_SUB_PORT: #.1D
      self.cli_client.AddTable('table_bridge_id_1d', 'action_set_bridge_id', str(br_port), str(bridge_id))
    elif bridge_port_type == SAI_BRIDGE_PORT_TYPE_PORT: #.1Q
      self.cli_client.AddTable('table_bridge_id_1q', 'action_set_bridge_id', str(vlan_id), str(bridge_id))
    return SAI_STATUS_SUCCESS

  def sai_thrift_remove_bridge_port(self, bridge_port_id):
    br_port_obj = self.bridge_ports.pop(bridge_port_id, None)
    bridge_port = br_port_obj.bridge_port
    port_id = br_port_obj.port_id
    vlan_id = br_port_obj.vlan_id
    self.cli_client.RemoveTableEntry('table_egress_br_port_to_if', str(bridge_port))
    if port_id in self.lags:
      l2_if = self.lags[port_id].l2_if
      bind_mode = self.lags[port_id].port_obj.bind_mode
    else:
      l2_if = self.ports[port_id].l2_if
      bind_mode = self.ports[port_id].bind_mode
    if bind_mode == SAI_PORT_BIND_MODE_SUB_PORT:
      self.cli_client.RemoveTableEntry('table_subport_ingress_interface_type', list_to_str([l2_if, vlan_id]))
    else:
      self.cli_client.RemoveTableEntry('table_port_ingress_interface_type', list_to_str([l2_if]))
    if br_port_obj.br_port_type == SAI_BRIDGE_PORT_TYPE_SUB_PORT: #.1D 
      self.cli_client.RemoveTableEntry('table_bridge_id_1d', str(bridge_port))
      self.cli_client.RemoveTableEntry('table_egress_set_vlan', str(bridge_port))
    else:
      self.cli_client.RemoveTableEntry('table_bridge_id_1q', str(vlan_id))
    return SAI_STATUS_SUCCESS

  def remove_bridge_port_config(self, br_port_obj):
    bridge_port = br_port_obj.bridge_port
    vlan_id = br_port_obj.vlan_id
    br_port_type = br_port_obj.br_port_type
    
    if br_port_type == SAI_BRIDGE_PORT_TYPE_SUB_PORT: #.1D 
      self.cli_client.RemoveTableEntry('table_bridge_id_1d', str(bridge_port))
    else:
      self.cli_client.RemoveTableEntry('table_bridge_id_1q', str(vlan_id))
    return SAI_STATUS_SUCCESS

  def sai_thirft_get_bridge_port_attribute(self, bridge_port_id, thrift_attr_list):
    bridge_port_obj = self.bridge_ports[bridge_port_id]
    for attr in thrift_attr_list:
      if attr.id == SAI_BRIDGE_PORT_ATTR_PORT_ID:
        attr.value.oid = bridge_port_obj.port_id
      if attr.id == SAI_BRIDGE_PORT_ATTR_VLAN_ID:
        attr.value.u16 = bridge_port_obj.vlan_id
      if attr.id == SAI_BRIDGE_PORT_ATTR_TYPE:
        attr.value.s32 = bridge_port_obj.br_port_type
    return sai_thrift_attribute_list_t(attr_list=thrift_attr_list, attr_count = len(thrift_attr_list))

  def sai_thrift_set_bridge_port_attribute(self, bridge_port_id, attr):
    bridge_port_obj = self.bridge_ports[bridge_port_id]
    self.remove_bridge_port_config(bridge_port_obj)
    if attr.id == SAI_BRIDGE_PORT_ATTR_BRIDGE_ID:
      bridge_port_obj.bridge_id = self.bridges[attr.value.oid].bridge_id
    self.config_bridge_port(bridge_port_obj)
    return SAI_STATUS_SUCCESS


handler = SaiHandler()
processor = switch_sai_rpc.Processor(handler)
transport = TSocket.TServerSocket(port=9092)
tfactory = TTransport.TBufferedTransportFactory()
pfactory = TBinaryProtocol.TBinaryProtocolFactory()

server = TServer.TSimpleServer(processor, transport, tfactory, pfactory)
print "Starting python server..."
server.serve()
print "done!"