from src.py.client import App
import customtkinter as ct

# This class only has structural objects, 
# that get modified from the App class they inherit from

class Main(App):
	def __init__(self):
		super().__init__()
		self.grid_rowconfigure(0, weight=2)
		self.grid_rowconfigure((1, 3), weight=10)
		self.grid_columnconfigure(0, weight=3)
		self.grid_columnconfigure(1, weight=20)

		# ---- SIDEBAR ---- #
		self.sidebar = ct.CTkFrame(self, corner_radius=0, width=200)
		self.sidebar.grid(row=0, column=0, rowspan=4, sticky="nsew")
		self.sidebar.grid_rowconfigure(0, weight=1)
		self.sidebar.grid_rowconfigure((1, 4), weight=4)

		self.main_title = ct.CTkLabel(self.sidebar, text=" Sc2Sensei", image=self.logo_img, compound="left",
			font=ct.CTkFont(size=29, weight="bold"))
		self.main_title.grid(row=0, column=0, padx=30)

		self.user_button = ct.CTkButton(self.sidebar, image=self.user_img, text="Username", fg_color="transparent",
			command=self.set_username, hover=False, font=ct.CTkFont(size=14, weight="bold"))
		self.user_button.grid(row=1, column=0)

		self.replays_input = ct.CTkButton(self.sidebar, text="Replays", command=self.set_replays_dir,
			fg_color="transparent", image=self.dir_img, hover=False, font=ct.CTkFont(size=14, weight="bold"))
		self.replays_input.grid(row=2, column=0)
		
		# ---- INFO FRAME ---- #
		self.info_frame = ct.CTkFrame(self.sidebar, corner_radius=0, fg_color="transparent")
		self.info_frame.grid(row=5, column=0)
		self.info_frame.grid_columnconfigure((0,1), weight=1)

		self.source_btn = ct.CTkButton(self.info_frame, image=self.source_img, text="", fg_color="transparent",
			hover=False, command=self.open_parent_dir, height=40, width=50)
		self.source_btn.grid(row=0, column=0, padx=20, pady=(0,10))
		
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

		self.welcome = ct.CTkLabel(self.upperbar, height=100, width=300, font=ct.CTkFont(size=18), 
			text=f"Welcome, {self.set_handle.get()['Username']}")
		self.welcome.grid(row=0, column=1, padx=50)

		# This only has effect at the first opening
		self.current_state_img = self.pause_img if self.uploader_state else self.play_img
		self.play_btn = ct.CTkButton(self.upperbar, text="", command=self.toggle_uploader, fg_color="transparent",
			image=self.current_state_img, hover=False, width=40)
		self.play_btn.grid(row=0, column=2)

		self.refresh_btn = ct.CTkButton(self.upperbar, text="", fg_color="transparent", image=self.refresh_img,
			hover=False, width=40, height=40, command=self.refresh)
		self.refresh_btn.grid(row=0, column=3, padx=(0, 10))

		# ---- REPLAYS FRAME ---- #
		self.replays_frame = ct.CTkFrame(self, corner_radius=0, fg_color="transparent")
		self.replays_frame.grid(row=1, column=1, sticky="ns")
		self.replays_frame.grid_rowconfigure(0, weight=1)
		self.replays_frame.grid_rowconfigure((1, 3), weight=20)
		self.set_last_replays(self.log_handle.get_last_replays(self.last_replays_n))
