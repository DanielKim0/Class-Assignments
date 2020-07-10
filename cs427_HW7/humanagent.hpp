// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 7
// Name of file: humanagent.hpp

#include "agent.hpp"

class HumanAgent : public Agent {
  public:
    HumanAgent(const Game& g) : Agent(g) {}
    virtual int chooseMove(Player pl);
};
