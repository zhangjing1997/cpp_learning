#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void rotate(FILE * f) {
  int column = 12;
  int row = 10;
  char line[row][column];
  int i = 0;
  int count_row = 0;
  char new_matrix[row][column];
  while (fgets(line[i], column, f) != NULL) {
    //whether the number of characters of each line is right
    if (strchr(line[i], '\n') == NULL) {
      fprintf(stderr, "line is longer than expected!\n");
      exit(EXIT_FAILURE);
    }
    if (strchr(line[i], '\0') != *(line + i) + 11) {
      fprintf(stderr, "line is shorter than expected!\n");
      exit(EXIT_FAILURE);
    }
    count_row++;
    //whether the number of rows is right
    if (count_row > 10) {
      fprintf(stderr, "row count is more than expected!\n");
      exit(EXIT_FAILURE);
    }
    //rotate
    for (int j = 0; j < 10; j++) {
      new_matrix[j][i] = line[i][j];
    }
    //transfer '\n' '\0' to each row
    new_matrix[i][10] = line[i][10];
    new_matrix[i][11] = line[i][11];
    i++;
  }
  if (count_row < 10) {
    fprintf(stderr, "row count is less than expected!\n");
    exit(EXIT_FAILURE);
  }
  //print out on standard output
  for (int k = 0; k < 10; k++) {
    for (int p = 0; p < 11; p++) {
      printf("%c", new_matrix[k][p]);
    }
  }
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: rotate matrix");
    return EXIT_FAILURE;
  }
  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    fprintf(stderr, "Usage: rotate matrix");
    return EXIT_FAILURE;
  }
  rotate(f);
  if (fclose(f) != 0) {
    fprintf(stderr, "Usage: rotate matrix");
  }
  return EXIT_SUCCESS;
}
