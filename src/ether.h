#pragma once

#define SNIF_ETHER_TYPE_IPV4 0x0800
#define SNIF_ETHER_TYPE_ARP 0x0806
#define SNIF_ETHER_TYPE_IPV6 0x86DD

#define MAC_ADDR_LEN 12

typedef struct EtherHeader {
    u_char src_mac[MAC_ADDR_LEN];
    u_char dst_mac[MAC_ADDR_LEN];
    u_short type;
} EtherHeader_t;
