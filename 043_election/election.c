#include "election.h"

//include any other headers you need here...
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STATE_POPULATION_LENGTH 64
#define MAX_STATE_ELECTORAL_VOTES_LENGTH 64

//Side function: count how many times that c appears in str
int count(const char * str, char c) {
  int count = 0;
  while (*str != '\0') {
    if (*str == c) {
      count++;
    }
    str++;
  }
  return count;
}

//Key function: read the state information from a given line
state_t parseLine(const char * line) {
  //STEP 1: write me
  //decide whether the information format is what we expect
  int len_line = strlen(line);
  if (len_line < 5) {
    fprintf(stderr, "The length of state information is too shorter to hold effective format!\n");
    exit(EXIT_FAILURE);
  }
  if (count(line, ':') != 2) {
    fprintf(stderr, "The format of state information is wrong!\n");
    exit(EXIT_FAILURE);
  }
  //read state information
  state_t state;
  char population[MAX_STATE_POPULATION_LENGTH] = {'\0'};
  char electoralVotes[MAX_STATE_ELECTORAL_VOTES_LENGTH] = {'\0'};
  int i_start = 0;
  int p = 0;  //The identifier of state.name(p = 0), population(p = 1) and electoralVotes(p = 2)
  for (int i = i_start; i < len_line; i++) {
    if (line[i] != ':') {
      if (p == 0) {
        state.name[i - i_start] = line[i];
      }
      if (p == 1) {
        population[i - i_start] = line[i];
      }
      if (p == 2) {
        electoralVotes[i - i_start] = line[i];
      }
    }
    else {
      if (p == 0) {
        state.name[i - i_start] = '\0';
      }
      if (p == 1) {
        population[i - i_start] = '\0';
      }
      if (p == 2) {
        electoralVotes[i - i_start] = '\0';
      }
      p++;
      i_start = i + 1;
    }
  }
  //transform the readed string for population and electoral votes into the expected actual number
  state.population = (uint64_t)strtoul(population, 0, 10);
  state.electoralVotes = (unsigned int)strtoul(electoralVotes, 0, 10);
  return state;
}

//Key funciton: count the total electoral votes that candidate A received in election
unsigned int countElectoralVotes(state_t * stateData, uint64_t * voteCounts, size_t nStates) {
  //STEP 2: write me
  unsigned int votes_won[nStates];
  unsigned int total_votes_won = 0;
  for (size_t i = 0; i < nStates; i++) {
    if ((double)voteCounts[i] / stateData[i].population > 0.5) {
      votes_won[i] = stateData[i].electoralVotes;
    }
    else {
      votes_won[i] = 0;  //"would not receive electoralVotes" equals "the vote_won is zero"
    }
    total_votes_won += votes_won[i];
  }
  return total_votes_won;
}

//Key function: print the percentage of votes that candidate A won if it requires recounts
void printRecounts(state_t * stateData, uint64_t * voteCounts, size_t nStates) {
  //STEP 3: write me
  double vote_won_fraction[nStates];
  for (size_t i = 0; i < nStates; i++) {
    vote_won_fraction[i] = (double)voteCounts[i] / stateData[i].population;
    //cast the result as a double to guarantee the later comparison right
    if ((vote_won_fraction[i] >= 0.495) && (vote_won_fraction[i] <= 0.505)) {
      printf("%s requires a recount (Candidate A has %.2f%% of the vote)\n",
             stateData[i].name,
             vote_won_fraction[i] * 100);
    }
  }
}

//Key function: print the largest percentage of votes that candidate A won from the corresponding state
void printLargestWin(state_t * stateData, uint64_t * voteCounts, size_t nStates) {
  //STEP 4: write me
  double vote_won_fraction[nStates];
  double max_fraction = 0;
  size_t max_index;
  for (size_t i = 0; i < nStates; i++) {
    vote_won_fraction[i] = (double)voteCounts[i] / stateData[i].population;
    if (vote_won_fraction[i] > max_fraction) {
      max_fraction = vote_won_fraction[i];  //update max_fraction
      max_index = i;                        //update the index of max_fraction
    }
  }
  printf("Candidate A won %s with %.2f%% of the vote\n",
         stateData[max_index].name,
         max_fraction * 100);
}
