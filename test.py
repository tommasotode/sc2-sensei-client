import client.gui as gui
import os
import ctypes as c
if __name__ == "__main__":
	core = c.CDLL(f"{os.getcwd()}/lib/core.so")
	core.debug_mode()
	# app = gui.Client()
	# app.main()
	# app.mainloop()