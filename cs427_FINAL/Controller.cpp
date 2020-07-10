// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: takehome
// Name of file: Controller.cpp

#include "Controller.hpp"
#include "Exceptions.hpp"
using namespace std;
#include <fstream>
#include <string>
#include <vector>

void Controller::go(string in, string out) {
  Inventory o = Inventory(in);
  Inventory n = Inventory();
  n.setParts(o.repairParts());
  writeInven(n, out);
}

void Controller::writeInven(Inventory inv, string out) { inv.printParts(out); }