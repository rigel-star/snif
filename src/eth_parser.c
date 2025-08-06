#include "eth_parser.h"

/**
 * NOTE: Returned string must be freed by the caller.
 */
char* parse_ipv4_to_json(const IPv4_t *ip) {
    cJSON *ip_json = cJSON_CreateObject();

    uint8_t version = ip->ver_ihl >> 4;
    uint8_t ihl = ip->ver_ihl & 0x0F;

    uint8_t dscp = ip->dscp_ecn >> 2;
    uint8_t ecn = ip->dscp_ecn & 0x03;

    uint16_t total_len = ntohs(ip->len);
    uint16_t id = ntohs(ip->id);
    uint16_t flags = ntohs(ip->flags_off) >> 13;
    uint16_t offset = ntohs(ip->flags_off) & 0x1FFF;

    cJSON_AddNumberToObject(ip_json, "Version", version);
    cJSON_AddNumberToObject(ip_json, "Internet Header Length", ihl);
    cJSON_AddNumberToObject(ip_json, "Differentiated Services Code Point", dscp);
    cJSON_AddNumberToObject(ip_json, "Explicit Congestion Notification", ecn);
    cJSON_AddNumberToObject(ip_json, "Total Length", total_len);
    cJSON_AddNumberToObject(ip_json, "Identification", id);
    cJSON_AddNumberToObject(ip_json, "Flags", flags);
    cJSON_AddNumberToObject(ip_json, "Offset", offset);
    cJSON_AddNumberToObject(ip_json, "Time to Live", ip->ttl);
    cJSON_AddNumberToObject(ip_json, "Protocol", ip->proto);
    cJSON_AddNumberToObject(ip_json, "Checksum", ntohs(ip->chk));

    char src_ip[16], dst_ip[16];
    snprintf(src_ip, sizeof(src_ip), "%u.%u.%u.%u", ip->src[0], ip->src[1], ip->src[2], ip->src[3]);
    snprintf(dst_ip, sizeof(dst_ip), "%u.%u.%u.%u", ip->dst[0], ip->dst[1], ip->dst[2], ip->dst[3]);

    cJSON_AddStringToObject(ip_json, "Source", src_ip);
    cJSON_AddStringToObject(ip_json, "Destination", dst_ip);
    cJSON_AddStringToObject(ip_json, "Protocol Name", ipv4_proto_name(ip->proto));

    puts(ipv4_proto_name(ip->proto));

    char *json_string = cJSON_PrintUnformatted(ip_json);
    cJSON_Delete(ip_json);

    return json_string; 
}

char* parse_ether_frame_to_json(EtherHeader_t *eth_header, const u_char *rest) {
    if (eth_header->type != ETHER_IPV4) {
        return NULL;
    }

    char* rest_parsed = parse_ipv4_to_json((const IPv4_t *)rest);
    if (!rest_parsed) {
        return NULL;
    }

    cJSON *eth_json = cJSON_CreateObject();

    if (eth_header->type == ETHER_IPV4) {
        cJSON_AddNumberToObject(eth_json, "Payload Type", ETHER_IPV4);    
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

    if (hdr.type == 0x8100) {
        puts("802.1Q VLAN Tagged");
    }
    else if (hdr.type <= 1500) {
        puts("IEEE 802.3 Ethernet Frame");
    }
    else {
        if (hdr.type == 0x0800) {
            char *json = parse_ether_frame_to_json(&hdr, (packet + 14));
            return json;
        }
    }
    return NULL;
}
