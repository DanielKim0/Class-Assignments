// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 6
// Name of file: machineagent.hpp
#include "agent.hpp"

class MachineAgent : public Agent {
  public:
    MachineAgent(const Game& g) : Agent(g) {}
    virtual int chooseMove(Player pl);
};
