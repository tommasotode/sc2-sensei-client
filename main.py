import src.gui as gui
import src.upload as up
import threading


if __name__ == "__main__":
	app = gui.ClientGUI()
	uploader = up.AutoUploader("C:/repos/Sc2SenseiClient/ReplayTest", True)
	if uploader == None:
		exit()

	up_thread = threading.Thread(target=uploader.start)
	up_thread.start()

	app.mainloop()