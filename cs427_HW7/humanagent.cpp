// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 7
// Name of file: humanagent.cpp

#include "humanagent.hpp"
#include "exception.hpp"
#include <cctype>

int HumanAgent::chooseMove(Player pl) {
    int move;
    string input;
    cout << "Please enter a pit number for " << pl
         << " or a command letter z y s q: \n";
    cin >> input;
    try {
        move = stoi(input);
    } catch (const std::invalid_argument& ia) {
        throw(ReadError("Non-number read!", input, cin));
    }

    if (cin.fail()) {
        Fatal("Stream Input Error!");
    }

    return move;
}
