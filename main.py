import src.py.client as client
import src.py.upload as up
import src.py.files as files
import threading

if __name__ == "__main__":
	app = client.AppGUI()
	uploader = up.Uploader()
	# If the necessary files aren't found
	if uploader == None:
		exit()

	up_thread = threading.Thread(target=uploader.start_auto_uploader)
	up_thread.start()

	# Must be in the main thread
	app.mainloop()