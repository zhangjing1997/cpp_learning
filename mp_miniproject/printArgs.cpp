#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "parse.h"

int main(int argc, char ** argv) {
  std::cout << std::endl;
  for (int i = 1; i < argc; i++) {
    std::cout << "arg" << i << ": " << argv[i] << "(end)" << std::endl;
  }
  return EXIT_SUCCESS;
}
