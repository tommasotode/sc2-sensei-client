#!/bin/bash

$data = "C:\repos\Sc2SenseiClient\.venv\Lib\site-packages\customtkinter;customtkinter"
$binary = "C:\repos\Sc2SenseiClient\bin\core.so;."

pyinstaller --noconfirm --onedir --add-data $data --add-binary $binary main.py 

Write-Output "------ Build Completed ------"
xcopy .\bin\ .\dist\main\bin\
xcopy .\img\ .\dist\main\img\
Write-Output "------ Images and Libraries Moved to dist\main ------"
Write-Output "------ FINISH ------"