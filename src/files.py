import json

class Settings:
	def __init__(self, file_path):
		self.file_path = file_path
		self.settings = self.get()

	def get(self):
		with open(self.file_path, "rb") as file:
			content = file.read()
		try:
			return json.loads(content)
		except Exception as e:
			print(f"Corrupted file: {e}")
			return None
	
	def set(self, settings):
		with open(self.file_path, "wb") as file:
			file.write(json.dumps(settings))

class Logs:
	def __init__(self, file_path):
		self.file_path = file_path

	def write_json(self, json):
		with open(self.file_path, "a") as file:
			file.write(json)