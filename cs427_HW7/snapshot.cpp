// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 7
// Name of file: snapshot.cpp

#include "snapshot.hpp"

Snapshot::Snapshot(vector<int> seed, Player player) {
    seeds = seed;
    activePlayer = player;
}

ostream& Snapshot::print(ostream& out) const {
    if (activePlayer == South) {
        out << "N ";
    } else {
        out << "S ";
    }

    for (auto i = seeds.begin(); i != seeds.end(); i++) {
        out << *i;
    }

    return out;
}

istream& Snapshot::read(istream& in) {
    string player;
    in >> player;
    int x;

    if (player == "N") {
        activePlayer = North;
    } else {
        activePlayer = South;
    }
    vector<int> seeds = {};

    do {
        in >> x;
        if (!in.fail())
            seeds.push_back(x); // got good data
    } while (in.good());
    if (!in.eof())
        throw Fatal("I/O error or bad data");
    return in;
}