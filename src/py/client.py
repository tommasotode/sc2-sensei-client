import customtkinter as ct
from PIL import Image as img
import os
import src.py.files as files
import webbrowser
from tkinter import filedialog
import functools


# TODO: Should I refactor the structure of the code?
# Maybe it would be better to use class functions and expand them for the GUI object,
# so I'm sure that I won't have scope problems
# Example: toggle_uploader only toggles state, and then i make a wrapper in the AppGUI class
# that sets the button according to the state


class App(ct.CTk):
	def __init__(self):
		super().__init__()
		self.set_handle = files.Settings()
		self.log_handle = files.Logs()
		self.settings = self.set_handle.get()
		self.uploader_state = self.set_handle.get()["UploaderState"]

	def get_replays_dir(self):
		folder = filedialog.askdirectory()
		self.set_handle.update("ReplaysDir", folder)
		return folder

	def get_username(self):
		dialog = ct.CTkInputDialog(text="Insert your username:", title="")
		dialog.overrideredirect(True)
		player_name = dialog.get_input()
		self.welcome.configure(text="Welcome, " + player_name)
		self.set_handle.update("Username", player_name)
		return player_name

	def set_last_replays(self, replays_list):
		if replays_list:
			for i, replay in enumerate(replays_list[::-1]):
				link = f"http://localhost:5000/replay_analysis?replay_id={replay['id']}"
				open_replay = functools.partial(webbrowser.open, url=link, new=2)
				tick = ct.CTkImage(img.open(f"{os.getcwd()}/img/done.png"), size=(30, 30))
				replay_btn = ct.CTkButton(self.replays_frame, text=replay["name"], fg_color="transparent", image=tick,
					command=open_replay, hover=False, width=400, height=70, font=ct.CTkFont(size=17))
				replay_btn.grid(row=i+1, pady=30)

	def toggle_uploader(self):
		# This function doesn't automatically start or stop the uploader
		# It has to be done outside (this one only interfaces the GUI)
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

class AppGUI(App):
	def __init__(self):
		super().__init__()

		ct.set_appearance_mode("dark")
		ct.set_default_color_theme("dark-blue")
		self.geometry("960x540")
		self.resizable(False, False)
		self.title("Sc2SenseiClient")
		self.iconbitmap(default=f"{os.getcwd()}/img/logo.ico")

		self.grid_rowconfigure(0, weight=2)
		self.grid_rowconfigure((1, 3), weight=10)
		self.grid_columnconfigure(0, weight=3)
		self.grid_columnconfigure(1, weight=20)

		# ---- SIDEBAR ---- #
		self.sidebar = ct.CTkFrame(self, corner_radius=0, width=200)
		self.sidebar.grid(row=0, column=0, rowspan=4, sticky="nsew")
		self.sidebar.grid_rowconfigure(0, weight=1)
		self.sidebar.grid_rowconfigure((1, 4), weight=4)

		self.logo_img = ct.CTkImage(img.open(f"{os.getcwd()}/img/logo.png"), size=(50, 50))
		self.main_title = ct.CTkLabel(self.sidebar, text=" Sc2Sensei", image=self.logo_img, compound="left",
			font=ct.CTkFont(size=29, weight="bold"))
		self.main_title.grid(row=0, column=0, padx=30)

		self.user_img = ct.CTkImage(img.open(f"{os.getcwd()}/img/user.png"), size=(25, 25))
		self.user_button = ct.CTkButton(self.sidebar, image=self.user_img, text="Username", fg_color="transparent",
			command=self.get_username, hover=False, font=ct.CTkFont(size=14, weight="bold"))
		self.user_button.grid(row=1, column=0)

		self.dir_img = ct.CTkImage(img.open(f"{os.getcwd()}/img/dir.png"), size=(25, 25))
		self.replays_input = ct.CTkButton(self.sidebar, text="Replays", command=self.get_replays_dir,
			fg_color="transparent", image=self.dir_img, hover=False, font=ct.CTkFont(size=14, weight="bold"))
		self.replays_input.grid(row=2, column=0)
		
		# ---- INFO FRAME ---- #
		self.info_frame = ct.CTkFrame(self.sidebar, corner_radius=0, fg_color="transparent")
		self.info_frame.grid(row=5, column=0)
		self.info_frame.grid_columnconfigure((0,1), weight=1)

		self.source_img = ct.CTkImage(img.open(f"{os.getcwd()}/img/code.png"), size=(20,20))
		self.source_btn = ct.CTkButton(self.info_frame, image=self.source_img, text="", fg_color="transparent",
			hover=False, command=self.open_parent_dir, height=40, width=50)
		self.source_btn.grid(row=0, column=0, padx=20, pady=(0,10))

		self.git_img = ct.CTkImage(img.open(f"{os.getcwd()}/img/git.png"), size=(20, 20))
		self.git_btn = ct.CTkButton(self.info_frame, image=self.git_img, text="", fg_color="transparent",
			hover=False, command=self.open_git_repo, height=40, width=50)
		self.git_btn.grid(row=0, column=1, padx=20, pady=(0,10))

		# ---- UPPERBAR	---- #
		self.upperbar = ct.CTkFrame(self, corner_radius=0, height=100)
		self.upperbar.grid(row=0, column=1, columnspan=4, sticky="nsew")
		self.upperbar.grid_rowconfigure(0, weight=1)
		self.upperbar.grid_columnconfigure(0, weight=16)
		self.upperbar.grid_columnconfigure(1, weight=8)
		self.upperbar.grid_columnconfigure(2, weight=1)


		# TODO: Find a way to center this
		self.welcome = ct.CTkLabel(self.upperbar, height=100, width=300, font=ct.CTkFont(size=18), 
			text=f"Welcome, {self.settings['Username']}")
		self.welcome.grid(row=0, column=1, padx=50)

		self.play_img = ct.CTkImage(img.open(f"{os.getcwd()}/img/play.png"), size=(20,20))
		self.pause_img = ct.CTkImage(img.open(f"{os.getcwd()}/img/pause.png"), size=(20,20))
		self.current_state_img = self.pause_img if self.uploader_state else self.play_img
		self.play_btn = ct.CTkButton(self.upperbar, text="", command=self.toggle_uploader, fg_color="transparent",
			image=self.current_state_img, hover=False, width=40)
		self.play_btn.grid(row=0, column=2)

		self.refresh_img = ct.CTkImage(img.open(f"{os.getcwd()}/img/refresh.png"), size=(20, 20))
		self.refresh_btn = ct.CTkButton(self.upperbar, text="", fg_color="transparent", image=self.refresh_img,
			hover=False, width=40, height=40, command=lambda:self.set_last_replays(self.log_handle.get_last_replays(3)))
		self.refresh_btn.grid(row=0, column=3, padx=(0, 10))

		# ---- REPLAYS FRAME ---- #
		self.replays_frame = ct.CTkFrame(self, fg_color="transparent")
		self.replays_frame.grid(row=1, column=1, sticky="ns")
		self.replays_frame.grid_rowconfigure(0, weight=1)
		self.replays_frame.grid_rowconfigure((1, 3), weight=20)
		self.set_last_replays(self.log_handle.get_last_replays(3))