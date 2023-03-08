import pickle

class Data:
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