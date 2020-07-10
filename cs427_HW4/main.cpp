// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 4
// Name of file: main.cpp

#include "board.hpp"
#include "tools.hpp"

void run(int argc, char* argv[]);
void full_test(int size, int seeds);

int main(int argc, char* argv[]) {
    banner();
    run(argc, argv);
    bye();
    return 0;
}

void run(int argc, char* argv[]) {
    int size = atoi(argv[1]);
    int seeds = atoi(argv[2]);
    full_test(size, seeds);
}

void full_test(int size, int seeds) {
    Player player = North;
    Board board = Board(size, seeds);
    Pit next = Pit(North, 2, "N2", seeds, nullptr, nullptr);
    Pit pit = Pit(North, 1, "N1", seeds, &next, nullptr);

    cout << "Printing Player\n";
    cout << player << "\n\n\n";

    cout << "Printing Pit\n";
    cout << pit << "\n";

    cout << "Printing Board\n";
    cout << board;

    cout << "Testing Drop\n";
    cout << "Before: \n";
    pit.dump(cout);
    pit.drop();
    cout << "After: \n";
    pit.dump(cout);

    cout << "\nTesting Empty\n";
    pit.dump(cout);
    cout << "Prints '1' if empty and '0' if not: " << pit.empty() << "\n";

    cout << "\nTesting Scoop\n";
    cout << "Before: \n";
    pit.dump(cout);
    pit.scoop();
    cout << "After: \n";
    pit.dump(cout);

    cout << "\nTesting Empty\n";
    pit.dump(cout);
    cout << "Prints '1' if empty and '0' if not: " << pit.empty() << "\n";
}
