import { useState, useEffect } from "react";
import "./PacketListenerPage.css";
import type EtherFrame from "../../packets/EtherFrame";
import IPv4Packet, { IPv4Protocol } from "../../packets/IPv4Packet";
import { EtherType } from "../../packets/EtherFrame";
import IPv4 from "./packets/IPv4";
import ARPPacket from "../../packets/ARPPacket";
import ARP from "./packets/ARP";
import { useSearchParams } from "react-router";
import TCPPacket from "../../packets/TCPPack";
import type Packet from "../../packets/Packet";

function constructIpv4Pacekt(obj: any): IPv4Packet {
	let ipPayload: Packet | undefined = undefined;
	const protocol = parseInt(obj['Protocol']);
	if (protocol === IPv4Protocol.TCP) {
		const pay = obj['Payload'];
		ipPayload = new TCPPacket(
			pay['Source Port'],
			pay['Destination Port'],
			pay['Sequence Number'],
			pay['Acknowledgement Number'],
			pay['Offset'],
			pay['Reserved'],
			pay['Flags'],
			pay['Window'],
			pay['Checksum'],
			pay['Urgent'],
			[],
			"TCP"
		);
	}

	const ip = new IPv4Packet(
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
		'IPv4',
		ipPayload
	);

	return ip;
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

function TCPInspector({ pack }: { pack: TCPPacket }) {
	return (
		<details>
			<summary>Transmission Control Protocol</summary>
			<div>
				<p>
					{pack.sport}
				</p>
				<p>
					{pack.dport}
				</p>
			</div>
		</details>
	);
}

function IPv4PacketInspector({ pack }: { pack: IPv4Packet }) {
	return (
		<>
		<details>
			<summary>Internet Protocol Version 4</summary>
			<div>
				<p>
					{pack.src}
				</p>
				<p>
					{pack.dst}
				</p>
			</div>
		</details>
		{pack.proto === IPv4Protocol.TCP &&
			<TCPInspector pack={pack.payload as TCPPacket} />
		}
		</>
	);
}

function ARPInspector({ pack }: { pack: ARPPacket }) {
	return (
		<>
		<details>
			<summary>Address Resolution Protocol</summary>
			<div>
				<p>
					{pack.sha}
				</p>
				<p>
					{pack.tha}
				</p>
			</div>
		</details>
		</>
	);
}

function PacketInspector({ frame }: { frame: EtherFrame }) {
	return (
		<>
		<details>
			<summary>Ethernet II</summary>
			<div>
				<p>
					{frame.source}
				</p>
				<p>
					{frame.destination}
				</p>
			</div>
		</details>
		{frame.etherType === EtherType.IPV4 &&
			<IPv4PacketInspector pack={frame.payload as IPv4Packet} />
		}
		{frame.etherType === EtherType.ARP &&
			<ARPInspector pack={frame.payload as ARPPacket} />
		}
		</>
	);
}

export default function PacketListenerPage() {
	const [searchParams] = useSearchParams();
    const [packets, setPackets] = useState<EtherFrame[]>([]);
	const [selectedPacket, setSelectedPacket] = useState<EtherFrame | undefined>(undefined);
	const [itface, setItface] = useState<string | null>(null);

	if (itface === "" || itface === null) {
		if (searchParams.get("__if")) {
			setItface(searchParams.get("__if"));
		}
		else {
			return (
				<div>
					<span>
						Error: valid interface name was not provided!
					</span>
				</div>
			);
		}
	}

	useEffect(() => {
		const ws = new WebSocket(`ws://localhost:12345/if/${itface}`);

		ws.onmessage = (event) => {
    		try {
				const parsed = JSON.parse(event.data);
				let frame: EtherFrame | null = null;

				if (parsed["Payload Type"] === EtherType.IPV4) {
					const ip = constructIpv4Pacekt(parsed['Payload']);
					frame = {
						etherType: EtherType.IPV4,
						destination: parsed['Destination'],
						source: parsed['Source'],
						payload: ip,
						length: parsed['Length']
					};
				}
				else if (parsed['Payload Type'] === EtherType.ARP) {
					const arp = constructArpPacket(parsed['Payload']);
					frame = {
						etherType: EtherType.ARP,
						destination: parsed['Destination'],
						source: parsed['Source'],
						payload: arp,
						length: parsed['Length']
					};
				}
				if (frame) {
					setPackets(prev => {
						const updated = [frame, ...prev];
						return updated.slice(0, 50);
					});
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
				<IPv4 
					ip={packet.payload as IPv4Packet} 
					frameLen={packet.length}
					onClick={() => {
						setSelectedPacket(packet);
					}}
				/>
			);
		}
		else if (packet.etherType === EtherType.ARP) {
			return (
				<ARP 
					arp={packet.payload as ARPPacket} 
					frameLen={packet.length} 
					onClick={() => {
						setSelectedPacket(packet);
					}}
				/>
			);
		}
	});

	return (
		<div style={{ 
			height: '100vh', 
			display: 'flex', 
			flexDirection: 'column' 
		}}>
			<div 
				id="packet-tbl-container" 
				style={{ flex: 1, overflow: 'auto' }}
			>
				<table id="packet-tbl">
					<thead>
						<tr>
							<th style={{ width: "20%" }}>Source</th>
							<th style={{ width: "20%" }}>Destination</th>
							<th style={{ width: "10%" }}>Protocol</th>
							<th style={{ width: "10%" }}>Length</th>
							<th style={{ width: "40%" }}>Info</th>
						</tr>
					</thead>
					<tbody>
						{rendered}
					</tbody>
				</table>
			</div>

			<div 
				style={{ flex: 1, overflow: 'auto', padding: '0.5rem', fontFamily: 'monospace' }}
				id="packet-inspector"
			>
				{selectedPacket === undefined && 
					<div>
						Select a packet to inspect.
					</div>
				}
				{selectedPacket &&
					<PacketInspector frame={selectedPacket} />
				}
			</div>
		</div>
	);
}