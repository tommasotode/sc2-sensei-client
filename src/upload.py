import os
import ctypes as c
import time
import src.files as files

class Uploader:
	def __init__(self, replays_path):
		self.core = c.CDLL(f"{os.getcwd()}/bin/core.so")
		self.set_handle = files.Settings()
		self.replays_path = replays_path
		self.username = self.set_handle.get()["Username"]
		if not self.core.check_files(c.c_char_p(self.replays_path.encode())):
			print("Aborting")
			return None
		self.log_handle = files.Logs()
		self.run = False

		self.core.upload_all_new.restype = c.c_char_p
		self.core.upload_last_n.restype = c.c_char_p
		self.core.get_dir_date.restype = c.c_longlong

	def start_auto_uploader(self):
		self.run = True
		while self.run:
			settings = self.set_handle.get()
			if settings["UploaderState"]:
				old_date = settings["LastModifiedDate"]
				new_date = self.core.get_dir_date(c.c_char_p(self.replays_path.encode()))
				if new_date > old_date:
					print("Directory has been modified\n")
					json_string = self.core.upload_all_new(c.c_longlong(old_date), 
						c.c_char_p(self.replays_path.encode()), c.c_char_p(self.username.encode()))
					self.set_handle.update("LastModifiedDate", new_date)
					self.log_handle.add_replays(json_string)
				else:
					print("Ok")

			time.sleep(5)

	def start_debug(self):
		result = self.core.debug_mode()
		return result
	
	def upload_last_replays(self, quantity):
		self.core.upload_last_n(quantity, self.replays_path)