#include <algorithm>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/queue.h>
#include <sys/types.h>
#include <fstream>
#include <sstream>
#include <set>
#include <iostream>
#include <getopt.h>
#include <assert.h>
#include <signal.h>

#include <cstring>
#include <thread>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "sai_rpc.h"

#define UNREFERENCED_PARAMETER(P)   (P)

extern "C" {
#include "sai.h"
#include "saistatus.h"
#include "switch_sai_rpc_server.h"
}


#define SWITCH_SAI_THRIFT_RPC_SERVER_PORT 9092

sai_switch_api_t* sai_switch_api;

std::map<std::string, std::string> gProfileMap;
std::map<std::set<int>, std::string> gPortMap;

std::vector<std::pair<sai_fdb_entry_t, sai_object_id_t>> gFdbMap;

sai_object_id_t gSwitchId; ///< SAI switch global object ID.


// Profile services
/* Get variable value given its name */
const char* test_profile_get_value(
        _In_ sai_switch_profile_id_t profile_id,
        _In_ const char* variable)
{
    UNREFERENCED_PARAMETER(profile_id);

    if (variable == NULL)
    {
        printf("variable is null\n");
        return NULL;
    }

    std::map<std::string, std::string>::const_iterator it = gProfileMap.find(variable);
    if (it == gProfileMap.end())
    {
        printf("%s: NULL\n", variable);
        return NULL;
    }

    return it->second.c_str();
}

std::map<std::string, std::string>::iterator gProfileIter = gProfileMap.begin();
/* Enumerate all the K/V pairs in a profile.
   Pointer to NULL passed as variable restarts enumeration.
   Function returns 0 if next value exists, -1 at the end of the list. */
int test_profile_get_next_value(
        _In_ sai_switch_profile_id_t profile_id,
        _Out_ const char** variable,
        _Out_ const char** value)
{
    UNREFERENCED_PARAMETER(profile_id);

    if (value == NULL)
    {
        printf("resetting profile map iterator");

        gProfileIter = gProfileMap.begin();
        return 0;
    }

    if (variable == NULL)
    {
        printf("variable is null");
        return -1;
    }

    if (gProfileIter == gProfileMap.end())
    {
        printf("iterator reached end");
        return -1;
    }

    *variable = gProfileIter->first.c_str();
    *value = gProfileIter->second.c_str();

    printf("key: %s:%s", *variable, *value);

    gProfileIter++;

    return 0;
}

const sai_service_method_table_t test_services = {
    test_profile_get_value,
    test_profile_get_next_value
};


struct cmdOptions
{
    std::string profileMapFile;
    std::string portMapFile;
    std::string initScript;
};

cmdOptions handleCmdLine(int argc, char **argv)
{

    cmdOptions options = {};

    while(true)
    {
        static struct option long_options[] =
        {
            { "profile",          required_argument, 0, 'p' },
            { "portmap",          required_argument, 0, 'f' },
            { "init-script",      required_argument, 0, 'S' },
            { 0,                  0,                 0,  0  }
        };

        int option_index = 0;

        int c = getopt_long(argc, argv, "p:f:S:", long_options, &option_index);

        if (c == -1)
            break;

        switch (c)
        {
            case 'p':
                printf("profile map file: %s\n", optarg);
                options.profileMapFile = std::string(optarg);
                break;

            case 'f':
                printf("port map file: %s\n", optarg);
                options.portMapFile = std::string(optarg);
                break;

            case 'S':
                printf("init script: %s\n", optarg);
                options.initScript = std::string(optarg);
                break;

            default:
                printf("getopt_long failure\n");
                exit(EXIT_FAILURE);
        }
    }

    return options;
}

void handleProfileMap(const std::string& profileMapFile)
{

    if (profileMapFile.size() == 0)
        return;

    std::ifstream profile(profileMapFile);

    if (!profile.is_open())
    {
        printf("failed to open profile map file: %s : %s\n", profileMapFile.c_str(), strerror(errno));
        exit(EXIT_FAILURE);
    }

    std::string line;

    while(getline(profile, line))
    {
        if (line.size() > 0 && (line[0] == '#' || line[0] == ';'))
            continue;

        size_t pos = line.find("=");

        if (pos == std::string::npos)
        {
            printf("not found '=' in line %s\n", line.c_str());
            continue;
        }

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        gProfileMap[key] = value;

        printf("insert: %s:%s\n", key.c_str(), value.c_str());
    }
}

void handlePortMap(const std::string& portMapFile)
{

    if (portMapFile.size() == 0)
        return;

    std::ifstream portmap(portMapFile);

    if (!portmap.is_open())
    {
        printf("failed to open port map file: %s : %s\n", portMapFile.c_str(), strerror(errno));
        exit(EXIT_FAILURE);
    }

    std::string line;

    while(getline(portmap, line))
    {
        if (line.size() > 0 && (line[0] == '#' || line[0] == ';'))
            continue;

        size_t pos = line.find(" ");

        if (pos == std::string::npos)
        {
            printf("not found ' ' in line %s\n", line.c_str());
            continue;
        }

        std::string fp_value = line.substr(0, pos);
        std::string lanes    = line.substr(pos + 1);

        // ::isspace : C-Style white space predicate. Locale independent.
        lanes.erase(std::remove_if(lanes.begin(), lanes.end(), ::isspace), lanes.end());

        std::istringstream iss(lanes);
        std::string lane_str;
        std::set<int> lane_set;

        while (getline(iss, lane_str, ','))
        {
            int lane = stoi(lane_str);
            lane_set.insert(lane);
        }

        gPortMap.insert(std::pair<std::set<int>,std::string>(lane_set,fp_value));
    }
}

void handleInitScript(const std::string& initScript)
{

    if (initScript.size() == 0)
        return;

    printf("Running %s ...\n", initScript.c_str());
    system(initScript.c_str());
}

int start_sai_thrift_rpc_server(int port)
{
    static char port_str[10];
    snprintf(port_str, sizeof(port_str), "%d", port);
    return start_p4_sai_thrift_rpc_server(port_str);
}

int
main(int argc, char* argv[])
{
    int rv = 0;

    auto options = handleCmdLine(argc, argv);
    handleProfileMap(options.profileMapFile);
    handlePortMap(options.portMapFile);

    sai_api_initialize(0, &test_services);
    sai_api_query(SAI_API_SWITCH, (void**)&sai_switch_api);


    handleInitScript(options.initScript);

    start_sai_thrift_rpc_server(SWITCH_SAI_THRIFT_RPC_SERVER_PORT);

    const sai_log_level_t log_level = SAI_LOG_LEVEL_NOTICE;

    sai_log_set(SAI_API_ACL, log_level);
    sai_log_set(SAI_API_BRIDGE, log_level);
    sai_log_set(SAI_API_BUFFER, log_level);
    sai_log_set(SAI_API_DEBUG_COUNTER, log_level);
    sai_log_set(SAI_API_FDB, log_level);
    sai_log_set(SAI_API_HOSTIF, log_level);
    sai_log_set(SAI_API_LAG, log_level);
    sai_log_set(SAI_API_MIRROR, log_level);
    sai_log_set(SAI_API_NEIGHBOR, log_level);
    sai_log_set(SAI_API_NEXT_HOP, log_level);
    sai_log_set(SAI_API_NEXT_HOP_GROUP, log_level);
    sai_log_set(SAI_API_POLICER, log_level);
    sai_log_set(SAI_API_PORT, log_level);
    sai_log_set(SAI_API_QOS_MAP, log_level);
    sai_log_set(SAI_API_ROUTE, log_level);
    sai_log_set(SAI_API_ROUTER_INTERFACE, log_level);
    sai_log_set(SAI_API_SWITCH, log_level);
    sai_log_set(SAI_API_TUNNEL, log_level);
    sai_log_set(SAI_API_VIRTUAL_ROUTER, log_level);
    sai_log_set(SAI_API_VLAN, log_level);
    sai_log_set(SAI_API_WRED, log_level);

    while (1) pause();

    return rv;
}
