#ifndef __CommandLine_H__
#define __CommandLine_H__

#include <assert.h>
#include <error.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#include "envMap.h"
#include "parse.h"
#include "path.h"

class CommandLine
{
 private:
  std::string line;               //the line read from standard input
  std::vector<std::string> args;  //the general arguments before parsing for piped commands
  int numCmd;                     //number of single commands given by the line
  std::string inFile;             //INPUT FILE for redirection(if any) but empty string(if not)
  std::string outFile;            //OUTPUT FILE for redirection(if any) but empty string(if not)
  std::string errFile;            //ERROR FILE for redirection(if any) but empty string(if not)
  EnvMap tempEnv;                 //a set for storing envrionment variables that have been SET
  EnvMap currEnv;                 //a set for storing environment variables that have been EXPORT
  std::string currDir;            //current working directory

 public:
  CommandLine(std::istream & stream,
              EnvMap & temp,
              EnvMap & curr,
              std::string & cwd);                    //constructor
  ~CommandLine();                                    //destructor
  std::string operator[](size_t index);              //index operator for specific arg
  const std::string operator[](size_t index) const;  //const index operator for specific arg
  std::string getLine();                             //get line
  size_t getNumArg();                                //get numArg
  std::vector<std::string> getArgs();                //get args
  EnvMap & getTempEnv();                             //get tempEnv
  EnvMap & getCurrEnv();                             //get currEnv
  std::string & getCurrDir();                        //get currDir
  void modifyTempEnv(EnvMap & newTempEnv);           //modify tempEnv for later update
  void modifyCurrEnv(EnvMap & newCurrEnv);           //modify currEnv for later update
  void modifyCurrDir(std::string newDir);            //modify currDir for later update
  void print();                                      //print line, in/out/err, argv
  void run();                                        //run the specified command
  void runSingleCmd();                               //run single command(numCmd = 1)
  void runPipedCmds();                               //run piped commands(numCmd > 1)
};

//constructor
CommandLine::CommandLine(std::istream & stream, EnvMap & temp, EnvMap & curr, std::string & cwd) :
    tempEnv(temp),
    currEnv(curr),
    currDir(cwd) {
  line = readLine(stream);                   //read the line from std::cin
  convertEval(line, tempEnv);                //do $ evaluation
  args = getGenArgs(line);                   //get general args
  stripEscp(args);                           //strip the excape symbols
  numCmd = getNumCmd(args);                  //decide the number of commands
  inFile = "", outFile = "", errFile = "";   //default initialization
  getRedir(args, inFile, outFile, errFile);  //decide inFile, outFile, errFile and shrink args
}

//destructor
CommandLine::~CommandLine() {}

std::string CommandLine::operator[](size_t index) {
  return args[index];
}

const std::string CommandLine::operator[](size_t index) const {
  return args[index];
}

std::string CommandLine::getLine() {
  return line;
}

std::vector<std::string> CommandLine::getArgs() {
  return args;
}

size_t CommandLine::getNumArg() {
  return args.size();
}

EnvMap & CommandLine::getTempEnv() {
  return tempEnv;
}

EnvMap & CommandLine::getCurrEnv() {
  return currEnv;
}

std::string & CommandLine::getCurrDir() {
  return currDir;
}

void CommandLine::modifyTempEnv(EnvMap & newTempEnv) {
  tempEnv = newTempEnv;
}

void CommandLine::modifyCurrEnv(EnvMap & newCurrEnv) {
  currEnv = newCurrEnv;
}

void CommandLine::modifyCurrDir(std::string newDir) {
  currDir = newDir;
}

//print inFile and outFile
void CommandLine::print() {
  std::cout << "line: " << line << "(End)" << std::endl;
  for (size_t i = 0; i < args.size(); i++) {
    std::cout << "arg" << i << ": " << args[i] << "(End)" << std::endl;
  }
  if (!inFile.empty()) {
    std::cout << "in: " << inFile << std::endl;
  }
  if (!outFile.empty()) {
    std::cout << "out: " << outFile << std::endl;
  }
  if (!errFile.empty()) {
    std::cout << "err: " << errFile << std::endl;
  }
  std::cout << "numCmd: " << numCmd << std::endl;
}

//void run
void CommandLine::run() {
  if (numCmd == 1) {
    runSingleCmd();
  }
  if (numCmd > 1) {
    runPipedCmds();
  }
}

//run single command
void CommandLine::runSingleCmd() {
  std::string ECE551PATH = "ECE551PATH";
  int ECE551PATHInd = currEnv.find(ECE551PATH);
  std::string ECE551PATHValue = currEnv[ECE551PATHInd].second;
  std::vector<std::string> paths = getSearchPaths(ECE551PATHValue);
  std::vector<char *> envp = currEnv.getEnvp();  //use currEnv
  std::vector<char *> argv = argsToArgv(args);
  int childPid = fork();
  switch (childPid) {
    case -1:
      perror("fork()");
      exit(EXIT_FAILURE);
    case 0:
      argv[0] = getProgPath(paths, argv[0], currDir);
      redirForSingleCmd(inFile, outFile, errFile);  //redirect for single command
      int ret;
      ret = execve(argv[0], &argv[0], &envp[0]);
      if (ret == -1) {
        perror("execve()");
        exit(EXIT_FAILURE);
      }
    default:
      int status;
      pid_t w;
      do {
        w = waitpid(childPid, &status, 0);
        if (w == -1) {
          perror("waitpid()");
          exit(EXIT_FAILURE);
        }
        if (WIFEXITED(status)) {
          std::cout << "Program exited with status 0" << std::endl;
          std::cout << "[replace 0 with the actual exit status]" << std::endl;
        }
        else if (WIFSIGNALED(status)) {
          std::cout << "Program was killed by signal 11fow" << std::endl;
          std::cout << "[replace 11 with the actual signal]" << std::endl;
        }
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  for (size_t i = 0; i < envp.size(); i++) {
    free(envp[i]);
  }
  for (size_t i = 0; argv[i] != NULL; i++) {
    free(argv[i]);
  }
}

//run piped commands
void CommandLine::runPipedCmds() {
  std::string ECE551PATH = "ECE551PATH";
  int ECE551PATHInd = currEnv.find(ECE551PATH);
  std::string ECE551PATHValue = currEnv[ECE551PATHInd].second;
  std::vector<std::string> paths = getSearchPaths(ECE551PATHValue);
  std::vector<char *> envp = currEnv.getEnvp();  //use currEnv
  //get arguments for multiple commands
  std::vector<std::vector<char *> > subArgvs = divideArgsForPipe(args);
  int numPipe = numCmd - 1;
  //create pipes
  int * pipeFds = new int[2 * numPipe];
  for (int p = 0; p < numPipe; p++) {
    if (pipe(pipeFds + 2 * p) < 0) {
      perror("pipe()");
      exit(EXIT_FAILURE);
    }
  }
  int childPid;
  int status;
  for (int i = 0; i < numCmd; i++) {
    childPid = fork();
    if (childPid == -1) {
      perror("fork()");
    }
    if (childPid == 0) {
      if (i == 0) {  //if first command
        std::string out = "";
        std::string err = "";
        redirForSingleCmd(inFile, out, err);
        dup2(pipeFds[1], 1);
      }
      else if (i == numCmd - 1) {  //if last command
        std::string in = "";
        redirForSingleCmd(in, outFile, errFile);
        dup2(pipeFds[2 * numCmd - 4], 0);
      }
      else {
        dup2(pipeFds[2 * i - 2], 0);
        dup2(pipeFds[2 * i + 1], 1);
      }
      for (int p = 0; p < 2 * numPipe; p++) {
        close(pipeFds[p]);
      }
      subArgvs[i][0] = getProgPath(paths, subArgvs[i][0], currDir);
      if (execve(subArgvs[i][0], &subArgvs[i][0], &envp[0]) < 0) {
        perror("execve()");
        exit(EXIT_FAILURE);
      }
    }
  }
  for (int p = 0; p < 2 * numPipe; p++) {
    close(pipeFds[p]);
  }
  for (int i = 0; i < numCmd; i++) {
    pid_t w;
    do {
      w = waitpid(-1, &status, 0);
      if (w == -1) {
        perror("waitpid()");
        exit(EXIT_FAILURE);
      }
      if (WIFEXITED(status)) {
        std::cout << "Program exited with status 0" << std::endl;
        std::cout << "[replace 0 with the actual exit status]" << std::endl;
      }
      else if (WIFSIGNALED(status)) {
        std::cout << "Program was killed by signal 11fow" << std::endl;
        std::cout << "[replace 11 with the actual signal]" << std::endl;
      }
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  //free memory
  delete[] pipeFds;
  for (int i = 0; i < numCmd; i++) {
    for (size_t j = 0; j < subArgvs[i].size(); j++) {
      free(subArgvs[i][j]);
    }
  }
  for (size_t i = 0; i < envp.size(); i++) {
    free(envp[i]);
  }
}

#endif
