import time
import src.py.files as files
from src.py.core import Core

class Uploader:
	def __init__(self):
		self.core = Core()
		self.settings_handle = files.Settings()
		self.logs_handle = files.Logs()

	def start_auto_uploader(self):
		self.run = True
		while self.run:
			settings = self.settings_handle.get()
			up_state = settings["UploaderState"]
			if up_state:
				username = settings["Username"]
				rep_path = settings["ReplaysDir"]
				old_date = settings["LastModifiedDate"]
				if self.core.check_files(rep_path):
					new_date = self.core.get_dir_date(rep_path)
				else:
					new_date = old_date - 1
				if new_date > old_date:
					localt = time.strftime('%H:%M:%S', time.localtime())
					print(f"Directory has been modified at {localt}\n")
					log = self.core.upload_all_new(old_date, rep_path, username)
					self.settings_handle.update("LastModifiedDate", new_date)
					self.logs_handle.add_replays(log)
				else:
					print("Ok")
			time.sleep(5)

	def start_debug(self):
		result = self.core.debug_mode()
		return result
	
	def upload_last_replays(self, quantity):
		self.core.upload_last_n(quantity, self.replays_path)