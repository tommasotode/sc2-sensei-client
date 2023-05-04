from tkinter import messagebox
import src.py.gui as gui
import src.py.upload as up
import threading



if __name__ == "__main__":
	# Check if there is an existing process with the same name as us
	# in that case, return without doing anything

	# reference 
	#####  "main??.exe" in (i.name() for i in psutil.process_iter())
	# Considera di rinominare main.py in sc2senseiclient.py
	# credo diventi il nome del processo nel task manager

	app = gui.Main()
	uploader = up.Uploader()
	# If the necessary files aren't found

	up_thread = threading.Thread(target=uploader.start_auto_uploader)
	up_thread.daemon = True
	up_thread.start()

	# We execute tray application
	# There is a tray icon that can be clicked
	# On click, run app.mainloop()
	# The first time, we also run it automatically

	

	# Must be in the main thread
	def on_closing():
		app.destroy()
	
	app.protocol("WM_DELETE_WINDOW", on_closing)
	app.mainloop()
