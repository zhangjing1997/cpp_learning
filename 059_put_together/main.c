#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "counts.h"
#include "kv.h"
#include "outname.h"

kvarray_t * readKVs(const char * fname);
void printCounts(counts_t * c, FILE * outFile);
void stripNewline(char * str);

counts_t * countFile(const char * filename, kvarray_t * kvPairs) {
  //WRITE ME
  size_t sz = 0;
  char * line = NULL;
  int numValueData = 0;
  char ** valueData = malloc(sizeof(*valueData));
  FILE * f = fopen(filename, "r");
  while (getline(&line, &sz, f) >= 0) {
    stripNewline(line);
    numValueData++;
    valueData = realloc(valueData, numValueData * sizeof(*valueData));
    valueData[numValueData - 1] = lookupValue(kvPairs, line);
    free(line);  //line just let us know where the valueData[i] points at, no later use
    sz = 0;
  }
  free(line);  //even though the while condition failed the last time, it still executes
  fclose(f);
  counts_t * counts = createCounts();
  for (int j = 0; j < numValueData; j++) {
    addCount(counts, valueData[j]);
  }
  free(
      valueData);  //we do not need to free valueData[j] because that that part of memory would be freed by free(kvPairs)
  return counts;
}

int main(int argc, char ** argv) {
  //WRITE ME (plus add appropriate error checking!)
  if (argc < 3) {
    fprintf(stderr, "No input files\n");
    exit(EXIT_FAILURE);
  }
  //read the key/value pairs from the file named by argv[1] (call the result kv)
  //count from 2 to argc (call the number you count i)
  //count the values that appear in the file named by argv[i], using kv as the key/value pair
  //   (call this result c)
  //compute the output file name from argv[i] (call this outName)
  //open the file named by outName (call that f)
  //print the counts from c into the FILE f
  //close f
  //free the memory for outName and c
  //free the memory for kv
  kvarray_t * kv = readKVs(argv[1]);
  for (int i = 2; i < argc; i++) {
    counts_t * c = countFile(argv[i], kv);
    char * outName = computeOutputFileName(argv[i]);
    FILE * f = fopen(outName, "w");
    printCounts(c, f);
    fclose(f);
    free(outName);
    freeCounts(c);
  }
  freeKVs(kv);
  /*ATTENTION: 
   we should free kv after the for loop because kv needs to be used iteratively rather than just one time,
   unless we include readKVs(argv[1]) into the loop*/
  return EXIT_SUCCESS;
}
