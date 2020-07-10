// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 3
// Name of file: dice.hpp

#include "tools.hpp"

class Dice {
  private:
    int k;
    int n;
    int* store;

    int RandomUniform(int n);

  public:
    Dice(int sides, int num) {
        k = sides;
        n = num;
        store = new int[num];
    }
    ~Dice() { delete[] store; }
    void print(ostream& out) const;
    void roll();
    int outcome();
};
