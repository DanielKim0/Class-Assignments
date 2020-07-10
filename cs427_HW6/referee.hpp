// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 6
// Name of file: referee.hpp

#include "game.hpp"
#include "humanagent.hpp"
#include "machineagent.hpp"
#include "tools.hpp"

class Referee {
  private:
    Game game;
    void doMove();
    Agent* south;
    Agent* north;
    bool early = false;

  public:
    Referee(int houses, int seeds, bool s, bool n) : game(houses, seeds) {
        if (n) {
            north = new MachineAgent(game);
        } else {
            north = new HumanAgent(game);
        }

        if (s) {
            south = new MachineAgent(game);
        } else {
            south = new HumanAgent(game);
        }
    }

    void playGame();
    void announceEnd();
};
