// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 6
// Name of file: main.cpp

#include "referee.hpp"
#include "tools.hpp"

void run(int argc, char* argv[]);
void play(int size, int seeds, bool s, bool n);
bool agent(char* str);

class ArgError {
    string str;

  public:
    ArgError(string s) : str(s) {}
    ostream& printError(ostream& out) const { return out << str; }
};

int main(int argc, char* argv[]) {
    banner();
    run(argc, argv);
    bye();
    return 0;
}

void run(int argc, char* argv[]) {
    if (argc > 5) {
        throw ArgError("Too many arguments!");
    } else if (argc < 5) {
        throw ArgError("Too few arguments!");
    } else {
        int size = atoi(argv[1]);
        int seeds = atoi(argv[2]);
        if (size < 1 || seeds < 2) {
            throw ArgError("Malformed size/seed input!");
        }
        bool s = agent(argv[3]);
        bool n = agent(argv[4]);
        play(size, seeds, s, n);
    }
}

bool agent(char* str) {
    if (strcmp(str, "machine") == 0) {
        return true;
    } else if (strcmp(str, "human") == 0) {
        return false;
    } else {
        throw ArgError("Argument not equal to 'human' or 'machine'!");
    }
}

void play(int size, int seeds, bool s, bool n) {
    Referee ref = Referee(size, seeds, s, n);
    ref.playGame();
}
