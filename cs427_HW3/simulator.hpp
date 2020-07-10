// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 3
// Name of file: simulator.hpp

#include "craps.hpp"
#include "tools.hpp"

class Simulator {
  private:
    int total;
    int rounds;
    int won;
    int rolls;

  public:
    Simulator(int tot) {
        total = tot;
        rounds = 0;
        won = 0;
        rolls = 0;
    }
    void print(ostream& out) const;
    void run();
};
