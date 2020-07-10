# Author: Daniel Kim
# netid: dk763
# Course Number: CS 427
# Assignment Number: 1

# Macro definitions
CXXFLAGS = -O1 -g -Wall -std=c++17
OBJ = aboutme.o tools.o
TARGET = aboutme

# Rules
all: $(TARGET)
$(TARGET): $(OBJ)
	$(CXX) -o $@ $(OBJ)
clean:
	rm -f $(OBJ) $(TARGET)

# Dependencies
aboutme.o: aboutme.cpp aboutme.hpp
tools.o: tools.cpp tools.hpp
