#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

//function of computing the index of the maximum element of an int array
int max_arr(int * array, int size) {
  int max = array[0];
  int max_index = 0;
  for (int j = 1; j < size; j++) {
    if (array[j] > max) {
      max = array[j];
      max_index = j;
    }
  }
  return max_index;
}

//get the frequency count of different letters and compare them to guess 'e'
int break_key(FILE * f) {
  int c;
  int index;
  int count[26] = {0};
  while ((c = fgetc(f)) != EOF) {
    if (isalpha(c)) {
      c = tolower(c);
      index = c - 'a';
      count[index]++;
    }
  }  //obtain the frequency count of different letters(a-z)
  int max_index = max_arr(count, 26);
  int key;
  if (('a' + max_index) >= 'e') {
    key = 'a' + max_index - 'e';
  }
  else {
    key = 'a' + max_index - 'e' + 26;
  }
  return key;
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: encrypt key");
    return EXIT_FAILURE;
  }
  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("Could not open file");
    return EXIT_FAILURE;
  }
  int key = break_key(f);
  if (fclose(f) != 0) {
    perror("Failed to close the input file!");
  }
  printf("%d\n", key);
  return EXIT_SUCCESS;
}
