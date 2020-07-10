// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 7
// Name of file: exception.hpp

#include <iostream>
#pragma once
using namespace std;

class ArgError {
    string str;

  public:
    ArgError(string s) : str(s) {}
    ostream& printError(ostream& out) const { return out << str; }
};

class ReadError {
    string msg;
    string command;
    istream& in;

  public:
    ReadError(string s, string c, istream& i) : msg(s), command(c), in(i) {}
    ostream& printError(ostream& out) const { return out << msg; }
    string getCommand() { return command; }
    istream& getStream() { return in; }
};

class IllegalMoveError {
    string msg;

  public:
    IllegalMoveError(string s) : msg(s) {}
    ostream& printError(ostream& out) const { return out << msg; }
};