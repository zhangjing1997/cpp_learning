#ifndef __ENVMAP_H__
#define __ENVMAP_H__

#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#define NO_VAR -1

class EnvMap
{
 private:
  int numVar;
  std::vector<std::pair<std::string, std::string> > varArr;

 public:
  EnvMap() : numVar(1), varArr(std::vector<std::pair<std::string, std::string> >(1)) {
    varArr[0].first = "ECE551PATH";
    varArr[0].second = getenv("PATH");
  }                                                                          //(default)constructor
  ~EnvMap();                                                                 //destructor
  std::pair<std::string, std::string> operator[](size_t index);              //index operator
  const std::pair<std::string, std::string> operator[](size_t index) const;  //const index operator
  int size();  //get the number of environment variables
  void add(std::string & varName, std::string & varValue);  //add an environment variable
  int find(std::string & varName);  //find an environment variable and return value
  std::vector<std::pair<std::string, std::string> > getVarArr();  //get varArr
  std::vector<char *> getEnvp();  //get the array of environment variables
};

//destructor
EnvMap::~EnvMap() {}

//index operator []
std::pair<std::string, std::string> EnvMap::operator[](size_t index) {
  return varArr[index];
}

//const index operator []
const std::pair<std::string, std::string> EnvMap::operator[](size_t index) const {
  return varArr[index];
}

//get the number of environment variables
int EnvMap::size() {
  return numVar;
}

//add an element to the environment variable set
void EnvMap::add(std::string & varName, std::string & varValue) {
  int varPos = find(varName);
  if (varPos == NO_VAR) {
    std::pair<std::string, std::string> varToAdd(varName, varValue);
    varArr.push_back(varToAdd);
    numVar++;
  }
  else {
    varArr[varPos].second = varValue;
  }
}

//find the value by the variable name
int EnvMap::find(std::string & varName) {
  for (int i = 0; i < numVar; i++) {
    if (varName.compare(varArr[i].first) == 0) {
      return i;
    }
  }
  return NO_VAR;  //return an empty string object
}

//get varArr
std::vector<std::pair<std::string, std::string> > EnvMap::getVarArr() {
  return varArr;
}

//get the array of environment variables
std::vector<char *> EnvMap::getEnvp() {
  std::vector<char *> envp;
  for (int i = 0; i < numVar; i++) {
    std::string var = varArr[i].first + "=" + varArr[i].second;
    envp.push_back(strdup(var.c_str()));
  }
  envp.push_back(NULL);
  return envp;
}

#endif
