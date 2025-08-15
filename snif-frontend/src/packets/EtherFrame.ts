import type Packet from "./Packet";

export enum EtherType {
	IPV4 	= 0x0800,
	ARP 	= 0x0806
};

export default interface EtherFrame {
	etherType:     	EtherType;
	length:			number;
	source:         string;
	destination:    string;
	payload:		Packet;
};