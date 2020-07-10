// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 7
// Name of file: startup.cpp

#include "startup.hpp"

void Startup::go() {
    if (argc < 2) {
        throw ArgError("Too few arguments!");
    }
    if (strcmp(argv[1], "-f") == 0) {
        restoreGame();
    } else {
        newGame();
    }
}

bool agent(string str) {
    if (str == "machine") {
        return true;
    } else if (str == "human") {
        return false;
    } else {
        Fatal("Argument not equal to 'human' or 'machine'!");
        return false;
    }
}

void Startup::newGame() {
    if (argc > 5) {
        throw ArgError("Too many arguments!");
    } else if (argc < 5) {
        throw ArgError("Too few arguments!");
    } else {
        int size = atoi(argv[1]);
        int seeds = atoi(argv[2]);
        if (size < 1 || seeds < 2) {
            throw ArgError("Malformed size/seed input!");
        }
        bool s = agent(string(argv[3]));
        bool n = agent(string(argv[4]));
        Referee ref = Referee(size, seeds, s, n);
        ref.playGame();
    }
}

void Startup::restoreGame() {
    if (argc > 3) {
        throw ArgError("Too many arguments!");
    } else if (argc < 3) {
        throw ArgError("Too few arguments!");
    } else {
        ifstream infile(argv[2]);
        if (!infile) {
            Fatal("File failed to open!");
        }
        readFile(infile);
    }
}

void Startup::readFile(ifstream& in) {
    int houses;
    in >> houses;
    if (in.fail()) {
        Fatal("Invalid file format!");
    }
    if (houses < 0) {
        Fatal("Invalid file format!");
    }

    string so;
    in >> so;
    bool s = agent(so);

    string no;
    in >> no;
    bool n = agent(no);

    string turn;
    in >> turn;
    if (in.fail()) {
        Fatal("Invalid file format!");
    }

    int x;
    int num = 0;
    vector<int> seeds = {};
    do {
        in >> x;
        if (!in.fail()) {
            num++;
            if (num > 2 * houses + 2)
                throw Fatal("Invalid file format!");
            seeds.push_back(x);
        }
    } while (in.good());
    if (!in.eof())
        throw Fatal("Invalid file format!");

    cout << num;
    if (num != 2 * houses + 2)
        throw Fatal("Invalid file format!");

    Referee ref = Referee(houses, s, n, seeds);
    ref.playGame();
}