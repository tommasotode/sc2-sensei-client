import os
import ctypes as c

def auto_upload(replays_path):
	data_path = f"{os.getcwd()}/data/date.dat".encode()
	core = c.CDLL(f"{os.getcwd()}/lib/core.so")
	replays = replays_path.encode()

	if not core.check_files(c.c_char_p(data_path), c.c_char_p(replays)):
		print("Aborting")
		exit()

	class Replay(c.Structure):
		_fields_ = [
			("play_date", c.c_longlong),
			("upload_date", c.c_char_p),
			("name", c.c_char_p),
			("replay_path", c.c_char_p),
			("state", c.c_ubyte)
		]
	core.upload_all_new.restype = c.POINTER(Replay) * 10
	up = c.POINTER(Replay) * 10
	
	old_date = core.get_file_date(c.c_char_p(data_path))
	new_date = core.get_dir_date(c.c_char_p(replays))
	if new_date > old_date:
		print("Directory has been modified\n")
		up = core.upload_all_new(c.c_longlong(old_date), c.c_char_p(replays))

		core.wrt_file_date(c.c_char_p(data_path), c.c_longlong(new_date))

	elif new_date == old_date:
		print("Ok")
	else:
		print("There was an error.")