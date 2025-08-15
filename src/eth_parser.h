#ifndef ETH_PARSER_H
#define ETH_PARSER_H

#include <pcap.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "arp/arp.h"
#include "vendor/cJSON/cJSON.h"
#include "ip/ipv4.h"

#define ETHER_TYPE_IPV4 0x0800
#define ETHER_TYPE_ARP 0x0806
#define ETHER_TYPE_IPV6 0x86DD

#define MAC_ADDR_LEN 12

typedef struct EtherHeader {
    u_char  src_mac[MAC_ADDR_LEN];
    u_char  dst_mac[MAC_ADDR_LEN];
    u_short type;
    int32_t len;
} EtherHeader_t;

char* parse_ether_frame_to_json(EtherHeader_t *eth_header, const u_char *rest);
char* parse_pkt_to_json(const u_char *packet, int len);

#endif
