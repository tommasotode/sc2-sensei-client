import os
import ctypes as c
import json
import time
import src.files as files

class AutoUploader:
	def __init__(self, replays_path, logging):
		self.core = c.CDLL(f"{os.getcwd()}/bin/core.so")
		self.data_path = f"{os.getcwd()}/data/date.dat".encode()
		self.replays_path = replays_path.encode()
		self.logging = logging

		if not self.core.check_files(c.c_char_p(self.data_path), c.c_char_p(self.replays_path)):
			print("Aborting")
			return None

		self.log_handle = files.Logs()

		self.core.upload_all_new.restype = c.c_char_p

	def start(self):
		self.run = True
		while self.run:
			old_date = self.core.get_file_date(c.c_char_p(self.data_path))
			new_date = self.core.get_dir_date(c.c_char_p(self.replays_path))

			if new_date > old_date:
				print("Directory has been modified\n")
				json_string = self.core.upload_all_new(c.c_longlong(old_date), c.c_char_p(self.replays_path))
				log = json.loads(json_string)
				self.log_handle.add_replays(log)
				self.core.wrt_file_date(c.c_char_p(self.data_path), c.c_longlong(new_date))
			elif new_date == old_date:
				print("Ok")
			else:
				print("There was an error.")

			time.sleep(10)
	
	def stop(self):
		self.run = False