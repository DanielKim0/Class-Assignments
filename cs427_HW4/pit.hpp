// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 4
// Name of file: pit.hpp

#include "player.hpp"
#include "tools.hpp"

class Pit {
  private:
    Pit* next;
    Pit* opposite;
    int contents;
    int number;
    string label;
    Player owner;
    bool store;

  public:
    Pit(Player player, int num, string lab, int cont, Pit* ne, Pit* opp) {
        owner = player;
        number = num;
        label = lab;
        contents = cont;
        store = (number == 0);
        opposite = opp;
        next = ne;
    }
    Pit() {}
    void scoop() { contents = 0; }
    void drop() { contents++; }
    bool empty() { return (contents == 0); }
    void dump(ostream& out) { out << "Contents: " << contents << "\n"; }
    ostream& print(ostream& out) {
        string nextlabel = next->label;
        string opplabel;

        if (opposite == nullptr) {
            opplabel = "None";
        } else {
            opplabel = opposite->label;
        }

        return out << "Owner: " << owner << "\nNumber: " << number
                   << "\nLabel: " << label << "\nNext: " << nextlabel
                   << "\nOpposite: " << opplabel << "\nContents: " << contents
                   << "\n\n";
    }
};

inline ostream& operator<<(std::ostream& out, Pit& pit) {
    pit.print(out);
    return out;
}
