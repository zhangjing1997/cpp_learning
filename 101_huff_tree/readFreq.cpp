#include "readFreq.h"

#include <stdio.h>

#include <cstdlib>
#include <fstream>

void printSym(std::ostream & s, unsigned sym) {
  if (sym > 256) {
    s << "INV";
  }
  else if (sym == 256) {
    s << "EOF";
  }
  else if (isprint(sym)) {
    char c = sym;
    s << "'" << c << "'";
  }
  else {
    std::streamsize w = s.width(3);
    s << std::hex << sym << std::dec;
    s.width(w);
  }
}

uint64_t * readFrequencies(const char * fname) {
  //WRITE ME!
  uint64_t * ans = new uint64_t[257]();
  std::ifstream ifs(fname);
  if (!ifs) {
    std::cerr << "cannot open the file" << std::endl;
    exit(EXIT_FAILURE);
  }
  while (ifs.good()) {
    int index = ifs.get();
    if (index <= 255 && index >= 0) {
      ans[index]++;
    }
  }
  ans[256]++;
  ifs.close();
  return ans;
}
