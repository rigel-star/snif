import type TCPPacket from "../../../packets/TCPPack";

export default function TCPInspector({ pack }: { pack: TCPPacket }) {
	return (
		<details className="w-full rounded-sm border border-gray-300 bg-white shadow-sm p-2">
			<summary className="cursor-pointer font-semibold text-blue-700">
				Transmission Control Protocol
			</summary>
			<p>Source Port: {pack.sport}</p>
			<p>Destination Port: {pack.dport}</p>
			<p>Sequence Number: {pack.seq ?? "-"}</p>
			<p>Acknowledgment: {pack.ack ?? "-"}</p>
		</details>
	);
}