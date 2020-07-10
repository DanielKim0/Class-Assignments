// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 7
// Name of file: pit.hpp

#pragma once
#include "player.hpp"
#include "tools.hpp"

class Pit {
  private:
    Pit* next;
    Pit* opposite;
    int contents;
    int number;
    string label;
    Player owner;
    bool store;

  public:
    Pit(Player player, int num, string lab, int cont, Pit* ne, Pit* opp) {
        owner = player;
        number = num;
        label = lab;
        contents = cont;
        store = (number == 0);
        opposite = opp;
        next = ne;
    }
    Pit() {}
    void scoop() { contents = 0; }
    void drop() { contents++; }
    void add(int num) { contents = contents + num; }
    bool empty() { return (contents == 0); }
    ostream& print(ostream& out) {
        if (contents < 10) {
            out << "[ " << contents << "]";
            return out;
        } else {
            out << "[" << contents << "]";
            return out;
        }
    }

    int sow() {
        int temp = contents;
        scoop();
        return temp;
    }

    Pit* getNext() { return next; }
    Pit* getOpp() { return opposite; }
    bool validCap(Player player) {
        return getContents() == 1 && !opposite->empty() && owner == player;
    }
    int getContents() { return contents; }

    bool equals(Pit* pit) {
        return (number == pit->number && owner == pit->owner);
    }

    void setContents(int cont) { contents = cont; } // TODO: change later!
};

inline ostream& operator<<(std::ostream& out, Pit& pit) {
    pit.print(out);
    return out;
}
