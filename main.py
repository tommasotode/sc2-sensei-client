import src.client as client
import src.upload as up
import src.files as files
import threading

if __name__ == "__main__":
	app = client.AppGUI()
	uploader = up.Uploader("C:/repos/Sc2SenseiClient/ReplayTest")
	logs = files.Logs()
	# If the necessary files aren't found
	if uploader == None:
		exit()

	up_thread = threading.Thread(target=uploader.start_auto_uploader)
	up_thread.start()


	# Must be in the main thread
	app.mainloop()