import os
import ctypes as c

if __name__ == "__main__":
	core = c.CDLL(f"{os.getcwd()}/bin/core.so")
	core.debug_mode()