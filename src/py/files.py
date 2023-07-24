import os
import json
from time import time

class FileHandle:
	def __init__(self):
		self.parent = f"{os.getenv('APPDATA')}/SC2Sensei"
		if not os.path.exists(self.parent):
			os.mkdir(self.parent)

	def get(self):
		result = None
		with open(self.path, "r") as file:
			try:
				result = json.load(file)
			except Exception as e:
				print(f"Can't load json file: {e}")
		
		return result
		
	def set(self, data):
		with open(self.path, "w") as file:
			json.dump(data, file, indent=2)