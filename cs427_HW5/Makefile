# Author: Daniel Kim
# netid: dk763
# Course Number: CS 427
# Assignment Number: 5
# Name of file: Makefile

# Macro definitions
CXXFLAGS = -O1 -g -Wall -std=c++17
OBJ = main.o referee.o game.o board.o tools.o
TARGET = kalah

# Rules
all: $(TARGET)
$(TARGET): $(OBJ)
	$(CXX) -o $@ $(OBJ)
clean:
	rm -f $(OBJ) $(TARGET)

# Dependencies
main.o: main.cpp
referee.o: referee.cpp referee.hpp player.hpp pit.hpp 
game.o: game.cpp game.hpp
board.o: board.cpp board.hpp
tools.o: tools.cpp tools.hpp
