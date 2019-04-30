#ifndef _PATH_H__
#define _PATH_H__

#include <dirent.h>
#include <limits.h>
#include <unistd.h>

#include <string>
#include <vector>

#include "parse.h"

std::vector<std::string> getSearchPaths(std::string & PATH);
std::string getParrDir(std::string & currDir);
char * getProgPath(std::vector<std::string> & paths, char * prog, std::string & currDir);

//parse environment variable PATH into vector of paths
std::vector<std::string> getSearchPaths(std::string & PATH) {
  size_t pos = 0;
  size_t nextColon;
  std::vector<std::string> paths;
  while ((nextColon = PATH.find(':', pos)) != std::string::npos) {
    std::string path = copySubStr(PATH, pos, nextColon);
    pos = nextColon + 1;
    paths.push_back(path);
  }
  std::string path = copySubStr(PATH, pos, PATH.size());
  paths.push_back(path);
  return paths;
}

//helper function for findProgPath : get the parent directory
std::string getParrDir(std::string & currDir) {
  std::string ans;
  char parrDir[PATH_MAX + 1];
  if (chdir("..") != 0) {  //change to the parent directory
    perror("chdir(..)");
  }
  if (getcwd(parrDir, sizeof(parrDir)) == NULL) {
    perror("getcwd()");
  }
  ans = parrDir;
  if (chdir(currDir.c_str()) != 0) {
    perror("chdir(): back to cwd");  //change back to the current directory
  }
  return ans;
}

//among paths, find the path of the specified program
char * getProgPath(std::vector<std::string> & paths, char * prog, std::string & currDir) {
  char * progPath = NULL;
  std::string progStr = prog;
  if (progStr.find('/', 0) != std::string::npos) {
    //expand relative path if any
    if (progStr.find('.', 0) == 0) {
      if (progStr.find('.', 1) == std::string::npos) {  //expand according to the current directory
        progStr.erase(0, 1);
        progStr.insert(0, currDir);
      }
      else if (progStr.find('.', 1) == 1) {  //expand according to the parrent directory
        std::string parrDir = getParrDir(currDir);
        progStr.erase(0, 2);
        progStr.insert(0, parrDir);
      }
      free(prog);
      progPath = strdup(progStr.c_str());
    }
    else {
      progPath = prog;  //prog itself is an absolute path
    }
  }
  else {  //search all the directories given by paths(from ECE551PATH)
    for (std::vector<std::string>::iterator it = paths.begin(); it != paths.end(); ++it) {
      std::string dirname = *it;
      DIR * dirp;
      dirp = opendir(dirname.c_str());
      if (dirp == NULL) {
        std::cerr << "cannot open the directory: " << dirname << std::endl;
        exit(EXIT_FAILURE);
      }
      struct dirent * dp;
      while ((dp = readdir(dirp)) != NULL) {
        if (strcmp(dp->d_name, prog) == 0) {
          progStr = dirname + '/' + prog;
          free(prog);
          progPath = strdup(progStr.c_str());
          break;
        }
      }
      if (closedir(dirp) != 0) {
        std::cerr << "cannot close the directory: " << dirname << std::endl;
      }
      if (progPath != NULL) {
        break;  //stop searching when found the target directory
      }
    }
    if (progPath == NULL) {
      std::cout << "Command " << prog << " not found" << std::endl;
      std::cout << "[replace " << prog << " with the actual command name]" << std::endl;
    }
  }
  return progPath;
}

#endif
