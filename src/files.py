import json
import os
from time import time

class FileHandle:
	def __init__(self):
		self.roaming = os.getenv("APPDATA")
		self.parent = f"{self.roaming}/SC2Sensei"
		if not os.path.exists(self.parent):
			os.mkdir(self.parent)

	def get(self):
		result = None
		with open(self.path, "r") as file:
			try:
				result = json.load(file)
			except Exception as e:
				print(f"Corrupted json file: {e}")
		
		return result
		
	def set(self, data):
		with open(self.path, "w") as file:
			json.dump(data, file, indent=2)

class Settings(FileHandle):
	def __init__(self):
		super().__init__()
		self.path = f"{self.parent}/data/settings.json"
		if not os.path.exists(f"{self.parent}/data"):
			os.mkdir(f"{self.parent}/data")
		if not os.path.exists(self.path):
			sample = {}
			sample["Username"] = ""
			sample["ReplaysDir"] = ""
			sample["UploaderState"] = True
			sample["LastModifiedDate"] = int(time())
			with open(self.path, "w") as file:
				json.dump(sample, file, indent=2)

	def update(self, setting, new_value):
		new_settings = self.get()
		new_settings[setting] = new_value
		self.set(new_settings)

class Logs(FileHandle):
	def __init__(self):
		super().__init__()
		self.path = f"{self.parent}/logs/logs.json"
		if not os.path.exists(f"{self.parent}/logs"):
			os.mkdir(f"{self.parent}/logs")
		if not os.path.exists(self.path):
			sample = {}
			sample["Replays"] = [] 
			with open(self.path, "w") as file:
				json.dump(sample, file, indent=2)

	def add_replays(self, log_string):
		old_log = self.get()
		new_log = json.loads(log_string)
		for replay in new_log["Replays"]:
			old_log["Replays"].append(replay)
		with open(self.path, "w") as file:
			json.dump(old_log, file, indent=2)

	def get_last_replays(self, quantity=3):
		logs = self.get()
		length = len(logs["Replays"])
		result = []
		if quantity > 5 or quantity < 1:
			print("[!] Invalid number of replays (min. 1 max. 5)")
			return None
		if length == 0:
			print("No replays in the logs")
			return None
		if quantity > length:
			print(f"Can only get {length} replays")
			for i in range(1, length+1):
				result.append(logs["Replays"][-i])
		else:
			for i in range(1, quantity+1):
				result.append(logs["Replays"][-i])
		
		return result
	