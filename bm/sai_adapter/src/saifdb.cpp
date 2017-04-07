#include "../inc/sai_adapter.h"

sai_status_t sai_adapter::create_fdb_entry(const sai_fdb_entry_t *fdb_entry,
                                           uint32_t attr_count,
                                           const sai_attribute_t *attr_list) {
  (*logger)->info("create_fdb_entry");
  sai_status_t status = SAI_STATUS_SUCCESS;
  // parsing attributes
  sai_fdb_entry_type_t entry_type;
  uint32_t bridge_port;
  sai_packet_action_t packet_action;
  sai_attribute_t attribute;
  for (uint32_t i = 0; i < attr_count; i++) {
    attribute = attr_list[i];
    switch (attribute.id) {
    case SAI_FDB_ENTRY_ATTR_TYPE:
      entry_type = (sai_fdb_entry_type_t)attribute.value.s32;
      // (*logger)->error("--> attr packet type="<<attribute.value.s32<<endl;
      // (*logger)->error("--> attr packet_static" <<
      // SAI_FDB_ENTRY_TYPE_STATIC
      // <<endl;
      break;
    case SAI_FDB_ENTRY_ATTR_BRIDGE_PORT_ID:
      bridge_port =
          switch_metadata_ptr->bridge_ports[attribute.value.oid]->bridge_port;
      break;
    case SAI_FDB_ENTRY_ATTR_PACKET_ACTION:
      packet_action = (sai_packet_action_t)attribute.value.s32;
      // (*logger)->error("--> attr
      // packet_action="<<attribute.value.s32<<endl;
      // (*logger)->error("--> attr packet_action_fwd=" <<
      // SAI_PACKET_ACTION_FORWARD
      // <<endl;
      break;
    default:
      (*logger)->error(
          "create_fdb_entry attribute.id = {} was dumped in sai_obj",
          attribute.id);
      break;
    }
  }

  // out_if_type = 0 # port_type (not lag or router). TODO: check how to do it
  // with SAI

  uint32_t bridge_id = get_bridge_id_from_fdb_entry(fdb_entry);
  (*logger)->info("create fdb - bridge_id = {}", bridge_id);
  if (packet_action == SAI_PACKET_ACTION_FORWARD) {
    if (entry_type == SAI_FDB_ENTRY_TYPE_STATIC) {
      BmAddEntryOptions options;
      BmMatchParams match_params;
      BmActionData action_data;
      uint64_t mac_address = parse_mac_64(fdb_entry->mac_address);
      match_params.push_back(parse_exact_match_param(mac_address, 6));
      match_params.push_back(parse_exact_match_param(bridge_id, 2));
      (*logger)->info("--> mac: {}, b_id: {}", mac_address, bridge_id);
      action_data.push_back(parse_param(bridge_port, 1));
      bm_client_ptr->bm_mt_add_entry(cxt_id, "table_fdb", match_params,
                                     "action_set_egress_br_port", action_data,
                                     options);
      action_data.clear();
      bm_client_ptr->bm_mt_add_entry(cxt_id, "table_learn_fdb", match_params,
                                     "_nop", action_data, options);
    }
  }
  return status;
}

sai_status_t sai_adapter::remove_fdb_entry(const sai_fdb_entry_t *fdb_entry) {
  (*logger)->info("remove_fdb_entry");
  sai_status_t status = SAI_STATUS_SUCCESS;
  BmAddEntryOptions options;
  BmMatchParams match_params;
  BmActionData action_data;
  uint64_t mac_address = parse_mac_64(fdb_entry->mac_address);
  match_params.push_back(parse_exact_match_param(mac_address, 6));
  uint32_t bridge_id = get_bridge_id_from_fdb_entry(fdb_entry);
  match_params.push_back(parse_exact_match_param(bridge_id, 2));
  BmMtEntry bm_entry;
  bm_client_ptr->bm_mt_get_entry_from_key(bm_entry, cxt_id, "table_fdb",
                                          match_params, options);
  bm_client_ptr->bm_mt_delete_entry(cxt_id, "table_fdb", bm_entry.entry_handle);
  bm_client_ptr->bm_mt_get_entry_from_key(bm_entry, cxt_id, "table_learn_fdb",
                                          match_params, options);

  bm_client_ptr->bm_mt_delete_entry(cxt_id, "table_learn_fdb",
                                    bm_entry.entry_handle);

  return status;
}

uint32_t
sai_adapter::get_bridge_id_from_fdb_entry(const sai_fdb_entry_t *fdb_entry) {
  if (fdb_entry->bridge_type == SAI_FDB_ENTRY_BRIDGE_TYPE_1Q) {
    sai_object_id_t vlan_obj_id =
        switch_metadata_ptr->GetVlanObjIdFromVid(fdb_entry->vlan_id);
    if (vlan_obj_id != 0) {
      return switch_metadata_ptr->vlans[vlan_obj_id]->bridge_id;
    } else {
      return fdb_entry->vlan_id;
    }
  } else {
    return switch_metadata_ptr->bridges[fdb_entry->bridge_id]->bridge_id;
  }
}