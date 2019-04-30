#include <stdio.h>
#include <stdlib.h>

unsigned power(unsigned x, unsigned y);
void run_check(unsigned x, unsigned y, unsigned expected_ans) {
  unsigned ans = power(x, y);
  if (ans != expected_ans) {
    printf("Fail the test(%d, %d)\n", x, y);
    exit(EXIT_FAILURE);
  }
}

int main() {
  run_check(2, 2, 4);
  run_check(10, 2, 100);
  run_check(0, 0, 1);
  run_check(-1, 3, -1);
}
