#include <sai.h>
#include <stdlib.h>
#include "../inc/sai_adapter_interface.h"
#include <stdio.h>
// static sai_api_service_t sai_api_service;
static S_O_Handle sai_adapter;
static sai_api_t api_id = SAI_API_UNSPECIFIED;
// switch_device_t device = 0;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

static const char *module[] = {
    "UNSPECIFIED",
    "SWITCH",
    "PORT",
    "FDB",
    "VLAN",
    "VIRTUAL_ROUTER",
    "ROUTE",
    "NEXT_HOP",
    "NEXT_HOP_GROUP",
    "ROUTER_INTERFACE",
    "NEIGHBOR",
    "ACL",
    "HOST_INTERFACE",
    "MIRROR",
    "SAMPLEPACKET",
    "STP",
    "LAG",
    "POLICER",
    "WRED",
    "QOS_MAP",
    "QUEUE",
    "SCHEDULER",
    "SCHEDULER_GROUP",
    "BUFFERS",
    "HASH",
    "UDF",
    "IPMC",
    "L2MC",
};

sai_status_t sai_api_query(sai_api_t sai_api_id, void **api_method_table) {
  sai_status_t status = SAI_STATUS_SUCCESS;

  // SAI_LOG_ENTER();

  if (!api_method_table) {
    status = SAI_STATUS_INVALID_PARAMETER;
    // SAI_LOG_ERROR("null api method table: %s", sai_status_to_string(status));
    return status;
  }

  status = sai_adapter_api_query(sai_adapter, sai_api_id, api_method_table);
  return status;
}

sai_status_t sai_api_initialize(uint64_t flags,
                                const service_method_table_t *services) {
  sai_adapter = create_sai_adapter();
}

sai_status_t sai_api_uninitialize(void) { 
    free_sai_adapter(sai_adapter);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */