// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 4
// Name of file: player.hpp

#include "tools.hpp"

enum Player { North, South };

inline ostream& operator<<(ostream& out, Player player) {
    switch (player) {
    case North:
        out << "North";
        break;
    case South:
        out << "South";
        break;
    }
    return out;
}
