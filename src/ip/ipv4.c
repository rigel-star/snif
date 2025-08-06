#include "ipv4.h"

char *ipv4_proto_name(uint16_t proto) {
    switch (proto) {
        case IPV4_ICMP: return "ICMP(Internet Control Message Protocol)";
        case IPV4_TCP: return "TCP(Transmission Control Protocol)";
        case IPV4_UDP: return "UDP(User Datagram Protocol)";
        case IPV4_OSPF: return "OSPF(Open Shortest Path First)";
        default: return "UNK(Unknown)";
    }
}
