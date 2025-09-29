#include "arp.h"

char* parse_arp_to_json(const ARP_t *arp) {
    cJSON *arp_json = cJSON_CreateObject();

    cJSON_AddNumberToObject(arp_json, "Hardware Type", ntohs(arp->htype));
    cJSON_AddNumberToObject(arp_json, "Protocol Type", ntohs(arp->ptype));
    cJSON_AddNumberToObject(arp_json, "Payload Type", 0x0806); // used by the frontend
    cJSON_AddNumberToObject(arp_json, "Hardware Length", arp->hlen);
    cJSON_AddNumberToObject(arp_json, "Protocol Length", arp->plen);
    cJSON_AddNumberToObject(arp_json, "Operation", ntohs(arp->op));

    char sender_mac[18], receiver_mac[18];
    snprintf(
        sender_mac,
        sizeof sender_mac,
        "%02x:%02x:%02x:%02x:%02x:%02x",
        arp->sha[0], arp->sha[1], arp->sha[2],
        arp->sha[3], arp->sha[4], arp->sha[5]
    );

    snprintf(
        receiver_mac,
        sizeof receiver_mac,
        "%02x:%02x:%02x:%02x:%02x:%02x",
        arp->tha[0], arp->tha[1], arp->tha[2],
        arp->tha[3], arp->tha[4], arp->tha[5]
    );

    cJSON_AddStringToObject(arp_json, "Sender Hardware Address", sender_mac);
    cJSON_AddStringToObject(arp_json, "Target Hardware Address", receiver_mac);

    char src_ip[16], dst_ip[16];
    snprintf(
        src_ip, 
        sizeof src_ip, 
        "%u.%u.%u.%u", 
        arp->spa[0], arp->spa[1], arp->spa[2], arp->spa[3]
    );

    snprintf(
        dst_ip, 
        sizeof dst_ip, 
        "%u.%u.%u.%u", 
        arp->tpa[0], arp->tpa[1], arp->tpa[2], arp->tpa[3]
    );

    cJSON_AddStringToObject(arp_json, "Sender Protocol Address", src_ip);
    cJSON_AddStringToObject(arp_json, "Target Protocol Address", dst_ip);

    char* arp_string = cJSON_PrintUnformatted(arp_json);
    cJSON_Delete(arp_json);
    
    return arp_string;
}
