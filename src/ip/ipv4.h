#ifndef IPV4_HEADER_H
#define IPV4_HEADER_H

#include <stdint.h>
#include <arpa/inet.h>
#include <stdio.h>

typedef struct __attribute__((packed)) IPv4 {
    uint8_t     ver_ihl;
    uint8_t     dscp_ecn;
    uint16_t    len;
    uint16_t    id;
    uint16_t    flags_off;
    uint8_t     ttl;
    uint8_t     proto;
    uint16_t    chk;
    uint8_t     src[4];
    uint8_t     dst[4];
} IPv4_t;

// sanity checks
_Static_assert(sizeof(IPv4_t) == 20, "IPv4_t must be 20 bytes");

// IPv4 protocol numbers list
#define IPV4_ICMP           0x01
#define IPV4_IGMP           0x02
#define IPV4_TCP            0x06
#define IPV4_UDP            0x11
#define IPV4_IPV6_ROUTE     0x2B
#define IPV4_DSR            0x20
#define IPV4_SWIPE          0x35
#define IPV4_TLSP           0x38
#define IPV4_SKIP           0x39
#define IPV4_SAT_EXPAK      0x40
#define IPV4_EIGRP          0x58
#define IPV4_OSPF           0x59
#define IPV4_L2TP           0x73

#define IPV4_PROTOCOL_NAME_IPV4_ICMP        "ICMP(Internet Control Message Protocol)"
#define IPV4_PROTOCOL_NAME_IPV4_IGMP        "ICMP(Internet Group Management Protocol)"
#define IPV4_PROTOCOL_NAME_IPV4_TCP         "TCP(Transmission Control Protocol)"
#define IPV4_PROTOCOL_NAME_IPV4_UDP         "UDP(User Datagram Protocol)"
#define IPV4_PROTOCOL_NAME_IPV4_IPV6_ROUTE  "IPv6-Route(Routing Header for IPv6)"
#define IPV4_PROTOCOL_NAME_IPV4_DSR         "DSR(Dynamic Source Routing Protocol)"
#define IPV4_PROTOCOL_NAME_IPV4_SWIPE       "SwIPe(Swipe IP Security Protocol)"
#define IPV4_PROTOCOL_NAME_IPV4_TLSP        "TLSP(Transport Layer Security Protocol)"
#define IPV4_PROTOCOL_NAME_IPV4_SKIP        "SKIP(Simple Key Management for IP)"
#define IPV4_PROTOCOL_NAME_IPV4_SAT_EXPAK   "SAT-EXPAK(SATNET and Backroom EXPAK)"
#define IPV4_PROTOCOL_NAME_IPV4_EIGRP       "EIGRP(Enhanced Interior Gateway Routing Protocol)"
#define IPV4_PROTOCOL_NAME_IPV4_OSPF        "OSPF(Open Shorted Path First)"
#define IPV4_PROTOCOL_NAME_IPV4_L2TP        "L2TP(Layer 2 Tunneling Protocol version 3)"

#define IPV4_PROTOCOL_LIST  \
    X(IPV4_ICMP, IPV4_PROTOCOL_NAME_IPV4_ICMP) \
    X(IPV4_IGMP, IPV4_PROTOCOL_NAME_IPV4_IGMP) \
    X(IPV4_TCP, IPV4_PROTOCOL_NAME_IPV4_TCP) \
    X(IPV4_UDP, IPV4_PROTOCOL_NAME_IPV4_UDP) \
    X(IPV4_IPV6_ROUTE, IPV4_PROTOCOL_NAME_IPV4_IPV6_ROUTE) \
    X(IPV4_DSR, IPV4_PROTOCOL_NAME_IPV4_DSR) \
    X(IPV4_SWIPE, IPV4_PROTOCOL_NAME_IPV4_SWIPE) \
    X(IPV4_TLSP, IPV4_PROTOCOL_NAME_IPV4_TLSP) \
    X(IPV4_SKIP, IPV4_PROTOCOL_NAME_IPV4_SKIP) \
    X(IPV4_SAT_EXPAK, IPV4_PROTOCOL_NAME_IPV4_SAT_EXPAK) \
    X(IPV4_EIGRP, IPV4_PROTOCOL_NAME_IPV4_EIGRP) \
    X(IPV4_OSPF, IPV4_PROTOCOL_NAME_IPV4_OSPF) \


#define IPV4_PROTOCOL_NAME(code) IPV4_PROTOCOL_NAME_##code

char* ipv4_proto_name(uint16_t proto);
char* parse_ipv4_to_json(const IPv4_t *ip);

#endif
