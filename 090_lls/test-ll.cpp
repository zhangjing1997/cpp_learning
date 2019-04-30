#include <assert.h>

#include <cstdlib>
#include <iostream>

#include "ll.h"

void decide(bool d) {
  if (d == false) {
    std::cout << "test failure!" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void testList(void) {
  //test constructor
  LinkedList<int> IL;
  bool d = (IL.getSize() == 0 && IL.head == NULL && IL.tail == NULL);
  decide(d);
  //test addFront
  IL.addFront(3);
  d = (IL.getSize() == 1 && IL[0] == 3);
  decide(d);
  IL.addFront(5);
  d = (IL.getSize() == 2 && IL[0] == 5 && IL[1] == 3);
  decide(d);
  //test addBack
  IL.addBack(2);
  d = (IL.getSize() == 3 && IL[0] == 5 && IL[1] == 3 && IL[2] == 2);
  decide(d);
  IL.addBack(7);
  d = (IL.getSize() == 4 && IL[0] == 5 && IL[1] == 3 && IL[2] == 2 && IL[3] == 7);
  decide(d);
  //test find
  bool d1 = (IL.find(5) == 0 && IL.find(2) == 2 && IL.find(7) == 3);
  bool d2 = (IL.find(10) != 1 && IL.find(10) != 2 && IL.find(10) != 3);
  decide(d1 & d2);
  //test copy constructor
  LinkedList<int> ILCopy(IL);
  d = (ILCopy.getSize() == IL.getSize());
  for (int i = 0; i < IL.getSize(); i++) {
    if (ILCopy[i] != IL[i]) {
      d = false;
    }
  }
  decide(d);
  //test destructor
  ILCopy.~LinkedList();
  d = (ILCopy.getSize() == 0 && ILCopy.head == NULL && ILCopy.tail == NULL);
  decide(d);
  //test assignment constructor
  ILCopy = IL;
  d = (ILCopy.getSize() == IL.getSize());
  for (int i = 0; i < IL.getSize(); i++) {
    if (ILCopy[i] != IL[i]) {
      d = false;
    }
  }
  decide(d);
  //test remove
  //case 1: non-exist element
  d = (IL.remove(10) == false && IL.getSize() == 4 && IL[0] == 5 && IL[1] == 3 && IL[2] == 2 &&
       IL[3] == 7);
  decide(d);
  //case 2: the first element
  d = (IL.remove(5) == true && IL.getSize() == 3 && IL[0] == 3 && IL[1] == 2 && IL[2] == 7);
  decide(d);
  //case 3: the middle element
  d = (IL.remove(2) == true && IL.getSize() == 2 && IL[0] == 3 && IL[1] == 7);
  decide(d);
  //case 4: the last element
  d = (IL.remove(7) == true && IL.getSize() == 1 && IL[0] == 3);
  decide(d);
  //case 5: will be empty
  d = (IL.remove(3) == true && IL.getSize() == 0 && IL.head == NULL && IL.tail == NULL);
  decide(d);
  //test assignment constructor for empty list
  ILCopy = IL;
  d = (ILCopy.getSize() == IL.getSize() && ILCopy.head == NULL && ILCopy.tail == NULL);
  decide(d);
  //test copy constructor for empty list
  LinkedList<int> ILCopy1(IL);
  d = (ILCopy1.getSize() == IL.getSize() && ILCopy1.head == NULL && ILCopy1.tail == NULL);
  decide(d);
}

int main(void) {
  testList();
  std::cout << "test success!" << std::endl;
  return EXIT_SUCCESS;
}
