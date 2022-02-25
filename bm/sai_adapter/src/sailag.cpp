#include "../inc/sai_adapter.h"

sai_status_t sai_adapter::create_lag(sai_object_id_t *lag_id,
                                     sai_object_id_t switch_id,
                                     uint32_t attr_count,
                                     const sai_attribute_t *attr_list) {
  Lag_obj *lag = new Lag_obj(sai_id_map_ptr);
  lag->l2_if = switch_metadata_ptr->GetNewL2IF();
  (*logger)->info("create_lag. l2_if = {}. sai_object_id = {}", lag->l2_if,
                  lag->sai_object_id);
  switch_metadata_ptr->lags[lag->sai_object_id] = lag;
  *lag_id = lag->sai_object_id;
  return SAI_STATUS_SUCCESS;
}
sai_status_t sai_adapter::remove_lag(sai_object_id_t lag_id) {
  (*logger)->info("remove_lag: {}", lag_id);
  sai_status_t status = SAI_STATUS_SUCCESS;
  Lag_obj *lag = switch_metadata_ptr->lags[lag_id];
  try {
    if (lag->handle_port_configurations != NULL_HANDLE) {
      bm_client_ptr->bm_mt_delete_entry(cxt_id, "table_port_configurations",
                                        lag->handle_port_configurations);
    }
    if (lag->handle_lag_hash != NULL_HANDLE) {
      bm_client_ptr->bm_mt_delete_entry(cxt_id, "table_lag_hash",
                                        lag->handle_lag_hash);
    }
  } catch (...) {
    status = SAI_STATUS_FAILURE;
    (*logger)->error("Unable to remove lag tables entries");
  }
  switch_metadata_ptr->lags.erase(lag->sai_object_id);
  sai_id_map_ptr->free_id(lag->sai_object_id);
  return status;
}
sai_status_t sai_adapter::create_lag_member(sai_object_id_t *lag_member_id,
                                            sai_object_id_t switch_id,
                                            uint32_t attr_count,
                                            const sai_attribute_t *attr_list) {
  (*logger)->info("create_lag_member ");
  Lag_member_obj *lag_member = new Lag_member_obj(sai_id_map_ptr);
  switch_metadata_ptr->lag_members[lag_member->sai_object_id] = lag_member;
  Port_obj *port;
  Lag_obj *lag;
  // parsing attributes
  sai_attribute_t attribute;
  for (uint32_t i = 0; i < attr_count; i++) {
    attribute = attr_list[i];
    switch (attribute.id) {
    case SAI_LAG_MEMBER_ATTR_PORT_ID:
      port = switch_metadata_ptr->ports[attribute.value.oid];
      lag_member->port = port;
      break;
    case SAI_LAG_MEMBER_ATTR_LAG_ID:
      lag = switch_metadata_ptr->lags[attribute.value.oid];
      lag_member->lag = lag;
      lag->lag_members.push_back(lag_member->sai_object_id);
      break;
    default:
      (*logger)->error(
          "while parsing lag member, attribute.id = was dumped in sai_obj",
          attribute.id);
      break;
    }
  }
  lag->port_obj = port;
  uint32_t l2_if = lag->l2_if;
  if (port->handle_ingress_lag != NULL_HANDLE) {
    bm_client_ptr->bm_mt_delete_entry(cxt_id, "table_ingress_lag",
                                      port->handle_ingress_lag);
    port->handle_ingress_lag = NULL_HANDLE;
  }
  if (lag->handle_lag_hash != NULL_HANDLE) {
    bm_client_ptr->bm_mt_delete_entry(cxt_id, "table_lag_hash",
                                      lag->handle_lag_hash);
    lag->handle_lag_hash = NULL_HANDLE;
  }

  BmAddEntryOptions options;
  BmMatchParams match_params;
  BmActionData action_data;
  match_params.push_back(parse_exact_match_param(port->hw_port, 2));
  action_data.push_back(parse_param(1, 1));
  action_data.push_back(parse_param(lag->l2_if, 1));
  port->handle_ingress_lag = bm_client_ptr->bm_mt_add_entry(
      cxt_id, "table_ingress_lag", match_params, "action_set_lag_l2if",
      action_data, options);
  match_params.clear();
  match_params.push_back(parse_exact_match_param(l2_if, 1));
  match_params.push_back(
      parse_exact_match_param(lag->lag_members.size() - 1, 1));
  action_data.clear();
  action_data.push_back(parse_param(port->hw_port, 1));
  lag_member->handle_egress_lag = bm_client_ptr->bm_mt_add_entry(
      cxt_id, "table_egress_lag", match_params, "action_set_out_port",
      action_data, options);
  action_data.clear();
  match_params.clear();
  match_params.push_back(parse_exact_match_param(l2_if, 1));
  action_data.push_back(parse_param(lag->lag_members.size(), 1));
  lag->handle_lag_hash = bm_client_ptr->bm_mt_add_entry(
      cxt_id, "table_lag_hash", match_params, "action_set_lag_hash_size",
      action_data, options);
  uint32_t port_l2if = port->l2_if;
  port->l2_if = l2_if;
  config_port(port);
  port->l2_if = port_l2if;
  *lag_member_id = lag_member->sai_object_id;
  return SAI_STATUS_SUCCESS;
}

sai_status_t sai_adapter::remove_lag_member(sai_object_id_t lag_member_id) {
  (*logger)->info("sai_remove_lag_member: {}", lag_member_id);
  BmAddEntryOptions options;
  BmMatchParams match_params;
  BmActionData action_data;
  Lag_member_obj *lag_member = switch_metadata_ptr->lag_members[lag_member_id];
  Lag_obj *lag = lag_member->lag;
  sai_status_t status = SAI_STATUS_SUCCESS;
  std::vector<sai_object_id_t>::iterator iter = std::find(
      lag->lag_members.begin(), lag->lag_members.end(), lag_member_id);
  size_t hash_index = std::distance(lag->lag_members.begin(), iter);
  (*logger)->info("hash_index = {}", hash_index);
  lag->lag_members.erase(lag->lag_members.begin() + hash_index);
  bm_client_ptr->bm_mt_delete_entry(cxt_id, "table_lag_hash",
                                    lag->handle_lag_hash);
  action_data.clear();
  match_params.clear();
  match_params.push_back(parse_exact_match_param(lag->l2_if, 1));
  action_data.push_back(parse_param(lag->lag_members.size(), 1));
  lag->handle_lag_hash = bm_client_ptr->bm_mt_add_entry(
      cxt_id, "table_lag_hash", match_params, "action_set_lag_hash_size",
      action_data, options);
  bm_client_ptr->bm_mt_delete_entry(cxt_id, "table_ingress_lag",
                                    lag_member->port->handle_ingress_lag);
  lag_member->port->handle_ingress_lag = NULL_HANDLE;
  bm_client_ptr->bm_mt_delete_entry(cxt_id, "table_egress_lag",
                                    lag_member->handle_egress_lag);
  lag_member->handle_egress_lag = NULL_HANDLE;

  if (hash_index != lag->lag_members.size()) {
    sai_object_id_t last_lag_member_id = lag->lag_members.back();
    Lag_member_obj *last_lag_member =
        switch_metadata_ptr->lag_members[last_lag_member_id];
    lag->lag_members.pop_back();
    lag->lag_members.insert(lag->lag_members.begin() + hash_index,
                            last_lag_member_id);
    bm_client_ptr->bm_mt_delete_entry(cxt_id, "table_egress_lag",
                                      last_lag_member->handle_egress_lag);
    match_params.clear();
    match_params.push_back(parse_exact_match_param(lag->l2_if, 1));
    match_params.push_back(parse_exact_match_param(hash_index, 1));
    action_data.clear();
    action_data.push_back(parse_param(last_lag_member->port->hw_port, 1));
    last_lag_member->handle_egress_lag = bm_client_ptr->bm_mt_add_entry(
        cxt_id, "table_egress_lag", match_params, "action_set_out_port",
        action_data, options);
  }
  switch_metadata_ptr->lag_members.erase(lag_member->sai_object_id);
  sai_id_map_ptr->free_id(lag_member->sai_object_id);
  return status;
}
