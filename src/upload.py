import os
import ctypes as c
import json

def auto_upload(replays_path):
	core = c.CDLL(f"{os.getcwd()}/bin/core.so")
	
	data_path = f"{os.getcwd()}/data/date.dat".encode()
	replays = replays_path.encode()

	if not core.check_files(c.c_char_p(data_path), c.c_char_p(replays)):
		print("Aborting")
		exit()


	old_date = core.get_file_date(c.c_char_p(data_path))
	new_date = core.get_dir_date(c.c_char_p(replays))
	
	core.upload_all_new.restype = c.c_char_p

	if new_date > old_date:
		print("Directory has been modified\n")
		arr = core.upload_all_new(c.c_longlong(old_date), c.c_char_p(replays))
		new = json.loads(arr)
		core.wrt_file_date(c.c_char_p(data_path), c.c_longlong(new_date))

	elif new_date == old_date:
		print("Ok")
	else:
		print("There was an error.")