// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 3
// Name of file: dice.cpp

#include "dice.hpp"

// Code from RandomUniform taken from the PSet pdf.
// A method to generate a random number uniformly over the range
// {0,...,n-1} using the library function random().
int Dice::RandomUniform(int n) {
    long int top = ((((RAND_MAX - n) + 1) / n) * n - 1) + n;
    long int r;
    do {
        r = random();
    } while (r > top);
    return (r % n);
}

void Dice::roll() {
    for (int i = 0; i < n; i++) {
        store[i] = RandomUniform(k) + 1;
    }
}

int Dice::outcome() {
    int out = 0;
    for (int i = 0; i < n; i++) {
        out = out + store[i];
    }
    return out;
}

void Dice::print(ostream& out) const {
    out << "Current Dice:\n";
    for (int k = 0; k < n; k++) {
        out << store[k] << "\n";
    }
    out << "Number of sides:\n";
    out << k << "\n";
    out << "Number of dice:\n";
    out << n << "\n\n";
}
