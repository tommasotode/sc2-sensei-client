import pickle

class PlayerData:
	def __init__(self, file_path):
		self.file_path = file_path
		self.settings = self.get()

	def get(self) -> dict:
		with open(self.file_path, "rb") as file:
			content = file.read()
		try:
			return pickle.loads(content)
		except Exception as e:
			print(f"Corrupted file: {e}")
			return None
	
	def set(self, settings: dict):
		with open(self.file_path, "wb") as file:
			file.write(pickle.dumps(settings))

class Logs:
	def __init__(self, file_path):
		self.file_path = file_path

	def write_json(self, json):
		with open(self.file_path, "a") as file:
			file.write(json)