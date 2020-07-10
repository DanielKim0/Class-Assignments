// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 2

#include "comparer.hpp"
#include "datapack.hpp"
#include "tools.hpp"

void run();

int main() {
    banner();
    run();
    bye();
    return 0;
}

// Runs the main program by creating a Comparer object, and gathering and merging the data.
void run() {
    cout << "Construct comparer, construct DataPacks, read data";
    Comparer comp;
    comp.gather();
    cout << "Call merge function";
    DataPack data = comp.merge();
}
