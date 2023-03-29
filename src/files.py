import json
import os

class FileHandle:
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
		self.path = f"{os.getcwd()}/data/settings.json"

	def update(self, setting, new_value):
		new_settings = self.get()
		new_settings[setting] = new_value
		self.set(new_settings)

class Logs(FileHandle):
	def __init__(self):
		self.path = f"{os.getcwd()}/logs/logs.json"

	def add_replays(self, log_string):
		old_log = self.get()
		new_log = json.loads(log_string)
		for replay in new_log["Replays"]:
			old_log["Replays"].append(replay)
		with open(self.path, "w") as file:
			json.dump(old_log, file, indent=2)

	def get_last_replays(self, quantity=3):
		logs = self.get()
		result = []
		if quantity > 5 or quantity < 1:
			print("[ERROR] Invalid number of replays (min. 1 max. 5)")
			return None
		
		if quantity > len(logs["Replays"]):
			print(f"[WARNING] Can only get {len(logs['Replays'])} replays")
			for i in range(1, len(logs["Replays"])):
				result.append(logs["Replays"][-i])
		
		for i in range(1, quantity):
			result.append(logs["Replays"][-i])

		return result