// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 4
// Name of file: board.hpp

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
        store = new Pit[(houses * 2 + 2)];

        for (int a = 0; a < houses * 2 + 2; a++) {
            int number = a % (houses + 1);

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

            if (a == houses * 2 + 1) {
                next = &store[0];
            } else {
                next = &store[a + 1];
            }

            if (owner == South) {
                label = "S" + std::to_string(number);
            } else {
                label = "N" + std::to_string(number);
            }

            temp = Pit(owner, number, label, seeds, next, opposite);
            store[a] = temp;
        }
    }
    ~Board() { delete[] store; }
    ostream& print(ostream& out) {
        for (int a = houses * 2 + 1; a > 0; a--) {
            out << store[a];
        }
        out << "\n";
        return out;
    }
};

inline ostream& operator<<(std::ostream& out, Board& board) {
    board.print(out);
    return (out);
}
