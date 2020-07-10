# Author: Daniel Kim
# netid: dk763
# Course Number: CS 427
# Assignment Number: 4
# Name of file: Makefile

# Macro definitions
CXXFLAGS = -O1 -g -Wall -std=c++17
OBJ = main.cpp player.hpp pit.hpp board.hpp tools.o
TARGET = testKalah

# Rules
all: $(TARGET)
$(TARGET): $(OBJ)
	$(CXX) -o $@ $(OBJ)
clean:
	rm -f $(OBJ) $(TARGET)

# Dependencies
tools.o: tools.cpp tools.hpp
