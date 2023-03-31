import flask
import ctypes
import base64

import api

app = flask.Flask("snif", static_folder="./static")

_pack_list = []
@app.route("/get", methods=["GET"])
def _get_next_pack():
    while len(_pack_list) == 0: pass # wait until data in available
    data = _pack_list.pop(0)
    resp = flask.make_response(flask.jsonify({"pack_data": list(data)}))
    resp.headers['Content-Type'] = "application/json"
    return resp

@app.route("/", methods=['GET', 'POST'])
def home_page():
    return flask.render_template("index.html", all_ifs = ["enp0s6", "lo"])

api.start_listening(_pack_list)
