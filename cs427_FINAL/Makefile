# Author: Daniel Kim
# netid: dk763
# Course Number: CS 427
# Assignment Number: takehome
# Name of file: Makefile

# Macro definitions
CXXFLAGS = -O1 -g -Wall -std=c++17
OBJ = main.o Controller.o Inventory.o
TARGET = repair

# Rules
all: $(TARGET)
$(TARGET): $(OBJ)
	$(CXX) -o $@ $(OBJ)
clean:
	rm -f $(OBJ) $(TARGET)

# Dependencies
main.o: main.cpp
Controller.o: Controller.cpp Controller.hpp Exceptions.hpp
Inventory.o: Inventory.cpp Inventory.hpp Part.hpp