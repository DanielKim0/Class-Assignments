// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 1

#include "aboutme.hpp"
#include "../tools.hpp"
#include <time.h>

// Main function that runs banner, run, and bye.
int main() {
    banner();
    run();
    bye();
}

// Runs the main program, as specified in the homework document.
void run() {
    string firstname;
    string lastname;
    int birth_year;
    int curr_year;
    int difference;

    std::cout << "Please enter your first name: ";
    std::cin >> firstname;
    std::cout << "Please enter your last name: ";
    std::cin >> lastname;
    std::cout << "Please enter the year of your birth: ";
    birth_year = input_year();
    curr_year = get_current_year();
    difference = curr_year - birth_year;
    std::cout << firstname << " " << lastname << " becomes " << difference
              << " years old in " << curr_year << ".";
}

int input_year() {
    int year;
    std::cin >> year;
    if (!cin.good()) {
        fatal("You did not input an integer!");
    }
    return year;
}

// Gets the current year by using time() and localtime().
int get_current_year() {
    time_t now;
    tm* now_local;
    int year;

    now = time(nullptr);
    now_local = localtime(&now);
    year = now_local->tm_year + 1900;
    return year;
}
