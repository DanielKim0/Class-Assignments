// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 6
// Name of file: agent.hpp

#pragma once
#include "game.hpp"
#include "player.hpp"
#include "tools.hpp"

class Agent {
  protected:
    const Game& game;

  public:
    Agent(const Game& g) : game(g) {}
    virtual ~Agent() {}

    virtual int chooseMove(Player pl) = 0;
};
