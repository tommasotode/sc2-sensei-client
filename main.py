from tkinter import messagebox
import src.py.gui as gui
import src.py.upload as up
import threading



if __name__ == "__main__":
	app = gui.Main()
	uploader = up.Uploader()
	# If the necessary files aren't found

	up_thread = threading.Thread(target=uploader.start_auto_uploader)
	up_thread.daemon = True
	up_thread.start()

	# Must be in the main thread
	def on_closing():
		app.destroy()
	
	app.protocol("WM_DELETE_WINDOW", on_closing)
	app.mainloop()
