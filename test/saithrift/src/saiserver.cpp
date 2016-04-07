#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/queue.h>
#include <sys/types.h>

#include <getopt.h>
#include <assert.h>
#include <signal.h>

#include <thread>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "switch_sai_rpc.h"
#include "switch_sai_rpc_server.h"

#define UNREFERENCED_PARAMETER(P)   (P)

extern "C" {
#include "sai.h"
#include "saistatus.h"
}


#define SWITCH_SAI_THRIFT_RPC_SERVER_PORT 9092

sai_switch_api_t* sai_switch_api;

std::map<std::string, std::string> g_pfmap;

void on_switch_state_change(_In_ sai_switch_oper_status_t switch_oper_status)
{
}

void on_fdb_event(_In_ uint32_t count,
                  _In_ sai_fdb_event_notification_data_t *data)
{
}

void on_port_state_change(_In_ uint32_t count,
                          _In_ sai_port_oper_status_notification_t *data)
{
}

void on_port_event(_In_ uint32_t count,
                   _In_ sai_port_event_notification_t *data)
{
}

void on_shutdown_request()
{
}

void on_packet_event(_In_ const void *buffer,
                     _In_ sai_size_t buffer_size,
                     _In_ uint32_t attr_count,
                     _In_ const sai_attribute_t *attr_list)
{
}

sai_switch_notification_t switch_notifications = {
    on_switch_state_change,
    on_fdb_event,
    on_port_state_change,
    on_port_event,
    on_shutdown_request,
    on_packet_event
};

// Profile services
/* Get variable value given its name */
const char* test_profile_get_value(
        _In_ sai_switch_profile_id_t profile_id,
        _In_ const char* variable)
{
    UNREFERENCED_PARAMETER(profile_id);

    std::map<std::string, std::string>::const_iterator it = g_pfmap.find(variable);
    if (it == g_pfmap.end())
    {
        return NULL;
    }

    return it->second.c_str();
}

/* Enumerate all the K/V pairs in a profile.
   Pointer to NULL passed as variable restarts enumeration.
   Function returns 0 if next value exists, -1 at the end of the list. */
int test_profile_get_next_value(
        _In_ sai_switch_profile_id_t profile_id,
        _Out_ const char** variable,
        _Out_ const char** value)
{
    UNREFERENCED_PARAMETER(profile_id);
    UNREFERENCED_PARAMETER(variable);
    UNREFERENCED_PARAMETER(value);

    return -1;
}

const service_method_table_t test_services = {
    test_profile_get_value,
    test_profile_get_next_value
};

#ifdef BRCMSAI
void sai_diag_shell()
{
    sai_status_t status;

    while (true)
    {
        sai_attribute_t attr;
        attr.id = SAI_SWITCH_ATTR_CUSTOM_RANGE_BASE + 1;
        status = sai_switch_api->set_switch_attribute(&attr);
        if (status != SAI_STATUS_SUCCESS)
        {
            return;
        }

        sleep(1);
    }
}
#endif

int
main(int argc, char* argv[])
{
    int rv = 0;

    sai_api_initialize(0, (service_method_table_t *)&test_services);
    sai_api_query(SAI_API_SWITCH, (void**)&sai_switch_api);
#ifdef BRCMSAI
    sai_status_t status = sai_switch_api->initialize_switch(0, "0xb850", "", &switch_notifications);
    if (status != SAI_STATUS_SUCCESS)
    {
        exit(EXIT_FAILURE);
    }

    std::thread bcm_diag_shell_thread = std::thread(sai_diag_shell);
    bcm_diag_shell_thread.detach();
#endif

    start_sai_thrift_rpc_server(SWITCH_SAI_THRIFT_RPC_SERVER_PORT);

    while (1) pause();

    return rv;
}
