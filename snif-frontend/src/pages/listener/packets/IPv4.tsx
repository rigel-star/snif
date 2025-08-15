import type IPv4Packet from "../../../packets/IPv4Packet";
import { getIpProtocolName } from "../../../packets/IPv4Packet";
import "./IPv4.css";

import React from "react";

export type IPv4Props = {
    ip: IPv4Packet,
	frameLen: number,
} & React.HTMLAttributes<HTMLTableRowElement>;

export default function IPv4({ ip, frameLen, onClick }: IPv4Props) {
	return (
		<tr className="packet-list-item" onClick={onClick}>
			<td>
				{ip.src}
			</td>
			<td>
				{ip.dst}
			</td>
			<td>
				{getIpProtocolName(ip.proto)}
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