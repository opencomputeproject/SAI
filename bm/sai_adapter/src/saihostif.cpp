#include "../inc/sai_adapter.h"

sai_status_t sai_adapter::create_hostif(sai_object_id_t *hif_id,
                                    sai_object_id_t switch_id,
                                    uint32_t attr_count,
                                    const sai_attribute_t *attr_list) {
	(*logger)->info("create_hostif");
  HostIF_obj *hostif = new HostIF_obj(sai_id_map_ptr);
  switch_metadata_ptr->hostifs[hostif->sai_object_id] = hostif;
  // parsing attributes
  sai_attribute_t attribute;
  for (uint32_t i = 0; i < attr_count; i++) {
    attribute = attr_list[i];
    switch (attribute.id) {
    case SAI_HOSTIF_ATTR_TYPE:
      hostif->hostif_type = (sai_hostif_type_t)attribute.value.s32;
      break;
    case SAI_HOSTIF_ATTR_OBJ_ID:
      hostif->port = switch_metadata_ptr->ports[attribute.value.oid];
      break;
    case SAI_HOSTIF_ATTR_NAME:
      hostif->netdev_name = string(attribute.value.chardata);
      break;
    }
  }

  *hif_id = hostif->sai_object_id;
}

sai_status_t sai_adapter::remove_hostif(sai_object_id_t hif_id) {
	(*logger)->info("remove_hostif");
}

sai_status_t sai_adapter::create_hostif_table_entry(
      sai_object_id_t *hif_table_entry, sai_object_id_t switch_id,
      uint32_t attr_count, const sai_attribute_t *attr_list) {
	(*logger)->info("create_hostif_table_entry");
  HostIF_Table_obj *hostif_table = new HostIF_Table_obj(sai_id_map_ptr);
  switch_metadata_ptr->hostif_tables[hostif_table->sai_object_id] = hostif_table;

  *hif_table_entry = hostif_table->sai_object_id;
}

sai_status_t sai_adapter::remove_hostif_table_entry(
      sai_object_id_t hif_table_entry) {
	(*logger)->info("remove_hostif_table_entry");
}

sai_status_t sai_adapter::create_hostif_trap_group(
      sai_object_id_t *hostif_trap_group_id, sai_object_id_t switch_id,
      uint32_t attr_count, const sai_attribute_t *attr_list) {
	(*logger)->info("create_hostif_trap_group");
  HostIF_Trap_Group_obj *hostif_trap_group = new HostIF_Trap_Group_obj(sai_id_map_ptr);
  switch_metadata_ptr->hostif_trap_groups[hostif_trap_group->sai_object_id] = hostif_trap_group;

  *hostif_trap_group_id = hostif_trap_group->sai_object_id;
}

sai_status_t sai_adapter::remove_hostif_trap_group(
      sai_object_id_t hostif_trap_group_id) {
	(*logger)->info("remove_hostif_trap_group");
}

sai_status_t sai_adapter::create_hostif_trap(sai_object_id_t *hostif_trap_id,
                                         sai_object_id_t switch_id,
                                         uint32_t attr_count,
                                         const sai_attribute_t *attr_list) {
	(*logger)->info("create_hostif_trap");
  HostIF_Trap_obj *hostif_trap = new HostIF_Trap_obj(sai_id_map_ptr);
  switch_metadata_ptr->hostif_traps[hostif_trap->sai_object_id] = hostif_trap;

  *hostif_trap_id = hostif_trap->sai_object_id;
}

sai_status_t sai_adapter::remove_hostif_trap(sai_object_id_t hostif_trap_id) {
	(*logger)->info("remove_hostif_trap");
}