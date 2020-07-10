// Author: Daniel Kim
// netid: dk763
// Course Number: CS 427
// Assignment Number: takehome
// Name of file: Inventory.cpp

#include "Inventory.hpp"

Inventory::Inventory(string in) {
  string s;
  int i;
  double f;

  ifstream file(in);
  if (!file) {
    throw IOerror("Error opening file!");
  }

  do {
    file >> s >> i >> f;
    if (!file.fail())
      parts.push_back(Part(s, i, f));
  } while (file.good());
  if (!file.eof())
    throw IOerror("Wrong input format!");
}

Part Inventory::combineParts(Part a, Part b) {
  return Part(a.name, a.quantity + b.quantity, max(a.price, b.price));
}

Part Inventory::mergeParts(Part p, vector<Part> *part) {
  unsigned int i = 0;
  while (i < part->size()) {
    if (p.name == (*part)[i].name) {
      p = combineParts(p, (*part)[i]);
      part->erase(part->begin() + i);
    } else {
      i++;
    }
  }
  return p;
}

void Inventory::sortParts(vector<Part> part) {
  sort(parts.begin(), parts.end(), CompareByName());
}

vector<Part> Inventory::repairParts() {
  vector<Part> *temp = &parts;
  vector<Part> repaired;
  while (temp->size() > 0) {
    Part p = *temp->begin();
    temp->erase(temp->begin());
    repaired.push_back(mergeParts(p, temp));
  }
  return repaired;
}

void Inventory::printParts(string out) {
  ofstream file(out);
  if (!file) {
    throw IOerror("Could not open ostream!");
  }

  file << fixed << setprecision(2);

  for (unsigned i = 0; i < parts.size(); i++) {
    file << parts[i];
    file << "\n";
  }

  if (!file) {
    throw IOerror("Error with outputting to file!");
  }
}
