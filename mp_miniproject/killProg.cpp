#include <cstdio>
#include <cstdlib>

int main() {
  int numArr[5] = {0, 1, 2, 3, 4};
  for (int i = 0; i < 5; i++) {
    if (numArr[i] == 1) {
      exit(EXIT_FAILURE);
    }
  }
  return EXIT_SUCCESS;
}
