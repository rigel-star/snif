#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>

#include "ether.h"

#define MAC_ADDR_LEN 12

#define SNIF

struct EtherHeader {
    u_char src_mac[MAC_ADDR_LEN];
    u_char dst_mac[MAC_ADDR_LEN];
    u_short type;
};

struct EtherHeader to_snif_ether_header(const u_char *packet) {
    struct ether_header *eth_header = (struct ether_header *) packet;
    struct EtherHeader ret;
    memcpy(ret.src_mac, eth_header->ether_shost, 12);
    memcpy(ret.dst_mac, eth_header->ether_dhost, 12);
    ret.type = eth_header->ether_type;
    return ret;
}

SNIF char *ether_type_to_str(u_short type) {
    switch (type) {
        case SNIF_ETHER_TYPE_ARP: return "ARP";
        case SNIF_ETHER_TYPE_IPV4: return "IPV5";
        case SNIF_ETHER_TYPE_IPV6: return "IPV6";
        default: {
            printf("%02x\n", type);
            return "UNK";
        }
    }
}

void dump_snif_ether_header(struct EtherHeader *eth_header) {
    printf(
        "Src: %02x:%02x:%02x:%02x:%02x:%02x\n", 
        eth_header->src_mac[0], eth_header->src_mac[1], eth_header->src_mac[2],
        eth_header->src_mac[3], eth_header->src_mac[4], eth_header->src_mac[5]
    );
    printf(
        "Dst: %02x:%02x:%02x:%02x:%02x:%02x\n", 
        eth_header->dst_mac[0], eth_header->dst_mac[1], eth_header->dst_mac[2],
        eth_header->dst_mac[3], eth_header->dst_mac[4], eth_header->dst_mac[5]
    );
    printf("Typ: %s\n", ether_type_to_str(eth_header->type));
}

// write to pipe
FILE *pkt_writer_fp;

void write_into_pipe(struct EtherHeader *eth_header) {
    char buffer[256];
    sprintf(
        buffer, 
        sizeof(buffer),
        "Src: %02x:%02x:%02x:%02x:%02x:%02x, Dst: %02x:%02x:%02x:%02x:%02x:%02x, Typ: %s",
        eth_header->src_mac[0], eth_header->src_mac[1], eth_header->src_mac[2],
        eth_header->src_mac[3], eth_header->src_mac[4], eth_header->src_mac[5],
        eth_header->dst_mac[0], eth_header->dst_mac[1], eth_header->dst_mac[2],
        eth_header->dst_mac[3], eth_header->dst_mac[4], eth_header->dst_mac[5],
        ether_type_to_str(eth_header->type)
    );
    fprintf(pkt_writer_fp, "%s", buffer);
    fflush(pkt_writer_fp);
}

void snif_packet_handler(u_char *args, const struct pcap_pkthdr *pkt_header, const u_char *packet) {
    struct EtherHeader header = to_snif_ether_header(packet);
    fprintf(pkt_writer_fp, "%s\n", header.src_mac);
}

/**
 * dev: Device to read from
 */
void snif_lookup_dev(char *dev) {
    char errbuff[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuff);
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuff);
        return 2;
    }

    pkt_writer_fp = fopen("/tmp/packets.pipe", "w");
    if (!pkt_writer_fp) {
        perror("Failed to open named pipe");
        return 1;
    }

    pcap_loop(handle, 5, snif_packet_handler, NULL);
    pcap_close(handle);
    fclose(pkt_writer_fp);
}