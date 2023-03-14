import json
import os

class Settings:
	def __init__(self):
		self.path = f"{os.getcwd()}/data/settings.json"

	def get(self):
		with open(self.path, "r") as file:
			content = file.read()
		try:
			return json.loads(content)
		except Exception as e:
			print(f"Corrupted file: {e}")
			return None
	
	def set(self, settings):
		with open(self.path, "w") as file:
			file.write(json.dumps(settings))

	def update(self, setting, new_value):
		new_settings = self.get()
		new_settings[setting] = new_value
		self.set(new_settings)


class Logs:
	def __init__(self):
		self.path = f"{os.getcwd()}/logs/logs.json"

	# I can use only what I need for the logs, since I get the parsed json
	# I have to make a preset json with only the structure, and I add to an array
	# only the replay objects that I need.

	def add_replays(self, log):
		new_log = self.get()
		for replay in log["Replays"]:
			new_log["Replays"].append(replay)
		with open(self.path, "w") as file:
			json.dump(new_log, file, indent=2)

	def get(self):
		with open(self.path, "r") as file:
			content = file.read()
		try:
			new_log = json.loads(content)
			return new_log
		except Exception as e:
			print(f"Corrupted file: {e}")
			return None