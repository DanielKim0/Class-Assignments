// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 7
// Name of file: referee.cpp

#include "referee.hpp"
using namespace std;

Snapshot createSnap(Game g);
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

void Referee::readCommand(string command, istream& in) {
    cout << "Command " << command << " pressed!\n";
    if (command == "q") {
        cout << "Quitting game at user's request\n";
        cout << "Game over -- goodbye!\n";
        exit(0);
    } else if (command == "z") {
        cout << "Undoing...\n";
        undo_op();
        game.printBoard(cout);
    } else if (command == "y") {
        cout << "Redoing...\n";
        redo_op();
        game.printBoard(cout);
    } else if (command == "s") {
        string filename;
        cin >> filename;
        dosave(filename);
    } else {
        cout << "Invalid command! Please try again.\n";
    }
    cout << "\n";
}

int Referee::chooseMove() {
    bool valid_move = false;
    int move;
    while (!valid_move) {
        try {
            if (game.activePlayer() == South) {
                move = south->chooseMove(South);
            } else {
                move = north->chooseMove(North);
            }
            valid_move = game.isValidMove(move);
        } catch (IllegalMoveError& err) {
            cout << "Illegal Move Error: ";
            err.printError(cout) << endl;
            cout << "\n";
        } catch (ReadError& err) {
            readCommand(err.getCommand(), err.getStream());
        }
    }
    return move;
}

void Referee::doMove() {
    int move;
    cout << "--------------------------------------------------------------\n"
         << game.activePlayer() << "'s turn:\n";
    game.printBoard(cout);
    cout << "\n";

    move = chooseMove();

    cout << game.activePlayer() << " playing move " << move << "\n";
    undo.push_back(createSnap());
    redo.clear();
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

void Referee::dosave(string filename) {
    cout << "Attempting to save current state to file " << filename << ":\n";
    ofstream fin(filename);
    if (!fin) {
        cout << "File failed to open.\n";
    } else {
        fin << game.getHouses() << " ";
        if (sou) {
            fin << "machine ";
        } else {
            fin << "human ";
        }

        if (nor) {
            fin << "machine\n";
        } else {
            fin << "human\n";
        }

        if (game.activePlayer() == South) {
            fin << "S";
        } else {
            fin << "N";
        }

        vector<int> seeds = game.gatherSeeds();
        for (unsigned int i = 0; i < seeds.size(); i++) {
            fin << " ";
            fin << seeds[i];
        }
    }
    cout << "File saved!\n"
}

void Referee::undo_op() {
    if (undo.empty()) {
        cout << "Unable to undo!\n";
    } else {
        redo.push_back(createSnap());
        game.restoreState(undo.back());
        undo.pop_back();
    }
}

void Referee::redo_op() {
    if (redo.empty()) {
        cout << "Unable to redo!\n";
    } else {
        undo.push_back(createSnap());
        game.restoreState(redo.back());
        redo.pop_back();
    }
}

Snapshot Referee::createSnap() {
    return Snapshot(game.gatherSeeds(), game.activePlayer());
}