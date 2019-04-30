#ifndef __PARSE_H__
#define __PARSE_H__

#include <fcntl.h>
#include <unistd.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "envMap.h"

#define NO_PIPE -1

//copy the substring from [start] to [end-1]
std::string copySubStr(std::string str, size_t start, size_t end) {
  std::string ans(str, start, end - start);
  return ans;
}

//read the command line from standard input
std::string readLine(std::istream & stream) {
  std::string line;
  std::getline(stream, line);
  return line;
}

//helper function for std::sort used in convertEval
bool compareVar(std::pair<std::string, std::string> var1,
                std::pair<std::string, std::string> var2) {
  if (var1.first.size() != var2.first.size()) {
    //compare length firstly (the longer one is larger)
    return (var1.first.size() < var2.first.size());
  }
  else {
    //alphabetically comparing when same length
    return (var1.first.compare(var2.first) < 0);
  }
}

//do the $ evaluations
void convertEval(std::string & line, EnvMap & tempEnv) {
  std::vector<std::pair<std::string, std::string> > varArr = tempEnv.getVarArr();
  //sort the variable array so that later searching would expands $ to the longest possible variable string
  std::sort(varArr.begin(), varArr.end(), compareVar);
  size_t pos = 0;
  size_t evalPos;
  while ((evalPos = line.find('$', pos)) != std::string::npos) {
    while (line.find('$', evalPos + 1) == evalPos + 1) {
      line.erase(evalPos, 1);
    }
    bool converted = false;
    for (int j = tempEnv.size() - 1; j >= 0; j--) {
      std::string varName = tempEnv[j].first;
      size_t varPos = line.find(varName, evalPos);
      if (varPos == evalPos + 1) {
        std::string varValue = tempEnv[j].second;
        line.erase(evalPos, varName.size() + 1);  //erase $ and the variable
        line.insert(evalPos, varValue);           //insert the corresponding value
        converted = true;                         //mark that the variable has been converted
        break;
      }
    }
    //if not find an already set variable
    if (converted == false) {
      //try to find the next evaluation position
      size_t nextEvalPos;
      if ((nextEvalPos = line.find('$', evalPos + 1) != std::string::npos)) {
        line.erase(evalPos, nextEvalPos - evalPos);
      }
      else {
        line.erase(evalPos, line.size());
        break;  //no need to do continue the loop
      }
    }
    pos = evalPos + 1;
  }
}

//divide the command line into arguments separated by non-escaped spaces
std::vector<std::string> getGenArgs(std::string & line) {
  std::vector<std::string> args;
  size_t pos = 0;
  size_t nextSpace;
  //skip the begining whitespaces
  while (line[pos] == ' ') {
    pos++;
  }
  while ((nextSpace = line.find(' ', pos)) != std::string::npos) {
    //skip escaped whitespaces
    while (nextSpace - 1 > 0 && line[nextSpace - 1] == '\\') {
      nextSpace = line.find(' ', nextSpace + 1);
    }
    //found separate whitespace after escaped whitespace -> copy the next subarg
    if (nextSpace != std::string::npos) {
      std::string arg = copySubStr(line, pos, nextSpace);
      args.push_back(arg);
      pos = nextSpace + 1;  //update the searching start pos to the next of nextSpace
      //skip continuous whitespace
      while (line[pos] == ' ') {
        pos++;
      }
    }
    //not found separate whitespace after escaped whitespace -> copy the rest
    else {
      std::string arg = copySubStr(line, pos, line.size());
      args.push_back(arg);
      return args;
    }
  }
  //copy the rest if pos has not reached the end
  if (pos < line.size()) {
    std::string arg = copySubStr(line, pos, line.size());
    args.push_back(arg);
  }
  return args;
}

//reparse the line for command(set) when there are 3 or more general args
std::vector<std::string> getArgsForSet(std::string & line) {
  std::vector<std::string> args(3);
  size_t pos = 0;
  size_t nextSpace;
  size_t i = 0;
  while (i < 2) {
    while (line[pos] == ' ') {  //skip the first multiple whitespaces
      pos++;
    }
    nextSpace = line.find(' ', pos);
    while (line[nextSpace - 1] == '\\') {  //skip escaped whitespaces
      nextSpace = line.find(' ', nextSpace + 1);
    }
    args[i] = copySubStr(line, pos, nextSpace);
    pos = nextSpace + 1;
    i++;
  }
  args[2] = copySubStr(line, pos, line.size());
  return args;
}

//iterate over each argument and strip the escape operator
void stripEscp(std::vector<std::string> & args) {
  for (size_t i = 0; i < args.size(); i++) {
    size_t pos = 0;
    size_t escPos;
    while ((escPos = args[i].find('\\', pos)) != std::string::npos) {
      args[i].erase(escPos, 1);
      pos = escPos + 1;
      if (pos >= args[i].size()) {
        break;
      }
    }
  }
}

//parse single argument vector for rediection
void getRedir(std::vector<std::string> & args,
              std::string & inFile,
              std::string & outFile,
              std::string & errFile) {
  if (args.size() == 0 || args[0].compare("set") == 0 || args[0].compare("export") == 0 ||
      args[0].compare("inc") == 0) {
    return;
  }
  int numIn = 0, numOut = 0, numErr = 0;
  for (size_t i = 0; i < args.size(); i++) {
    if (args[i] == "<" && i + 1 < args.size() && !args[i + 1].empty()) {
      numIn++;
      if (numIn == 1) {
        inFile = args[i + 1];
        args.erase(args.begin() + i);  //elements behind shifts forward
        args.erase(args.begin() + i);
        i--;
        continue;
      }
      args.erase(args.begin() + i);  //elements behind shifts forward
      args.erase(args.begin() + i);
      i--;
    }
    else if (args[i] == ">" && i + 1 < args.size() && !args[i + 1].empty()) {
      numOut++;
      if (numOut == 1) {
        outFile = args[i + 1];
        args.erase(args.begin() + i);  //elements behind shifts forward
        args.erase(args.begin() + i);
        i--;
        continue;
      }
      args.erase(args.begin() + i);  //elements behind shifts forward
      args.erase(args.begin() + i);
      i--;
    }
    else if (args[i] == "2>" && i + 1 < args.size() && !args[i + 1].empty()) {
      numErr++;
      if (numErr == 1) {
        errFile = args[i + 1];
        args.erase(args.begin() + i);  //elements behind shifts forward
        args.erase(args.begin() + i);
        i--;
        continue;
      }
      args.erase(args.begin() + i);  //elements behind shifts forward
      args.erase(args.begin() + i);
      i--;
    }
  }
}

//transform args(vector of std::string) into argv(vector of const char*)
std::vector<char *> argsToArgv(std::vector<std::string> & args) {
  std::vector<char *> argv(args.size());
  for (size_t pos = 0; pos < args.size(); pos++) {
    argv[pos] = strdup(args[pos].c_str());  //send to free() later!!!
  }
  argv.push_back(NULL);
  return argv;
}

//find the index of first "|" starting from ind in args
int findPipe(std::vector<std::string> & args, size_t ind) {
  int ans = NO_PIPE;
  for (size_t i = ind; i < args.size(); i++) {
    if (args[i].compare("|") == 0) {
      ans = i;
      break;
    }
  }
  return ans;
}

//decide the number of commands by parsing args
int getNumCmd(std::vector<std::string> & args) {
  if (args.size() == 0) {
    return 0;
  }
  int numPipe = 0;
  size_t ind = 0;  //starting index for searching "|"
  int pipeInd;     //index of "|" in args
  while ((pipeInd = findPipe(args, ind)) != NO_PIPE) {
    numPipe++;
    ind = pipeInd + 1;
  }
  return numPipe + 1;
}

//divide general argument vector by "|" into multiple sub-arguments
std::vector<std::vector<char *> > divideArgsForPipe(std::vector<std::string> & args) {
  std::vector<std::vector<char *> > subArgvs;
  size_t ind = 0;
  int pipeInd;
  while ((pipeInd = findPipe(args, ind)) != NO_PIPE) {
    std::vector<std::string> subArgs;
    for (int i = ind; i < pipeInd; i++) {
      subArgs.push_back(args[i]);
    }
    subArgvs.push_back(argsToArgv(subArgs));
    ind = pipeInd + 1;
  }
  std::vector<std::string> subArgs;
  for (size_t i = ind; i < args.size(); i++) {
    subArgs.push_back(args[i]);
  }
  subArgvs.push_back(argsToArgv(subArgs));
  return subArgvs;
}

//redirect input, output and error
void redirForSingleCmd(std::string & inFile, std::string & outFile, std::string & errFile) {
  int tmpin = dup(0);
  int tmpout = dup(1);
  int tmperr = dup(2);
  int fdin;
  if (!inFile.empty()) {
    if ((fdin = open(inFile.c_str(), O_RDONLY)) == -1) {
      perror("open()");
    }
  }
  else {
    fdin = dup(tmpin);
  }
  dup2(fdin, 0);  //redirect input
  close(fdin);
  int fdout;
  if (!outFile.empty()) {
    fdout =
        open(outFile.c_str(), O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
  }
  else {
    fdout = dup(tmpout);
  }
  dup2(fdout, 1);  //redirect output
  close(fdout);
  int fderr;
  if (!errFile.empty()) {
    fderr =
        open(errFile.c_str(), O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
  }
  else {
    fderr = dup(tmperr);
  }
  dup2(fderr, 2);  //redirect error output
  close(fderr);
}

#endif
