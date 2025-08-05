import flask
import threading
from ctypes import *
from sniffer_api import *
import queue

app = flask.Flask("snif", static_folder="./static")

pkt_queues = {
    
}

def __writer(interface: str, count: int, sniffer: Sniffer):
    global pkt_queues
    pkt_queues[interface] = queue.Queue()
    if_q: queue.Queue = pkt_queues[interface]

    for _ in range(count):
        pkt = sniffer.next_packet()
        if_q.put(pkt)


def start_sniffer(interface: str):
    sniffer = Sniffer(interface)
    writer = threading.Thread(target=__writer, args=(interface, 100, sniffer))
    writer.start()


@app.route("/", methods=["GET"])
def home():
    if_mgr = InterfaceManager()
    return flask.render_template("index.html", all_ifs = if_mgr.get_all_ifs())

@app.route("/if/<interface>")
def snif_interface(interface: str):
    start_sniffer(interface)
    return flask.render_template("interface.html", interface=interface)

@app.route("/if/<interface>/get")
def get_data_for_interface(interface: str):
    q = pkt_queues.get(interface, None)
    pkt_item = q.get()
    return {
        "packets": pkt_item
    }

app.run(host="0.0.0.0", port=9000, debug=True, threaded=True)