import time

from src.py.settings import Settings
from src.py.logs import Logs
from src.py.core import Core

class Uploader:
	def __init__(self):
		self.core = Core()
		self.settings_handle = Settings()
		self.logs_handle = Logs()

	def start_auto_uploader(self):
		self.run = True
		while self.run:
			settings = self.settings_handle.get()
			up_state = settings["UploaderState"]

			# TODO: up_state is now completely dependent on settings, it can't be changed in any way
			# maybe it would be better to make some functions to handle it so it becomes more easy
			if up_state:
				username = settings["Username"]
				old_date = settings["LastModifiedDate"]
				rep_path = settings["ReplaysDir"]
				
				if self.core.check_files(rep_path):
					new_date = self.core.get_dir_date(rep_path)
				else:
					new_date = old_date - 1
				
				if new_date > old_date:
					localtime = time.strftime('%H:%M:%S', time.localtime())
					print(f"New replay at {localtime}\n")
					log = self.core.upload_all_new(old_date, rep_path, username)
					self.settings_handle.update("LastModifiedDate", new_date)
					self.logs_handle.add_replays(log)
				
				# TODO: Remove this
				else:
					print("Ok")
			
			time.sleep(5)

	def stop_auto_uploader(self):
		self.run = False

	def start_debug(self):
		result = self.core.debug_mode()
		return result