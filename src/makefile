OBJS = main.cpp chip8.cpp
CC = g++

INCLUDE_PATHS = -IC:\mingw_dev_lib\include\SDL2

LIBRARY_PATHS = -LC:\mingw_dev_lib\lib

# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
# COMPILER_FLAGS = -w -Wl,-subsystem,windows

LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2

OBJ_NAME = chip8.exe

all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)