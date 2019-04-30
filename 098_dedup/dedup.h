#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <vector>

class File
{
 protected:
  std::string filename;
  std::vector<std::string> lines;

 public:
  File(std::string f) : filename(f), lines(std::vector<std::string>()) { readFile(); }
  std::vector<std::string> getlines() const { return lines; }
  std::string getfilename() { return filename; }

  //helper function: read file
  void readFile() {
    std::ifstream ifs(
        filename.c_str());  //the same file could be opened ONLY ONCE without closing previously
    if (!ifs) {
      std::cerr << "cannot open the file: " << filename << std::endl;
      exit(EXIT_FAILURE);
    }
    //std::cout << "open the file: " << filename << std::endl;
    std::string line;
    while (!ifs.eof()) {
      getline(ifs, line);
      lines.push_back(line);
    }
    ifs.close();
  }

  bool operator==(File & rhs) {
    if (lines.size() == rhs.lines.size()) {
      for (size_t i = 0; i < lines.size(); i++) {
        if (lines[i].compare(rhs.lines[i]) != 0) {
          return false;
        }
      }
      return true;
    }
    return false;
  }
};

//inherit from File
class FileHasher : public File
{
 private:
  size_t hashval;

 public:
  FileHasher(std::string f) : File(f), hashval() { hash(); }

  size_t gethashval() { return hashval; }

  //hash function
  void hash() {
    std::string lines_str;
    std::hash<std::string> str_hash;
    for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it) {
      lines_str.append(*it);
    }
    hashval = str_hash(lines_str);
  }
};

//this is a hash table to store FileHasher as the vector's element
class FileTable
{
 private:
  std::vector<std::list<FileHasher> > * table;
  size_t size;

 public:
  FileTable() : table(new std::vector<std::list<FileHasher> >(1000)) {}
  ~FileTable() { delete table; }

  //helper function for read directory recursively
  void addCheckRemove(std::string filename) {
    FileHasher fh(filename);
    size_t pos = fh.gethashval() % size;
    if ((*table)[pos].size() == 0) {
      (*table)[pos].push_front(fh);
      return;
    }
    else {
      for (std::list<FileHasher>::iterator it = (*table)[pos].begin(); it != (*table)[pos].end();
           ++it) {
        if (fh.gethashval() == (*it).gethashval()) {
          if (fh == (*it)) {
            std::cout << "#Removing " << filename << " (duplicate of " << (*it).getfilename()
                      << ")." << std::endl;
            std::cout << "rm " << filename << std::endl;
            return;
          }
        }
      }
      (*table)[pos].push_front(fh);  //THE LAST BUG
    }
  }

  void readDir(std::string dirname) {
    DIR * dirp;
    dirp = opendir(dirname.c_str());
    std::string strdir(dirname);
    if (dirp == NULL) {
      std::cerr << "cannot open the directory" << std::endl;
      closedir(dirp);
      exit(EXIT_FAILURE);
    }
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
      std::string strfile(dp->d_name);
      if (strfile == "." || strfile == "..") {  //could be seen when using command "ls -a"
        continue;
      }
      if (dp->d_type == DT_LNK) {
        continue;
      }
      if (dp->d_type == DT_REG) {
        std::string filepathname = strdir + "/" + dp->d_name;
        //std::cout << "addCheckRemove file: " << filepathname << std::endl;
        addCheckRemove(filepathname);  //add->check(->remove) if regular file
        continue;
      }
      if (dp->d_type == DT_DIR) {
        std::string dirpathname = strdir + "/" + dp->d_name;
        //std::cout << "read directory: " << dirpathname << std::endl;
        readDir(dirpathname);  //recursion if sub-directory
      }
    }
    if (closedir(dirp) != 0) {
      std::cerr << "cannot close the directory" << std::endl;
      exit(EXIT_FAILURE);
    }
  }
};
