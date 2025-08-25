import type IPv4Packet from "../../../packets/IPv4Packet";
import { IPv4Protocol } from "../../../packets/IPv4Packet";
import type TCPPacket from "../../../packets/TCPPack";
import TCPInspector from "./TCPInspector";

export default function IPv4Inspector({ pack }: { pack: IPv4Packet }) {
	return (
		<>
		<details className="w-full rounded-sm border border-gray-300 bg-white shadow-sm p-2">
			<summary className="cursor-pointer font-semibold text-blue-700">
				Internet Protocol Version 4
			</summary>
			<div>
				<p>
					Source Address: {pack.src}
				</p>
				<p>
					Destination Address: {pack.dst}
				</p>
			</div>
		</details>
		{pack.proto === IPv4Protocol.TCP &&
			<TCPInspector pack={pack.payload as TCPPacket} />
		}
		</>
	);
}