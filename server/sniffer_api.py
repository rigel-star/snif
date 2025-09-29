'''
MIT License

Copyright (c) 2023 Kagati Foundation

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
'''

import ctypes
import json

ETHER_IPV4 = 0x0800

lib_sniffer = ctypes.CDLL("../lib/libsnif.so")

'''
These functions are for capturing and maanging
packets
'''
lib_sniffer.snif_open.argtypes = [ctypes.c_char_p]
lib_sniffer.snif_open.restype = ctypes.c_void_p

lib_sniffer.snif_next.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_size_t]
lib_sniffer.snif_next.restype = ctypes.c_int

lib_sniffer.snif_close.argtypes = [ctypes.c_void_p]
lib_sniffer.snif_close.restype = None

'''
Interface related functions
'''
lib_sniffer.get_all_interfaces_names.argtypes = [ctypes.POINTER(ctypes.c_int)]
lib_sniffer.get_all_interfaces_names.restype = ctypes.POINTER(ctypes.c_char_p)

lib_sniffer.free_interfaces_names.argtypes = [ctypes.POINTER(ctypes.c_char_p), ctypes.c_int]
lib_sniffer.free_interfaces_names.restype = None

# buffer size for a standard packet
OUT_BUF_SIZE = 4096

class Sniffer:
    def __init__(self, device: str):
        self.handle = lib_sniffer.snif_open(device.encode('utf-8'))
        if not self.handle:
            raise RuntimeError(f"Failed to open device {device}")

    def next_packet(self) -> dict:
        buf = ctypes.create_string_buffer(OUT_BUF_SIZE)
        result = lib_sniffer.snif_next(self.handle, buf, OUT_BUF_SIZE)
        if result == 0:
            return buf.value.decode('utf-8')
        else:
            return None

    def close(self):
        if self.handle:
            lib_sniffer.snif_close(self.handle)
            self.handle = None

    def __del__(self):
        self.close()


class InterfaceManager:
    def get_all_interface_names(self):
        self.count = ctypes.c_int()
        self.interfaces = lib_sniffer.get_all_interfaces_names(ctypes.byref(self.count))
        if not self.interfaces:
            raise RuntimeError(f"Failed to get interface names")
        return [self.interfaces[i].decode("UTF-8") for i in range(self.count.value)]

    def __del__(self):
        lib_sniffer.free_interfaces_names(self.interfaces, self.count)


if __name__ == "__main__":
    sniffer = Sniffer("en0") # on my macOS machine
    for _ in range(100):
        packet = sniffer.next_packet()
        if packet is not None:
            try:
                packet_json = json.loads(packet)
                if int(packet_json['Payload Type']) == ETHER_IPV4:
                    payload = packet_json['Payload']
                    protocol = payload.get("Protocol")
                    protocol_name = payload.get("Protocol Name")
                    print(f"{protocol} = {protocol_name}")
            except json.JSONDecodeError as err:
                print(err)