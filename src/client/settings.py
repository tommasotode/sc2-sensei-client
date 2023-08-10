import os
import json
from time import time

from src.client.files import FileHandle

class Settings(FileHandle):
	def __init__(self):
		super().__init__()
		self.path = f"{self.parent}/data/settings.json"
		
		if not os.path.exists(f"{self.parent}/data"):
			os.mkdir(f"{self.parent}/data")
		
		if not os.path.exists(self.path):
			empty = {}
			empty["Username"] = ""
			empty["ReplaysDir"] = ""
			empty["UploaderState"] = False
			empty["LastModifiedDate"] = int(time())
			with open(self.path, "w") as file:
				json.dump(empty, file, indent=2)

	def update(self, setting, new_value):
		new_settings = self.get()
		new_settings[setting] = new_value
		self.set(new_settings)