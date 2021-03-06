// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 6
// Name of file: board.hpp
#pragma once
#include "pit.hpp"
#include "tools.hpp"

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
    Board(int numHouses, int numSeeds) {
        houses = numHouses;
        seeds = numSeeds;
        store = new Pit[(houses * 2 + 2)]; // N0 to S6

        for (int a = 0; a < houses * 2 + 2; a++) {
            int number = a % (houses + 1);
            int seed = seeds;
            if (number == 0) {
                seed = 0;
            }

            if (a - houses > 0) {
                owner = South;
            } else {
                owner = North;
            }

            if (number == 0) {
                opposite = nullptr;
            } else {
                opposite = &store[houses * 2 + 2 - a];
            }

            if (a == 0) {
                next = &store[houses * 2 + 1];
            } else {
                next = &store[a - 1];
            }

            if (owner == South) {
                label = "S" + std::to_string(number);
            } else {
                label = "N" + std::to_string(number);
            }

            temp = Pit(owner, number, label, seed, next, opposite);
            store[a] = temp;
        }
    }
    ~Board() { delete[] store; }

    ostream& print(ostream& out) const;
    Pit* getPit(int num, Player player) const;
    bool checkEmpty(Player player);
    void moveAll(Player player);
    bool pitEmpty(Player pl, int p) const;
};

inline ostream& operator<<(std::ostream& out, const Board& board) {
    board.print(out);
    return (out);
}
