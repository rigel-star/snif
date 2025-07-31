import flask
import ctypes
import base64
import threading

import api as libsnif

app = flask.Flask("snif", static_folder="./static")

@app.route("/", methods=["GET"])
def home():
    libsnif.__listen_to_pkt_pipe(b"eth0")
    return flask.render_template("index.html", all_ifs = libsnif.__get_all_interfaces_names())

if __name__ == '__main__':
    app.run(host="0.0.0.0", port=9000, debug=True, threaded=True)
