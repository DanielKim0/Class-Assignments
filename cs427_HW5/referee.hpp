// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 5
// Name of file: referee.hpp

#include "game.hpp"
#include "tools.hpp"

class Referee {
  private:
    Game game;
    void doMove();

  public:
    Referee(int houses, int seeds) : game(houses, seeds) {}

    void playGame();
    void announceEnd();
};
