#include "eth_parser.h"
#include "arp.h"

EtherHeader_t parse_ether_pkt(const u_char *packet, int len) {
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
        if (hdr.type == 0x0806) {
            const ARP_t *arp = (const ARP_t*) (packet + 14);
            printf(
                "%02x:%02x:%02x:%02x:%02x:%02x\n", 
                arp->sha[0], arp->sha[1], arp->sha[2],
                arp->sha[3], arp->sha[4], arp->sha[5]
            );
        }
        printf("%04x\n", hdr.type);
    }

    return hdr;
}