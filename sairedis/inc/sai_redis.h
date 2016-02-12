#ifndef __SAI_REDIS__
#define __SAI_REDIS__

#include "stdint.h"
#include "stdio.h"

#include "sai.h"

extern service_method_table_t           g_services;

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
#define UTILS_LOG(level, fmt, arg ...) printf("log TODO")

#define REDIS_LOG_ENTER()           UTILS_LOG(SAI_LOG_DEBUG, "%s: [\n", __FUNCTION__)
#define REDIS_LOG_EXIT()            UTILS_LOG(SAI_LOG_DEBUG, "%s: ]\n", __FUNCTION__)
#define REDIS_LOG_DBG(fmt, arg ...) UTILS_LOG(SAI_LOG_DEBUG, fmt, ## arg)
#define REDIS_LOG_INF(fmt, arg ...) UTILS_LOG(SAI_LOG_INFO, fmt, ## arg)
#define REDIS_LOG_WRN(fmt, arg ...) UTILS_LOG(SAI_LOG_WARN, fmt, ## arg)
#define REDIS_LOG_ERR(fmt, arg ...) UTILS_LOG(SAI_LOG_ERROR, fmt, ## arg)
#define REDIS_LOG_NTC(fmt, arg ...) UTILS_LOG(SAI_LOG_NOTICE, fmt, ## arg)

#endif // __SAI_REDIS__

