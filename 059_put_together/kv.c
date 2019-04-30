#include "kv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//This function is to transform the '\n' of the string read from f into '\0'
void stripNewline(char * str) {
  char * p = strchr(str, '\n');
  if (p != NULL) {
    *p = '\0';
  }
}

//This function is to read one kvpair from one line of f
kvpair_t readAkvpair(char * str) {
  kvpair_t kvPair;
  kvPair.key = NULL;
  kvPair.value = NULL;
  stripNewline(str);
  char * ptr = strchr(str, '=');
  *ptr = '\0';  //let the key and value could be read directly
  kvPair.key = str;
  kvPair.value = ptr + 1;
  return kvPair;
}

//This function is to read KVpairs from f
kvarray_t * readKVs(const char * fname) {
  //WRITE ME
  FILE * f = fopen(fname, "r");
  if (f == NULL) {
    return NULL;  //Could not open file->indicate failure
  }
  kvarray_t * ans = malloc(sizeof(*ans));
  ans->numKvpairs = 0;
  ans->kvarray = malloc(ans->numKvpairs * sizeof(*ans->kvarray));
  char * line = NULL;
  size_t sz = 0;
  while (getline(&line, &sz, f) >= 0) {
    char * ptr = strchr(line, '=');  //find the division between key and value
    int lineLen = strlen(line);
    if ((ptr > line) & (ptr < line + lineLen - 1)) {
      ans->numKvpairs++;  //compute the number of effective pairs by the incremental method
      ans->kvarray = realloc(ans->kvarray, ans->numKvpairs * sizeof(*ans->kvarray));
      ans->kvarray[ans->numKvpairs - 1] = readAkvpair(line);
    }
    line = NULL;
    sz = 0;
  }
  free(line);  //the conditional statement of while still executes->allocate memory
  if (fclose(f) != 0) {
    fprintf(stderr, "Could not close file");
  }
  return ans;
}

void freeKVs(kvarray_t * pairs) {
  //WRITE ME
  for (int i = 0; i < pairs->numKvpairs; i++) {
    free(pairs->kvarray[i].key);
  }
  free(pairs->kvarray);
  free(pairs);
}

void printKVs(kvarray_t * pairs) {
  //WRITE ME
  for (int i = 0; i < pairs->numKvpairs; i++) {
    printf("key = '%s' value = '%s'\n", pairs->kvarray[i].key, pairs->kvarray[i].value);
  }
}

char * lookupValue(kvarray_t * pairs, const char * key) {
  //WRITE ME
  for (int i = 0; i < pairs->numKvpairs; i++) {
    if (strcmp(pairs->kvarray[i].key, key) == 0) {
      return pairs->kvarray[i].value;
    }
  }
  return NULL;
}
