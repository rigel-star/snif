import type Packet from "./Packet";

export default class TCPPacket implements Packet {
    constructor(
        public sport:       number,
        public dport:       number,
        public seq:         number,
        public ack:         number,
        public dataOffset:  number,
        public reserved:    number,
        public flags:       number,
        public win:         number,
        public chk:         number,
        public urg:         number,
        public options:     number[],
        public name:        string,
        public payload?:    Packet,
    ) {}

    toJSON(): object {
        throw new Error("Method not implemented.");
    }
}
