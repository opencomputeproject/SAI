#include <sai.h>
#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>

#define ETHER_ADDR_LEN  6
#define CPU_HDR_LEN  6
#define MAC_LEARN_TRAP_ID 512

void server_internal_init_switch() {
    printf("Switch init with default configurations\n");
    sai_status_t status = SAI_STATUS_SUCCESS;
    sai_switch_api_t *switch_api;
    status = sai_api_query(SAI_API_SWITCH, (void **) &switch_api);
    if (status != SAI_STATUS_SUCCESS) {
        printf("sai_api_query failed!!!\n");
    }
    uint32_t count =0;
    sai_object_id_t s_id =1;
    status = switch_api->create_switch(&s_id,count,NULL);
    printf("Switch inititated\n");
    return;
  }

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

// typedef void(*sai_packet_event_notification_fn)(
//         _In_ sai_object_id_t switch_id,
//         _In_ const void *buffer,
//         _In_ sai_size_t buffer_size,
//         _In_ uint32_t attr_count,
//         _In_ const sai_attribute_t *attr_list);
void* fdb_miss_event_notification(sai_object_id_t switch_id,
	            const void *buffer, sai_size_t buffer_size,
	            uint32_t attr_count, const sai_attribute_t *attr_list) {
	printf("FDB MISS.\n");

	// Learn new mac.
}

typedef struct _ethernet_hdr_t {
    uint8_t dst_addr[ETHER_ADDR_LEN]; 
    uint8_t src_addr[ETHER_ADDR_LEN];
    uint16_t ether_type; 
} ethernet_hdr_t;

typedef struct _cpu_hdr_t {
    unsigned int ingress_port : 8;
    unsigned int bridge_port : 8;
    unsigned int bridge_id : 16;
    unsigned int trap_id : 16;
} cpu_hdr_t;

void ReverseBytes( uint8_t *byte_arr, int size )
{
    uint8_t tmp;
    for(int lo=0, hi=size-1; hi>lo; lo++, hi--)
    {
        tmp = byte_arr[lo];
        byte_arr[lo] = byte_arr[hi];
        byte_arr[hi] = tmp;
    }

}

void print_mac(const uint8_t* mac) {
  int i;
  for (i=ETHER_ADDR_LEN-1;i>0;i--) {
    printf("%.2x:", mac[i]);
  }
  printf("%.2x\n",mac[0]);
}

void packetHandler(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
  // uint64_t* num;
  cpu_hdr_t* cpu_hdr = (cpu_hdr_t*) packet;
  ReverseBytes((uint8_t*) cpu_hdr, CPU_HDR_LEN);
  ethernet_hdr_t* ether = (ethernet_hdr_t*) (packet + CPU_HDR_LEN);
  ReverseBytes((uint8_t*) &(ether->ether_type), 2);
  ReverseBytes(ether->dst_addr, 6);
  ReverseBytes(ether->src_addr, 6);
  
  printf("packet captured:\n");
  printf("trap_id: %d. bridge_id: %d. ingress_port: %d. bridge_port: %d.\n",cpu_hdr->trap_id, cpu_hdr->bridge_id, cpu_hdr->ingress_port, cpu_hdr->bridge_port);
  printf("source MAC:\n");
  print_mac(ether->src_addr);
  printf("dest MAC:\n");
  print_mac(ether->dst_addr);
  printf("ether_type = 0x%.4x\n",ether->ether_type);
  sai_object_id_t bridge_port = temp_sai_get_bridge_port(cpu_hdr->bridge_port);
  printf("bridge_port_sai_obj_id = %d\n", bridge_port);

  sai_bridge_api_t *bridge_api;
  sai_status_t status = SAI_STATUS_SUCCESS;
  status = sai_api_query(SAI_API_BRIDGE, (void **) &bridge_api);
  if (status != SAI_STATUS_SUCCESS) {
      printf("sai_api_query failed!!!\n");
      return SAI_STATUS_NOT_IMPLEMENTED; 
  }

  sai_attribute_t attr[3];
  attr[0].id = SAI_BRIDGE_PORT_ATTR_TYPE;
  attr[1].id = SAI_BRIDGE_PORT_ATTR_VLAN_ID;
  attr[2].id = SAI_BRIDGE_PORT_ATTR_BRIDGE_ID;
  bridge_api->get_bridge_port_attribute(bridge_port, 3, attr);
  sai_fdb_entry_bridge_type_t bridge_type;
  switch (attr[0].value.s32) {
    case SAI_BRIDGE_PORT_TYPE_PORT:
      bridge_type = SAI_FDB_ENTRY_BRIDGE_TYPE_1Q;
      break;
    case SAI_BRIDGE_PORT_TYPE_SUB_PORT:
      bridge_type = SAI_FDB_ENTRY_BRIDGE_TYPE_1D;
      break;
    default:
      printf("packet arrived from non port bridge_port (not supported yet)\n");
      break;
  }
  create_fdb_entry(bridge_port, ether->src_addr, bridge_type, attr[1].value.u16, attr[2].value.oid);
}

void create_fdb_entry(sai_object_id_t bridge_port_id, sai_mac_t mac, sai_fdb_entry_bridge_type_t bridge_type, sai_vlan_id_t vlan_id, sai_object_id_t bridge_id) {
    sai_fdb_api_t *fdb_api;
    sai_status_t status = SAI_STATUS_SUCCESS;
    status = sai_api_query(SAI_API_FDB, (void **) &fdb_api);
    if (status != SAI_STATUS_SUCCESS) {
        printf("sai_api_query failed!!!\n");
        return SAI_STATUS_NOT_IMPLEMENTED; 
    }
    sai_attribute_t attr[3];
    attr[0].id = SAI_FDB_ENTRY_ATTR_TYPE;
    attr[0].value.s32 = SAI_FDB_ENTRY_TYPE_STATIC;

    attr[1].id = SAI_FDB_ENTRY_ATTR_BRIDGE_PORT_ID;
    attr[1].value.oid = bridge_port_id;

    attr[2].id = SAI_FDB_ENTRY_ATTR_PACKET_ACTION;
    attr[2].value.s32 = SAI_PACKET_ACTION_FORWARD;

    sai_fdb_entry_t sai_fdb_entry;
    sai_fdb_entry.switch_id = 0;
    for (int i=0; i<ETHER_ADDR_LEN;i++) { 
      sai_fdb_entry.mac_address[i] = mac[i];
    }
    sai_fdb_entry.bridge_type = bridge_type;
    if (bridge_type == SAI_FDB_ENTRY_BRIDGE_TYPE_1Q) {
      sai_fdb_entry.vlan_id =  (sai_vlan_id_t) vlan_id;
    }
    sai_fdb_entry.bridge_id = bridge_id;
    fdb_api->create_fdb_entry(&sai_fdb_entry,3,attr);
    printf("fdb learned\n");
}

int main(int argc, char **argv)
{
    printf("Starting learning app.\n");
	sai_object_id_t switch_id = 1;
	sai_api_initialize(0, &test_services);
  server_internal_init_switch();
    const char *dev = "cpu_port";
    pcap_t *descr;
    char errbuf[PCAP_ERRBUF_SIZE];
    printf("pcap started on dev %s\n", dev);
    descr = pcap_open_live(dev, BUFSIZ, 0, -1, errbuf);
    if (descr == NULL) {
      printf("pcap_open_live() failed: %s\n",errbuf);
      return 1;
    }

    if (pcap_loop(descr, 10, packetHandler, NULL) < 0) {
      printf("pcap_loop() failed: %s\n",pcap_geterr(descr));
      return 1;
    }

    // sai_bridge_api_t *bridge_api;
    // sai_status_t status = SAI_STATUS_SUCCESS;
    // status = sai_api_query(SAI_API_BRIDGE, (void **) &bridge_api);
    // if (status != SAI_STATUS_SUCCESS) {
    //     printf("sai_api_query failed!!!\n");
    //     return SAI_STATUS_NOT_IMPLEMENTED; 
    // }

    sai_api_uninitialize();
}

   // sai_port_api_t* port_api;
 //    // sai_api_query(SAI_API_PORT, (void**)&port_api);

 //    // Set packet callback function
 //    sai_switch_api_t* switch_api;
 //    sai_api_query(SAI_API_SWITCH, (void**)&switch_api);
 //    sai_attribute_t sai_attr;
 //    sai_attr.id = SAI_SWITCH_ATTR_PACKET_EVENT_NOTIFY;
 //    sai_attr.value.ptr = fdb_miss_event_notification;
 //    switch_api->set_switch_attribute(switch_id, &sai_attr);

 //    sai_hostif_api_t* hostif_api;
  // sai_api_query(SAI_API_HOSTIF, (void**)&hostif_api); 

 //    // create trap group (currently only 1.)
 //    sai_object_id_t prio_group;
  // sai_attribute_t sai_attr_list[2];
  // sai_attr_list[1].id=SAI_HOSTIF_TRAP_GROUP_ATTR_QUEUE;
  // sai_attr_list[1].value.u32 = 0; // high_queue_id; // high_queue_id is a queue element created via QoS SAI API 
  // sai_attr_list[2].id= SAI_HOSTIF_TRAP_GROUP_ATTR_POLICER;
  // sai_attr_list[2].value.oid = 0; // high_policer_id; //high_policer_id is a policer element created via policer SAI API
  // hostif_api->create_hostif_trap_group(&prio_group, switch_id, 2, sai_attr_list);


  // // Configuring Trap-IDs
 //    sai_attribute_t sai_trap_attr[3];
 //    sai_object_id_t host_trap_id[1];
  // // configure STP trap_id
 //    // sai_trap_attr[0].id=SAI_HOSTIF_TRAP_ATTR_TRAP_GROUP;
 //    // sai_trap_attr[0].value=&high_prio_group;
 //    // sai_trap_attr[1].id= SAI_HOSTIF_TRAP_ATTR_TRAP_ACTION;
 //    // sai_trap_attr[1].value= SAI_PACKET_ACTION_TRAP;
 //    // sai_trap_attr[2].id= SAI_HOSTIF_TRAP_ATTR_TRAP_TYPE;
 //    // sai_trap_attr[2].value= SAI_HOSTIF_TRAP_TYPE_STP;
 //    // hostif_api->create_hostif_trap(&host_trap_id[1],2, sai_trap_attr);
 //    // configure FDB miss trap-id 
 //    sai_trap_attr[0].id = SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_TRAP_GROUP;
 //    sai_trap_attr[0].value.oid = prio_group;
 //    sai_trap_attr[1].id = SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_TYPE;
 //    sai_trap_attr[1].value.s32 = SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_FDB;
 //    sai_trap_attr[2].id = SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_TRAP_PRIORITY;
 //    sai_trap_attr[2].value.u32 = 0;
 //    hostif_api->create_hostif_user_defined_trap(&host_trap_id[0], switch_id, 3, sai_trap_attr);


 //    // Configuring Host tables
 //    sai_object_id_t host_table_entry[1];
  // sai_attribute_t sai_if_channel_attr[3];
  // // sai_if_channel_attr[0].id=SAI_HOSTIF_TABLE_ENTRY_ATTR_TYPE;
  // // sai_if_channel_attr[0].value= SAI_HOST_INTERFACE_TABLE_ENTRY_TYPE_TRAP_ID;
  // // sai_if_channel_attr[1].id= SAI_HOSTIF_TABLE_ENTRY_ATTR_TRAP_ID;
  // // sai_if_channel_attr[1].value=host_trap_id[1]; // Object referencing STP trap
  // // sai_if_channel_attr[2].id= SAI_HOSTIF_TABLE_ENTRY_ATTR_CHANNEL_TYPE;
  // // sai_if_channel_attr[2].value=SAI_HOST_INTERFACE_TABLE_ENTRY_CHANNEL_TYPE_CB;
  // // hostif_api->create_hostif_table_entry(&host_table_entry[0], 3, sai_if_channel_attr); 
  // sai_if_channel_attr[0].id=SAI_HOSTIF_TABLE_ENTRY_ATTR_TYPE;
  // sai_if_channel_attr[0].value.s32 = SAI_HOSTIF_TABLE_ENTRY_TYPE_TRAP_ID;
  // sai_if_channel_attr[1].id = SAI_HOSTIF_TABLE_ENTRY_ATTR_TRAP_ID;
  // sai_if_channel_attr[1].value.oid = host_trap_id[0]; // Object referencing FDB trap
  // sai_if_channel_attr[2].id = SAI_HOSTIF_TABLE_ENTRY_ATTR_CHANNEL_TYPE;
  // sai_if_channel_attr[2].value.s32 = SAI_HOSTIF_TABLE_ENTRY_CHANNEL_TYPE_CB;
  // hostif_api->create_hostif_table_entry(&host_table_entry[0], switch_id, 3, sai_if_channel_attr); 