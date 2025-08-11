import threading
from ctypes import *
from sniffer_api import *
import websockets
import asyncio

pkt_queues = {}
clients = {}

async def broadcast(interface: str):
    while True:
        pkt = await pkt_queues[interface].get()
        if interface in clients:
            dead_clients = set()
            for ws in clients[interface]:
                try:
                    await ws.send(json.dumps(pkt))
                except:
                    dead_clients.add(ws)
            clients[interface] -= dead_clients


def sniffer_thread(interface, loop):
    global pkt_queues

    snif = Sniffer(interface)
    while True:
        pkt = snif.next_packet()
        if pkt is not None:
            try:
                pkt_json = json.loads(pkt)
                asyncio.run_coroutine_threadsafe(
                    pkt_queues[interface].put(pkt_json),
                    loop
                )
            except json.JSONDecodeError:
                continue


async def handle_client(ws):
    global clients
    interface = "en0"

    if interface not in clients:
        clients[interface] = set()

    clients[interface].add(ws)
    if interface not in pkt_queues:
        pkt_queues[interface] = asyncio.Queue()
        loop = asyncio.get_running_loop()
        threading.Thread(target=sniffer_thread, args=(interface, loop), daemon=True).start()
        asyncio.create_task(broadcast(interface))

    try:
        async for msg in ws:
            pass
    finally:
        await ws.close()
        clients[interface].remove(ws)


async def main():
    server = await websockets.serve(handle_client, 'localhost', 12345)
    await server.wait_closed()

if __name__ == "__main__":
    asyncio.run(main())