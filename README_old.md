# Chess 3.33

Chess 3.33 is a **video game** based on the original chess, adapted for quick games and infinite **arcade/roguelike-style** progress.
The application is developed in **C++**, with **SFML** as the main dependency along with CMake and Make for compilation.

# Features

 - **Game modes**: Arcade, Versus, and Practice
 - Local **statistics** to compare records
 - **Board reduced** to 6x6 for fast gameplay
 - Classic piece **movements reduced or altered**
 - **4 original pieces** to use or face off against
 - **Guided tutorial** to learn the movements of each piece


# Installation
Go to the Release section, find the latest release, download the one specified for your system, extract it, find the exectuable "main" and run it. Easy

#  Building from source

## Cloning the Source
Get a copy of the latest source code from the releases section and extract it or clone the repository. To clone the repository, run:

    git clone https://github.com/Jaludex/Chess-3.33.git
   
   If cloning was successful, you will see that files and directories have appeared.

## Linux
Make sure you have **G++, CMake, and Makefile** installed, as well as the libraries “libxrandr-dev”, “libxcursor-dev”, "libopengl-dev", "libgl1-mesa-dev", "libudev-dev", "libfreetype-dev" "libvorbis-dev" "libflac-dev", and “libxi-dev”. You can install each of these with:

    sudo apt install build-essential cmake libxrandr-dev libxcursor-dev libopengl-dev libxi-dev libgl1-mesa-dev libudev-dev libfreetype-dev libvorbis-dev libflac-dev

Then, in the root directory of the project, run:

    cmake -B build

Wait for the initial build process. If it completed successfully, run:

    cmake --build build

This will compile **SFML 3.0.2** as well as the project. Wait for the process to finish. Go to "../Chess-3.33/build/bin", where you will find the “main” file to run.

## Windows
W.I.P.

# Dependencies
The project dependencies are:

 - [SFML](https://github.com/SFML/SFML) 3.0.2
 - [CMAKE](https://cmake.org/)
 - [MAKE](https://www.gnu.org/software/make/)
 - [JSON](https://github.com/nlohmann/json) for C++ by [nlohmann](https://github.com/nlohmann)

# Licensing
The project is licensed under the terms of the MIT License. You can find a copy of the license in the "LICENSE" file.

#  Credits
Chess 3.33 is developed as a final project for the “PR3” (Programming 3) course in the B2025 semester at the ULA (Universidad de los Andes).

Development Team:

 - Augusto Reinoza
 - Francisco Grimaldo
 - Jesús León

Made with <3 and hours of lost sleep.

## Special Thanks
Thanks to our teacher and our tutors in this course, they are incredible.
