# Author: Daniel Kim
# netid: dk763
# Course Number: CS 427
# Assignment Number: 3

# Macro definitions
CXXFLAGS = -O1 -g -Wall -std=c++17
OBJ = main.cpp simulator.o craps.o dice.o tools.o
TARGET = craps

# Rules
all: $(TARGET)
$(TARGET): $(OBJ)
	$(CXX) -o $@ $(OBJ)
clean:
	rm -f $(OBJ) $(TARGET)

# Dependencies
simulator.o: simulator.cpp simulator.hpp
craps.o: craps.cpp craps.hpp
dice.o: dice.cpp dice.hpp
tools.o: tools.cpp tools.hpp
