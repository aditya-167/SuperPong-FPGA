# SuperPong-FPGA

## Description
Course Project (ECE 532) - Classiscal Pong Game on Xillinx FPGA that uses a camera to control the position of the players' racquets.

# Authors
Aditya Srichandan
Haoran Wei
Harsimrat Singh Wadhawan
Yashraj Singh Dhillon

# Videos
[Click Here](./docs/README.md)

# Repository Overview

## Brief Overview
Software stored in the sw directory.
Hardware stored in the hw directory.
Image Processing project stored in the img directory.

## Structure
assets - Static images.

game_source_pc - SDL based Pong game source for PC.

hw - HDMI system hardware project.

img - Image processing system hardware project.

repo - Directory used by Vivado for caching and databasing.

src - Main HDMI system and game source code.

.gitignore - Contains various ignored files to stop this repository from being populated with large synthesized caches.

# Requirements

	1. Vivado 2016.4
	2. Xilinx SDK 2016.4

# Optional Requirements

## SDL Game Simulation
1. MinGW 7.3.0 (Minimalist GNU for Windows 7.3.0 i686-posix-dwarf-rev0) [Click Here to Download](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/6.4.0/threads-posix/dwarf/i686-6.4.0-release-posix-dwarf-rt_v5-rev0.7z/download)

# How to build the SDL game
1. Clone this repository to a suitable location.
2. CD into the game_source_pc directory of this repository.
3. Open a terminal in the game_source_pc directory.
4. In the terminal, execute the following command:
 ```gcc -Isrc/include -Lsrc/lib -o main main.c -lmingw32 -lSDL2main -lSDL2```
 5. An application called **main.exe** will be generated which can be executed to play the game.

# How to setup the Vivado project (Hardware)

	Open the hw/HDMI.xpr file in Vivado 2016.4 for modifying the hardware project.

# How to setup the Xilinx SDK project (Software)

1. Clone this repository to a suitable location on your PC.
2. Open the Xilinx SDK.
3. Create a new workspace at a suitable location on your PC.
4. Click on File > Import > Existing Projects into Workspace.
![Fig. 1](/assets/generalproj.bmp)
5. Enter the location of the **sw** directory in the root directory textbox.
![Fig. 1](/assets/root0dir.bmp)
6. Click Finish.

## Running the project on the board.
1. Click on Xilinx Tools > Program FPGA.
2. Choose **hdmi_wrapper_hw_platform_0** for the hardware platform.
![Fig. 1](/assets/prog.bmp)
3. Click Program.
4. Run the program by creating a debug configuraation by clicking on the green play button.
5. Connect to the Nexys Video board using a terminal.
6. Enter '1' into the terminal to launch the game.
