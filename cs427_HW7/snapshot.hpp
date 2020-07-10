// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 7
// Name of file: snapshot.hpp

#pragma once
#include "player.hpp"
#include <vector>

class Game;
class Snapshot {
  private:
    Player activePlayer;
    vector<int> seeds;
    friend class Game;

  public:
    Snapshot(vector<int> seed, Player player);
    Player getActive() const { return activePlayer; }
    vector<int> getSeeds() const { return seeds; }
    ostream& print(ostream& out) const;
    istream& read(istream& in);
};
