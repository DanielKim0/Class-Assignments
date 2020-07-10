// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: takehome
// Name of file: CompareByName.hpp

#include "Part.hpp"

class CompareByName {
public:
  bool operator()(Part a, Part b) {
    // returns True if a's name is first
    int comp = a.name.compare(b.name);
    return comp < 0;
  }
};