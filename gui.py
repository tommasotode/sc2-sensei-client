import customtkinter as ct
from PIL import Image as img
from upload import auto_upload

class Client(ct.CTk):
	def __init__(self):
		super().__init__()
		ct.set_appearance_mode("dark")
		ct.set_default_color_theme("dark-blue")
		self.geometry("960x540")
		self.minsize(960, 540)
		self.title("Sc2SenseiClient")
		self.iconbitmap("logo.ico")
		
		self.grid_rowconfigure(0, weight=1)
		self.grid_columnconfigure(1, weight=1)

		self.logo = ct.CTkImage(img.open("logo.png"), size=(40, 40))

		self.navigation_frame = ct.CTkFrame(self, corner_radius=0)
		self.navigation_frame.grid(row=0, column=0, sticky="nsew")
		self.navigation_frame.grid_rowconfigure(4, weight=1)

		self.navigation_frame_label = ct.CTkLabel(self.navigation_frame, text=" Sc2Sensei",
					    image=self.logo, compound="left",
						font=ct.CTkFont(size=22, weight="bold"))
		self.navigation_frame_label.grid(row=0, column=0, padx=20, pady=20)


	def main(self):
		#TEMP
		replays_path = "C:/Users/tomma/sc2-sensei/upload_client/ReplayTest"
		auto_upload(replays_path)
		self.after(5000, self.main)