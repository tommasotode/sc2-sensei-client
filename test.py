import py.uploader as up
import src.py.files as files

if __name__ == "__main__":
	set_handle = files.Settings()
	replays_path = set_handle.get()["ReplaysDir"]
	uploader = up.Uploader(replays_path)
	uploader.start_debug()