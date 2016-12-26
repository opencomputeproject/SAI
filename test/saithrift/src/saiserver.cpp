#include <sys/socket.h>
#include <sys/queue.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <getopt.h>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <set>

#include <thread>
#include <chrono>

#include "switch_sai_rpc.h"
#include "switch_sai_rpc_server.h"

#define UNREFERENCED_PARAMETER(P) (P)

extern "C" {
#include "sai.h"
#include "saistatus.h"
}

#define SWITCH_SAI_THRIFT_RPC_SERVER_PORT 9092

sai_switch_api_t *sai_switch_api; ///< SAI switch API.

std::map<std::string, std::string> gProfileMap; ///< Profile map.
std::map<std::set<int>, std::string> gPortMap; ///< Port map.

sai_object_id_t gSwitchId; ///< SAI switch global object ID.

void on_switch_state_change(_In_ sai_switch_oper_status_t switch_oper_status) { }
void on_fdb_event(_In_ uint32_t count, _In_ sai_fdb_event_notification_data_t *data) { }
void on_port_state_change(_In_ uint32_t count, _In_ sai_port_oper_status_notification_t *data) { }
void on_port_event(_In_ uint32_t count) { }
void on_shutdown_request() { }
void on_packet_event(_In_ const void *buffer,
                     _In_ sai_size_t buffer_size,
                     _In_ uint32_t attr_count,
                     _In_ const sai_attribute_t *attr_list) { }

/// @brief Profile services.
/// @details Get variable value given its name.
/// @param profile_id Profile ID.
/// @param variable Variable.
/// @returns Value in string.
const char* test_profile_get_value(_In_ sai_switch_profile_id_t profile_id,
                                   _In_ const char* variable)
{
    UNREFERENCED_PARAMETER(profile_id);

    if (variable == nullptr)
    {
        std::printf("variable is null\n");
        return nullptr;
    }

    std::map<std::string, std::string>::const_iterator it = gProfileMap.find(variable);

    if (it == gProfileMap.end())
    {
        std::printf("%s: NULL\n", variable);
        return nullptr;
    }

    return it->second.c_str();
}

std::map<std::string, std::string>::iterator gProfileIter = gProfileMap.begin();

/// @brief Profile services.
/// @details Enumerate all the K/V pairs in a profile.
/// @details Pointer to NULL passed as variable restarts enumeration.
/// @details Function returns 0 if next value exists, -1 at the end of the list.
/// @param profile_id Profile ID.
/// @param variable Variable.
/// @param value Value.
/// @returns Operation result.
int test_profile_get_next_value(_In_ sai_switch_profile_id_t profile_id,
                                _Out_ const char** variable,
                                _Out_ const char** value)
{
    UNREFERENCED_PARAMETER(profile_id);

    if (value == nullptr)
    {
        std::printf("resetting profile map iterator");
        gProfileIter = gProfileMap.begin();
        return 0;
    }

    if (variable == nullptr)
    {
        std::printf("variable is null");
        return -1;
    }

    if (gProfileIter == gProfileMap.end())
    {
        std::printf("iterator reached end");
        return -1;
    }

    *variable = gProfileIter->first.c_str();
    *value = gProfileIter->second.c_str();

    std::printf("key: %s:%s", *variable, *value);

    gProfileIter++;

    return 0;
}

service_method_table_t test_services = { test_profile_get_value, test_profile_get_next_value };

#ifdef BRCMSAI
void sai_diag_shell()
{
    sai_status_t status;

    while (true)
    {
        sai_attribute_t attr;
        attr.id = SAI_SWITCH_ATTR_SWITCH_SHELL_ENABLE;
        attr.value.booldata = true;
        status = sai_switch_api->set_switch_attribute(&attr);
        if (status != SAI_STATUS_SUCCESS)
        {
            return;
        }

        sleep(1);
    }
}
#endif

/// @struct cmdLineOptions.
/// @brief Defines command line options.
typedef struct cmdLineOptions
{
    std::string profileMapFile; ///< Profile map file path.
    std::string portMapFile; ///< Port map file path.
} cmdLineOptions_t;

/// @brief Executes init script.
void execInitScript(const std::string &initScript)
{
    if (initScript.empty()) { return; }

    std::printf("Running %s ...\n", initScript.c_str());
    system(initScript.c_str());
}

/// @brief Parses command line arguments.
/// @param argc Number of command line arguments.
/// @param argv Command line arguments array.
/// @returns Command line options.
static cmdLineOptions_t parseCmdLineArgs(int argc, char **argv) noexcept
{
    cmdLineOptions_t options = {};

    while (true)
    {
        static struct option long_options[] =
        {
            { "profile", required_argument, 0, 'p' },
            { "portmap", required_argument, 0, 'f' },
            { 0,         0,                 0,  0  }
        };

        int option_index = 0;

        int c = getopt_long(argc, argv, "p:f:", long_options, &option_index);

        if (c == -1) { break; }

        switch (c)
        {
            case 'p':
                std::cout << "Profile map file: " << optarg << "." << std::endl;
                options.profileMapFile.assign(optarg);
                break;

            case 'f':
                std::cout << "Port map file: " << optarg << "." << std::endl;
                options.portMapFile.assign(optarg);
                break;

            default:
                std::cout << "Function 'getopt_long' failure." << std::endl;
                std::exit(EXIT_FAILURE);
        }
    }

    return options;
}

/// @brief Parses profile map file.
/// @param profileMapFile Profile map file path.
/// @returns Operation result.
/// @retval true  Success.
/// @retval false Failure.
static bool parseProfileMap(const std::string &profileMapFile) noexcept
{
    std::cout << "Start parse profile map file." << std::endl;

    if (profileMapFile.empty())
    { std::cout << "No profile map file path specified." << std::endl; return false; }

    std::ifstream profile(profileMapFile, std::ifstream::in);

    if (!profile.is_open())
    {
        std::cout << "Failed to open profile map file: " << profileMapFile << ". ";
        std::cout << "Errno: " << std::strerror(errno) << ".";
        std::cout << std::endl;

        return false;
    }

    std::string line;

    while (std::getline(profile, line))
    {
        if (!line.empty() && (line[0] == '#' || line[0] == ';')) { continue; }

        const auto pos = line.find('=');

        if (pos == std::string::npos)
        { std::cout << "Not found '=' in line: " << line << "." << std::endl; continue; }

        const auto key = line.substr(0, pos);
        const auto value = line.substr(pos + 1);

        gProfileMap[key] = value;

        std::cout << "Insert: " << key << ":" << value << "." << std::endl;
    }

    std::cout << "Finish parse profile map file." << std::endl;

    return true;
}

/// @brief Parses port map file.
/// @param portMapFile Port map file path.
/// @returns Operation result.
/// @retval true  Success.
/// @retval false Failure.
static bool parsePortMap(const std::string &portMapFile) noexcept
{
    std::cout << "Start parse port map file." << std::endl;

    if (portMapFile.empty())
    { std::cout << "No port map file path specified." << std::endl; return false; }

    std::ifstream portmap(portMapFile, std::ifstream::in);

    if (!portmap.is_open())
    {
        std::cout << "Failed to open port map file: " << portMapFile << ". ";
        std::cout << "Errno: " << std::strerror(errno) << ".";
        std::cout << std::endl;

        return false;
    }

    std::string line;

    while (std::getline(portmap, line))
    {
        if (!line.empty() && (line[0] == '#' || line[0] == ';')) { continue; }

        auto pos = line.find(' ');

        if (pos == std::string::npos)
        { std::cout << "Not found ' ' in line: " << line << "." << std::endl; continue; }

        auto fp_value = line.substr(0, pos);
        auto lanes = line.substr(pos + 1);

        // ::isspace : C-Style white space predicate. Locale independent.
        lanes.erase(std::remove_if(lanes.begin(), lanes.end(), ::isspace), lanes.end());

        std::istringstream iss(lanes);
        std::string lane_str;
        std::set<int> lane_set;

        while (std::getline(iss, lane_str, ','))
        {
            int lane;

            try { lane = std::stoi(lane_str, nullptr, 10); }
            catch (...) { std::cout << "Failed to parse hardware lane: " << lane_str << "."; return false; }

            lane_set.insert(lane);
        }

        gPortMap.insert(std::pair<std::set<int>,std::string>(lane_set,fp_value));
    }

    for (const auto& map : gPortMap)
    {
        std::cout << "Insert: ";

        for (const auto& lane : map.first) { std::cout << lane << ","; }

        std::cout << " " << map.second << "." << std::endl;
    }

    std::cout << "Finish parse port map file." << std::endl;

    return true;
}

/// @brief Initializes switch.
/// @returns Operation result.
/// @retval true  Success.
/// @retval false Failure.
static bool initSwitch() noexcept
{
    std::cout << "Start switch initialization." << std::endl;

    sai_api_initialize(0, &test_services);
    sai_api_query(SAI_API_SWITCH, reinterpret_cast<void**>(&sai_switch_api));

    if (sai_switch_api == nullptr)
    { std::cout << "Failed to initialize switch API." << std::endl; return false; }

    constexpr std::uint32_t hwIdSz = 256;

    char hwId[hwIdSz + 1];
    std::memset(hwId, '\0', sizeof(hwId));
    std::memcpy(hwId, gProfileMap["hwId"].c_str(), gProfileMap["hwId"].size() + 1);

    constexpr std::uint32_t attrCnt = 2;

    sai_attribute_t attr[attrCnt];
    std::memset(attr, '\0', sizeof(attr));

    attr[0].id = SAI_SWITCH_ATTR_INIT_SWITCH;
    attr[0].value.booldata = true;

    attr[1].id = SAI_SWITCH_ATTR_SWITCH_HARDWARE_INFO;
    attr[1].value.s8list.list = reinterpret_cast<int8_t*>(hwId);

    if (sai_switch_api->create_switch(&gSwitchId, 2, attr) != SAI_STATUS_SUCCESS)
    { std::cout << "Failed to create switch." << std::endl; return false; }

    std::cout << "Finish switch initialization." << std::endl;

    return true;
}

/// @brief Initializes logging engine.
static void initLogEngine() noexcept
{
    sai_log_set(SAI_API_SWITCH, SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_PORT, SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_FDB, SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_VLAN, SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_VIRTUAL_ROUTER, SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_ROUTE, SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_NEXT_HOP, SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_NEXT_HOP_GROUP, SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_ROUTER_INTERFACE, SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_NEIGHBOR, SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_ACL, SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_HOST_INTERFACE, SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_MIRROR, SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_SAMPLEPACKET, SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_STP, SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_LAG, SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_POLICER, SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_WRED, SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_QOS_MAPS, SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_QUEUE, SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_SCHEDULER, SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_SCHEDULER_GROUP, SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_BUFFERS, SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_HASH, SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_UDF, SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_TUNNEL, SAI_LOG_LEVEL_NOTICE);
}

/// @brief Server entry point.
/// @param argc Number of command line arguments.
/// @param argv Command line arguments array.
int main(int argc, char* argv[])
{
    auto options = parseCmdLineArgs(argc, argv);

    if (!parseProfileMap(options.profileMapFile)) { std::exit(EXIT_FAILURE); }
    if (!parsePortMap(options.portMapFile)) { std::exit(EXIT_FAILURE); }

    initLogEngine();

    if (!initSwitch()) { std::exit(EXIT_FAILURE); }

    // execInitScript(options.initScript);

#ifdef BRCMSAI
    std::thread bcm_diag_shell_thread = std::thread(sai_diag_shell);
    bcm_diag_shell_thread.detach();
#endif

    start_sai_thrift_rpc_server(SWITCH_SAI_THRIFT_RPC_SERVER_PORT);

    while (true) { pause(); }

    return EXIT_SUCCESS;
}
