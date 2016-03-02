#ifndef __SAI_REDIS__
#define __SAI_REDIS__

#include "stdint.h"
#include "stdio.h"

#include "sai.h"
#include "sai_serialize.h"
#include "sai_attribute_list.h"

#include "common/dbconnector.h"
#include "common/producertable.h"
#include "common/consumertable.h"
#include "common/redishash.h"
#include "common/select.h"

extern service_method_table_t           g_services;
extern ssw::DBConnector                *g_db;
extern ssw::ProducerTable              *g_asicState;

extern ssw::ProducerTable              *g_redisGetProducer;
extern ssw::ConsumerTable              *g_redisGetConsumer;

extern ssw::RedisHash *g_vidToRid;
extern ssw::RedisHash *g_ridToVid;

extern const sai_acl_api_t              redis_acl_api;
extern const sai_buffer_api_t           redis_buffer_api;
extern const sai_fdb_api_t              redis_fdb_api;
extern const sai_hash_api_t             redis_hash_api;
extern const sai_hostif_api_t           redis_host_interface_api;
extern const sai_lag_api_t              redis_lag_api;
extern const sai_mirror_api_t           redis_mirror_api;
extern const sai_neighbor_api_t         redis_neighbor_api;
extern const sai_next_hop_api_t         redis_next_hop_api;
extern const sai_next_hop_group_api_t   redis_next_hop_group_api;
extern const sai_policer_api_t          redis_policer_api;
extern const sai_port_api_t             redis_port_api;
extern const sai_qos_map_api_t          redis_qos_map_api;
extern const sai_queue_api_t            redis_queue_api;
extern const sai_route_api_t            redis_route_api;
extern const sai_router_interface_api_t redis_router_interface_api;
extern const sai_samplepacket_api_t     redis_samplepacket_api;
extern const sai_scheduler_api_t        redis_scheduler_api;
extern const sai_scheduler_group_api_t  redis_scheduler_group_api;
extern const sai_stp_api_t              redis_stp_api;
extern const sai_switch_api_t           redis_switch_api;
extern const sai_udf_api_t              redis_udf_api;
extern const sai_virtual_router_api_t   redis_router_api;
extern const sai_vlan_api_t             redis_vlan_api;
extern const sai_wred_api_t             redis_wred_api;

#define UNREFERENCED_PARAMETER(X)
#define REDIS_LOG(level, fmt, arg ...) {\
    fprintf(stderr, "(level:%d): ", level); \
    fprintf(stderr, fmt, ##arg); \
    fprintf(stderr, "\n"); }

#define REDIS_LOG_ENTER()   REDIS_LOG(SAI_LOG_DEBUG, "%s: >", __FUNCTION__)
#define REDIS_LOG_EXIT()    REDIS_LOG(SAI_LOG_DEBUG, "%s: <", __FUNCTION__)
#define REDIS_LOG_DBG(fmt, arg ...) REDIS_LOG(SAI_LOG_DEBUG, fmt, ##arg)
#define REDIS_LOG_INF(fmt, arg ...) REDIS_LOG(SAI_LOG_INFO, fmt, ##arg)
#define REDIS_LOG_WRN(fmt, arg ...) REDIS_LOG(SAI_LOG_WARN, fmt, ##arg)
#define REDIS_LOG_ERR(fmt, arg ...) REDIS_LOG(SAI_LOG_ERROR, fmt, ##arg)
#define REDIS_LOG_NTC(fmt, arg ...) REDIS_LOG(SAI_LOG_NOTICE, fmt, ##arg)

sai_object_id_t redis_create_virtual_object_id(
        _In_ sai_object_type_t object_type);

// separate methods are needed for vlan to not confuse with object_id

sai_status_t redis_generic_create(
        _In_ sai_object_type_t object_type,
        _Out_ sai_object_id_t* object_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

sai_status_t redis_generic_create(
        _In_ sai_object_type_t object_type,
        _In_ const sai_fdb_entry_t *fdb_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

sai_status_t redis_generic_create(
        _In_ sai_object_type_t object_type,
        _In_ const sai_neighbor_entry_t* neighbor_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

sai_status_t redis_generic_create(
        _In_ sai_object_type_t object_type,
        _In_ const sai_unicast_route_entry_t* unicast_route_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

sai_status_t redis_generic_create_vlan(
        _In_ sai_object_type_t object_type,
        _In_ sai_vlan_id_t vlan_id);


sai_status_t redis_generic_remove(
        _In_ sai_object_type_t object_type,
        _In_ sai_object_id_t object_id);

sai_status_t redis_generic_remove(
        _In_ sai_object_type_t object_type,
        _In_ const sai_fdb_entry_t* fdb_entry);

sai_status_t redis_generic_remove(
        _In_ sai_object_type_t object_type,
        _In_ const sai_neighbor_entry_t* neighbor_entry);

sai_status_t redis_generic_remove(
        _In_ sai_object_type_t object_type,
        _In_ const sai_unicast_route_entry_t* unicast_route_entry);

sai_status_t redis_generic_remove_vlan(
        _In_ sai_object_type_t object_type,
        _In_ sai_vlan_id_t vlan_id);


sai_status_t redis_generic_set(
        _In_ sai_object_type_t object_type,
        _In_ sai_object_id_t object_id,
        _In_ const sai_attribute_t *attr);

sai_status_t redis_generic_set(
        _In_ sai_object_type_t object_type,
        _In_ const sai_fdb_entry_t *fdb_entry,
        _In_ const sai_attribute_t *attr);

sai_status_t redis_generic_set(
        _In_ sai_object_type_t object_type,
        _In_ const sai_neighbor_entry_t* neighbor_entry,
        _In_ const sai_attribute_t *attr);

sai_status_t redis_generic_set(
        _In_ sai_object_type_t object_type,
        _In_ const sai_unicast_route_entry_t* unicast_route_entry,
        _In_ const sai_attribute_t *attr);

sai_status_t redis_generic_set_vlan(
        _In_ sai_object_type_t object_type,
        _In_ sai_vlan_id_t vlan_id,
        _In_ const sai_attribute_t *attr);


sai_status_t redis_generic_get(
        _In_ sai_object_type_t object_type,
        _In_ sai_object_id_t object_id,
        _In_ uint32_t attr_count,
        _Out_ sai_attribute_t *attr_list);

sai_status_t redis_generic_get(
        _In_ sai_object_type_t object_type,
        _In_ const sai_fdb_entry_t *fdb_entry,
        _In_ uint32_t attr_count,
        _Out_ sai_attribute_t *attr_list);

sai_status_t redis_generic_get(
        _In_ sai_object_type_t object_type,
        _In_ const sai_neighbor_entry_t* neighbor_entry,
        _In_ uint32_t attr_count,
        _Out_ sai_attribute_t *attr_list);

sai_status_t redis_generic_get(
        _In_ sai_object_type_t object_type,
        _In_ const sai_unicast_route_entry_t* unicast_route_entry,
        _In_ uint32_t attr_count,
        _Out_ sai_attribute_t *attr_list);

sai_status_t redis_generic_get_vlan(
        _In_ sai_object_type_t object_type,
        _In_ sai_vlan_id_t vlan_id,
        _In_ uint32_t attr_count,
        _Out_ sai_attribute_t *attr_list);


#endif // __SAI_REDIS__
