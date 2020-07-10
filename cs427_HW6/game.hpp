// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 6
// Name of file: game.hpp
#pragma once
#include "board.hpp"
#include "tools.hpp"

class Game {
  private:
    Board board;
    Player active = South;
    int numHouses;
    int numSeeds;

    bool oneMore = false;
    bool capture = false;
    int captured = 0;

    Pit* sow(int move);

  public:
    Game(int houses, int seeds) : board(houses, seeds) {
        numHouses = houses;
        numSeeds = seeds;
    }

    int getHouses() const { return numHouses; }
    int getSeeds() { return numSeeds; }
    bool getOneMore() { return oneMore; }
    bool getCapture() { return capture; }
    int getCaptured() { return captured; }

    ostream& printBoard(ostream& out) const;
    int doCapture(Pit* pit);
    int getStore(Player player);
    bool isValidMove(int move) const;
    void doMove(int move);
    Player activePlayer() const;
    Player oppositePlayer();
    bool isOver();
    void endGame();
    void switchPlayer();
    bool pitEmpty(Player pl, int p) const;
};
