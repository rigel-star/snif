import ctypes

lib = ctypes.CDLL("./lib/libsnif.so")

'''
These functions are for capturing and maanging
packets
'''
lib.snif_open.argtypes = [ctypes.c_char_p]
lib.snif_open.restype = ctypes.c_void_p

lib.snif_next.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_size_t]
lib.snif_next.restype = ctypes.c_int

lib.snif_close.argtypes = [ctypes.c_void_p]
lib.snif_close.restype = None

'''
Interface related functions
'''
lib.get_all_interfaces_names.argtypes = [ctypes.POINTER(ctypes.c_int)]
lib.get_all_interfaces_names.restype = ctypes.POINTER(ctypes.c_char_p)

lib.free_interfaces_names.argtypes = [ctypes.POINTER(ctypes.c_char_p), ctypes.c_int]
lib.free_interfaces_names.restype = None

# buffer size for a standard packet
OUT_BUF_SIZE = 4096

class Sniffer:
    def __init__(self, device: str):
        self.handle = lib.snif_open(device.encode('utf-8'))
        if not self.handle:
            raise RuntimeError(f"Failed to open device {device}")

    def next_packet(self) -> dict:
        buf = ctypes.create_string_buffer(OUT_BUF_SIZE)
        result = lib.snif_next(self.handle, buf, OUT_BUF_SIZE)
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


class InterfaceManager:
    def get_all_ifs(self):
        self.count = ctypes.c_int()
        self.ifs = lib.get_all_interfaces_names(ctypes.byref(self.count))
        if not self.ifs:
            raise RuntimeError(f"Failed to get interface names")
        return [self.ifs[i].decode("UTF-8") for i in range(self.count.value)]

    def __del__(self):
        lib.free_interfaces_names(self.ifs, self.count)


if __name__ == "__main__":
    snif = Sniffer("en0") # on my macOS machine
    for _ in range(100):
        pkt = snif.next_packet()
        if pkt is not None:
            print(pkt, end="\n\n")