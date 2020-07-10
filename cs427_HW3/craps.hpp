// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 3
// Name of file: craps.hpp

#include "dice.hpp"
#include "tools.hpp"

class Craps {
  private:
    int state;
    int rolls;
    bool point_determined;
    int point;

    int getRoll();
    void firstRoll();
    void otherRoll();
    void oneRoll();

  public:
    Craps() {
        state = 0;
        rolls = 0;
        point_determined = false;
        point = 0;
    }
    void print(ostream& out) const;
    int get_rolls() const { return rolls; }
    void play();
    int status() const { return state; }
};
