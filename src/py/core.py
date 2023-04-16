import time
import ctypes
import os

start = time.time()
core = ctypes.CDLL(f"{os.getcwd()}/bin/core.so")
# print("asd")
end = time.time()
print((end - start)*1000)