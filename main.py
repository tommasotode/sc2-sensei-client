import src.gui as gui
import src.upload as up
import threading


if __name__ == "__main__":
	app = gui.ClientGUI()
	uploader = up.AutoUploader("C:/repos/Sc2SenseiClient/ReplayTest")
	if uploader == None:
		exit()

	uploader.start()
	app.mainloop()