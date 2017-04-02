#include <iostream>
#include <pcap.h>
#include <stdint.h>
#include <iomanip>
#include <algorithm>

#define ETHER_ADDR_LEN  6
#define CPU_HDR_LEN  6
using namespace std;

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

void ReverseBytes( uint8_t *start, int size )
{
    uint8_t *istart = start;
    uint8_t *iend = istart + size;
    std::reverse(istart, iend);
}

void print_mac(const uint8_t* mac) {
  int i;
  for (i=0;i<ETHER_ADDR_LEN-1;i++) {
    cout << setfill('0') << setw(2) << hex << static_cast<int>(mac[i]) << ":";
  }
  cout << setfill('0') << setw(2) << static_cast<int>(mac[ETHER_ADDR_LEN-1]) << endl;
}

void packetHandler(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
  // uint64_t* num;
  cpu_hdr_t* cpu_hdr = (cpu_hdr_t*) packet;
  cout << "cpu header debug. RAW print: 0x" << setfill('0') << setw(2) << hex << *((uint64_t*) cpu_hdr) << endl;
  ReverseBytes((uint8_t*) cpu_hdr, CPU_HDR_LEN);
  cout << "cpu header debug (reversed). RAW print: 0x" << setfill('0') << setw(2) << hex << *((uint64_t*) cpu_hdr) << endl;
  ethernet_hdr_t* ether = (ethernet_hdr_t*) (packet + CPU_HDR_LEN);
  ReverseBytes((uint8_t*) &(ether->ether_type), 2);
  
  cout << "packet captured:" << endl;
  cout << "trap_id: " << dec << cpu_hdr->trap_id << ". bridge_id: " << cpu_hdr->bridge_id << ". ingress_port: " << cpu_hdr->ingress_port
       << ". bridge_port:" << cpu_hdr->bridge_port << endl;
  cout << "source MAC: ";
  print_mac(ether->src_addr);
  cout << "dest MAC: ";
  print_mac(ether->dst_addr);
  cout << "ether_type = 0x" << setfill('0') << setw(4) << hex << ether->ether_type << endl;
}

int main() {
  const char *dev = "host_port";
  pcap_t *descr;
  char errbuf[PCAP_ERRBUF_SIZE];

  cout << "pcap started on dev " << dev << endl;

  descr = pcap_open_live(dev, BUFSIZ, 0, -1, errbuf);
  if (descr == NULL) {
      cout << "pcap_open_live() failed: " << errbuf << endl;
      return 1;
  }

  if (pcap_loop(descr, 10, packetHandler, NULL) < 0) {
      cout << "pcap_loop() failed: " << pcap_geterr(descr);
      return 1;
  }

  cout << "capture finished" << endl;

  return 0;
}