import type ARPPacket from "../../../packets/ARPPacket";

export default function ARPInspector({ pack }: { pack: ARPPacket }) {
	return (
		<>
        <details className="w-full rounded-sm border border-gray-300 bg-white shadow-sm p-2">
			<summary className="cursor-pointer font-semibold text-blue-700">
                Address Resolution Protocol
			</summary>
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