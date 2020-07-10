// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 7
// Name of file: machineagent.cpp

#include "machineagent.hpp"

int MachineAgent::chooseMove(Player pl) {
    for (int i = 1; i < game.getHouses(); i++) {
        if (!game.pitEmpty(pl, i)) {
            return i;
        }
    }
    return -1;
}
