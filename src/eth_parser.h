#pragma once

#include <pcap.h>
#include <stdlib.h>
#include <string.h>
#include "ether.h"

EtherHeader_t parse_ether_pkt(const u_char *packet, int len);