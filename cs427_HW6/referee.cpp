// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 6
// Name of file: referee.cpp

#include "referee.hpp"

void Referee::playGame() {
    cout << "Welcome to Kalah!\n\n";
    while (!game.isOver() && !early) {
        doMove();
    }
    if (!early) {
        game.endGame();
        announceEnd();
    }
    delete south;
    delete north;
}

void Referee::doMove() {
    int move;
    cout << "----------\n" << game.activePlayer() << "'s turn:\n";
    game.printBoard(cout);
    cout << "\n";

    if (game.activePlayer() == South) {
        move = south->chooseMove(South);
    } else {
        move = north->chooseMove(North);
    }

    if (move == -1) {
        early = true;
        return;
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
    cout << "\n";
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
