#include <fcntl.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "builtin.h"
#include "cmdLine.h"
#include "envMap.h"
#include "parse.h"

int main() {
  //Initialize environment variables
  EnvMap tempEnv;
  EnvMap currEnv;
  //Initialize current working directory as /home/jz268/ece551/mp_miniproject"
  if (chdir("/home/jz268/ece551/mp_miniproject") != 0) {
    perror("chdir()");
  }
  //Initialize prompt
  std::string currDir = "/home/jz268/ece551/mp_miniproject";
  std::string prompt = "myShell:" + currDir + " $ ";
  //Loop for executing
  for (;;) {
    std::cout << prompt;  //show prompt waiting for user's input
    CommandLine cl(std::cin, tempEnv, currEnv, currDir);
    //check if eof is encountered
    if (std::cin.eof()) {
      return EXIT_FAILURE;
    }
    //check if the command is "exit"
    if (cl.getNumArg() > 0 && cl[0].compare("exit") == 0) {
      return EXIT_FAILURE;
    }
    //check if there is not any input
    if (cl.getNumArg() == 0) {
      continue;
    }
    //RUN THE COMMANDS GIVEN BY STANDARD INPUT
    bool isBuiltin = false;
    bool builtinExecuted = false;
    runBuiltins(cl, isBuiltin, builtinExecuted);
    //if it is builtin command
    if (isBuiltin == true) {
      //if it is executed successfully
      if (builtinExecuted == true) {
        //after executing built-in command
        tempEnv = cl.getTempEnv();              //update tempEnv
        currEnv = cl.getCurrEnv();              //update currEnv
        currDir = cl.getCurrDir();              //update currDir
        prompt = "myShell:" + currDir + " $ ";  //update prompt
      }
      continue;
    }
    //if it is NOT builtin command
    else {
      cl.run();  //run it as an un-builtin command
    }
  }
  return EXIT_SUCCESS;
}
