//  ----------------------------------------------------------------------------
//  Header file for all DataPack programs.                          datapack.hpp
//  Created by Alice Fischer on Sunday August 23, 2009
// -----------------------------------------------------------------------------
#pragma once
#include "tools.hpp" // includes all the other headers you need.

typedef float BT; // Generic base type for data pack; BT must implement <=
#define LENGTH 20

class DataPack {
  private:
    // ------------------------------------------------------------------
    int place;
    int n;     // The current number of items in the array.
    int max;   // Allocation length of the array.
    BT* store; // For a dynamically allocated data array.

  public:
    // -------------------------------------------------------------------
    DataPack() {
        n = 0;
        place = 0;
        max = LENGTH;
        store = new BT[max];
        cout << "Store allocated.\n";
    }
    ~DataPack() {
        delete[] store;
        cout << "Store deallocated.\n";
    }

    // -------------------------------------------------------------------------
    void read();
    int size() const { return n; }
    int loc() const { return place; }
    void print(ostream& out) const;
    void sort();
    BT next();
    void append(BT number);
};
