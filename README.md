# chippy-emu
🎮 Interpreter for the Chip-8, written in C++.  
![chippy](./res/logo.png)

## About
The Chip-8 is an interpreted programming language, developed by Joseph Weisbecker. It was initially used on the COSMAC VIP and Telmac 1800 8-bit microcomputers in the mid-1970s. CHIP-8 programs are run on a CHIP-8 virtual machine. It was made to allow video games to be more easily programmed for these computers. From [wikipedia](https://en.wikipedia.org/wiki/CHIP-8).

## Usage 
The chip-8 interpreter is found in `chip8.cpp` and `chip8.hpp` which exposes methods for use in your own program. Otherwise `main.cpp` contains a sample emulator written with SDL. This can be built using:
```
make
make install
```

## Acknowledgements
Some resources I used were:  
[Cowgod's Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)  
[Lawrence Muller's Guide](http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/)
