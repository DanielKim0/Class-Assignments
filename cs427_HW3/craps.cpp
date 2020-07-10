// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 3
// Name of file: craps.cpp

#include "craps.hpp"

int Craps::getRoll() {
    int roll;
    Dice dice(6, 2);
    dice.roll();
    roll = dice.outcome();
    return roll;
}

void Craps::firstRoll() {
    int first = getRoll();
    if (first == 7 || first == 11) {
        state = 2;
    } else if (first == 2 || first == 3 || first == 12) {
        state = 1;
    } else {
        point_determined = true;
        point = first;
    }
}

void Craps::otherRoll() {
    int roll = getRoll();
    if (roll == point) {
        state = 2;
    } else if (roll == 7) {
        state = 1;
    }
}

void Craps::oneRoll() {
    if (rolls == 0) {
        firstRoll();
    } else {
        otherRoll();
    }
    rolls++;
}

void Craps::play() {
    while (state == 0) {
        oneRoll();
    }
}

void Craps::print(ostream& out) const {
    out << "Current state: (0=continuing, 1=lost, 2=won):\n";
    out << state << "\n";
    out << "Number of rolls:\n";
    out << rolls << "\n";
    out << "Point determined:\n";
    out << point_determined << "\n";
    out << "Point:\n";
    out << point << "\n\n";
}
