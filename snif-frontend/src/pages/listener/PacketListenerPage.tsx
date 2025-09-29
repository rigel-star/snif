import { useState, useRef, useEffect } from "react";
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
import ToolBar from "./ToolBar";
import EthernetInspector from "./inspectors/EthernetInspector";

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

export default function PacketListenerPage() {
	const [searchParams] = useSearchParams();
    const [packets, setPackets] = useState<EtherFrame[]>([]);
	const [selectedPacket, setSelectedPacket] = useState<EtherFrame | undefined>(undefined);
	const [itface, setItface] = useState<string | null>(null);
    const [filters, setFilters] = useState<string[]>([]);

	const sockRef = useRef<WebSocket | null>(null);

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

	const startConnection = () => {
		if (sockRef.current) {
			return;
		}

		const ws = new WebSocket(`ws://localhost:12346/if/${itface}`);
		sockRef.current = ws;

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
						const updated = [...prev, frame];
						return updated;
					});
				}
			} catch (error) {
				console.error("Failed to parse packet:", error);
			}
		}

		ws.onclose = () => {
			sockRef.current = null;
		}
	}

 	useEffect(() => {
        if (sockRef.current && sockRef.current.readyState === WebSocket.OPEN) {
            sockRef.current.send(JSON.stringify({ type: "filters_update", filters }));
        }
    }, [filters]);

	const rendered = packets.map((packet, index) => {
		if (packet.etherType === EtherType.IPV4) {
			return (
				<IPv4 
					ip={packet.payload as IPv4Packet} 
					frameLen={packet.length}
					onClick={() => {
						setSelectedPacket(packet);
					}}
					index={index + 1}
				/>
			);
		}
		else if (packet.etherType === EtherType.ARP) {
			return (
				<ARP 
					index={index + 1}
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
		<div 
			className="flex flex-col h-screen p-2"
		>
			<ToolBar 
				onStart={() => {
					setPackets([]);
					startConnection();
				}}

				onStop={() => {
					if (sockRef.current) {
						sockRef.current.close();
					}
				}}

				onApply={(filters) => setFilters(filters)}
			/>
			<div 
				id="packet-tbl-container"
				className="flex-1 overflow-auto"
			>
				<table 
					id="packet-tbl"
					className="shadow-sm"
				>
					<thead>
						<tr className="h-5 text-black rounded-sm">
							<th className="rounded-tl-sm rounded-bl-sm" style={{ width: "5%" }}>No.</th>
							<th style={{ width: "20%" }}>Source</th>
							<th style={{ width: "20%" }}>Destination</th>
							<th style={{ width: "10%" }}>Protocol</th>
							<th style={{ width: "10%" }}>Length</th>
							<th className="rounded-tr-sm rounded-br-sm" style={{ width: "35%" }}>Info</th>
						</tr>
					</thead>
					<tbody>
						{rendered}
					</tbody>
				</table>
			</div>

			<div 
				style={{ padding: '0.5rem' }}
				id="packet-inspector"
				className="flex-1 overflow-auto font-mono"
			>
				{selectedPacket === undefined && 
					<div>
						Select a packet to inspect.
					</div>
				}
				{selectedPacket &&
					<EthernetInspector frame={selectedPacket} />
				}
			</div>
		</div>
	);
}