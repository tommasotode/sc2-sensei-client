import src.client as client
import src.upload as up
import threading

if __name__ == "__main__":
	app = client.App()
	uploader = up.AutoUploader("C:/repos/Sc2SenseiClient/ReplayTest", True)
	if uploader == None:
		exit()

	# TODO: Put settings here and interface it with the gui


	up_thread = threading.Thread(target=uploader.start)
	up_thread.start()

	# Must be in the main thread
	app.mainloop()