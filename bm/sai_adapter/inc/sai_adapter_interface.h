#ifndef SAI_ADAPTER_INTERFACE_H
#define SAI_ADAPTER_INTERFACE_H
#include <sai.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef void *S_O_Handle;
S_O_Handle create_sai_adapter();
void free_sai_adapter(S_O_Handle);

// api
sai_status_t sai_adapter_api_query(S_O_Handle, sai_api_t, void **);

// switch
sai_status_t sai_adapter_create_switch(S_O_Handle, sai_object_id_t *, uint32_t,
                                       const sai_attribute_t *);
sai_status_t sai_adapter_get_switch_attribute(S_O_Handle, sai_object_id_t,
                                              sai_uint32_t, sai_attribute_t *);

// port
sai_status_t sai_adapter_create_port(S_O_Handle, sai_object_id_t *,
                                     sai_object_id_t, uint32_t,
                                     const sai_attribute_t *);
sai_status_t sai_adapter_remove_port(S_O_Handle, sai_object_id_t);
sai_status_t sai_adapter_set_port_attribute(S_O_Handle, sai_object_id_t port_id,
                                            sai_attribute_t *);
sai_status_t sai_adapter_get_port_attribute(S_O_Handle, sai_object_id_t port_id,
                                            uint32_t, sai_attribute_t *);

// bridge
sai_status_t sai_adapter_create_bridge(S_O_Handle, sai_object_id_t *,
                                       sai_object_id_t, uint32_t,
                                       const sai_attribute_t *);
sai_status_t sai_adapter_remove_bridge(S_O_Handle, sai_object_id_t);
sai_status_t sai_adapter_get_bridge_attribute(S_O_Handle,
                                              sai_object_id_t bridge_id,
                                              uint32_t, sai_attribute_t *);

// bridge_port
sai_status_t sai_adapter_create_bridge_port(S_O_Handle, sai_object_id_t *,
                                            sai_object_id_t, uint32_t,
                                            const sai_attribute_t *);
sai_status_t sai_adapter_remove_bridge_port(S_O_Handle, sai_object_id_t);
sai_status_t sai_adapter_get_bridge_port_attribute(
    S_O_Handle, sai_object_id_t bridge_port_id, uint32_t, sai_attribute_t *);

// FDB
sai_status_t sai_adapter_create_fdb_entry(S_O_Handle, sai_fdb_entry_t *,
                                          uint32_t, const sai_attribute_t *);
sai_status_t sai_adapter_remove_fdb_entry(S_O_Handle, sai_fdb_entry_t *);

// VLAN
sai_status_t sai_adapter_create_vlan(S_O_Handle, sai_object_id_t *,
                                     sai_object_id_t, uint32_t,
                                     const sai_attribute_t *);
sai_status_t sai_adapter_remove_vlan(S_O_Handle, sai_object_id_t);
sai_status_t sai_adapter_set_vlan_attribute(S_O_Handle, sai_object_id_t,
                                            const sai_attribute_t *);
sai_status_t sai_adapter_get_vlan_attribute(S_O_Handle, sai_object_id_t,
                                            const uint32_t, sai_attribute_t *);
sai_status_t sai_adapter_create_vlan_member(S_O_Handle, sai_object_id_t *,
                                            sai_object_id_t, uint32_t,
                                            const sai_attribute_t *);
sai_status_t sai_adapter_remove_vlan_member(S_O_Handle, sai_object_id_t);
sai_status_t sai_adapter_set_vlan_member_attribute(S_O_Handle, sai_object_id_t,
                                                   const sai_attribute_t *);
sai_status_t sai_adapter_get_vlan_member_attribute(S_O_Handle, sai_object_id_t,
                                                   const uint32_t,
                                                   sai_attribute_t *);
sai_status_t sai_adapter_get_vlan_stats(S_O_Handle, sai_object_id_t,
                                        const sai_vlan_stat_t *, uint32_t,
                                        uint64_t *);
sai_status_t sai_adapter_clear_vlan_stats(S_O_Handle, sai_object_id_t,
                                          const sai_vlan_stat_t *, uint32_t);

// LAG
sai_status_t sai_adapter_create_lag(S_O_Handle, sai_object_id_t *,
                                    sai_object_id_t, uint32_t,
                                    const sai_attribute_t *);
sai_status_t sai_adapter_remove_lag(S_O_Handle, sai_object_id_t);
sai_status_t sai_adapter_create_lag_member(S_O_Handle, sai_object_id_t *,
                                           sai_object_id_t, uint32_t,
                                           const sai_attribute_t *);
sai_status_t sai_adapter_remove_lag_member(S_O_Handle, sai_object_id_t);

// hostif
sai_status_t sai_adapter_create_hostif(S_O_Handle, sai_object_id_t *,
                                       sai_object_id_t, uint32_t,
                                       const sai_attribute_t *);
sai_status_t sai_adapter_remove_hostif(S_O_Handle, sai_object_id_t);
sai_status_t sai_adapter_create_hostif_table_entry(S_O_Handle,
                                                   sai_object_id_t *,
                                                   sai_object_id_t, uint32_t,
                                                   const sai_attribute_t *);
sai_status_t sai_adapter_remove_hostif_table_entry(S_O_Handle, sai_object_id_t);
sai_status_t sai_adapter_create_hostif_trap_group(S_O_Handle, sai_object_id_t *,
                                                  sai_object_id_t, uint32_t,
                                                  const sai_attribute_t *);
sai_status_t sai_adapter_remove_hostif_trap_group(S_O_Handle, sai_object_id_t);
sai_status_t sai_adapter_create_hostif_trap(S_O_Handle, sai_object_id_t *,
                                            sai_object_id_t, uint32_t,
                                            const sai_attribute_t *);
sai_status_t sai_adapter_remove_hostif_trap(S_O_Handle, sai_object_id_t);

#ifdef __cplusplus
}
#endif
#endif