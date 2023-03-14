import customtkinter as ct
from PIL import Image as img
from os import getcwd
import src.files as files
import webbrowser

class App(ct.CTk):
	def __init__(self):
		super().__init__()
		
		self.set_handle = files.Settings()
		self.log_handle = files.Logs()

	def get_last_replays(self):
		logs = self.log_handle.get()
		if len(logs["Replays"]) == 0:
			return None
		last = []
		reverse = logs["Replays"][::-1]
		last.append(reverse[0])
		if len(logs["Replays"]) == 2:
			last.append(reverse[1])
		else:
			last.append(reverse[2])

		return last

	def get_input(self):
		dialog = ct.CTkInputDialog(text="Type in a number:", title="")
		dialog.overrideredirect(True)
		player_name = dialog.get_input()
		self.welcome.configure(text="Welcome, " + player_name)
		
		# Write player name in the file
		self.settings.update("Name", player_name)

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
		self.grid_rowconfigure((1,4), weight=10)
		self.grid_columnconfigure(1, weight=1)

		#	----	SIDEBAR		----	#
		self.sidebar = ct.CTkFrame(self, corner_radius=0)
		self.sidebar.grid(row=0, column=0, rowspan=5, sticky="nsew")
		self.sidebar.grid_rowconfigure(1, weight=1)
		self.sidebar.grid_rowconfigure((2,5), weight=2)

		self.logo = ct.CTkImage(img.open(f"{getcwd()}/img/logo.png"), size=(40, 40))
		self.main_title = ct.CTkLabel(self.sidebar, text=" Sc2Sensei", image=self.logo, compound="left",
				font=ct.CTkFont(size=22, weight="bold"))
		self.main_title.grid(row=0, column=0, padx=20, pady=35)

		self.home = ct.CTkImage(img.open(f"{getcwd()}/img/home.png"), size=(20,20))
		self.home_button = ct.CTkButton(self.sidebar, image=self.home, text="Home", fg_color="transparent", 
				command=self.get_input, hover=False)
		self.home_button.grid(row=1, column=0)
		
		self.inputb = ct.CTkButton(self, text="Open CTkInputDialog", command=self.get_input)
		self.inputb.grid(row=2, column=0, padx=20, pady=(10, 10))

		#	----	UPPERBAR	----	#
		self.upperbar = ct.CTkFrame(self, corner_radius=0, height=90)
		self.upperbar.grid(row=0, column=1, columnspan=5, sticky="nsew")
		self.upperbar.grid_rowconfigure(0, weight=1)
		self.upperbar.grid_columnconfigure(0, weight=1)

		self.welcome = ct.CTkLabel(self.upperbar, height=90, text="ciao")
		self.welcome.grid(row=0, column=0)

		#	----	REPLAYS FRAME	----	#
		#	Here will be displayed last 3 uploaded replays, which will
		#	be clickable and will take to the link of the analyzed replay
		self.tick = ct.CTkImage(img.open(f"{getcwd()}/img/done.png"), size=(20,20))
		self.replays_list = {}
		
		for i in range(3):

			replay = ct.CTkButton(self, text=f"prova{i}", fg_color="transparent", image=self.tick, 
				command=lambda: self.open_replay("https://sc2sensei.top/"), hover=False)
			

			replay.grid(row=i+1, column=1)
			
			# TODO: Add links to the logging, so I can get them with get_last_replays() and put them in open_replay() 


		# self.replay_info = ct.CTkButton(self, image=self.tick, text="Replay", fg_color="transparent", 
		# 		command=self.get_input, hover=False)
		# self.replay_info.grid(row=2, column=1)