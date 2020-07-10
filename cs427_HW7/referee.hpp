// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 7
// Name of file: referee.hpp

#include "exception.hpp"
#include "game.hpp"
#include "humanagent.hpp"
#include "machineagent.hpp"
#include "tools.hpp"
#include <string>
#include <vector>

class Referee {
  private:
    Game game;
    void doMove();
    Agent* south;
    Agent* north;
    bool early = false;
    bool sou;
    bool nor;

    vector<Snapshot> undo;
    vector<Snapshot> redo;
    void dosave(string filename);
    int chooseMove();
    void readCommand(string command, istream& in);
    Snapshot createSnap();

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
        sou = s;
        nor = n;
    }

    Referee(int houses, bool s, bool n, vector<int> vec) : game(houses) {
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
        sou = s;
        nor = n;
        game.setSeeds(vec);
    }

    void playGame();
    void announceEnd();

    void undo_op();
    void redo_op();
};
