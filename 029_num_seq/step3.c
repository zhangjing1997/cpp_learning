// This file is for Step 3.
// You should do
//  Step 3 (A): write seq3
//  Step 3 (B): write main to test seq3
//  Step 3 (C): write countEvenInSeq3Range
//  Step 3 (D): add test cases to main to test countEvenInSeq3Range
//
// Be sure to #include any header files you need!

#include <stdio.h>
#include <stdlib.h>

int seq3(int x, int y) {
  return x * y + 2 * y - 3 * x;
}

int countEvenInSeq3Range(int xLow, int xHi, int yLow, int yHi) {
  int countEven = 0;
  for (int i = xLow; i < xHi; i++) {
    for (int j = yLow; j < yHi; j++) {
      if (seq3(i, j) % 2 == 0) {
        countEven++;
      }
    }
  }
  return countEven;
}

int main() {
  //test cases for seq3
  printf("seq3(%d, %d) = %d\n", 1, 1, seq3(1, 1));
  printf("seq3(%d, %d) = %d\n", 2, 1, seq3(2, 1));
  printf("seq3(%d, %d) = %d\n", 1, 2, seq3(1, 1));
  printf("seq3(%d, %d) = %d\n", -5, -4, seq3(-5, -4));
  printf("seq3(%d, %d) = %d\n", -4, -5, seq3(-4, -5));
  //test cases for countEvenInSeq3range
  printf(
      "countEveninseq3range(%d, %d, %d, %d) = %d\n", 0, 2, 0, 3, countEvenInSeq3Range(0, 2, 0, 3));
  printf("countEveninseq3range(%d, %d, %d, %d) = %d\n",
         -5,
         -1,
         -4,
         -2,
         countEvenInSeq3Range(-5, -1, -4, -2));
  printf("countEveninseq3range(%d, %d, %d, %d) = %d\n",
         0,
         2,
         -5,
         -4,
         countEvenInSeq3Range(0, 2, -5, -4));
  return 0;
}
