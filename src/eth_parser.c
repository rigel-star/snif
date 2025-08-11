#include "eth_parser.h"

char* parse_ether_frame_to_json(EtherHeader_t *eth_header, const u_char *rest) {
    char* rest_parsed = NULL;
    
    if (eth_header->type == ETHER_TYPE_IPV4) {
        rest_parsed = parse_ipv4_to_json((const IPv4_t *) rest);
    }
    else if (eth_header->type == ETHER_TYPE_ARP) {
        rest_parsed = parse_arp_to_json((const ARP_t*) rest);
    }
    else {
        fprintf(stderr, "Unsupported Ethertype: 0x%04x\n", eth_header->type);
        return NULL;
    }

    if (!rest_parsed) {
        return NULL;
    }

    cJSON *eth_json = cJSON_CreateObject();

    if (eth_header->type == ETHER_TYPE_IPV4) {
        cJSON_AddNumberToObject(eth_json, "Payload Type", ETHER_TYPE_IPV4);    
    }
    else if (eth_header->type == ETHER_TYPE_ARP) {
        cJSON_AddNumberToObject(eth_json, "Payload Type", ETHER_TYPE_ARP);    
    }

    char src_mac[18], dst_mac[18];
    snprintf(
        src_mac, 
        sizeof(src_mac), 
        "%02x:%02x:%02x:%02x:%02x:%02x",
        eth_header->src_mac[0], eth_header->src_mac[1], eth_header->src_mac[2],
        eth_header->src_mac[3], eth_header->src_mac[4], eth_header->src_mac[5]
    );

    snprintf(
        dst_mac, 
        sizeof(dst_mac), 
        "%02x:%02x:%02x:%02x:%02x:%02x",
        eth_header->dst_mac[0], eth_header->dst_mac[1], eth_header->dst_mac[2],
        eth_header->dst_mac[3], eth_header->dst_mac[4], eth_header->dst_mac[5]
    );

    cJSON_AddStringToObject(eth_json, "Source", src_mac);
    cJSON_AddStringToObject(eth_json, "Destination", dst_mac);

    cJSON *payload_json = cJSON_Parse(rest_parsed);
    free(rest_parsed);

    if (!payload_json) {
        cJSON_Delete(eth_json);
        return NULL;
    }

    cJSON_AddItemToObject(eth_json, "Payload", payload_json);
    cJSON_AddNumberToObject(eth_json, "Length", eth_header->len);

    char *out_json = cJSON_PrintUnformatted(eth_json);
    cJSON_Delete(eth_json);

    return out_json; 
}

char* parse_pkt_to_json(const u_char *packet, int len) {
    if (len < 14) {
        printf("Packet too short for Ethernet header.\n");
        exit(EXIT_FAILURE);
    }

    EtherHeader_t hdr;
    memcpy(hdr.dst_mac, packet, 6);
    memcpy(hdr.src_mac, packet + 6, 6);
    hdr.type = (packet[12] << 8) | packet[13];
    hdr.len = len;

    int payload_off = 14;

    while (hdr.type == 0x8100) {
        if (len < payload_off + 4) {
            fprintf(stderr, "Packet too short for VLAN tag.\n");
            return NULL;
        }

        hdr.type = (packet[payload_off + 2] << 8) | packet[payload_off + 3];
        payload_off += 4;
    }
    
    if (hdr.type <= 1500) {
        fprintf(stderr, "Skipping IEEE 802.3 frame (unsupported).\n");
        return NULL;
    }

    char *json = parse_ether_frame_to_json(&hdr, (packet + payload_off));
    return json;
}
