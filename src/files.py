import json
import os

class FileHandle:
	def get(self):
		with open(self.path, "r") as file:
			content = file.read()
		try:
			return json.loads(content)
		except Exception as e:
			print(f"Corrupted file: {e}")
		
		return None
		
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

	def add_replays(self, log):
		new_log = self.get()
		for replay in log["Replays"]:
			new_log["Replays"].append(replay)
		with open(self.path, "w") as file:
			json.dump(new_log, file, indent=2)

	def get_last_replays(self, quantity=3):
		logs = self.get()
		result = []
		if quantity > 5 or quantity < 1:
			print("Invalid number of replays [VALID = 1-5]")
			return None
		elif quantity > len(logs["Replays"]):
			print("Not enought replays in the logs")
		
		for i in range(quantity):
			# how to get the elements independently of the size?
			result.append(logs["Replays"][-i])

		