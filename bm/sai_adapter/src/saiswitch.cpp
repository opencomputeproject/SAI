#include "../inc/sai_adapter.h"

sai_status_t sai_adapter::create_switch(sai_object_id_t *switch_id,
                                        uint32_t attr_count,
                                        const sai_attribute_t *attr_list) {
  (*logger)->info("create switch");
  if (switch_list_ptr->size() != 0) {
    (*logger)->debug(
        "currently one switch is supportred, returning operating switch_id: {}",
        (*switch_list_ptr)[0]);
    return (*switch_list_ptr)[0];
  } else {
    BmAddEntryOptions options;
    BmMatchParams match_params;
    BmMtEntry entry;

    // Create Default 1Q Bridge, and switch_obj (not sure if switch is needed).
    Bridge_obj *bridge = new Bridge_obj(sai_id_map_ptr);
    (*logger)->info("Default 1Q bridge. sai_object_id {} bridge_id {}",
                    bridge->sai_object_id, bridge->bridge_id);
    Sai_obj *switch_obj = new Sai_obj(sai_id_map_ptr);
    switch_metadata_ptr->bridges[bridge->sai_object_id] = bridge;
    switch_metadata_ptr->default_bridge_id = bridge->sai_object_id;

    for (int i = 0; i < switch_metadata_ptr->hw_port_list.count; i++) {
      int hw_port = switch_metadata_ptr->hw_port_list.list[i];

      // Create Default Ports (one for each hw_port)
      Port_obj *port = new Port_obj(sai_id_map_ptr);
      switch_metadata_ptr->ports[port->sai_object_id] = port;
      port->hw_port = hw_port;
      port->l2_if = hw_port;
      (*logger)->info("Default port_id {}. hw_port = {}", port->sai_object_id,
                      port->hw_port);

      // Create Default Bridge ports and connect to 1Q bridge
      BridgePort_obj *bridge_port = new BridgePort_obj(sai_id_map_ptr);
      switch_metadata_ptr->bridge_ports[bridge_port->sai_object_id] =
          bridge_port;
      bridge_port->port_id = port->sai_object_id;
      bridge_port->bridge_port = hw_port;
      bridge_port->bridge_id = bridge->sai_object_id;
      bridge->bridge_port_list.push_back(bridge_port->sai_object_id);
      (*logger)->info("Default bridge_port_id {}. bridge_port = {}",
                      bridge_port->sai_object_id, bridge_port->bridge_port);

      // Store default table entries
      match_params.clear();
      match_params.push_back(parse_exact_match_param(port->l2_if, 1));
      bm_client_ptr->bm_mt_get_entry_from_key(
          entry, cxt_id, "table_port_ingress_interface_type", match_params,
          options);
      bridge_port->handle_port_ingress_interface_type = entry.entry_handle;
      match_params.clear();
      match_params.push_back(
          parse_exact_match_param(bridge_port->bridge_port, 1));
      bm_client_ptr->bm_mt_get_entry_from_key(
          entry, cxt_id, "table_egress_br_port_to_if", match_params, options);
      bridge_port->handle_egress_br_port_to_if = entry.entry_handle;
    }
    switch_list_ptr->push_back(switch_obj->sai_object_id);
    return switch_obj->sai_object_id;
  }
}

sai_status_t sai_adapter::get_switch_attribute(sai_object_id_t switch_id,
                                               sai_uint32_t attr_count,
                                               sai_attribute_t *attr_list) {
  (*logger)->info("get_switch_attribute");
  int i;
  int index = 0;
  for (i = 0; i < attr_count; i++) {
    switch ((attr_list + i)->id) {
    case SAI_SWITCH_ATTR_DEFAULT_1Q_BRIDGE_ID:
      (attr_list + i)->value.oid = switch_metadata_ptr->default_bridge_id;
      break;
    case SAI_SWITCH_ATTR_PORT_LIST:
      for (port_id_map_t::iterator it = switch_metadata_ptr->ports.begin();
           it != switch_metadata_ptr->ports.end(); ++it) {
        (attr_list + i)->value.objlist.list[index] = it->first;
        index += 1;
      }
      break;
    case SAI_SWITCH_ATTR_PORT_NUMBER:
      (attr_list + i)->value.u32 = switch_metadata_ptr->hw_port_list.count;
      break;
    }
  }
  return SAI_STATUS_SUCCESS;
}

// sai_status_t sai_adapter::sai_get_switch_attribute(sai_object_id_t
// switch_id,sai_uint32_t attr_count,sai_attribute_t *attr_list){
// }
