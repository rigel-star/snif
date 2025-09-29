ETHER_TYPE = {
    0x0800: "ipv4",
    0x0806: "arp",
}

IPV4_PROTOCOLS = {
    1: "icmp",
    2: "igmp",
    6: "tcp",
    17: "udp",
    41: "encap",
    89: "ospf",
    132: "sctp"
}

ALL_PAYLOAD_TYPES = ETHER_TYPE | IPV4_PROTOCOLS