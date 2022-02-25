#include "../inc/sai_adapter_interface.h"
#include "../inc/sai_adapter.h"
#include "../inc/switch_meta_data.h"

extern "C" {
S_O_Handle create_sai_adapter() { 
  return new sai_adapter();
}
void free_sai_adapter(S_O_Handle p) {
  sai_adapter *q = (sai_adapter *)p;
  // q->~sai_adapter();
  delete q;
}

// API
sai_status_t sai_adapter_api_query(S_O_Handle p, sai_api_t sai_api_id,
                                   void **api_method_table) {
  sai_adapter *q = (sai_adapter *)p;
  return q->sai_api_query(sai_api_id, api_method_table);
}
// SWITCH
sai_status_t sai_adapter_create_switch(S_O_Handle p, sai_object_id_t *switch_id,
                                       uint32_t attr_count,
                                       const sai_attribute_t *attr_list) {
  sai_adapter *q = (sai_adapter *)p;
  return q->create_switch(switch_id, attr_count, attr_list);
}
sai_status_t sai_adapter_get_switch_attribute(S_O_Handle p,
                                              sai_object_id_t switch_id,
                                              sai_uint32_t attr_count,
                                              sai_attribute_t *attr_list) {
  sai_adapter *q = (sai_adapter *)p;
  return q->get_switch_attribute(switch_id, attr_count, attr_list);
}

// PORT
sai_status_t sai_adapter_create_port(S_O_Handle p, sai_object_id_t *port_id,
                                     sai_object_id_t switch_id,
                                     uint32_t attr_count,
                                     const sai_attribute_t *attr_list) {
  sai_adapter *q = (sai_adapter *)p;
  return q->create_port(port_id, switch_id, attr_count, attr_list);
}

sai_status_t sai_adapter_remove_port(S_O_Handle p, sai_object_id_t port_id) {
  sai_adapter *q = (sai_adapter *)p;
  return q->remove_port(port_id);
}

sai_status_t sai_adapter_set_port_attribute(S_O_Handle p,
                                            sai_object_id_t port_id,
                                            sai_attribute_t *attr) {
  sai_adapter *q = (sai_adapter *)p;
  return q->set_port_attribute(port_id, attr);
}

sai_status_t sai_adapter_get_port_attribute(S_O_Handle p,
                                            sai_object_id_t port_id,
                                            uint32_t attr_count,
                                            sai_attribute_t *attr_list) {
  sai_adapter *q = (sai_adapter *)p;
  return q->get_port_attribute(port_id, attr_count, attr_list);
}

// BRIDGE
sai_status_t sai_adapter_create_bridge(S_O_Handle p, sai_object_id_t *bridge_id,
                                       sai_object_id_t switch_id,
                                       uint32_t attr_count,
                                       const sai_attribute_t *attr_list) {
  sai_adapter *q = (sai_adapter *)p;
  return q->create_bridge(bridge_id, switch_id, attr_count, attr_list);
}
sai_status_t sai_adapter_remove_bridge(S_O_Handle p,
                                       sai_object_id_t bridge_id) {
  sai_adapter *q = (sai_adapter *)p;
  return q->remove_bridge(bridge_id);
}
sai_status_t get_bridge_attribute(S_O_Handle p, sai_object_id_t bridge_id,
                                  uint32_t attr_count,
                                  sai_attribute_t *attr_list) {
  sai_adapter *q = (sai_adapter *)p;
  return q->get_bridge_attribute(bridge_id, attr_count, attr_list);
}

// BRIDGE PORT
sai_status_t sai_adapter_create_bridge_port(S_O_Handle p,
                                            sai_object_id_t *bridge_port_id,
                                            sai_object_id_t switch_id,
                                            uint32_t attr_count,
                                            const sai_attribute_t *attr_list) {
  sai_adapter *q = (sai_adapter *)p;
  return q->create_bridge_port(bridge_port_id, switch_id, attr_count,
                               attr_list);
}
sai_status_t sai_adapter_remove_bridge_port(S_O_Handle p,
                                            sai_object_id_t bridge_port_id) {
  sai_adapter *q = (sai_adapter *)p;
  return q->remove_bridge_port(bridge_port_id);
}
sai_status_t get_bridge_port_attribute(S_O_Handle p,
                                       sai_object_id_t bridge_port_id,
                                       uint32_t attr_count,
                                       sai_attribute_t *attr_list) {
  sai_adapter *q = (sai_adapter *)p;
  return q->get_bridge_port_attribute(bridge_port_id, attr_count, attr_list);
}

// FDB
sai_status_t sai_adapter_create_fdb_entry(S_O_Handle p,
                                          sai_fdb_entry_t *fdb_entry,
                                          uint32_t attr_count,
                                          const sai_attribute_t *attr_list) {
  sai_adapter *q = (sai_adapter *)p;
  return q->create_fdb_entry(fdb_entry, attr_count, attr_list);
}
sai_status_t sai_adapter_remove_fdb_entry(S_O_Handle p,
                                          sai_fdb_entry_t *fdb_entry) {
  sai_adapter *q = (sai_adapter *)p;
  return q->remove_fdb_entry(fdb_entry);
}

// VLAN
sai_status_t sai_adapter_create_vlan(S_O_Handle p, sai_object_id_t *vlan_id,
                                     sai_object_id_t switch_id,
                                     uint32_t attr_count,
                                     const sai_attribute_t *attr_list) {
  sai_adapter *q = (sai_adapter *)p;
  return q->create_vlan(vlan_id, switch_id, attr_count, attr_list);
}
sai_status_t sai_adapter_remove_vlan(S_O_Handle p, sai_object_id_t vlan_id) {
  sai_adapter *q = (sai_adapter *)p;
  return q->remove_vlan(vlan_id);
}

sai_status_t sai_adapter_set_vlan_attribute(S_O_Handle p,
                                            sai_object_id_t vlan_id,
                                            const sai_attribute_t *attr) {
  sai_adapter *q = (sai_adapter *)p;
  return q->set_vlan_attribute(vlan_id, attr);
}

sai_status_t sai_adapter_get_vlan_attribute(S_O_Handle p,
                                            sai_object_id_t vlan_id,
                                            const uint32_t attr_count,
                                            sai_attribute_t *attr_list) {
  sai_adapter *q = (sai_adapter *)p;
  return q->get_vlan_attribute(vlan_id, attr_count, attr_list);
}

sai_status_t sai_adapter_create_vlan_member(S_O_Handle p,
                                            sai_object_id_t *vlan_member_id,
                                            sai_object_id_t switch_id,
                                            uint32_t attr_count,
                                            const sai_attribute_t *attr_list) {
  sai_adapter *q = (sai_adapter *)p;
  return q->create_vlan_member(vlan_member_id, switch_id, attr_count,
                               attr_list);
}
sai_status_t sai_adapter_remove_vlan_member(S_O_Handle p,
                                            sai_object_id_t vlan_member_id) {
  sai_adapter *q = (sai_adapter *)p;
  return q->remove_vlan_member(vlan_member_id);
}
sai_status_t sai_adapter_set_vlan_member_attribute(
    S_O_Handle p, sai_object_id_t vlan_member_id, const sai_attribute_t *attr) {
  sai_adapter *q = (sai_adapter *)p;
  return q->set_vlan_member_attribute(vlan_member_id, attr);
}
sai_status_t sai_adapter_get_vlan_member_attribute(
    S_O_Handle p, sai_object_id_t vlan_member_id, const uint32_t attr_count,
    sai_attribute_t *attr_list) {
  sai_adapter *q = (sai_adapter *)p;
  return q->get_vlan_member_attribute(vlan_member_id, attr_count, attr_list);
}
sai_status_t sai_adapter_get_vlan_stats(S_O_Handle p, sai_object_id_t vlan_id,
                                        const sai_vlan_stat_t *counter_ids,
                                        uint32_t number_of_counters,
                                        uint64_t *counters) {
  sai_adapter *q = (sai_adapter *)p;
  return q->get_vlan_stats(vlan_id, counter_ids, number_of_counters, counters);
}
sai_status_t sai_adapter_clear_vlan_stats(S_O_Handle p, sai_object_id_t vlan_id,
                                          const sai_vlan_stat_t *counter_ids,
                                          uint32_t number_of_counters) {
  sai_adapter *q = (sai_adapter *)p;
  return q->clear_vlan_stats(vlan_id, counter_ids, number_of_counters);
}

// LAG
sai_status_t sai_adapter_create_lag(S_O_Handle p, sai_object_id_t *lag_id,
                                    sai_object_id_t switch_id,
                                    uint32_t attr_count,
                                    const sai_attribute_t *attr_list) {
  sai_adapter *q = (sai_adapter *)p;
  return q->create_lag(lag_id, switch_id, attr_count, attr_list);
}
sai_status_t sai_adapter_remove_lag(S_O_Handle p, sai_object_id_t lag_id) {
  sai_adapter *q = (sai_adapter *)p;
  return q->remove_lag(lag_id);
}
sai_status_t sai_adapter_create_lag_member(S_O_Handle p,
                                           sai_object_id_t *lag_member_id,
                                           sai_object_id_t switch_id,
                                           uint32_t attr_count,
                                           const sai_attribute_t *attr_list) {
  sai_adapter *q = (sai_adapter *)p;
  return q->create_lag_member(lag_member_id, switch_id, attr_count, attr_list);
}
sai_status_t sai_adapter_remove_lag_member(S_O_Handle p,
                                           sai_object_id_t lag_member_id) {
  sai_adapter *q = (sai_adapter *)p;
  return q->remove_lag_member(lag_member_id);
}

// hostif
sai_status_t sai_adapter_create_hostif(S_O_Handle p, sai_object_id_t *hif_id,
                                       sai_object_id_t switch_id,
                                       uint32_t attr_count,
                                       const sai_attribute_t *attr_list) {
  sai_adapter *q = (sai_adapter *)p;
  return q->create_hostif(hif_id, switch_id, attr_count, attr_list);
}
sai_status_t sai_adapter_remove_hostif(S_O_Handle p, sai_object_id_t hif_id) {
  sai_adapter *q = (sai_adapter *)p;
  return q->remove_hostif(hif_id);
}
sai_status_t sai_adapter_create_hostif_table_entry(
    S_O_Handle p, sai_object_id_t *hif_table_entry, sai_object_id_t switch_id,
    uint32_t attr_count, const sai_attribute_t *attr_list) {
  sai_adapter *q = (sai_adapter *)p;
  return q->create_hostif_table_entry(hif_table_entry, switch_id, attr_count,
                                      attr_list);
}
sai_status_t sai_adapter_remove_hostif_table_entry(
    S_O_Handle p, sai_object_id_t hif_table_entry) {
  sai_adapter *q = (sai_adapter *)p;
  return q->remove_hostif_table_entry(hif_table_entry);
}
sai_status_t sai_adapter_create_hostif_trap_group(
    S_O_Handle p, sai_object_id_t *hostif_trap_group_id,
    sai_object_id_t switch_id, uint32_t attr_count,
    const sai_attribute_t *attr_list) {
  sai_adapter *q = (sai_adapter *)p;
  return q->create_hostif_trap_group(hostif_trap_group_id, switch_id,
                                     attr_count, attr_list);
}
sai_status_t sai_adapter_remove_hostif_trap_group(
    S_O_Handle p, sai_object_id_t hostif_trap_group_id) {
  sai_adapter *q = (sai_adapter *)p;
  return q->remove_hostif_trap_group(hostif_trap_group_id);
}
sai_status_t sai_adapter_create_hostif_trap(S_O_Handle p,
                                            sai_object_id_t *hostif_trap_id,
                                            sai_object_id_t switch_id,
                                            uint32_t attr_count,
                                            const sai_attribute_t *attr_list) {
  sai_adapter *q = (sai_adapter *)p;
  return q->create_hostif_trap(hostif_trap_id, switch_id, attr_count,
                               attr_list);
}
sai_status_t sai_adapter_remove_hostif_trap(S_O_Handle p,
                                            sai_object_id_t hostif_trap_id) {
  sai_adapter *q = (sai_adapter *)p;
  return q->remove_hostif_trap(hostif_trap_id);
}
}