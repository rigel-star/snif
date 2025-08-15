#ifndef ARP_PROTO_H
#define ARP_PROTO_H

#include <stdint.h>
#include <stdio.h>
#include <arpa/inet.h>
#include "../vendor/cJSON/cJSON.h"

typedef struct __attribute__((__packed__)) ARP {
    uint16_t    htype;  // Hardware type (for e.g.: 1 for Ethernet)
    uint16_t    ptype;  // Protocol type (for: e.g.: 0x0800 for IPv4)
    uint8_t     hlen;   // Hardware address length (bytes) — 6 for MAC
    uint8_t     plen;   // Protocol address length (bytes) — 4 for IPv4
    uint16_t    op;     // Operation code: 1=Request, 2=Reply
    uint8_t     sha[6]; // Sender hardware address (MAC)
    uint8_t     spa[4]; // Sender protocol address (IPv4)
    uint8_t     tha[6]; // Target hardware address (MAC)
    uint8_t     tpa[4]; // Target protocol address (IPv4)
} ARP_t;

char* parse_arp_to_json(const ARP_t *arp);

#endif
