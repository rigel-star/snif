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

char* parse_tcp_to_json(const u_char* data) {
    const TCP_t *tcp = ((const TCP_t*) data);

    cJSON* tcp_json = cJSON_CreateObject();

    uint16_t src_port = ntohs(tcp->sport);
    uint16_t dst_port = ntohs(tcp->dport);

    uint32_t seq_num = ntohs(tcp->seq);
    uint32_t ack_num = ntohs(tcp->ack);

    uint8_t off = tcp->off_res >> 4;
    uint8_t res = tcp->off_res & 0xF;

    uint8_t flags = tcp->flags;
    uint16_t window = ntohs(tcp->win);

    uint16_t chk = ntohs(tcp->chk);
    uint16_t urg = ntohs(tcp->urg);

    size_t tcp_head_len = off * 4;
    size_t tcp_opts_len = tcp_head_len - sizeof(TCP_t);
    if (tcp_opts_len > 0) {
        // there are options present for this TCP header
    }

    cJSON_AddNumberToObject(tcp_json, "Source Port", src_port);
    cJSON_AddNumberToObject(tcp_json, "Destination Port", dst_port);
    cJSON_AddNumberToObject(tcp_json, "Sequence Number", seq_num);
    cJSON_AddNumberToObject(tcp_json, "Acknowledgement Number", ack_num);
    cJSON_AddNumberToObject(tcp_json, "Acknowledgement Number", ack_num);
    cJSON_AddNumberToObject(tcp_json, "Offset", off);
    cJSON_AddNumberToObject(tcp_json, "Reserved", res);
    cJSON_AddNumberToObject(tcp_json, "Flags", flags);
    cJSON_AddNumberToObject(tcp_json, "Window", window);
    cJSON_AddNumberToObject(tcp_json, "Checksum", chk);
    cJSON_AddNumberToObject(tcp_json, "Urgent Pointer", urg);

    char* json_string = cJSON_PrintUnformatted(tcp_json);
    cJSON_Delete(tcp_json);

    return json_string;
}

/**
 * NOTE: Returned string must be freed by the caller.
 */
char* parse_ipv4_to_json(const u_char* data) {
    const IPv4_t* ip = ((const IPv4_t*) data);

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

    if (ip->proto == IPV4_TCP) {
        size_t ip_head_len = ihl * 4;
        const u_char* tcp_data = data + ip_head_len;
        char* tcp_parsed = parse_tcp_to_json(tcp_data);
        if (tcp_parsed) {
            cJSON *tcp_obj = cJSON_Parse(tcp_parsed);
            if (tcp_obj) {
                cJSON_AddItemToObject(ip_json, "Payload", tcp_obj);
            }
            free(tcp_parsed);
        }
    }

    char *json_string = cJSON_PrintUnformatted(ip_json);
    cJSON_Delete(ip_json);

    return json_string; 
}
