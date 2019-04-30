#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "function.h"

int binarySearchForZero(Function<int, int> * f, int low, int high) {
  if (low == high) {
    return low;
  }
  if (low == high - 1) {
    return low;
  }  //key base case
  int mid = low + (high - low) / 2;
  double mid_value = f->invoke(mid);
  if (mid_value < 0) {
    low = mid;
  }
  if (mid_value > 0) {
    high = mid;
  }
  if (mid_value == 0) {
    return mid;
  }
  return binarySearchForZero(f, low, high);
}
