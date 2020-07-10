// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 3
// Name of file: main.cpp

#include "simulator.hpp"
#include "tools.hpp"

void run(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    banner();
    run(argc, argv);
    bye();
    return 0;
}

void run(int argc, char* argv[]) {
    int num = atoi(argv[1]);

    cout << "Seed used:\n";
    if (argc > 2) {
        srand(atoi(argv[2]));
        cout << argv[2] << "\n";
    } else {
        srand(time(NULL));
        cout << time(NULL) << "\n";
    }

    cout << "Number of simulations:\n";
    cout << num << "\n";
    Simulator sim(num);
    sim.run();
    sim.print(cout);
}
