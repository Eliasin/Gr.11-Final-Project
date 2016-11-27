CC = gcc
SRC = $(wildcard src/*.cpp)
OBJS = $(addprefix build/, $(notdir $(SRC:.cpp=.o)))
INCLUDE_PATHS = -Iinclude
LIBRARY_PATHS = -Llib
LINKER_FLAGS = -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -lstdc++
COMPILER_FLAGS = -std=c++14 -m32
OUTPUT = bin/Summative.exe

all:
	$(CC) $(SRC) $(INCLUDE_PATHS) $(LINKER_FLAGS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) -o $(OUTPUT)
