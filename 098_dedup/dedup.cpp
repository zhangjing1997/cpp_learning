#include "dedup.h"

#include <stdlib.h>

#include <cstdlib>
#include <iostream>
#include <string>

int main(int argc, char ** argv) {
  if (argc < 2) {
    std::cerr << "no input pathname" << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << "#!/bin/bash" << std::endl;
  FileTable ft;
  for (int i = 1; i < argc; i++) {
    const std::string orgdir = argv[i];
    ft.readDir(orgdir);
  }
  //std::cout << "searching completed!" << std::endl;
  return EXIT_SUCCESS;
}
