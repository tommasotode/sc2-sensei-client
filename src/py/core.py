import ctypes as ct
import os
import json

ct.windll.kernel32.SetDllDirectoryW(None)

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
		result = self.lib.check_files(ct.c_char_p(replays_path.encode()))

		return bool(result)

	def check_username(self, username:str) -> bool:
		code = self.lib.check_username(ct.c_char_p(username.encode()))
		
		return bool(code)

	def get_dir_date(self, replays_path:str) -> int:
		result = self.lib.get_dir_date(ct.c_char_p(replays_path.encode()))

		return int(result)
	
	def upload_all_new(self, old_date:int, replays_path:str, username:str) -> dict:
		result = self.lib.upload_all_new(ct.c_longlong(old_date),
			ct.c_char_p(replays_path.encode()), ct.c_char_p(username.encode()))
		
		return json.loads(result)
	
	def upload_last_n(self, number:int, replays_path:str, username:str) -> dict:
		result = self.lib.upload_last_n(ct.c_ushort(number), 
			ct.c_char_p(replays_path.encode()), ct.c_char_p(username.encode()))
		
		return json.loads(result)
	
	def debug_mode(self) -> bool:
		result = self.lib.debug_mode()

		return bool(result)