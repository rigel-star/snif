import { useEffect, useState } from "react";
import "./HomePage.css";
import { Link } from "react-router";

interface NetworkInterface {
    name: string;
}

export default function HomePage() {
    const [interfaces, setInterfaces] = useState<NetworkInterface[]>([]);

    useEffect(() => {
        const ws = new WebSocket(`ws://localhost:12345/ifs`);

        ws.onmessage = (event) => {
            try {
                const ifs = JSON.parse(event.data);
                if (ifs) {
                    const _ifs = [];
                    for (let inter of ifs['ifs']) {
                        _ifs.push({
                            name: inter
                        });
                    }
                    setInterfaces(_ifs);
                }
            }
            catch (err) {
                console.error(err);
            }
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
                    {interfaces.map(({ name }) => (
                        <tr 
                            key={name}
                            onClick={() => {
                                
                            }}
                        >
                            <td>
                                <Link to={`/if?__if=${name}`}>
                                    {name}
                                </Link>
                            </td>
                        </tr>
                    ))}
                </tbody>
            </table>
        </main>
    );
};