import type Packet from "./Packet";

export default class ARPPacket implements Packet {
    constructor(
        public htype:   number,
        public ptype:   number,
        public hlen:    number,
        public plen:    number,
        public op:      number,
        public sha:     string,
        public spa:     string,
        public tha:     string,
        public tpa:     string,
        public name:    string
    ) {}

    toJSON(): object {
        throw new Error("Method not implemented.");
    }

    isRequest(): boolean {
        return this.op === 1;
    }

    isReply(): boolean {
        return this.op === 2;
    }
}
