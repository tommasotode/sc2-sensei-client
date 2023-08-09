# Sc2SenseiClient
A windows client that automatically uploads replays to [SC2Sensei](https://sc2sensei.top/), as soon as you finish playing.

## GUI
The user interface is meant to be simple and to just have what is strictly needed.
The front-end of the client is completely autonomous from the actual auto-uploader, and it isn't necessary to keep the GUI open for it to work.
The uploader will be displayed on the system tray, and will be automatically started on windows boot, so you won't need to think about it.

## Updates
Updates are completely automatized, as soon as a new release gets uploaded to GitHub, the client will automatically update without any interruption needed.
If you want to see the release notes, you can still get to the [latest release page](https://github.com/TommasoTodescato/SC2SenseiClient/releases/latest/) to see it.

## Download
On every release, the client will be already built and compiled for you, with a shortcut to the launcher executable, but if you prefer you can [build it from source](#build).

## Collaboration
The whole project is under MIT License, so you can feel free to collaborate if you would like to add some features or to change something.
### Build
The uploader and the updater are made with C, using [libcurl](https://curl.se/libcurl/) and [miniz](https://github.com/richgel999/miniz).
The GUI is made with Python 3.11.0 using [CustomTkinter](https://github.com/TomSchimansky/CustomTkinter)

To build the project from source, you will need:
  - [MSYS2](https://www.msys2.org/) (mingw-w64) with the following packages:
      - gcc
      - base-devel
      - development
      - mingw-w64-x86_64-toolchain
      - mingw-w64-x86_64-curl
  - GNU make
  - Python 3.11.0

You won't need to think about dependencies (unless you want to add some).
In that case, you can add DLLs to /bin, and link headers in the makefile.
