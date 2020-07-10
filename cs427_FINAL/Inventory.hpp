// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: takehome
// Name of file: Inventory.hpp

using namespace std;
#include "CompareByName.hpp"
#include "Exceptions.hpp"
#include <algorithm>
#include <iomanip>
#include <vector>

class Inventory {
private:
  vector<Part> parts;
  Part combineParts(Part a, Part b);
  Part mergeParts(Part p, vector<Part> *part);

public:
  Inventory(){};
  Inventory(string in);
  void sortParts(vector<Part> part);
  void setParts(vector<Part> part) { parts = part; }
  void placePart(Part p) { parts.push_back(p); }
  void printParts(string out);
  vector<Part> repairParts();
};