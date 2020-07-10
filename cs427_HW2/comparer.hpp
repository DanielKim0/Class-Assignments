// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 2

#include "tools.hpp"
#include "datapack.hpp"

typedef float BT;
#define LENGTH 20

// Class definition for Comparer, which does not need a function-based definition.
class Comparer {
    private:
        DataPack original;
        DataPack compare;
    public:
        void print(ostream& out) const;
        void gather();
        DataPack merge();
};
