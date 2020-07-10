// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 5
// Name of file: main.cpp

#include "referee.hpp"
#include "tools.hpp"

void run(int argc, char* argv[]);
void play(int size, int seeds);

int main(int argc, char* argv[]) {
    banner();
    run(argc, argv);
    bye();
    return 0;
}

void run(int argc, char* argv[]) {
    int size = atoi(argv[1]);
    int seeds = atoi(argv[2]);
    play(size, seeds);
}

void play(int size, int seeds) {
    Referee ref = Referee(size, seeds);
    ref.playGame();
}
