import type EtherFrame from "../../packets/EtherFrame";
import { EtherType } from "../../packets/EtherFrame";

export enum Filter {
    TCP     = "tcp",
    UDP     = "udp",
    ARP     = "arp"
};

// Map string values to Filter enum
const stringToFilterMap: Record<string, Filter> = {
	"tcp": Filter.TCP,
	"udp": Filter.UDP,
	"arp": Filter.ARP
};

/**
 * Convert a string to a Filter enum value.
 * Returns undefined if the string is not valid.
 */
export function stringToFilter(value: string): Filter | undefined {
	return stringToFilterMap[value.toLowerCase()];
}

/**
 * Strict version: throws if invalid
 */
export function stringToFilterStrict(value: string): Filter {
	const f = stringToFilterMap[value.toLowerCase()];
	if (!f) throw new Error(`Invalid filter string: ${value}`);
	return f;
}

export function stringsToFilters(values: string[]): Filter[] {
	return values
		.map(v => stringToFilterStrict(v))
		.filter((f): f is Filter => f !== undefined);
}

export default class FilterPipeline {
    private packets: EtherFrame[];

    constructor(packets: EtherFrame[]) {
        this.packets = packets;
    }

    public filterArp() {
        this.packets = this.packets.filter(packet => packet.etherType === EtherType.ARP);
    }

    public filterTcp() {
        this.packets = this.packets.filter(packet => packet.etherType === EtherType.IPV4);
    }

    public filter(filters: Filter[]) {
        filters.forEach(filter => {
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