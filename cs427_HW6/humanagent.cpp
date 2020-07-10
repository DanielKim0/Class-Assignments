// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 6
// Name of file: humanagent.cpp
#include "humanagent.hpp"
#include <cctype>

int HumanAgent::chooseMove(Player pl) {
    bool valid_move = false;
    int move;
    bool first = true;

    while (!valid_move) {
        if (!first) {
            cout << "Invalid move! Please try again.\n\n"
                 << "----------\n";
        }

        string input;
        cout << "Please enter a pit number for " << pl << " (q to quit): \n";
        cin >> input;
        if (input == "q") {
            cout << "Quitting game at user's request\n";
            cout << "Game over -- goodbye!\n";
            return -1;
        } else {
            try {
                move = stoi(input);
                valid_move = game.isValidMove(move);
            } catch (const std::invalid_argument& ia) {
                valid_move = false;
            }
            first = false;
        }
    }
    return move;
}
