# Author: Daniel Kim
# netid: dk763
# Course Number: CS 427
# Assignment Number: 2

# Macro definitions
CXXFLAGS = -O1 -g -Wall -std=c++17
OBJ = main.cpp comparer.o datapack.o tools.o
TARGET = comparer

# Rules
all: $(TARGET)
$(TARGET): $(OBJ)
	$(CXX) -o $@ $(OBJ)
clean:
	rm -f $(OBJ) $(TARGET)

# Dependencies
comparer.o: comparer.cpp comparer.hpp
datapack.o: datapack.cpp datapack.hpp
tools.o: tools.cpp tools.hpp
