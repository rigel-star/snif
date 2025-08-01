#pragma once

#include <stdint.h>

typedef struct IPv4 {
    uint8_t ver_ihl;
    uint8_t dscp_ecn;
    uint16_t len;
    uint16_t id;
    uint8_t flags_off;
    uint8_t ttl;
    uint8_t proto;
    uint16_t chk;
    uint8_t src[4];
    uint8_t dst[4];
} IPv4_t;