import type ARPPacket from "../../../packets/ARPPacket";
import type EtherFrame from "../../../packets/EtherFrame";
import { EtherType } from "../../../packets/EtherFrame";
import IPv4Packet from "../../../packets/IPv4Packet";
import ARPInspector from "./ARPInspector";
import IPv4Inspector from "./IPV4Inspector";

export default function EthernetInspector({ frame }: { frame: EtherFrame }) {
	return (
		<>
		<details className="w-full rounded-sm border border-gray-300 bg-white shadow-sm p-2">
			<summary className="cursor-pointer font-semibold text-blue-700">
                Ethernet II
			</summary>
			<div>
				<p>
					Source Address: {frame.source}
				</p>
				<p>
					Destination Address: {frame.destination}
				</p>
			</div>
		</details>
		{frame.etherType === EtherType.IPV4 &&
			<IPv4Inspector pack={frame.payload as IPv4Packet} />
		}
		{frame.etherType == EtherType.ARP &&
			<ARPInspector pack={frame.payload as ARPPacket} />
		}
		</>
	);
}