import type ARPPacket from "../../../packets/ARPPacket";
import "./ARP.css";

import React from "react";

export type ARPProps = {
	index: number;
    arp: ARPPacket,
	frameLen: number,
} & React.HTMLAttributes<HTMLTableRowElement>;

export default function IPv4({ arp, frameLen, onClick, index }: ARPProps) {
	return (
		<tr onClick={onClick}>
			<td className="p-2">
				{index}
			</td>
			<td>
				{arp.sha}
			</td>
			<td>
				{arp.tha}
			</td>
			<td>
				ARP
			</td>
			<td>
				{frameLen}
			</td>
			<td>
				{arp.op === 1 &&
					<span>
    		            Who has {arp.tpa}? Tell {arp.spa}
					</span>
				}
				{arp.op === 2 &&
					<span>
    		            {arp.spa} it at {arp.sha}
					</span>
				}
			</td>
		</tr>
	);
};