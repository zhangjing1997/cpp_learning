#ifndef __BUILTIN_H__
#define __BUILTIN_H__

#include <assert.h>
#include <limits.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "cmdLine.h"
#include "envMap.h"

//prototypes for functions implemented after runBuiltins
bool mshCd(CommandLine & cl);
bool mshSet(CommandLine & cl);
bool mshExport(CommandLine & cl);
bool mshInc(CommandLine & cl);

//main function for running builtin commands
void runBuiltins(CommandLine & cl, bool & isBuiltin, bool & isExecuted) {
  //create an array to store builtin commands names
  std::vector<const char *> builtinCmds;
  const char * cdName[] = {"cd", "set", "export", "inc", NULL};
  for (int i = 0; cdName[i] != NULL; i++) {
    builtinCmds.push_back(cdName[i]);
  }
  //create an array of builtin-in function pointers
  bool (*builtinFuncs[])(CommandLine &) = {&mshCd, &mshSet, &mshExport, &mshInc};
  for (size_t i = 0; i < builtinCmds.size(); i++) {
    if (cl[0].compare(builtinCmds[i]) == 0) {
      isBuiltin = true;
      //std::cout << "Execute BuiltinCmd: " << cl[0] << std::endl;  //for test
      if ((*builtinFuncs[i])(cl) == true) {
        isExecuted = true;
        break;
      }
    }
  }
}

//cd directory
bool mshCd(CommandLine & cl) {
  char cwd[PATH_MAX + 1];
  //if cd gets no argument
  if (cl.getNumArg() == 1) {
    //back to the root directory
    if (chdir("/") != 0) {
      perror("chdir()");
    }
    else {
      //get the current directory
      if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd()");
      }
      //modify the field currDir in cl
      else {
        cl.modifyCurrDir(cwd);
      }
    }
    return true;
  }
  //if cd gets more than one argument
  else if (cl.getNumArg() > 2) {
    std::cerr << "-bash cd: too many arguments" << std::endl;
    return false;
  }
  //if cd gets exactly one argument
  else {
    char * dirname = strdup(cl[1].c_str());
    //change to the specified directory
    if (chdir(dirname) != 0) {
      free(dirname);
      perror("chdir()");
    }
    //if successfully
    else {
      free(dirname);
      //get the current directory
      if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd()");
      }
      //modify the field currDir in cl
      else {
        cl.modifyCurrDir(cwd);
      }
    }
    return true;
  }
}

//helper function: check the validity of variable name
bool checkVarName(std::string & varName) {
  std::string numbers = "0123456789";
  std::string lowcaseLetters;
  std::string capitalLetters;
  for (int i = 0; i < 26; i++) {
    lowcaseLetters.push_back('a' + i);
    capitalLetters.push_back('A' + i);
  }
  //combine all the valid elements into a single string
  std::string validElements = numbers + lowcaseLetters + capitalLetters + '_';
  if (varName.find_first_not_of(validElements) != std::string::npos) {
    std::cerr << varName << " is not a valid variable" << std::endl;
    return false;
  }
  return true;
}

//set var value
bool mshSet(CommandLine & cl) {
  std::string line = cl.getLine();
  EnvMap tempEnv = cl.getTempEnv();
  std::string varNameToSet;
  std::string varValueToSet;
  if (cl.getNumArg() == 1) {  //test
    std::cerr << "-bash set: too few arguments" << std::endl;
    return false;
  }
  std::vector<std::string> newArgs;
  if (cl.getNumArg() == 2) {
    varNameToSet = cl[1];
    //check validity
    if (checkVarName(varNameToSet) == false) {
      return false;
    }
    size_t pos = 0;
    size_t varPos = line.find(varNameToSet, pos);
    if (varPos + varNameToSet.size() + 1 < line.size()) {
      varValueToSet = copySubStr(line, varPos + varNameToSet.size() + 1, line.size());
    }
    else {
      varValueToSet = "";
    }
    tempEnv.add(varNameToSet, varValueToSet);
    cl.modifyTempEnv(tempEnv);  //update tempEnv in the current object
  }
  else {
    convertEval(line, tempEnv);
    std::vector<std::string> argsForSet = getArgsForSet(line);  //re-parse line over rules of set
    varNameToSet = argsForSet[1];
    //check validity
    if (checkVarName(varNameToSet) == false) {
      return false;
    }
    varValueToSet = argsForSet[2];
    tempEnv.add(varNameToSet, varValueToSet);
    cl.modifyTempEnv(tempEnv);  //update tempEnv in the current object
  }
  return true;
}

//export var
bool mshExport(CommandLine & cl) {
  EnvMap temp = cl.getTempEnv();
  EnvMap curr = cl.getCurrEnv();
  int numVar = cl.getNumArg() - 1;  //number of variables to export
  for (int i = 1; i < numVar + 1; i++) {
    std::string varName = cl[i];  //the ith variable name
    int varInd;
    if ((varInd = temp.find(varName)) == NO_VAR) {
      std::cerr << varName << "has not been set" << std::endl;
      return false;
    }
    std::string varValue = temp[varInd].second;
    curr.add(varName, varValue);
    cl.modifyCurrEnv(curr);  //update currEnv
  }
  return true;
}

//helper function: convert number(int) into std::string
std::string numToString(int num) {
  std::ostringstream oss;
  oss << num;
  return oss.str();
}

//inc var
bool mshInc(CommandLine & cl) {
  EnvMap temp = cl.getTempEnv();
  int numVar = cl.getNumArg() - 1;  //number of variables to inc
  for (int i = 1; i < numVar + 1; i++) {
    std::string varName = cl[i];  //the ith variable name
    std::string varValue;
    int varInd;
    //if the variable has been set
    if ((varInd = temp.find(varName)) != NO_VAR) {
      varValue = temp[varInd].second;
      //if varValue represents a number(base10)
      if (varValue.find_first_not_of("0123456789") == std::string::npos) {
        int num = atoi(varValue.c_str());
        num++;
        varValue = numToString(num);  //update varValue
      }
      //if varValue does NOT represents a number(base10)
      else {
        int num = 1;
        varValue = numToString(num);  //update varValue
      }
      temp.add(varName, varValue);  //add the variable
      cl.modifyTempEnv(temp);       //update tempEnv
    }
    //if the variable has NOT been set
    else {
      if (checkVarName(varName) == false) {
        return false;
      }
      int num = 1;
      varValue = numToString(num);  //set value for it
      temp.add(varName, varValue);  //add the newly-set variable
      cl.modifyTempEnv(temp);       //update tempEnv
    }
  }
  return true;
}

#endif
