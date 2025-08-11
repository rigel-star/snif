import { useState, useEffect } from "react";
import "./PacketListenerPage.css";
import type EtherFrame from "../../packets/EtherFrame";
import IPv4Packet from "../../packets/IPv4Packet";
import { EtherType } from "../../packets/EtherFrame";
import IPv4 from "./packets/IPv4";
import ARPPacket from "../../packets/ARPPacket";
import ARP from "./packets/ARP";

function constructIpv4Pacekt(obj: any): IPv4Packet {
	return new IPv4Packet(
		obj['Version'],
		obj['Internet Header Length'],
		obj['Differentiated Services Code Point'],
		obj['Explicit Congestion Notification'],
		obj['Total Length'],
		obj['Identification'],
		obj['Flags'],
		obj['Offset'],
		obj['Time to Live'],
		obj['Protocol'],
		obj['Checksum'],
		obj['Source'],
		obj['Destination'],
		'IPv4'
	);
}

function constructArpPacket(obj: any): ARPPacket {
	return new ARPPacket(
		obj['Hardware Type'],
		obj['Protocol Type'],
		obj['Hardware Length'],
		obj['Protocol Length'],
		obj['Operation'],
		obj['Sender Hardware Address'],
		obj['Sender Protocol Address'],
		obj['Target Hardware Address'],
		obj['Target Protocol Address'],
		"ARP"
	);
}

export default function PacketListenerPage() {
    const [packets, setPackets] = useState<EtherFrame[]>([]);

	useEffect(() => {
		const interfaceName = "en0";
		const ws = new WebSocket(`ws://localhost:12345/if/${interfaceName}`);

		ws.onmessage = (event) => {
    		try {
				const parsed = JSON.parse(event.data);
				if (parsed["Payload Type"] === EtherType.IPV4) {
					const ip = constructIpv4Pacekt(parsed['Payload']);
					const etherType = parseInt(parsed['Payload Type']);
					const frame: EtherFrame = {
						etherType,
						destination: parsed['Destination'],
						source: parsed['Source'],
						payload: ip,
						length: parsed['Length']
					};
					setPackets((prev) => [...prev, frame]);
				}
				else if (parsed['Payload Type'] === EtherType.ARP) {
					const arp = constructArpPacket(parsed['Payload']);
					console.log(arp);
					const etherType = parseInt(parsed['Payload Type']);
					const frame: EtherFrame = {
						etherType,
						destination: parsed['Destination'],
						source: parsed['Source'],
						payload: arp,
						length: parsed['Length']
					};
					setPackets((prev) => [...prev, frame]);
				}
			} catch (error) {
				console.error("Failed to parse packet:", error);
			}
		};

		return () => {
			ws.close();
		};
	}, []);

	const rendered = packets.map((packet) => {
		if (packet.etherType === EtherType.IPV4) {
			return (
				<IPv4 ip={packet.payload as IPv4Packet} frameLen={packet.length} />
			);
		}
		else if (packet.etherType == EtherType.ARP) {
			return (
				<ARP arp={packet.payload as ARPPacket} frameLen={packet.length} />
			);
		}
	});

	return (
		<div>
			<table id="packet-list">
				<thead>
					<tr>
						<th>Source</th>
						<th>Destination</th>
						<th>Protocol</th>
						<th>Length</th>
						<th>Info</th>
					</tr>
				</thead>
				<tbody>
					{rendered}	
				</tbody>
			</table>
		</div>
	);
}