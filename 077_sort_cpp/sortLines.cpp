#include <algorithm>  //sort
#include <cstdlib>    //EXIT_FAILURE/SUCCESS
#include <fstream>    //ifstream
#include <iostream>   //std:cout
#include <string>     //std::string
#include <vector>     //std::vector

void getSortPrint(std::istream & stream) {
  std::vector<std::string> lines;
  std::string line;
  while (getline(stream, line)) {
    lines.push_back(line);
  }
  std::sort(lines.begin(), lines.end());
  for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it) {
    std::cout << *it << '\n';
  }
}

int main(int argc, char ** argv) {
  if (argc == 1) {
    getSortPrint(std::cin);
  }
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      std::ifstream infile(argv[i]);
      if (!infile) {
        std::cerr << "Unable to open file " << argv[i];
        exit(EXIT_FAILURE);
      }
      getSortPrint(infile);
      infile.close();
    }
  }
  return EXIT_SUCCESS;
}
