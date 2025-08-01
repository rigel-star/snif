import flask
import os
import threading
from ctypes import *
import logging
import sys

libsnif = CDLL("./lib/libsnif.so")

libsnif.get_all_interfaces_names.argtypes = [POINTER(c_int)]
libsnif.get_all_interfaces_names.restype = POINTER(c_char_p)

libsnif.free_interfaces_names.argtypes = [POINTER(c_char_p), c_int]
libsnif.free_interfaces_names.restype = None

libsnif.snif_lookup_dev.argtypes = [c_char_p]
libsnif.snif_lookup_dev.restype = None

def __get_all_interfaces_names():
    count = c_int()

    names_ptr = libsnif.get_all_interfaces_names(byref(count))
    if not names_ptr:
        raise RuntimeError("Failed to get interface names")

    interface_names = [names_ptr[i].decode() for i in range(count.value)]

    libsnif.free_interfaces_names(names_ptr, count)
    return interface_names


app = flask.Flask("snif", static_folder="./static")

# log = logging.getLogger('werkzeug')
# log.setLevel(logging.ERROR)

pipe_path = "/tmp/packets.pipe"
pkt_data = {
    "en0": []
}

def listen_to_pipe():
    with open(pipe_path, 'r') as pipe:
        while True:
            line = pipe.readline()
            if line:
                pkt_data["en0"].append(line)


def start_sniffer(interface: str):
    snif_starter = threading.Thread(target=lambda: libsnif.snif_lookup_dev(bytes(interface, encoding="UTF-8")), daemon=True)
    snif_starter.start()

    pkt_reader = threading.Thread(target=listen_to_pipe, daemon=True)
    pkt_reader.start()


@app.route("/", methods=["GET"])
def home():
    return flask.render_template("index.html", all_ifs = __get_all_interfaces_names())

@app.route("/if/<interface>")
def snif_interface(interface: str):
    start_sniffer(interface)
    return flask.render_template("interface.html", interface=interface)

@app.route("/if/<interface>/get")
def get_data_for_interface(interface: str):
    chunk_size = 5
    data_list = pkt_data.get(interface, [])

    chunk = data_list[:chunk_size]
    pkt_data[interface] = data_list[chunk_size:]
    return {
        "packets": [chunk]
    }

app.run(host="0.0.0.0", port=9000, debug=True, threaded=True)