import type EtherFrame from "../../packets/EtherFrame";
import { EtherType } from "../../packets/EtherFrame";

export enum Filter {
    TCP     = "tcp",
    UDP     = "udp",
    ARP     = "arp"
}

export default class FilterPipeline {
    private packets: EtherFrame[];
    private filters: string[];

    constructor(packets: EtherFrame[], filters: string[]) {
        this.packets = packets;
        this.filters = filters;
    }

    public filterArp() {
        this.packets = this.packets.filter(packet => packet.etherType === EtherType.ARP);
    }

    public filterTcp() {
        this.packets = this.packets.filter(packet => packet.etherType === EtherType.IPV4);
    }

    public filter() {
        this.filters.forEach(filter => {
            if (filter === "tcp") {
                this.filterTcp();
            }
            else if (filter === "arp") {
                this.filterArp();
            }
        });
        return this.packets;
    }
}