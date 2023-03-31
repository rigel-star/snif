#!/bin/python3

import threading
import ctypes
import queue
import sys
import typing

_recv_buffer = (ctypes.c_char * 2048)()
_api_buffer = None

def _read_data():
    counter = 0
    while True:
        data = _recv_buffer.value
        if data and len(data) > 0:
            if _api_buffer is not None:
                _api_buffer.append(data[:])
            _recv_buffer.value = bytes()
            '''
            if len(data) <= 13:
                typ = data[12]
            else:
                typ = (data[12] << 8) | data[13]
            
            if typ == 0x0800:
                print(f"{counter}) IPV4")
                counter += 1
            elif typ == 0x86DD:
                print(f"{counter}) IPV6")
                counter += 1
            '''

def start_listening(buffer: typing.List = None):
    global _api_buffer
    _api_buffer = buffer

    clib = ctypes.CDLL("./lib/libsnif.so")
    fn_recv_ether_raw = clib.recv_ether_raw
    fn_recv_ether_raw.argtypes = [ctypes.POINTER(ctypes.c_char)]

    fn_listen_on_intf = clib.listen_on_intf
    fn_listen_on_intf.argtypes = [ctypes.POINTER(ctypes.c_char)]
    clib.listen_on_intf("enp0s6".encode())

    ether_recv_thread = threading.Thread(target=clib.recv_ether_raw, args=(_recv_buffer,))
    ether_recv_thread.start()

    read_thread = threading.Thread(target=_read_data)
    read_thread.start()

if __name__ == "__main__":
    main()
