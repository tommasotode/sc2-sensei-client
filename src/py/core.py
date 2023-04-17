import ctypes as ct
import os

class Core:
	def __init__(self):
		self.lib = ct.CDLL(f"{os.getcwd()}/bin/core.so")
		self.lib.upload_all_new.restype = ct.c_char_p
		self.lib.get_dir_date.restype = ct.c_longlong
		self.lib.check_files.restype = ct.c_bool
		self.lib.debug_mode.restype = ct.c_bool
	
	def upload_all_new(self, old_date:int, replays_path:str, username:str) -> str:
		result = self.lib.upload_all_new(ct.c_longlong(old_date),
			ct.c_char_p(replays_path.encode()), ct.c_char_p(username.encode()))
		
		return result
	
	def get_dir_date(self, replays_path:str) -> int:
		result = self.lib.get_dir_date(ct.c_char_p(replays_path.encode()))

		return result
	
	def check_files(self, replays_path:str) -> bool:
		result = self.lib.check_files(ct.c_char_p(replays_path.encode()))

		return result
	
	def debug_mode(self) -> bool:
		result = self.lib.debug_mode()

		return result