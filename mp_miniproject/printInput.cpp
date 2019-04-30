#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

//read and print the first argv[1] lines of standard input
int main(int argc, char ** argv) {
  if (argc != 2) {
    std::cerr << "too few arguments" << std::endl;
    exit(EXIT_FAILURE);
  }
  int n = atoi(argv[1]);
  std::vector<std::string> lines;
  std::string line;
  while (!std::cin.eof()) {
    getline(std::cin, line);
    lines.push_back(line);
  }
  int size = lines.size();
  int max = std::min(n, size);
  for (int i = 0; i < max; i++) {
    std::cout << lines[i] << std::endl;
  }
  return EXIT_SUCCESS;
}
