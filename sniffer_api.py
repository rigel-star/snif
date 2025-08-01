import ctypes
import json

lib = ctypes.CDLL("./lib/libsnif.so")

lib.snif_open.argtypes = [ctypes.c_char_p]
lib.snif_open.restype = ctypes.c_void_p

lib.snif_next.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int]
lib.snif_next.restype = ctypes.c_int

lib.snif_close.argtypes = [ctypes.c_void_p]
lib.snif_close.restype = None

class Sniffer:
    def __init__(self, device: str):
        self.handle = lib.snif_open(device.encode('utf-8'))
        if not self.handle:
            raise RuntimeError(f"Failed to open device {device}")

    def next_packet(self) -> dict:
        buf = ctypes.create_string_buffer(512)
        result = lib.snif_next(self.handle, buf, ctypes.sizeof(buf))
        if result == 0:
            return buf.value.decode('utf-8')
        else:
            return None

    def close(self):
        if self.handle:
            lib.snif_close(self.handle)
            self.handle = None

    def __del__(self):
        self.close()



sniffer = Sniffer("en0")

for _ in range(5):
    pkt = sniffer.next_packet()
    if pkt:
        print("Packet:", pkt)
    else:
        print("No packet or timeout")

sniffer.close()
