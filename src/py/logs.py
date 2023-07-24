import os
import json

from src.py.files import FileHandle

class Logs(FileHandle):
	def __init__(self):
		super().__init__()
		self.path = f"{self.parent}/logs/logs.json"
		
		if not os.path.exists(f"{self.parent}/logs"):
			os.mkdir(f"{self.parent}/logs")
		
		if not os.path.exists(self.path):
			empty = {}
			empty["Replays"] = [] 
			with open(self.path, "w") as file:
				json.dump(empty, file, indent=2)

	def add_replays(self, log):
		new_log = self.get()
		for replay in log["Replays"]:
			new_log["Replays"].append(replay)
		
		with open(self.path, "w") as file:
			json.dump(new_log, file, indent=2)

	def get_last_replays(self, quantity=3):
		logs = self.get()
		length = len(logs["Replays"])
		
		if quantity > 5 or quantity < 1:
			print(f"[!] Can't get {quantity} replays from the logs (min. 1 max. 5)")
			return None
		if length == 0:
			print("No replays in the logs")
			return None
		
		replays = []
		if quantity > length:
			print(f"Can only get {length} replays")
			for i in range(1, length+1):
				replays.append(logs["Replays"][-i])
		else:
			for i in range(1, quantity+1):
				replays.append(logs["Replays"][-i])
		
		return replays