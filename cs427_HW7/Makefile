# Author: Daniel Kim
# netid: dk763
# Course Number: CS 427
# Assignment Number: 7
# Name of file: Makefile

# Macro definitions
CXXFLAGS = -O1 -g -Wall -std=c++17
OBJ = main.o startup.o snapshot.o humanagent.o machineagent.o referee.o game.o board.o tools.o
TARGET = kalah

# Rules
all: $(TARGET)
$(TARGET): $(OBJ)
	$(CXX) -o $@ $(OBJ)
clean:
	rm -f $(OBJ) $(TARGET)

# Dependencies
main.o: main.cpp
startup.o: startup.cpp startup.hpp exception.hpp
snapshot.o: snapshot.cpp snapshot.hpp
humanagent.o: agent.hpp humanagent.cpp humanagent.hpp
machineagent.o: agent.hpp machineagent.cpp machineagent.hpp
referee.o: referee.cpp referee.hpp player.hpp pit.hpp
game.o: game.cpp game.hpp
board.o: board.cpp board.hpp
tools.o: tools.cpp tools.hpp
