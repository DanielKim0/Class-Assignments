// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 5
// Name of file: game.hpp

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

    int getHouses() { return numHouses; }
    int getSeeds() { return numSeeds; }
    bool getOneMore() { return oneMore; }
    bool getCapture() { return capture; }
    int getCaptured() { return captured; }

    ostream& printBoard(ostream& out);
    int doCapture(Pit* pit);
    int getStore(Player player);
    bool isValidMove(int move);
    void doMove(int move);
    Player activePlayer();
    Player oppositePlayer();
    bool isOver();
    void endGame();
    void switchPlayer();
};
