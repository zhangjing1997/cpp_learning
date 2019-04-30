// This file is for Step 1.
// You should do
//  Step 1 (A): write seq1
//  Step 1 (B): write main to test seq1
//  Step 1 (C): write printSeq1Range
//  Step 1 (D): add test cases to main to test printSeq1Range
//
// Be sure to #include any header files you need!

#include <stdio.h>
#include <stdlib.h>

int seq1(int x) {
  return 4 * x - 3;
}

void printSeq1Range(int low, int high) {
  int i;
  if (low < high) {
    for (i = low; i < high; i++) {
      printf("%d", seq1(i));
      if (i == high - 1) {
        printf("\n");
        break;
      }
      printf(", ");
    }
  }
  else {
    printf("\n");
  }
}

int main() {
  //test cases for seq1
  printf("seq1(%d) = %d\n", -4, seq1(-4));
  printf("seq1(%d) = %d\n", -1, seq1(-1));
  printf("seq1(%d) = %d\n", 0, seq1(0));
  printf("seq1(%d) = %d\n", 5, seq1(5));
  printf("seq1(%d) = %d\n", 10, seq1(10));
  //test cases for printSeq1range
  printf("printSeq1Range(%d, %d)\n", -2, 6);
  printSeq1Range(-2, 6);
  printf("printSeq1Range(%d, %d)\n", 3, 7);
  printSeq1Range(3, 7);
  printf("printSeq1Range(%d, %d)\n", -4, -1);
  printSeq1Range(-4, -1);
  printf("printSeq1Range(%d, %d)\n", 7, 3);
  printSeq1Range(7, 3);
  printf("printSeq1Range(%d, %d)\n", 0, 0);
  printSeq1Range(0, 0);
  return 0;
}
