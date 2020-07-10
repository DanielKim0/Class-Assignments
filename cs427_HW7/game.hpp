// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 7
// Name of file: game.hpp

#pragma once
#include "board.hpp"
#include "snapshot.hpp"
#include "tools.hpp"
#include <vector>

class Game {
  private:
    Board board;
    Player active = South;
    int numHouses;

    bool oneMore = false;
    bool capture = false;
    int captured = 0;

    Pit* sow(int move);

  public:
    Game(int houses, int seeds) : board(houses, seeds) { numHouses = houses; }
    Game(int houses) : board(houses, 0) { numHouses = houses; }

    int getHouses() const { return numHouses; }
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

    vector<int> gatherSeeds() const;
    void setSeeds(vector<int> vec);
    const Snapshot saveState() const;
    void restoreState(const Snapshot& ss);
};
