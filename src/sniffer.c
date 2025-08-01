#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eth_parser.h"

#define SNIF

SNIF pcap_t* snif_open(char *dev) {
    char errbuff[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuff);
    if (!handle) {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuff);
        return NULL;
    }
    return handle;
}

SNIF void snif_close(pcap_t *handle) {
    if (handle) pcap_close(handle);
}

SNIF int snif_next(pcap_t *handle, char *out_buf) {
    struct pcap_pkthdr *header;
    const u_char *packet;
    int res = pcap_next_ex(handle, &header, &packet);

    if (res == 1) {
        char *json = parse_pkt_to_json(packet, header->caplen);
        snprintf(out_buf, strlen(json), "%s", json);
        free(json);
        return 0;
    }
    else if (res == 0) {
        return -1;
    }
    else {
        return -2;
    }
}
