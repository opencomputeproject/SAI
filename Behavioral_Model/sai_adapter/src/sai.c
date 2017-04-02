#include "../../../inc/sai.h"
// #include "sai_bm_c_api.h"
#include <stdlib.h>
#include "../inc/sai_adapter_interface.h"
// static sai_api_service_t sai_api_service;
static S_O_Handle sai_adapter;
static sai_api_t api_id = SAI_API_UNSPECIFIED;
// switch_device_t device = 0;

const char *sai_profile_get_value(_In_ sai_switch_profile_id_t profile_id,
                                  _In_ const char *variable) {
  return NULL;
}

/*
 * Enumerate all the K/V pairs in a profile.
 * Pointer to NULL passed as variable restarts enumeration.
 * Function returns 0 if next value exists, -1 at the end of the list.
 */
int sai_profile_get_next_value(_In_ sai_switch_profile_id_t profile_id,
                               _Out_ const char **variable,
                               _Out_ const char **value) {
  return -1;
}

const service_method_table_t sai_services = {
    .profile_get_value = sai_profile_get_value,
    .profile_get_next_value = sai_profile_get_next_value};

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

sai_status_t sai_api_uninitialize(void) { free_sai_adapter(sai_adapter); }

#ifdef __cplusplus
}
#endif /* __cplusplus */