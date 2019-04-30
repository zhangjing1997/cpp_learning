#include <iostream>

#include "bstset.h"

void check(bool b, bool expected) {
  if (b != expected) {
    exit(EXIT_FAILURE);
  }
}

int main() {
  BstSet<int> bs;
  int seq[] = {7, 2, 9, 1, 5, 8, -1};
  for (int i = 0; seq[i] != -1; i++) {
    bs.add(seq[i]);
  }
  for (int i = 0; seq[i] != -1; i++) {
    check(bs.contains(seq[i]), true);
  }
  check(bs.contains(10), false);
  BstSet<int> bs1(bs);
  for (int i = 0; seq[i] != -1; i++) {
    bs1.remove(seq[i]);
    check(bs1.contains(seq[i]), false);
  }
  BstSet<int> bs2;
  bs2 = bs;
  for (int i = 0; seq[i] != -1; i++) {
    check(bs2.contains(seq[i]), true);
  }
  std::cout << "test success!" << std::endl;
  return EXIT_SUCCESS;
}
