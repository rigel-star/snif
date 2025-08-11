import type IPv4Packet from "../../../packets/IPv4Packet";
import "./IPv4.css";

export interface IPv4Props {
    ip: IPv4Packet,
	frameLen: number,
};

export default function IPv4({ ip, frameLen }: IPv4Props) {
	return (
		<tr>
			<td>
				{ip.src}
			</td>
			<td>
				{ip.dst}
			</td>
			<td>
				IPv4
			</td>
			<td>
				{frameLen}
			</td>
			<td>
				<span>IPv4 packet</span>
			</td>
		</tr>
	);
};