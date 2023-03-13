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

	def new(self, new_log):
		with open(self.path, "a") as file:
			json.dump(new_log, file, indent=2)
			file.write("\n")

	def get(self):
		with open(self.path, "r") as file:
			content = file.read()
		try:
			new_log = json.loads(content)
			return new_log
		except Exception as e:
			print(f"Corrupted file: {e}")
			return None
		
