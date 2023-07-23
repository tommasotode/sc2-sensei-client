import customtkinter as ct
import os
import src.py.files as files
import webbrowser
import functools
from src.py.core import Core
from PIL import Image as img
from tkinter import filedialog

class App(ct.CTk):
	def __init__(self):
		super().__init__()
		ct.set_appearance_mode("dark")
		ct.set_default_color_theme("dark-blue")
		self.geometry("960x540")
		self.resizable(False, False)
		self.title("Sc2SenseiClient")
		self.iconbitmap(default=f"{os.getcwd()}/img/logo.ico")
		
		self.logo_img = ct.CTkImage(img.open(f"{os.getcwd()}/img/logo.png"), size=(50, 50))
		self.user_img = ct.CTkImage(img.open(f"{os.getcwd()}/img/user.png"), size=(25, 25))
		self.dir_img = ct.CTkImage(img.open(f"{os.getcwd()}/img/dir.png"), size=(25, 25))
		self.source_img = ct.CTkImage(img.open(f"{os.getcwd()}/img/code.png"), size=(20,20))
		self.git_img = ct.CTkImage(img.open(f"{os.getcwd()}/img/git.png"), size=(20, 20))
		self.play_img = ct.CTkImage(img.open(f"{os.getcwd()}/img/play.png"), size=(20,20))
		self.pause_img = ct.CTkImage(img.open(f"{os.getcwd()}/img/pause.png"), size=(20,20))
		self.refresh_img = ct.CTkImage(img.open(f"{os.getcwd()}/img/refresh.png"), size=(20, 20))
		self.tick_img = ct.CTkImage(img.open(f"{os.getcwd()}/img/done.png"), size=(30, 30))

		self.core = Core()
		self.set_handle = files.Settings()
		self.log_handle = files.Logs()

		self.last_replays_n = 3
		self.uploader_state = self.set_handle.get()["UploaderState"]
		self.valid_username = False
		self.valid_directory = False

	def get_input(self, text):
		dialog = ct.CTkInputDialog(text=text, title="")
		dialog.overrideredirect(True)
		input = dialog.get_input()
		return input
	
	def set_replays_dir(self):
		folder = filedialog.askdirectory()
		self.valid_directory = self.core.check_files(folder)
		if self.valid_directory:
			self.set_handle.update("ReplaysDir", folder)

		return folder

	def set_username(self):
		player_name = self.get_input("What's your username?")
		self.valid_username = self.core.check_username(player_name)
		if self.valid_username:
			print("\nValid username!\n")
			self.welcome.configure(text="Welcome, " + player_name)
			self.set_handle.update("Username", player_name)
		else:
			print("\nUsername is NOT valid!\n")
			
		return player_name

	def set_last_replays(self, replays_list):
		if not replays_list:
			return None
		for i, replay in enumerate(replays_list[::-1]):
			link = f"http://localhost:5000/replay_analysis?replay_id={replay['id']}"
			open_replay = functools.partial(webbrowser.open, url=link, new=2)
			replay_btn = ct.CTkButton(self.replays_frame, text=replay["name"], fg_color="transparent", 
			    image=self.tick_img, command=open_replay, hover=False, width=400, height=70, font=ct.CTkFont(size=17))
			replay_btn.grid(row=i+1, pady=30)

	def refresh(self):
		replays_list = self.log_handle.get_last_replays(self.last_replays_n)
		self.set_last_replays(replays_list)

	def toggle_uploader(self):
		# This function doesn't automatically start or stop the uploader
		self.uploader_state = not self.uploader_state
		if self.uploader_state:
			self.play_btn.configure(image=self.pause_img)
		else:
			self.play_btn.configure(image=self.play_img)
		
		self.set_handle.update("UploaderState", self.uploader_state)

	def open_git_repo(self):
		webbrowser.open("https://github.com/TommasoTodescato/Sc2SenseiClient", 2)

	def open_parent_dir(self):
		os.startfile(f"{os.getenv('APPDATA')}\\SC2Sensei")