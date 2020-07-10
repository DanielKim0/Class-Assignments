// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 6
// Name of file: board.cpp

#include "board.hpp"

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
