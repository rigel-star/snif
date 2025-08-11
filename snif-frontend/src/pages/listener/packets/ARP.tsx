import type ARPPacket from "../../../packets/ARPPacket";
import "./ARP.css";

export interface ARPProps {
    arp: ARPPacket,
	frameLen: number,
};

export default function IPv4({ arp, frameLen }: ARPProps) {
	return (
		<tr>
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
				<span>
                    Who has {arp.tpa}? Tell {arp.spa}
                </span>
			</td>
		</tr>
	);
};