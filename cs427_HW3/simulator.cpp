// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 3
// Name of file: simulator.cpp

#include "simulator.hpp"
#include "tools.hpp"

void Simulator::run() {
    for (int i = 0; i < total; i++) {
        rounds++;
        Craps game;
        game.play();
        if (game.status() == 2) {
            won++;
        }
        rolls += game.get_rolls();
    }
}

void Simulator::print(ostream& out) const {
    out << "Wins/Games:\n";
    out << won << "/" << rounds << "\n";
    out << "Rolls per Round:\n";
    out << (float)rolls / (float)rounds << "\n";
}
