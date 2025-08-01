#include "eth_parser.h"
#include "arp.h"
#include "ipv4.h"

char* parse_ipv4_to_json(const IPv4_t *ip) {
    char *buffer = malloc(512);
    if (!buffer) return NULL;

    uint8_t version = ip->ver_ihl >> 4;
    uint8_t ihl = ip->ver_ihl & 0x0F;

    uint8_t dscp = ip->dscp_ecn >> 2;
    uint8_t ecn = ip->dscp_ecn & 0x03;

    uint16_t total_len = ntohs(ip->len);
    uint16_t id = ntohs(ip->id);
    uint16_t flags = ntohs(ip->flags_off) >> 13;
    uint16_t offset = ntohs(ip->flags_off) & 0x1FFF;

    snprintf(
        buffer,
        512,
        "{"
            "\"version\": %" PRIu8 ", "
            "\"ihl\": %" PRIu8 ", "
            "\"dscp\": %" PRIu8 ", "
            "\"ecn\": %" PRIu8 ", "
            "\"total_length\": %" PRIu16 ", "
            "\"id\": %" PRIu16 ", "
            "\"flags\": %" PRIu16 ", "
            "\"fragment_offset\": %" PRIu16 ", "
            "\"ttl\": %" PRIu8 ", "
            "\"protocol\": %" PRIu8 ", "
            "\"checksum\": %" PRIu16 ", "
            "\"src\": \"%u.%u.%u.%u\", "
            "\"dst\": \"%u.%u.%u.%u\""
        "}",
        version,
        ihl,
        dscp,
        ecn,
        total_len,
        id,
        flags,
        offset,
        ip->ttl,
        ip->proto,
        ntohs(ip->chk),
        ip->src[0], ip->src[1], ip->src[2], ip->src[3],
        ip->dst[0], ip->dst[1], ip->dst[2], ip->dst[3]
    );

    return buffer;
}

char* parse_ether_frame_to_json(EtherHeader_t *eth_header, const u_char *rest) {    
    if (eth_header->type == IPV4) {
        const IPv4_t *ip = (const IPv4_t*) (rest);
        return parse_ipv4_to_json(ip);
    }
    return "Ethernet Sth Else";
}

void parse_pkt_to_json(const u_char *packet, int len) {
    if (len < 14) {
        printf("Packet too short for Ethernet header.\n");
        exit(EXIT_FAILURE);
    }

    EtherHeader_t hdr;
    memcpy(hdr.dst_mac, packet, 6);
    memcpy(hdr.src_mac, packet + 6, 6);

    hdr.type = (packet[12] << 8) | packet[13];

    if (hdr.type == 0x8100) {
        puts("802.1Q VLAN Tagged");
    }
    else if (hdr.type <= 1500) {
        puts("IEEE 802.3 Ethernet Frame");
    }
    else {
        if (hdr.type == 0x0800) {
            char *json = parse_ether_frame_to_json(&hdr, (packet + 14));
            printf("%s\n", json);
            free(json);
        }
    }
}