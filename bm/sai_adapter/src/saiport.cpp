#include "../inc/sai_adapter.h"

sai_status_t sai_adapter::create_port(sai_object_id_t *port_id,
                                      sai_object_id_t switch_id,
                                      uint32_t attr_count,
                                      const sai_attribute_t *attr_list) {
  Port_obj *port = new Port_obj(sai_id_map_ptr);
  switch_metadata_ptr->ports[port->sai_object_id] = port;
  (*logger)->info("--> new port sai_id = {}, tot port num: {}",
                  port->sai_object_id, switch_metadata_ptr->ports.size());
  // parsing attributes
  sai_attribute_t attribute;
  for (uint32_t i = 0; i < attr_count; i++) {
    attribute = attr_list[i];
    set_parsed_port_attribute(port, attribute);
  }
  BmAddEntryOptions options;
  BmMatchParams match_params;
  BmActionData action_data;
  action_data.clear();
  match_params.clear();
  match_params.push_back(parse_exact_match_param(port->hw_port, 2));
  action_data.push_back(parse_param(0, 1));
  action_data.push_back(parse_param(port->sai_object_id, 1));
  port->handle_lag_if = bm_client_ptr->bm_mt_add_entry(
      cxt_id, "table_ingress_lag", match_params, "action_set_lag_l2if",
      action_data, options);
  action_data.clear();
  match_params.clear();
  match_params.push_back(parse_exact_match_param(port->sai_object_id, 1));
  action_data.push_back(parse_param(port->pvid, 2));
  action_data.push_back(parse_param(port->bind_mode, 1));
  action_data.push_back(parse_param(port->mtu, 4));
  action_data.push_back(parse_param(port->drop_tagged, 1));
  action_data.push_back(parse_param(port->drop_untagged, 1));
  port->handle_port_cfg = bm_client_ptr->bm_mt_add_entry(
      cxt_id, "table_port_configurations", match_params,
      "action_set_port_configurations", action_data, options);
  *port_id = port->sai_object_id;
  return SAI_STATUS_SUCCESS;
}

sai_status_t sai_adapter::remove_port(sai_object_id_t port_id) {
  (*logger)->info("sai_remove_port: {} ", port_id);
  Port_obj *port = switch_metadata_ptr->ports[port_id];
  try {
    bm_client_ptr->bm_mt_delete_entry(cxt_id, "table_ingress_lag",
                                      port->handle_lag_if);
    bm_client_ptr->bm_mt_delete_entry(cxt_id, "table_port_configurations",
                                      port->handle_port_cfg);
  } catch (...) {
    (*logger)->info("--> unable to remove port tables entries");
  }
  switch_metadata_ptr->ports.erase(port->sai_object_id);
  sai_id_map_ptr->free_id(port->sai_object_id);
  (*logger)->debug("--> ports.size after remove: {}",
                   switch_metadata_ptr->ports.size());
  return SAI_STATUS_SUCCESS;
}

sai_status_t sai_adapter::set_port_attribute(sai_object_id_t port_id,
                                             const sai_attribute_t *attr) {
  Port_obj *port;
  port_id_map_t::iterator it = switch_metadata_ptr->ports.find(port_id);
  if (it != switch_metadata_ptr->ports.end()) {
    (*logger)->info("set port {} attribute (port).", port_id);
    port = (Port_obj *)it->second;
  } else {
    (*logger)->info("set port {} attribute (lag).", port_id);
    port = ((Lag_obj *)sai_id_map_ptr->get_object(port_id))->port_obj;
  }
  set_parsed_port_attribute(port, *attr);
  config_port(port);
  return SAI_STATUS_SUCCESS;
}

sai_status_t sai_adapter::get_port_attribute(sai_object_id_t port_id,
                                             uint32_t attr_count,
                                             sai_attribute_t *attr_list) {
  Port_obj *port = (Port_obj *)sai_id_map_ptr->get_object(port_id);
  for (int i = 0; i < attr_count; i++) {
    get_parsed_port_attribute(port, attr_list + i);
  }
  return SAI_STATUS_SUCCESS;
}

void sai_adapter::set_parsed_port_attribute(Port_obj *port,
                                            sai_attribute_t attribute) {
  (*logger)->info("set_parsed_port_attribute. attribute id = {}", attribute.id);
  (*logger)->trace("vlan = {} | bind_mode = {} | hw_lane_list = {} | "
                   "drop_untagged = {} | drop_tagged = {}",
                   SAI_PORT_ATTR_PORT_VLAN_ID, SAI_PORT_ATTR_BIND_MODE,
                   SAI_PORT_ATTR_HW_LANE_LIST, SAI_PORT_ATTR_DROP_UNTAGGED,
                   SAI_PORT_ATTR_DROP_TAGGED);
  switch (attribute.id) {
  case SAI_PORT_ATTR_PORT_VLAN_ID:
    port->pvid = attribute.value.u16;
    break;
  case SAI_PORT_ATTR_BIND_MODE:
    port->bind_mode = attribute.value.s32;
    break;
  case SAI_PORT_ATTR_HW_LANE_LIST:
    port->hw_port = attribute.value.u32list.list[0];
    break;
  case SAI_PORT_ATTR_DROP_UNTAGGED:
    port->drop_untagged = attribute.value.booldata;
    break;
  case SAI_PORT_ATTR_DROP_TAGGED:
    port->drop_tagged = attribute.value.booldata;
    break;
  }
}

void sai_adapter::get_parsed_port_attribute(Port_obj *port,
                                            sai_attribute_t *attribute) {
  switch (attribute->id) {
  case SAI_PORT_ATTR_PORT_VLAN_ID:
    attribute->value.u16 = port->pvid;
    break;
  case SAI_PORT_ATTR_BIND_MODE:
    attribute->value.s32 = port->bind_mode;
    break;
  case SAI_PORT_ATTR_HW_LANE_LIST:
    attribute->value.u32list.count = 1;
    attribute->value.u32list.list[0] = port->hw_port;
    break;
  case SAI_PORT_ATTR_DROP_UNTAGGED:
    attribute->value.booldata = port->drop_untagged;
    break;
  case SAI_PORT_ATTR_DROP_TAGGED:
    attribute->value.booldata = port->drop_tagged;
    break;
  }
}

void sai_adapter::config_port(Port_obj *port) {
  BmAddEntryOptions options;
  BmMatchParams match_params;
  match_params.clear();
  match_params.push_back(parse_exact_match_param(port->l2_if, 1));
  BmActionData action_data;
  action_data.clear();
  action_data.push_back(parse_param(port->pvid, 2));
  action_data.push_back(parse_param(port->bind_mode, 1));
  action_data.push_back(parse_param(port->mtu, 4));
  action_data.push_back(parse_param(port->drop_tagged, 1));
  action_data.push_back(parse_param(port->drop_untagged, 1));
  try {
    BmMtEntry entry;
    bm_client_ptr->bm_mt_get_entry_from_key(
        entry, cxt_id, "table_port_configurations", match_params, options);
    bm_client_ptr->bm_mt_delete_entry(cxt_id, "table_port_configurations",
                                      entry.entry_handle);
  } catch (...) {
    (*logger)->warn("--> InvalidTableOperation while removing "
                    "table_port_configurations entry");
  }
  try {
    port->handle_port_cfg = bm_client_ptr->bm_mt_add_entry(
        cxt_id, "table_port_configurations", match_params,
        "action_set_port_configurations", action_data, options);
  } catch (...) {
    (*logger)->warn("--> InvalidTableOperation while adding "
                    "table_port_configurations entry");
  }
}