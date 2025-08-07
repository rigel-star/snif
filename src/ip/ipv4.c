#include "ipv4.h"
#include "../vendor/cJSON/cJSON.h"

/**
 * Get the protocol's name from an IPv4 protocol number.
 */
char *ipv4_proto_name(uint16_t proto) {
    switch (proto) {
#define X(code, name) case code: return name;
        IPV4_PROTOCOL_LIST
#undef X
        default: return "UNK(Unknown)";
    }
}

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

    char *json_string = cJSON_PrintUnformatted(ip_json);
    cJSON_Delete(ip_json);

    return json_string; 
}
