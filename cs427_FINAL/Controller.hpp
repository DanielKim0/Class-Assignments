// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: takehome
// Name of file: Controller.hpp

#include "Exceptions.hpp"
#include "Inventory.hpp"

class Controller {
public:
  void go(string in, string out);
  void writeInven(Inventory inv, string out);
};