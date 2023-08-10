import os
import json
import ctypes as ct

class Core:
	def __init__(self):
		self.lib = ct.CDLL(f"{os.getcwd()}/bin/core.so")
		self.lib.check_files.restype = ct.c_bool
		self.lib.check_username.restype = ct.c_bool
		self.lib.get_dir_date.restype = ct.c_longlong
		self.lib.upload_all_new.restype = ct.c_char_p
		self.lib.upload_last_n.restype = ct.c_char_p
		self.lib.debug_mode.restype = ct.c_bool

	def check_files(self, replays_path:str) -> bool:
		replays_path = ct.c_char_p(replays_path.encode())
		result = self.lib.check_files(replays_path)

		return bool(result)

	def check_username(self, username:str) -> bool:
		username = ct.c_char_p(username.encode())
		code = self.lib.check_username(username)
		
		return bool(code)

	def get_dir_date(self, replays_path:str) -> int:
		replays_path = ct.c_char_p(replays_path.encode())
		result = self.lib.get_dir_date(replays_path)

		return int(result)
	
	def upload_all_new(self, old_date:int, replays_path:str, username:str) -> dict:
		old_date = ct.c_longlong(old_date)
		replays_path = ct.c_char_p(replays_path.encode())
		username = ct.c_char_p(username.encode())
		result = self.lib.upload_all_new(old_date, replays_path, username)
		
		return json.loads(result)
	
	def upload_last_n(self, n:int, replays_path:str, username:str) -> dict:
		n = ct.c_ushort(n)
		replays_path = ct.c_char_p(replays_path.encode())
		username = ct.c_char_p(username.encode())
		result = self.lib.upload_last_n(n, replays_path, username)
		
		return json.loads(result)
	
	def debug_mode(self) -> bool:
		result = self.lib.debug_mode()

		return bool(result)