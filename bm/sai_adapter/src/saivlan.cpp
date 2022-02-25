#include "../inc/sai_adapter.h"

sai_status_t sai_adapter::create_vlan(sai_object_id_t *vlan_id,
                                      sai_object_id_t switch_id,
                                      uint32_t attr_count,
                                      const sai_attribute_t *attr_list) {
  (*logger)->info("create_vlan ");
  Vlan_obj *vlan = new Vlan_obj(sai_id_map_ptr);
  switch_metadata_ptr->vlans[vlan->sai_object_id] = vlan;
  // parsing attributes
  sai_attribute_t attribute;
  for (uint32_t i = 0; i < attr_count; i++) {
    attribute = attr_list[i];
    switch (attribute.id) {
    case SAI_VLAN_ATTR_VLAN_ID:
      vlan->vid = (uint32_t)attribute.value.u16; // TODO correct casting type
      break;
    }
  }

  uint32_t bridge_id = switch_metadata_ptr->GetNewBridgeID(vlan->vid);
  vlan->bridge_id = bridge_id;
  if (vlan->vid != bridge_id) {
    BmMatchParams match_params;
    BmActionData action_data;
    BmAddEntryOptions options;
    match_params.push_back(parse_exact_match_param(vlan->vid, 2));
    action_data.push_back(parse_param(bridge_id, 2));
    vlan->handle_id_1q = bm_client_ptr->bm_mt_add_entry(
        cxt_id, "table_bridge_id_1q", match_params, "action_set_bridge_id",
        action_data, options);
  }
  *vlan_id = vlan->sai_object_id;
  return SAI_STATUS_SUCCESS;
}

sai_status_t sai_adapter::remove_vlan(sai_object_id_t vlan_id) {
  (*logger)->info("remove_vlan: {}", vlan_id);
  Vlan_obj *vlan = switch_metadata_ptr->vlans[vlan_id];
  if (vlan->handle_id_1q != NULL_HANDLE) {
    bm_client_ptr->bm_mt_delete_entry(cxt_id, "table_bridge_id_1q",
                                      vlan->handle_id_1q);
  }
  switch_metadata_ptr->vlans.erase(vlan->sai_object_id);
  sai_id_map_ptr->free_id(vlan->sai_object_id);
  // (*logger)->info("vlans.size={}",switch_metadata_ptr->vlans.size());
  return SAI_STATUS_SUCCESS;
}

sai_status_t sai_adapter::create_vlan_member(sai_object_id_t *vlan_member_id,
                                             sai_object_id_t switch_id,
                                             uint32_t attr_count,
                                             const sai_attribute_t *attr_list) {
  (*logger)->info("create_vlan_member");
  Vlan_member_obj *vlan_member = new Vlan_member_obj(sai_id_map_ptr);
  switch_metadata_ptr->vlan_members[vlan_member->sai_object_id] = vlan_member;
  // parsing attributes
  sai_attribute_t attribute;
  for (uint32_t i = 0; i < attr_count; i++) {
    attribute = attr_list[i];
    switch (attribute.id) {
    case SAI_VLAN_MEMBER_ATTR_VLAN_ID:
      vlan_member->vlan_oid = (sai_object_id_t)attribute.value.oid;
      break;
    case SAI_VLAN_MEMBER_ATTR_BRIDGE_PORT_ID:
      vlan_member->bridge_port_id = (sai_object_id_t)attribute.value.oid;
      break;
    case SAI_VLAN_MEMBER_ATTR_VLAN_TAGGING_MODE:
      vlan_member->tagging_mode = (uint32_t)attribute.value.s32;
      break;
    default:
      (*logger)->error(
          "while parsing vlan member, attribute.id = {} was dumped in "
          "sai_obj",
          attribute.id);
      break;
    }
  }
  Vlan_obj *vlan = switch_metadata_ptr->vlans[vlan_member->vlan_oid];
  vlan_member->vid = vlan->vid;
  vlan->vlan_members.push_back(vlan_member->sai_object_id);
  uint32_t port_id =
      switch_metadata_ptr->bridge_ports[vlan_member->bridge_port_id]->port_id;
  uint32_t bridge_port =
      switch_metadata_ptr->bridge_ports[vlan_member->bridge_port_id]
          ->bridge_port;
  uint32_t out_if;
  if (switch_metadata_ptr->lags.find(port_id) !=
      switch_metadata_ptr->lags.end()) {
    out_if = switch_metadata_ptr->lags[port_id]->l2_if;
  } else {
    out_if = switch_metadata_ptr->ports[port_id]->hw_port;
  }
  BmAddEntryOptions options;
  BmMatchParams match_params;
  BmActionData action_data;
  if (vlan_member->tagging_mode == SAI_VLAN_TAGGING_MODE_TAGGED) {
    uint32_t vlan_pcp = 0;
    uint32_t vlan_cfi = 0;
    match_params.push_back(parse_exact_match_param(out_if, 1));
    match_params.push_back(parse_exact_match_param(vlan_member->vid, 2));
    match_params.push_back(parse_valid_match_param(false));
    action_data.push_back(parse_param(vlan_pcp, 1));
    action_data.push_back(parse_param(vlan_cfi, 1));
    action_data.push_back(parse_param(vlan_member->vid, 2));
    vlan_member->handle_egress_vlan_tag = bm_client_ptr->bm_mt_add_entry(
        cxt_id, "table_egress_vlan_tag", match_params,
        "action_forward_vlan_tag", action_data, options);
  } else if (vlan_member->tagging_mode ==
             SAI_VLAN_TAGGING_MODE_PRIORITY_TAGGED) {
    uint32_t vlan_pcp = 0;
    uint32_t vlan_cfi = 0;
    match_params.push_back(parse_exact_match_param(out_if, 1));
    match_params.push_back(parse_exact_match_param(vlan_member->vid, 2));
    match_params.push_back(parse_valid_match_param(false));
    action_data.push_back(parse_param(vlan_pcp, 1));
    action_data.push_back(parse_param(vlan_cfi, 1));
    action_data.push_back(parse_param(0, 2));
    vlan_member->handle_egress_vlan_tag = bm_client_ptr->bm_mt_add_entry(
        cxt_id, "table_egress_vlan_tag", match_params,
        "action_forward_vlan_tag", action_data, options);
  } else {
    (*logger)->info("table_egress_vlan_tag debug.  out_if = {}, vid = {}",
                    out_if, vlan_member->vid);
    match_params.push_back(parse_exact_match_param(out_if, 1));
    match_params.push_back(parse_exact_match_param(vlan_member->vid, 2));
    match_params.push_back(parse_valid_match_param(true));
    action_data.clear();
    vlan_member->handle_egress_vlan_tag = bm_client_ptr->bm_mt_add_entry(
        cxt_id, "table_egress_vlan_tag", match_params,
        "action_forward_vlan_untag", action_data, options);
  }
  match_params.clear();
  match_params.push_back(parse_exact_match_param(bridge_port, 1));
  match_params.push_back(parse_exact_match_param(vlan_member->vid, 2));
  action_data.clear();
  vlan_member->handle_egress_vlan_filtering = bm_client_ptr->bm_mt_add_entry(
      cxt_id, "table_egress_vlan_filtering", match_params, "_nop", action_data,
      options);
  vlan_member->handle_ingress_vlan_filtering = bm_client_ptr->bm_mt_add_entry(
      cxt_id, "table_ingress_vlan_filtering", match_params, "_nop", action_data,
      options);
  *vlan_member_id = vlan_member->sai_object_id;
  return SAI_STATUS_SUCCESS;
}

sai_status_t sai_adapter::remove_vlan_member(sai_object_id_t vlan_member_id) {
  (*logger)->info("remove vlan_member: vlan_member_id = {}", vlan_member_id);
  sai_status_t status = SAI_STATUS_SUCCESS;
  Vlan_member_obj *vlan_member =
      switch_metadata_ptr->vlan_members[vlan_member_id];
  try {
    bm_client_ptr->bm_mt_delete_entry(
        cxt_id, "table_egress_vlan_filtering",
        vlan_member->handle_egress_vlan_filtering);
    bm_client_ptr->bm_mt_delete_entry(
        cxt_id, "table_ingress_vlan_filtering",
        vlan_member->handle_ingress_vlan_filtering);
    if (vlan_member->tagging_mode == SAI_VLAN_TAGGING_MODE_TAGGED) {
      bm_client_ptr->bm_mt_delete_entry(cxt_id, "table_egress_vlan_tag",
                                        vlan_member->handle_egress_vlan_tag);
    } else if (vlan_member->tagging_mode ==
               SAI_VLAN_TAGGING_MODE_PRIORITY_TAGGED) {
      bm_client_ptr->bm_mt_delete_entry(cxt_id, "table_egress_vlan_tag",
                                        vlan_member->handle_egress_vlan_tag);
    } else {
      bm_client_ptr->bm_mt_delete_entry(cxt_id, "table_egress_vlan_tag",
                                        vlan_member->handle_egress_vlan_tag);
    }
  } catch (...) {
    (*logger)->warn(" Unable to delete vlan member tables.");
    return SAI_STATUS_FAILURE;
  };
  // get parent vlan member list, and remove the memeber by value - TODO
  // consider map instead of vector.
  std::vector<sai_object_id_t> *vlan_members =
      &switch_metadata_ptr->vlans[vlan_member->vlan_oid]->vlan_members;
  vlan_members->erase(std::remove(vlan_members->begin(), vlan_members->end(),
                                  vlan_member->sai_object_id),
                      vlan_members->end());
  switch_metadata_ptr->vlan_members.erase(vlan_member->sai_object_id);
  sai_id_map_ptr->free_id(vlan_member->sai_object_id);
  return status;
}

sai_status_t sai_adapter::set_vlan_attribute(sai_object_id_t vlan_id,
                                             const sai_attribute_t *attr) {
  (*logger)->info("TODO : set_vlan_attribute not implemened");
  return SAI_STATUS_NOT_IMPLEMENTED;
  // implementation
}
sai_status_t sai_adapter::get_vlan_attribute(sai_object_id_t vlan_id,
                                             const uint32_t attr_count,
                                             sai_attribute_t *attr_list) {
  (*logger)->info("TODO : get_vlan_attribute not implemened");
  return SAI_STATUS_NOT_IMPLEMENTED;
  // implementation
}
sai_status_t
sai_adapter::set_vlan_member_attribute(sai_object_id_t vlan_member_id,
                                       const sai_attribute_t *attr) {
  (*logger)->info("TODO : set_vlan_member_attribute not implemened");
  return SAI_STATUS_NOT_IMPLEMENTED;

  // implementation
}
sai_status_t
sai_adapter::get_vlan_member_attribute(sai_object_id_t vlan_member_id,
                                       const uint32_t attr_count,
                                       sai_attribute_t *attr_list) {
  (*logger)->info("TODO : get_vlan_member_attribute not implemened");
  return SAI_STATUS_NOT_IMPLEMENTED;
  // implementation
}
sai_status_t sai_adapter::get_vlan_stats(sai_object_id_t vlan_id,
                                         const sai_vlan_stat_t *counter_ids,
                                         uint32_t number_of_counters,
                                         uint64_t *counters) {
  (*logger)->info("TODO : get_vlan_stats not implemened");
  return SAI_STATUS_NOT_IMPLEMENTED;

  // implementation
}
sai_status_t sai_adapter::clear_vlan_stats(sai_object_id_t vlan_id,
                                           const sai_vlan_stat_t *counter_ids,
                                           uint32_t number_of_counters) {
  (*logger)->info("TODO : clear_vlan_stats not implemened");
  // implementation
}