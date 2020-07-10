// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 7
// Name of file: board.hpp
#pragma once
#include "pit.hpp"
#include "tools.hpp"
#include <vector>

class Board {
  private:
    int houses;
    int seeds;
    Pit* store;

    Player owner;
    Pit* opposite;
    Pit* next;
    string label;
    Pit temp;

  public:
    Board(int numHouses, int numSeeds);
    ~Board() { delete[] store; }

    ostream& print(ostream& out) const;
    Pit* getPit(int num, Player player) const;
    bool checkEmpty(Player player);
    void moveAll(Player player);
    bool pitEmpty(Player pl, int p) const;

    vector<int> gatherSeeds() const;
    void setContents(vector<int> vec);
};

inline ostream& operator<<(std::ostream& out, const Board& board) {
    board.print(out);
    return (out);
}
