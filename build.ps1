#!/bin/bash

$data = "C:\repos\Sc2SenseiClient\.venv\Lib\site-packages\customtkinter;customtkinter"
$binary = "C:\repos\Sc2SenseiClient\bin\core.so;."

pyinstaller --noconfirm --onedir --add-data $data --add-binary $binary main.py 

$bin = ".\dist\main\bin\"
$img = ".\dist\main\img\"

xcopy .\bin\ $bin
xcopy .\img\ $img

$path = ".\dist\main\release"
$time = Get-Date
$release = Read-Host "[?] release version: "

Set-Content -Path $path -Value $release -Force
Add-Content -Path $path -Value $time -NoNewLine