// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: takehome
// Name of file: Exceptions.hpp

#include <iostream>
#pragma once
using namespace std;

class IOerror {
  string msg;

public:
  IOerror(string s) : msg(s) {}
  ostream &what(ostream &out) const { return out << msg; }
};