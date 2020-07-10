// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 7
// Name of file: startup.hpp

#include "exception.hpp"
#include "game.hpp"
#include "referee.hpp"
#include <vector>

class Startup {
  private:
    int argc;
    char** argv;
    void newGame();
    void restoreGame();
    void readFile(ifstream& in);

  public:
    Startup(int ac, char* av[]) {
        argc = ac;
        argv = av;
    }
    void go();
};
