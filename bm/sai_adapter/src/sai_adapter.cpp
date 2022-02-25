#include "../inc/sai_adapter.h"

StandardClient *sai_adapter::bm_client_ptr;
sai_id_map_t *sai_adapter::sai_id_map_ptr;
Switch_metadata *sai_adapter::switch_metadata_ptr;
std::vector<sai_object_id_t> *sai_adapter::switch_list_ptr;
std::shared_ptr<spdlog::logger> *sai_adapter::logger;
bool sai_adapter::pcap_loop_started;
std::mutex sai_adapter::m;

sai_adapter::sai_adapter()
    : //  constructor pre initializations
      socket(new TSocket("localhost", bm_port)),
      transport(new TBufferedTransport(socket)),
      bprotocol(new TBinaryProtocol(transport)),
      protocol(new TMultiplexedProtocol(bprotocol, "standard")),
      bm_client(protocol) {
  // logger
  logger_o = spdlog::get("logger");
  if (logger_o == 0) {
    logger_o = spdlog::basic_logger_mt("logger", "logs/log.txt");
    logger_o->flush_on(spdlog::level::info);   // make err
    spdlog::set_pattern("[thread %t] [%l] %v "); // add %T for time
  }
  logger = &logger_o;
  
  // start P4 link
  switch_list_ptr = &switch_list;
  switch_metadata_ptr = &switch_metadata;
  switch_metadata.hw_port_list.list = list;
  switch_metadata.hw_port_list.count = 8;
  bm_client_ptr = &bm_client;
  sai_id_map_ptr = &sai_id_map;
  transport->open();

  // api set
  switch_api.create_switch = &sai_adapter::create_switch;
  switch_api.get_switch_attribute = &sai_adapter::get_switch_attribute;

  port_api.create_port = &sai_adapter::create_port;
  port_api.remove_port = &sai_adapter::remove_port;
  port_api.set_port_attribute = &sai_adapter::set_port_attribute;
  port_api.get_port_attribute = &sai_adapter::get_port_attribute;

  bridge_api.create_bridge = &sai_adapter::create_bridge;
  bridge_api.remove_bridge = &sai_adapter::remove_bridge;
  bridge_api.get_bridge_attribute = &sai_adapter::get_bridge_attribute;
  bridge_api.create_bridge_port = &sai_adapter::create_bridge_port;
  bridge_api.remove_bridge_port = &sai_adapter::remove_bridge_port;
  bridge_api.get_bridge_port_attribute =
      &sai_adapter::get_bridge_port_attribute;

  fdb_api.create_fdb_entry = &sai_adapter::create_fdb_entry;
  fdb_api.remove_fdb_entry = &sai_adapter::remove_fdb_entry;

  vlan_api.create_vlan = &sai_adapter::create_vlan;
  vlan_api.remove_vlan = &sai_adapter::remove_vlan;
  vlan_api.set_vlan_attribute = &sai_adapter::set_vlan_attribute;
  vlan_api.get_vlan_attribute = &sai_adapter::get_vlan_attribute;
  vlan_api.create_vlan_member = &sai_adapter::create_vlan_member;
  vlan_api.remove_vlan_member = &sai_adapter::remove_vlan_member;
  vlan_api.set_vlan_member_attribute = &sai_adapter::set_vlan_member_attribute;
  vlan_api.get_vlan_member_attribute = &sai_adapter::get_vlan_member_attribute;
  vlan_api.get_vlan_stats = &sai_adapter::get_vlan_stats;
  vlan_api.clear_vlan_stats = &sai_adapter::clear_vlan_stats;

  lag_api.create_lag = &sai_adapter::create_lag;
  lag_api.remove_lag = &sai_adapter::remove_lag;
  lag_api.create_lag_member = &sai_adapter::create_lag_member;
  lag_api.remove_lag_member = &sai_adapter::remove_lag_member;

  hostif_api.create_hostif = &sai_adapter::create_hostif;
  hostif_api.remove_hostif = &sai_adapter::remove_hostif;
  hostif_api.create_hostif_table_entry = &sai_adapter::create_hostif_table_entry;
  hostif_api.remove_hostif_table_entry = &sai_adapter::remove_hostif_table_entry;
  hostif_api.create_hostif_trap_group = &sai_adapter::create_hostif_trap_group;
  hostif_api.remove_hostif_trap_group = &sai_adapter::remove_hostif_trap_group;
  hostif_api.create_hostif_trap = &sai_adapter::create_hostif_trap;
  hostif_api.remove_hostif_trap = &sai_adapter::remove_hostif_trap;

  startSaiAdapterMain();
  printf("startSaiAdapterMain\n");
  (*logger)->info("BM connection started on port {}", bm_port);
}

sai_adapter::~sai_adapter() {
  endSaiAdapterMain();
  transport->close();
  (*logger)->info("BM clients closed\n");
}

sai_status_t sai_adapter::sai_api_query(sai_api_t sai_api_id,
                                        void **api_method_table) {
  switch (sai_api_id) {
  case SAI_API_PORT:
    *api_method_table = &port_api;
    break;
  case SAI_API_BRIDGE:
    *api_method_table = &bridge_api;
    break;
  case SAI_API_FDB:
    *api_method_table = &fdb_api;
    break;
  case SAI_API_SWITCH:
    *api_method_table = &switch_api;
    break;
  case SAI_API_VLAN:
    *api_method_table = &vlan_api;
    break;
  case SAI_API_LAG:
    *api_method_table = &lag_api;
    break;
  case SAI_API_HOSTIF:
    *api_method_table = &hostif_api;
    break;
  default:
    (*logger)->info("api requested was %d, while sai_api_port is %d\n",
                    sai_api_id, SAI_API_PORT);
    return SAI_STATUS_FAILURE;
  }
  return SAI_STATUS_SUCCESS;
}

void sai_adapter::internal_init_switch() {
  sai_object_id_t switch_id2;
  (*logger)->info("Switch init with default configurations");
  switch_api.create_switch(&switch_id2, 0, NULL);
  (*logger)->info("Switch init with default configurations done");
  return;
}

void sai_adapter::startSaiAdapterMain() {
  internal_init_switch();
  pcap_loop_started = false;
  SaiAdapterThread = std::thread(&sai_adapter::SaiAdapterMain, this);
  {
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk,[]{return pcap_loop_started;});
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(500)); // TODO consider later release of lock
  (*logger)->info("Sniffer initialization done");
}

void sai_adapter::endSaiAdapterMain() {
  pcap_breakloop(adapter_pcap);
  pcap_close(adapter_pcap);
  SaiAdapterThread.join();
}

void sai_adapter::SaiAdapterMain() {
  (*logger)->info("SAI Adapter Thread Started");
  // Change to sai_adapter network namespace (hostif_net)
  int fd = open("/var/run/netns/hostif_net",
                O_RDONLY); /* Get descriptor for namespace */
  if (fd == -1) {
    (*logger)->error("open netns fd failed");
    release_pcap_lock();
    return;
  }
  if (setns(fd, 0) == -1) { /* Join that namespace */
    (*logger)->error("setns failed");
    release_pcap_lock();
    return;
  }

  PacketSniffer();
  (*logger)->info("SAI Adapter Thread Ended");
}

std::string parse_param(uint64_t param, uint32_t num_of_bytes) {
  std::string my_string = std::string(
      static_cast<char *>(static_cast<void *>(&param)), num_of_bytes);
  std::reverse(my_string.begin(), my_string.end());
  return my_string;
}

BmMatchParam parse_exact_match_param(uint64_t param, uint32_t num_of_bytes) {
  BmMatchParam match_param;
  match_param.type = BmMatchParamType::type::EXACT;
  BmMatchParamExact match_param_exact;
  match_param_exact.key = parse_param(param, num_of_bytes);
  match_param.__set_exact(match_param_exact);
  return match_param;
}

BmMatchParam parse_valid_match_param(bool param) {
  BmMatchParam match_param;
  match_param.type = BmMatchParamType::type::VALID;
  BmMatchParamValid match_param_valid;
  match_param_valid.key = param;
  match_param.__set_valid(match_param_valid);
  return match_param;
}

uint64_t parse_mac_64(uint8_t const mac_8[6]) {
  uint64_t mac_64 = 0;
  memcpy(&mac_64, mac_8, 6);
  return mac_64;
}