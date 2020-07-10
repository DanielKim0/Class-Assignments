// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 7
// Name of file: board.cpp

#include "board.hpp"

Board::Board(int numHouses, int numSeeds) {
    houses = numHouses;
    seeds = numSeeds;
    store = new Pit[(houses * 2 + 2)]; // N0 to S?

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

ostream& Board::print(ostream& out) const {
    Pit pit;

    out << "       ";
    for (int a = 1; a < houses + 1; a++) {
        out << "    "
            << "N" << a;
    }
    out << "\n";

    out << "        ";
    for (int a = 1; a < houses + 1; a++) {
        pit = *getPit(a, North);
        out << "  " << pit;
    }
    out << "\n";

    out << "  ";
    pit = *getPit(0, North);
    out << "N0 " << pit;
    for (int a = 0; a < houses; a++) {
        out << "      ";
    }
    pit = *getPit(0, South);
    out << pit << " S0";
    out << "\n";

    out << "        ";
    for (int a = houses; a > 0; a--) {
        pit = *getPit(a, South);
        out << "  " << pit;
    }
    out << "\n";

    out << "       ";
    for (int a = houses; a > 0; a--) {
        out << "    "
            << "S" << a;
    }
    out << "\n";

    return out;
}

Pit* Board::getPit(int num, Player player) const {
    if (player == South) {
        return &store[num + houses + 1];
    } else {
        return &store[num];
    }
}

bool Board::checkEmpty(Player player) {
    for (int i = 0; i < houses; i++) {
        if (!getPit(i + 1, player)->empty()) {
            return false;
        }
    }
    return true;
}

void Board::moveAll(Player player) {
    int contents;
    for (int i = 0; i < houses; i++) {
        contents = getPit(i + 1, player)->sow();
        getPit(0, player)->add(contents);
    }
}

bool Board::pitEmpty(Player pl, int p) const { return getPit(p, pl)->empty(); }

vector<int> Board::gatherSeeds() const {
    vector<int> vect = {};
    for (int a = houses * 2 + 1; a >= 0; a--) {
        vect.push_back(store[a].getContents());
    }
    return vect;
}

void Board::setContents(vector<int> vec) {
    int j = 0;
    for (auto i = vec.rbegin(); i != vec.rend(); ++i) {
        store[j].setContents(*i);
        j++;
    }
}
