import os
import ctypes as c
import time
import src.files as files

class Uploader:
	def __init__(self):
		self.core = c.CDLL(f"{os.getcwd()}/bin/core.so")
		self.settings_handle = files.Settings()
		self.logs_handle = files.Logs()
		self.replays_path = self.settings_handle.get()["ReplaysDir"]
		
		if not self.core.check_files(c.c_char_p(self.replays_path.encode())) \
													and self.replays_path != "":
			print("Aborting")
			return None
		
		self.run = False
		self.core.upload_all_new.restype = c.c_char_p
		self.core.upload_last_n.restype = c.c_char_p
		self.core.get_dir_date.restype = c.c_longlong

	def start_auto_uploader(self):
		self.run = True
		while self.run:
			settings = self.settings_handle.get()
			up_state = settings["UploaderState"]
			if up_state:
				username = settings["Username"]
				rep_path = settings["ReplaysDir"]
				old_date = settings["LastModifiedDate"]
				new_date = self.core.get_dir_date(c.c_char_p(rep_path.encode()))
				if new_date > old_date:
					localt = time.strftime('%H:%M:%S', time.localtime())
					print(f"Directory has been modified at {localt}\n")
					
					json_string = self.core.upload_all_new(c.c_longlong(old_date), 
						c.c_char_p(rep_path.encode()), c.c_char_p(username.encode()))
					self.settings_handle.update("LastModifiedDate", new_date)
					self.logs_handle.add_replays(json_string)
				else:
					print("Ok")
			time.sleep(5)

	def start_debug(self):
		result = self.core.debug_mode()
		return result
	
	def upload_last_replays(self, quantity):
		self.core.upload_last_n(quantity, self.replays_path)