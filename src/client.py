import customtkinter as ct
from PIL import Image as img
from os import getcwd
import src.files as files
import webbrowser
import tkinter as tk
from tkinter import filedialog

class App(ct.CTk):
	def __init__(self):
		super().__init__()
		
		self.set_handle = files.Settings()
		self.log_handle = files.Logs()
		self.settings = self.set_handle.get()

		print(self.set_handle.get()["Username"])

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

	def open_replay(self, link):
		webbrowser.open(link, new=1)

	

class AppGUI(App):
	def __init__(self):
		super().__init__()

		ct.set_appearance_mode("dark")
		ct.set_default_color_theme("dark-blue")
		self.geometry("960x540")
		self.resizable(False, False)
		self.title("Sc2SenseiClient")
		self.iconbitmap(default=f"{getcwd()}/img/logo.ico")
		
		self.grid_rowconfigure(0, weight=1)
		self.grid_rowconfigure((1,3), weight=10)
		self.grid_columnconfigure(0, weight=1)
		self.grid_columnconfigure(1, weight=20)

		#	----	SIDEBAR		----	#
		self.sidebar = ct.CTkFrame(self, corner_radius=0)
		self.sidebar.grid(row=0, column=0, rowspan=5, sticky="nsew")
		self.sidebar.grid_rowconfigure(0, weight=1)
		self.sidebar.grid_rowconfigure((1,2), weight=1)
		self.sidebar.grid_rowconfigure(3, weight=2)

		self.logo = ct.CTkImage(img.open(f"{getcwd()}/img/logo.png"), size=(40, 40))
		self.main_title = ct.CTkLabel(self.sidebar, text=" Sc2Sensei", image=self.logo, compound="left",
				font=ct.CTkFont(size=28, weight="bold"))
		self.main_title.grid(row=0, column=0, padx=25, pady=10)

		self.user = ct.CTkImage(img.open(f"{getcwd()}/img/user.png"), size=(20,20))
		self.user_button = ct.CTkButton(self.sidebar, image=self.user, text="Username", fg_color="transparent", 
				command=self.get_username, hover=False, font=ct.CTkFont(size=14, weight="bold"))
		self.user_button.grid(row=1, column=0, pady=50)
		
		self.dir = ct.CTkImage(img.open(f"{getcwd()}/img/dir.png"), size=(20,20))
		self.replays_input = ct.CTkButton(self.sidebar, text="Replays", command=self.get_replays_dir,
				fg_color="transparent", image=self.dir, hover=False, font=ct.CTkFont(size=14, weight="bold"))
		self.replays_input.grid(row=2, column=0)

		#	----	UPPERBAR	----	#
		self.upperbar = ct.CTkFrame(self, corner_radius=0, height=30)
		self.upperbar.grid(row=0, column=1, columnspan=3, sticky="nsew")
		self.upperbar.grid_rowconfigure(0, weight=1)
		self.upperbar.grid_columnconfigure(0, weight=1)

		self.welcome = ct.CTkLabel(self.upperbar, height=90, text=f"Welcome, {self.settings['Username']}")
		self.welcome.grid(row=0, column=0)

		#	----	REPLAYS FRAME	----	#
		self.tick = ct.CTkImage(img.open(f"{getcwd()}/img/done.png"), size=(20,20))
		self.replays_list = {}
		
		for i in range(3):

			replay = ct.CTkButton(self, text=f"prova{i}", fg_color="transparent", image=self.tick, 
				command=lambda:self.open_replay("https://sc2sensei.top/"), hover=False)
			
			replay.grid(row=i+1, column=1)