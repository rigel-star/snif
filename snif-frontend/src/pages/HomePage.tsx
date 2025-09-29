import { useEffect, useRef, useState } from "react";
import "./HomePage.css";
import { Link } from "react-router";

interface NetworkInterface {
    name: string;
    isWireless: boolean;
}

export default function HomePage() {
    const [interfaces, setInterfaces] = useState<NetworkInterface[]>([]);
    const wsRef = useRef<WebSocket>(null);

    useEffect(() => {
        const ws = new WebSocket(`ws://localhost:12346/ifs`);
        wsRef.current = ws;

        ws.onmessage = (event) => {
            try {
                const ifs = JSON.parse(event.data);
                if (ifs) {
                    const _ifs = [];
                    for (let inter of ifs['ifs']) {
                        _ifs.push({
                            name: inter.replace("(Wi-Fi)", ""),
                            isWireless: inter.includes("(Wi-Fi)")
                        });
                    }
                    setInterfaces(_ifs);
                }
            }
            catch (err) {
                console.error(err);
            }
        };

        return () => {
            ws.close();
        };
    }, []);

    return (
        <main className="main-container">
            <h1 className="heading">Welcome to Snif</h1>
            <table className="interfaces-table">
                <thead>
                    <tr>
                        <th>Interfaces</th>
                    </tr>
                </thead>
                <tbody>
                    {interfaces.map(({ name, isWireless }) => (
                        <tr 
                            key={name}
                            onClick={() => {
                                
                            }}
                        >
                            <td>
                                <Link to={`/if?__if=${name}`}>
                                    {name} {isWireless ? "(Wi-Fi)" : ""}
                                </Link>
                            </td>
                        </tr>
                    ))}
                </tbody>
            </table>
        </main>
    );
};