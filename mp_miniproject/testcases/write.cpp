#include <cstdio>
#include <cstdlib>
#include <string>

int main() {
  FILE * f = fopen("large.txt", "w");
  if (f == NULL) {
    printf("Error opening file!\n");
    exit(1);
  }
  std::string letters;
  for (int j = 0; j < 3000; j++) {
    for (size_t i = 0; i < 26; i++) {
      letters.push_back('A' + i);
    }
  }
  fprintf(f, "Some text: %s\n", letters.c_str());
}
