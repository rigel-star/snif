#pragma once

#include <pcap.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "ether.h"
#include "arp.h"
#include "arp.h"
#include "vendor/cJSON/cJSON.h"
#include "ip/ipv4.h"

#define ETHER_IPV4 0x0800
#define ETHER_ARP 0x0806

char* parse_ipv4_to_json(const IPv4_t *ip);
char* parse_ether_frame_to_json(EtherHeader_t *eth_header, const u_char *rest);
char* parse_pkt_to_json(const u_char *packet, int len);
