import type Packet from "./Packet";

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