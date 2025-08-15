import type Packet from "./Packet";

export enum IPv4Protocol {
    ICMP    = 1,
    TCP     = 6,
    UDP     = 17,
    GRE     = 47,
    ESP     = 50,
    AH      = 51,
    EIGRP   = 88,
    OSPF    = 89,
    SCTP    = 132,
}

export function getIpProtocolName(proto: number): string {
	switch (proto) {
		case IPv4Protocol.TCP: 	return "TCP";
		case IPv4Protocol.UDP: 	return "UDP";
		case IPv4Protocol.ICMP: return "ICMP";
		case IPv4Protocol.OSPF: return "OSPF";
		default: 				return "UNK(Unknown)"
	}
}

export default class IPv4Packet implements Packet {
    constructor(
        public version:     number,
        public ihl:         number,
        public dscp:        number,
        public ecn:         number,
        public len:         number,
        public id:          number,
        public flags:       number,
        public offset:      number,
        public ttl:         number,
        public proto:       number,
        public chk:         number,
        public src:         string,
        public dst:         string,
        public name:        string,
        public payload?:    Packet
    ) {}

    toJSON(): object {
        throw new Error("Method not implemented.");
    }
}