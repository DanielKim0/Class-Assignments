// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: takehome
// Name of file: main.cpp

#include "Controller.hpp"

void run(int argc, char *argv[]);
void play(int size, int seeds, bool s, bool n);
bool agent(char *str);

int main(int argc, char *argv[]) {
  try {
    Controller c = Controller();
    c.go(argv[1], argv[2]);
  } catch (IOerror &err) {
    err.what(cerr);
    return 1;
  }
  return 0;
}
