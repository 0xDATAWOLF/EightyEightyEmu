# EightyEightyEmu (The world's most original emulator!)

A wonderfully inglorious attempt at emulating space invaders on the 8080 microprocesser. You can find more information on [Emulator101](http://emulator101.com/) on the how and the why. This project is written entirely in C as a means to master VIM and modern C (as I am a C-style C++ programmer, I'd like to move entirely to C). The design philosophy of this project is to get to point A to point B in the most obvious way possible. 

## Emulator API Design

As mentioned in the introduction, this emulator is built in C. I won't be providing any documentation for the API, but I will do my best to document the source code as needed if anyone is interested in studying how I got to where I was.

### Windows & Linux Compatability

The file `main.c` contains two entry points for both Windows and Linux operating systems. If you are compiling to either of these systems and you aren't using the include `CMakeLists.txt` as your project builder, than make sure link the appropriate libraries. Otherwise, use CMake (it will save you a lot of headache).

### Platform Abstractions

Both Windows and Linux will contain their respective platform abstractions such that each code which governs the emulator doesn't interact with with the platform it is running on. Please reference `main.c` for more information.

### Includes

This library will require third party libraries for graphics and GUI controls. (OpenGL and IMGUI) These will be set up in CMake accordingly.

### ROMs

This project requires the `spaceinvaders.*` ROM files to be placed in the `rom/` directory. It is your responsibility to source this ROM legally. Otherwise, this emulator is functionally useless without it.