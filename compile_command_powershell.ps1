#!/bin/bash

pyinstaller --noconfirm --onedir --windowed --add-data "d:/git_repositories/sc2senseiclient/.venv/lib/site-packages/customtkinter;customtkinter/" --add-binary "D:/Git_Repositories/Sc2SenseiClient/bin/core.so;." main.py 

echo "------ Build Completed ----------"
xcopy .\img\ .\dist\main\img\
xcopy .\bin\ .\dist\main\bin\
echo "------ Images and Libraries Moved to dist\main ----------"
echo "------ FINISH ------------"
