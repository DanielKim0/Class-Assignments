// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: 2

#include "datapack.hpp"
#include "comparer.hpp"

// Gathers the data by calling the read and sort functions in the DataPack, making sure to print the data as specified.
void Comparer::gather() {
    cout << "\nRead data:\n";
    cout << "\nDataPack 1\n";
    original.read();
    cout << "DataPack 2\n";
    compare.read();
    cout << "\nUnsorted data:";
    cout << "\nDataPack 1\n";
    original.print(cout);
    cout << "\nDataPack 2\n";
    compare.print(cout);
    cout << "\nBeginning to sort.";
    original.sort();
    compare.sort();
    cout << "\nSorted results:\n";
    cout << "\nDataPack 1\n";
    original.print(cout);
    cout << "\nDataPack 2\n";
    compare.print(cout);
}

// Merges the two DataPack arrays by performing a merge, accessing the data in the DataPacks by calling the next method.
DataPack Comparer::merge() {
    cout << "\nBeginning to merge.\n";
    DataPack result;
    BT original_num = original.next();
    BT compare_num = compare.next();
    bool match = false;
    BT matched;
    while (original.loc() < original.size() && compare.loc() < compare.size()) {
        if (match && original_num == matched) {
            original_num = original.next();
        } else if (match && compare_num == matched) {
            compare_num = compare.next();
        } else {
            if (original_num < compare_num) {
                original_num = original.next();
            } else if (original_num > compare_num) {
                compare_num = compare.next();
            } else {
                match = true;
                matched = original_num;
                result.append(original_num);
                original_num = original.next();
                compare_num = compare.next();
            }
        }
    }
    cout << "\nMerged results:\n";
    result.print(cout);
    return result;
}
