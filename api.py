from ctypes import *
import os

libsnif = CDLL("./libsnif.so")

libsnif.get_all_interfaces_names.argtypes = [POINTER(c_int)]
libsnif.get_all_interfaces_names.restype = POINTER(c_char_p)

libsnif.free_interfaces_names.argtypes = [POINTER(c_char_p), c_int]
libsnif.free_interfaces_names.restype = None

libsnif.snif_lookup_dev.argtypes = [c_char_p]
libsnif.snif_lookup_dev.restype = None

def __listen_to_pkt_pipe(dev: bytes):
    pipe_path = "/tmp/packets.pipe"

    if not os.path.exists(pipe_path):
        os.mkfifo(pipe_path)

    print("Listening to packets...")
    # Open pipe for reading **before** calling C function
    with open(pipe_path, 'r') as pipe:
        # Start C packet capture in a separate thread or process ideally
        # but if calling inline, use threading or subprocess to avoid blocking

        # Start a thread or process for the C capture
        import threading
        t = threading.Thread(target=libsnif.snif_lookup_dev, args=(dev,), daemon=True)
        t.start()

        while True:
            line = pipe.readline()
            if line:
                print(f"Received: {line.strip()}")


def __get_all_interfaces_names():
    count = c_int()

    names_ptr = libsnif.get_all_interfaces_names(byref(count))
    if not names_ptr:
        raise RuntimeError("Failed to get interface names")

    interface_names = [names_ptr[i].decode() for i in range(count.value)]

    print(interface_names)

    libsnif.free_interfaces_names(names_ptr, count)
    return interface_names