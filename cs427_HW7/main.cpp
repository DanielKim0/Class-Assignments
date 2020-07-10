// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 7
// Name of file: main.cpp

#include "startup.hpp"
#include "tools.hpp"

void run(int argc, char* argv[]);
void play(int size, int seeds, bool s, bool n);
bool agent(char* str);

int main(int argc, char* argv[]) {
    banner();
    Startup s = Startup(argc, argv);
    s.go();
    bye();
    return 0;
}
