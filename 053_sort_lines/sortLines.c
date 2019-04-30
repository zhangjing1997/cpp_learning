#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//This function is used to figure out the ordering
//of the strings in qsort.  You do not need
//to modify it.
int stringOrder(const void * vp1, const void * vp2) {
  const char * const * p1 = vp1;
  const char * const * p2 = vp2;
  return strcmp(*p1, *p2);
}
//This function will sort and print data (whose length is count).
void sortData(char ** data, size_t count) {
  qsort(data, count, sizeof(char *), stringOrder);
}

//this function will import data from input, sort the lines and print it out
void print_sort_get_data(FILE * f) {
  size_t sz = 0;
  char * line = NULL;
  size_t i = 0;  //ith line of inputfile
  char ** data = NULL;
  while (getline(&line, &sz, f) > 0) {
    data = realloc(data, (i + 1) * sizeof(*data));
    data[i] = line;
    line = NULL;
    i++;
  }
  free(line);
  sortData(data, i);
  for (size_t j = 0; j < i; j++) {
    printf("%s", data[j]);
    free(data[j]);
  }
  free(data);
}

int main(int argc, char ** argv) {
  //WRITE YOUR CODE HERE!
  if (argc == 1) {
    print_sort_get_data(stdin);
  }
  if (argc > 1) {
    for (int k = 1; k < argc; k++) {
      FILE * f = fopen(argv[k], "r");
      if (f == NULL) {
        fprintf(stderr, "Could not open file[%d]", k);
        exit(EXIT_FAILURE);
      }
      print_sort_get_data(f);
      if (fclose(f) != 0) {
        fprintf(stderr, "Failed to close the inputfile[%d]", k);
        exit(EXIT_FAILURE);
      }
    }
  }
  return EXIT_SUCCESS;
}
