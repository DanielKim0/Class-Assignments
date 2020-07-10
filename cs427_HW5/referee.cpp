// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 5
// Name of file: referee.cpp

#include "referee.hpp"

void Referee::playGame() {
    cout << "Welcome to Kalah!\n\n";
    while (!game.isOver()) {
        doMove();
    }
    game.endGame();
    announceEnd();
}

void Referee::doMove() {
    bool valid_move = false;
    int move;
    cout << "----------\n" << game.activePlayer() << "'s turn:\n";
    game.printBoard(cout);

    while (!valid_move) {
        string input;
        cout << "Please enter a pit number for " << game.activePlayer()
             << " (q to quit): \n";
        cin >> input;
        if (input == "q") {
            cout << "Quitting game at user's request\n";
            cout << "Game over -- goodbye!\n";
            exit(0);
        } else {
            move = stoi(input);
            valid_move = game.isValidMove(move);
        }
    }
    cout << game.activePlayer() << " playing move " << move << "\n";
    game.doMove(move);

    if (game.getOneMore()) {
        cout << game.activePlayer() << " gets another turn\n";
    } else {
        if (game.getCapture()) {
            cout << "Captured " << game.getCaptured() << " seeds\n";
        }
        cout << "Turn is over\n";
        game.switchPlayer();
    }
}

void Referee::announceEnd() {
    int south = game.getStore(South);
    int north = game.getStore(North);
    cout << "South has " << south << " points.\n";
    cout << "North has " << north << " points.\n";
    if (south > north) {
        cout << "South wins!\n";
    } else if (south < north) {
        cout << "North wins!\n";
    } else {
        cout << "It's a tie!\n";
    }
}
