#include "counts.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

counts_t * createCounts(void) {
  //WRITE ME
  counts_t * c = malloc(sizeof(*c));
  c->size = 0;
  c->timesUnknown = 0;
  c->array = malloc(c->size * sizeof(*c->array));
  return c;
}

void addCount(counts_t * c, const char * name) {
  //WRITE ME
  int i = 0;
  if (name == NULL) {
    c->timesUnknown++;
  }
  else {
    for (; i < c->size; i++) {
      if (strcmp(name, c->array[i].str) == 0) {
        c->array[i].times++;  //if find the same name before, directly add times at it
        break;
      }
    }
    if (i == c->size) {
      c->size++;
      c->array = realloc(c->array, c->size * sizeof(*c->array));
      c->array[c->size - 1].str = name;
      c->array[c->size - 1].times = 1;
    }
  }
}

void printCounts(counts_t * c, FILE * outFile) {
  //WRITE ME
  for (int j = 0; j < c->size; j++) {
    fprintf(outFile, "%s: %d\n", c->array[j].str, c->array[j].times);
  }
  if (c->timesUnknown != 0) {
    fprintf(outFile, "<unknown>: %d\n", c->timesUnknown);
  }
}

void freeCounts(counts_t * c) {
  //WRITE ME
  free(c->array);
  free(c);
}
