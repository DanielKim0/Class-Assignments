// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: takehome
// Name of file: Part.hpp

using namespace std;
#include <fstream>
#include <string>
#pragma once

class Inventory;
class CompareByName;
class Part {
  friend Inventory;
  friend CompareByName;

private:
  string name;
  int quantity;
  double price;

public:
  Part(string n, int q, double p) {
    name = n;
    quantity = q;
    price = p;
  }
  ostream &print(ostream &out) {
    out << name << " ";
    out << quantity << " ";
    out << price << " ";
    return out;
  }
};

inline ostream &operator<<(std::ostream &out, Part &part) {
  return part.print(out);
}
