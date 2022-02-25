#include <sai.h>
#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

const char* test_profile_get_value(
    _In_ sai_switch_profile_id_t profile_id,
    _In_ const char* variable)
{
    // UNREFERENCED_PARAMETER(profile_id);

    if (!strcmp(variable, "SAI_KEY_INIT_CONFIG_FILE")) {
        return "/usr/share/sai_2410.xml";
    }
    else if (!strcmp(variable, "KV_DEVICE_MAC_ADDRESS")) {
        return "20:03:04:05:06:00";
    }
    else if (!strcmp(variable, "SAI_KEY_L3_ROUTE_TABLE_SIZE")) {
        //return "1000";
    }
    else if (!strcmp(variable, "SAI_KEY_L3_NEIGHBOR_TABLE_SIZE")) {
        //return "2000";
    }

    return NULL;
}

/* Enumerate all the K/V pairs in a profile.
Pointer to NULL passed as variable restarts enumeration.
Function returns 0 if next value exists, -1 at the end of the list. */
int test_profile_get_next_value(
    _In_ sai_switch_profile_id_t profile_id,
    _Out_ const char** variable,
    _Out_ const char** value)
{
    // UNREFERENCED_PARAMETER(profile_id);
    // UNREFERENCED_PARAMETER(variable);
    // UNREFERENCED_PARAMETER(value);

    return -1;
}

const service_method_table_t test_services = {
    test_profile_get_value,
    test_profile_get_next_value
};

/* Enumerate all the K/V pairs in a profile.
Pointer to NULL passed as variable restarts enumeration.
Function returns 0 if next value exists, -1 at the end of the list. */

int main(int argc, char **argv)
{
    printf("sai_api sai_api_initialize\n");
    sai_port_api_t* port_api;

    sai_api_initialize(0, &test_services);
    printf("sai_api_initialized\n");
    sai_api_query(SAI_API_PORT, (void**)&port_api);
    sai_object_id_t port_id;
    sai_object_id_t switch_id = 0;
    uint32_t attr_count = 0;
    sai_attribute_t *attr_list = NULL;
    printf("sai_create_port\n");
    sai_status_t status = port_api->create_port(&port_id, switch_id, attr_count, attr_list);
    printf("port_created\n");
    sai_api_uninitialize();
    printf("sai_api_uninitialized\n");
}
