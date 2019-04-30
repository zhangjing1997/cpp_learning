#include <stdio.h>
#include <stdlib.h>

size_t maxSeq(int * array, size_t n) {
  if (n == 0) {
    return 0;
  }
  if (n == 1) {
    return 1;
  }
  size_t seq_length = 1;
  size_t maxSeq_length = 0;
  for (size_t i = 0; i < n - 1; i++) {
    if (array[i + 1] > array[i]) {
      seq_length++;
    }
    else {
      seq_length = 1;
    }
    if (seq_length > maxSeq_length) {
      maxSeq_length = seq_length;
    }
  }
  return maxSeq_length;
}
