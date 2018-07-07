OBJS = main.cpp

# specifies compiler
CC = g++

INCLUDE_PATHS = -IC:\SDL2_MingGW_x86\include\SDL2
LIBRARY_PATHS = -LC:\SDL2_MingGW_x86\lib

# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -w -Wl,-subsystem,windows

# libraries that are linked against
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2

# name of exe
OBJ_NAME = a

all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
