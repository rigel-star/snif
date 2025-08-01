#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>

#include "ether.h"
#include "eth_parser.h"

#define SNIF

FILE *pkt_writer_fp;

SNIF void snif_packet_handler(u_char *args, const struct pcap_pkthdr *pkt_header, const u_char *packet) {
    parse_pkt_to_json(packet, pkt_header->caplen);
}

/**
 * dev: Device to read from
 */
SNIF int snif_lookup_dev(char *dev) {
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
    return 0;
}

int main() {
    snif_lookup_dev("en0");
}