import { useEffect, useState } from "react";

function PacketListPage() {
	const [packets, setPackets] = useState<string[]>([]);

	useEffect(() => {
		const interfaceName = "en0";
		const ws = new WebSocket(`ws://localhost:12345/if/${interfaceName}`);

		ws.onmessage = (event) => {
			let pkt = event.data;
			let pkts = [...packets];
			pkts.push(pkt);
			setPackets(pkts);
		};
	});

	let pkts = packets.map(packet => {
		return (
			<li>
				{packet}
			</li>
		);
	});

	return (
		<div>
			<ul>
				{pkts}
			</ul>
		</div>
	);
}

export default function App() {
	return (
		<PacketListPage />
	);
}