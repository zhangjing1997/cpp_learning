#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

#include "bstmap.h"

void checklookup(BstMap<std::string, int> & bm, std::string key, int value) {
  if (bm.lookup(key) != value) {
    exit(EXIT_FAILURE);
    ;
  }
}

int main() {
  BstMap<std::string, int> x;
  x.add("KI", 11);
  x.add("LJ", 23);
  x.add("JT", 0);
  x.remove("KI");
  checklookup(x, "LJ", 23);
  checklookup(x, "JT", 0);
  x.remove("LJ");
  checklookup(x, "JT", 0);
  x.remove("JT");
  if (x.getroot() != NULL) {
    exit(EXIT_FAILURE);
  }
  BstMap<std::string, int> x1(x);
  if (x1.getroot() != NULL) {
    exit(EXIT_FAILURE);
  }
  BstMap<std::string, int> bm;
  //normal test for add & lookup
  bm.add("Kyrie Irving", 11);
  bm.add("Lebron James", 23);
  bm.add("Jason Tatum", 0);
  bm.add("Lonzo Ball", 2);
  bm.add("Lance", 6);
  bm.add("Kevin Durrant", 35);
  bm.add("Stephen Curry", 30);
  bm.add("James Hardern", 13);
  checklookup(bm, "Kyrie Irving", 11);
  checklookup(bm, "Lebron James", 23);
  checklookup(bm, "Lonzo Ball", 2);
  checklookup(bm, "Jason Tatum", 0);
  checklookup(bm, "Lance", 6);
  checklookup(bm, "Kevin Durrant", 35);
  checklookup(bm, "Stephen Curry", 30);
  checklookup(bm, "James Hardern", 13);
  //test for lookup(non-exist key)
  try {
    bm.lookup("Ben Simons");
  }
  catch (const std::invalid_argument & ia) {
    std::cerr << "Invalid argument:" << ia.what() << std::endl;
  }
  //add an already-existed key with  different value
  bm.add("Lebron James", 6);
  checklookup(bm, "Lebron James", 6);
  //copy
  BstMap<std::string, int> bm1(bm);
  checklookup(bm1, "Kyrie Irving", 11);
  checklookup(bm1, "Lebron James", 6);
  checklookup(bm1, "Lonzo Ball", 2);
  checklookup(bm1, "Jason Tatum", 0);
  checklookup(bm1, "Lance", 6);
  checklookup(bm1, "Kevin Durrant", 35);
  checklookup(bm1, "Stephen Curry", 30);
  checklookup(bm1, "James Hardern", 13);
  //remove one node with 0 child
  bm.remove("Kevin Durrant");
  checklookup(bm, "Jason Tatum", 0);
  //remove one node with two children
  bm1.remove("Jason Tatum");
  checklookup(bm1, "James Hardern", 13);
  checklookup(bm1, "Kevin Durrant", 35);
  //remove one node with 1 child
  bm.remove("Lonzo Ball");
  //assignment
  BstMap<std::string, int> bm2;
  bm2 = bm;
  checklookup(bm2, "Jason Tatum", 0);
  checklookup(bm2, "Kyrie Irving", 11);
  checklookup(bm2, "Lebron James", 6);
  checklookup(bm2, "Lance", 6);
  checklookup(bm2, "James Hardern", 13);
  checklookup(bm2, "Stephen Curry", 30);
  std::cout << "test success!" << std::endl;
  return EXIT_SUCCESS;
}
