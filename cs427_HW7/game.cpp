// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 7
// Name of file: game.cpp

#include "game.hpp"
#include "exception.hpp"

bool Game::isValidMove(int move) const {
    if (move == 0) {
        throw IllegalMoveError("Cannot pick store as a move.");
        return false;
    } else if ((move > numHouses) || (move < 0)) {
        throw IllegalMoveError("Invalid pit number.");
        return false;
    } else if (board.getPit(move, active)->empty()) {
        throw IllegalMoveError("Pit is empty.");
        return false;
    } else {
        return true;
    }
}

void Game::doMove(int move) {
    oneMore = false;
    capture = false;
    captured = 0;

    Pit* pit = sow(move);
    if (pit->equals(board.getPit(0, active))) {
        oneMore = true;

    } else if (pit->validCap(active)) {
        capture = true;
        captured = doCapture(pit);
    }
}

Pit* Game::sow(int move) {
    Pit* pit = board.getPit(move, active);
    int contents = pit->sow();
    while (contents > 0) {
        pit = pit->getNext();
        if (!(pit->equals(board.getPit(0, oppositePlayer())))) {
            pit->drop();
            contents--;
        }
    }
    return pit;
}

int Game::doCapture(Pit* pit) {
    pit->sow();
    int captured = pit->getOpp()->sow() + 1;
    board.getPit(0, active)->add(captured);
    return captured;
}

Player Game::activePlayer() const { return active; }

Player Game::oppositePlayer() {
    if (active == North) {
        return South;
    } else {
        return North;
    }
}

bool Game::isOver() {
    if (board.checkEmpty(South)) {
        active = North;
        return true;
    } else if (board.checkEmpty(North)) {
        active = South;
        return true;
    } else {
        return false;
    }
}

void Game::endGame() { board.moveAll(active); }

int Game::getStore(Player player) {
    return board.getPit(0, player)->getContents();
}

void Game::switchPlayer() { active = oppositePlayer(); }

ostream& Game::printBoard(std::ostream& out) const {
    out << board;
    return (out);
}

bool Game::pitEmpty(Player pl, int p) const { return board.pitEmpty(pl, p); }

vector<int> Game::gatherSeeds() const { return board.gatherSeeds(); }

void Game::setSeeds(vector<int> vec) { board.setContents(vec); }

void Game::restoreState(const Snapshot& ss) {
    active = ss.getActive();
    board.setContents(ss.getSeeds());
}

const Snapshot Game::saveState() const {
    return Snapshot(gatherSeeds(), activePlayer());
}