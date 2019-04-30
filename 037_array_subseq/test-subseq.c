#include <stdio.h>
#include <stdlib.h>

size_t maxSeq(int * array, size_t n);  //declare the prototype for maxSeq
void run_check(int * array, size_t n, int expected_ans) {
  int ans = maxSeq(array, n);
  if (ans != expected_ans) {
    exit(EXIT_FAILURE);
  }
}

int main() {
  int array1[5] = {1, 2, 1, 2, 3};  //only detect the first increasing sequence
  run_check(array1, 5, 3);
  int array3[3] = {-2, -1};  //not detect the automatically filled-in 0
  run_check(array3, 3, 3);
  int array4[4] = {1, 1, 1, 1};  //take undecreasing as increasing
  run_check(array4, 4, 1);
  int array5[] = {};  //empty array
  run_check(array5, 0, 0);
  exit(EXIT_SUCCESS);
}
