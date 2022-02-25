#include "../inc/sai_adapter.h"

sai_status_t sai_adapter::create_bridge(sai_object_id_t *bridge_id,
                                        sai_object_id_t switch_id,
                                        uint32_t attr_count,
                                        const sai_attribute_t *attr_list) {
  Bridge_obj *bridge = new Bridge_obj(sai_id_map_ptr);
  switch_metadata_ptr->bridges[bridge->sai_object_id] = bridge;

  // parsing attributes
  sai_attribute_t attribute;
  for (uint32_t i = 0; i < attr_count; i++) {
    attribute = attr_list[i];
    switch (attribute.id) {
    case SAI_BRIDGE_ATTR_TYPE:
      bridge->bridge_type = (sai_bridge_type_t)attribute.value.s32;
      break;
    }
  }

  if (bridge->bridge_type == SAI_BRIDGE_TYPE_1D) {
    uint32_t bridge_id_num = switch_metadata_ptr->GetNewBridgeID(0);
    bridge->bridge_id = bridge_id_num;
    (*logger)->info("--> Created new .1D bridge {} (sai_object_id={})",
                    bridge->bridge_id, bridge->sai_object_id);
  } else { // 1Q bridge
    (*logger)->info("--> Created new .1Q bridge(sai_object_id={})",
                    bridge->sai_object_id);
  }

  *bridge_id = bridge->sai_object_id;
  return SAI_STATUS_SUCCESS;
}

sai_status_t sai_adapter::remove_bridge(sai_object_id_t bridge_id) {
  (*logger)->info("remove bridge: bridge_id {}", bridge_id);
  Bridge_obj *bridge = switch_metadata_ptr->bridges[bridge_id];
  switch_metadata_ptr->bridges.erase(bridge->sai_object_id);
  sai_id_map_ptr->free_id(bridge->sai_object_id);
  // (*logger)->info("bridges.size={}",switch_metadata_ptr->bridges.size());
  return SAI_STATUS_SUCCESS;
}

sai_status_t sai_adapter::get_bridge_attribute(sai_object_id_t bridge_id,
                                               uint32_t attr_count,
                                               sai_attribute_t *attr_list) {
  int i;
  Bridge_obj *bridge = (Bridge_obj *)sai_id_map_ptr->get_object(bridge_id);
  for (i = 0; i < attr_count; i++) {
    switch ((attr_list + i)->id) {
    case SAI_BRIDGE_ATTR_TYPE:
      (attr_list + i)->value.s32 = bridge->bridge_type;
      break;
    case SAI_BRIDGE_ATTR_PORT_LIST:
      (attr_list + i)->value.objlist.count = bridge->bridge_port_list.size();
      (attr_list + i)->value.objlist.list = &bridge->bridge_port_list[0];
      break;
    }
  }
}

// Bridge Port

sai_status_t sai_adapter::create_bridge_port(sai_object_id_t *bridge_port_id,
                                             sai_object_id_t switch_id,
                                             uint32_t attr_count,
                                             const sai_attribute_t *attr_list) {
  uint32_t bridge_port_num = switch_metadata_ptr->GetNewBridgePort();
  BridgePort_obj *bridge_port = new BridgePort_obj(sai_id_map_ptr);
  switch_metadata_ptr->bridge_ports[bridge_port->sai_object_id] = bridge_port;
  bridge_port->bridge_port = bridge_port_num;
  sai_attribute_t attribute;
  for (uint32_t i = 0; i < attr_count; i++) {
    attribute = attr_list[i];
    switch (attribute.id) {
    case SAI_BRIDGE_PORT_ATTR_VLAN_ID:
      bridge_port->vlan_id = attribute.value.u16;
      break;
    case SAI_BRIDGE_PORT_ATTR_BRIDGE_ID:
      bridge_port->bridge_id = attribute.value.oid;
      break;
    case SAI_BRIDGE_PORT_ATTR_TYPE:
      bridge_port->bridge_port_type =
          (sai_bridge_port_type_t)attribute.value.s32;
      break;
    case SAI_BRIDGE_PORT_ATTR_PORT_ID:
      bridge_port->port_id = attribute.value.oid;
      break;
    }
  }

  switch_metadata_ptr->bridges[bridge_port->bridge_id]
      ->bridge_port_list.push_back(bridge_port->sai_object_id);

  BmAddEntryOptions options;
  BmMatchParams match_params;
  BmActionData action_data;
  match_params.clear();
  action_data.clear();
  int32_t l2_if_type;
  // 1D
  if (bridge_port->bridge_port_type == SAI_BRIDGE_PORT_TYPE_SUB_PORT) {
    uint32_t bridge_id =
        switch_metadata_ptr->bridges[bridge_port->bridge_id]->bridge_id;
    match_params.push_back(
        parse_exact_match_param(bridge_port->bridge_port, 1));
    action_data.push_back(parse_param(bridge_id, 2));
    bridge_port->handle_id_1d = bm_client_ptr->bm_mt_add_entry(
        cxt_id, "table_bridge_id_1d", match_params, "action_set_bridge_id",
        action_data, options);
    action_data.clear();
    action_data.push_back(parse_param(bridge_port->vlan_id, 2));
    bridge_port->handle_egress_set_vlan = bm_client_ptr->bm_mt_add_entry(
        cxt_id, "table_egress_set_vlan", match_params, "action_set_vlan",
        action_data, options);
    l2_if_type = 2;
  }
  // 1Q
  else if (bridge_port->bridge_port_type == SAI_BRIDGE_PORT_TYPE_PORT) {
    // match_params.clear();
    // match_params.push_back(parse_exact_match_param(bridge_port->vlan_id, 2));
    // action_data.clear();
    // action_data.push_back(parse_param(bridge_id, 2));
    // if (bridge_port->handle_id_1q == NULL_HANDLE) {
    //   try {
    //     BmMtEntry entry;
    //     bm_client_ptr->bm_mt_get_entry_from_key(
    //         entry, cxt_id, "table_bridge_id_1q", match_params, options);
    //     bm_client_ptr->bm_mt_delete_entry(cxt_id, "table_bridge_id_1q",
    //                                       entry.entry_handle);
    //   } catch (...) {
    //     (*logger)->debug("--> InvalidTableOperation while removing "
    //                      "table_bridge_id_1q entry");
    //   }
    //   (*logger)->debug("--> adding table_bridge_id_1q entry, key:{}",
    //                    bridge_port->vlan_id);
    //   bridge_port->handle_id_1q = bm_client_ptr->bm_mt_add_entry(
    //       cxt_id, "table_bridge_id_1q", match_params, "action_set_bridge_id",
    //       action_data, options);
    // }
    l2_if_type = 3;
  }

  uint32_t bind_mode;
  uint32_t l2_if;
  uint32_t is_lag;
  port_id_map_t::iterator it =
      switch_metadata_ptr->ports.find(bridge_port->port_id);
  if (it != switch_metadata_ptr->ports.end()) { // port_id is port
    Port_obj *port = (Port_obj *)it->second;
    bind_mode = port->bind_mode;
    l2_if = port->l2_if;
    is_lag = 0;
  } else { // port_id is lag
    Lag_obj *lag = (Lag_obj *)sai_id_map_ptr->get_object(bridge_port->port_id);
    bind_mode = lag->port_obj->bind_mode;
    l2_if = lag->l2_if;
    is_lag = 1;
  }

  match_params.clear();
  match_params.push_back(parse_exact_match_param(bridge_port->bridge_port, 1));
  action_data.clear();
  action_data.push_back(parse_param(l2_if, 1));
  action_data.push_back(parse_param(is_lag, 1));
  bridge_port->handle_egress_br_port_to_if = bm_client_ptr->bm_mt_add_entry(
      cxt_id, "table_egress_br_port_to_if", match_params,
      "action_forward_set_outIfType", action_data, options);
  if (bind_mode == SAI_PORT_BIND_MODE_SUB_PORT) {
    match_params.clear();
    match_params.push_back(
        parse_exact_match_param(l2_if, 1)); // TODO p4 table match is on l2_if
    match_params.push_back(parse_exact_match_param(bridge_port->vlan_id, 2));
    action_data.clear();
    action_data.push_back(parse_param(l2_if_type, 1));
    action_data.push_back(parse_param(bridge_port->bridge_port, 1));
    bridge_port->handle_subport_ingress_interface_type =
        bm_client_ptr->bm_mt_add_entry(
            cxt_id, "table_subport_ingress_interface_type", match_params,
            "action_set_l2_if_type", action_data, options);
  } else {
    match_params.clear();
    match_params.push_back(parse_exact_match_param(l2_if, 1));
    action_data.clear();
    action_data.push_back(parse_param(l2_if_type, 1));
    action_data.push_back(parse_param(bridge_port->bridge_port, 1));
    bridge_port->handle_port_ingress_interface_type =
        bm_client_ptr->bm_mt_add_entry(
            cxt_id, "table_port_ingress_interface_type", match_params,
            "action_set_l2_if_type", action_data, options);
  }
  *bridge_port_id = bridge_port->sai_object_id;
  return SAI_STATUS_SUCCESS;
}

sai_status_t sai_adapter::remove_bridge_port(sai_object_id_t bridge_port_id) {
  sai_status_t status = SAI_STATUS_SUCCESS;
  BridgePort_obj *bridge_port =
      switch_metadata_ptr->bridge_ports[bridge_port_id];

  if (bridge_port->handle_id_1d != NULL_HANDLE) {
    try {
      bm_client_ptr->bm_mt_delete_entry(cxt_id, "table_bridge_id_1d",
                                        bridge_port->handle_id_1d);
    } catch (...) {
      (*logger)->debug("--> Unable to remove table_bridge_id_1d entry, "
                       "possible entry override");
    }
  }
  if (bridge_port->handle_egress_set_vlan != NULL_HANDLE) {
    try {
      bm_client_ptr->bm_mt_delete_entry(cxt_id, "table_egress_set_vlan",
                                        bridge_port->handle_egress_set_vlan);
    } catch (...) {
      (*logger)->debug("--> Unable to remove table_egress_set_vlan entry, "
                       "possible entry override");
    }
  }
  if (bridge_port->handle_egress_br_port_to_if != NULL_HANDLE) {
    try {
      bm_client_ptr->bm_mt_delete_entry(
          cxt_id, "table_egress_br_port_to_if",
          bridge_port->handle_egress_br_port_to_if);
    } catch (...) {
      (*logger)->debug("--> Unable to remove table_egress_br_port_to_if entry, "
                       "possible entry override");
    }
  }
  if (bridge_port->handle_subport_ingress_interface_type != NULL_HANDLE) {
    try {
      bm_client_ptr->bm_mt_delete_entry(
          cxt_id, "table_subport_ingress_interface_type",
          bridge_port->handle_subport_ingress_interface_type);
    } catch (...) {
      (*logger)->debug("--> Unable to remove "
                       "table_subport_ingress_interface_type entry, possible "
                       "entry override");
    }
  }
  if (bridge_port->handle_port_ingress_interface_type != NULL_HANDLE) {
    try {
      bm_client_ptr->bm_mt_delete_entry(
          cxt_id, "table_port_ingress_interface_type",
          bridge_port->handle_port_ingress_interface_type);
    } catch (...) {
      (*logger)->debug("--> Unable to remove table_port_ingress_interface_type "
                       "entry, possible entry override");
    }
  }
  (*logger)->debug("deleted bridge port {} bm_entries",
                   bridge_port->sai_object_id);
  switch_metadata_ptr->bridge_ports.erase(bridge_port->sai_object_id);
  std::vector<sai_object_id_t> *vec =
      &switch_metadata_ptr->bridges[bridge_port->bridge_id]->bridge_port_list;
  vec->erase(std::remove(vec->begin(), vec->end(), bridge_port->sai_object_id),
             vec->end());

  sai_id_map_ptr->free_id(bridge_port->sai_object_id);
  (*logger)->debug("deleted bridge port {} bm_entries",
                   bridge_port->sai_object_id);
  return status;
}

sai_status_t
sai_adapter::get_bridge_port_attribute(sai_object_id_t bridge_port_id,
                                       uint32_t attr_count,
                                       sai_attribute_t *attr_list) {
  (*logger)->debug("get_bridge_port_attribute: bridge_port_id {}",
                   bridge_port_id);
  BridgePort_obj *bridge_port =
      (BridgePort_obj *)sai_id_map_ptr->get_object(bridge_port_id);
  for (int i = 0; i < attr_count; i++) {
    switch ((attr_list + i)->id) {
    case SAI_BRIDGE_PORT_ATTR_PORT_ID:
      (attr_list + i)->value.oid = bridge_port->port_id;
      break;
    case SAI_BRIDGE_PORT_ATTR_VLAN_ID:
      (attr_list + i)->value.u16 = bridge_port->vlan_id;
      break;
    case SAI_BRIDGE_PORT_ATTR_TYPE:
      (attr_list + i)->value.s32 = bridge_port->bridge_port_type;
      break;
    case SAI_BRIDGE_PORT_ATTR_BRIDGE_ID:
      (attr_list + i)->value.oid = bridge_port->bridge_port_type;
      break;
    }
  }
}