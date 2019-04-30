#include "node.h"
void Node::buildMap(BitString b, std::map<unsigned, BitString> & theMap) {
  //WRITE ME!
  if (sym != NO_SYM) {
    assert(left == NULL && right == NULL);
    theMap.insert(std::pair<unsigned, BitString>(sym, b));
  }
  if (sym == NO_SYM) {
    (*left).buildMap(b.plusZero(), theMap);
    (*right).buildMap(b.plusOne(), theMap);
  }
}
