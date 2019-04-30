#ifndef __T_MATRIX_H___
#define __T_MATRIX_H___

#include <assert.h>

#include <cstdlib>
#include <iostream>
#include <vector>

//YOUR CODE GOES HERE!
template<typename T>
class Matrix
{
 private:
  int numRows;
  int numColumns;
  std::vector<T> ** rows;

 public:
  Matrix() : numRows(0), numColumns(0), rows(NULL) {}
  Matrix(int r, int c) : numRows(r), numColumns(c), rows(new std::vector<T> *[numRows]) {
    for (int i = 0; i < numRows; i++) {
      rows[i] = new std::vector<T>(numColumns);
    }
  }

  Matrix(const Matrix & rhs) : numRows(rhs.numRows), numColumns(rhs.numColumns) {
    if (rhs.rows == NULL) {
      rows = NULL;
    }
    else {
      rows = new std::vector<T> *[numRows];
      for (int i = 0; i < numRows; i++) {
        rows[i] = new std::vector<T>;
        *rows[i] = rhs[i];  //vector = (assignment copy)
      }
    }
  }

  ~Matrix() {
    if (rows != NULL) {
      for (int i = 0; i < numRows; i++) {
        delete rows[i];
      }
      delete[] rows;
    }
  }

  Matrix & operator=(const Matrix & rhs) {
    numRows = rhs.numRows;
    numColumns = rhs.numColumns;
    if (rhs.rows == NULL) {
      rows = NULL;
      return *this;
    }
    if (this != &rhs) {
      std::vector<T> ** temp = new std::vector<T> *[numRows];
      for (int i = 0; i < numRows; i++) {
        temp[i] = new std::vector<T>;
        *temp[i] = rhs[i];  //vector = (assignment copy)
      }
      delete[] rows;
      rows = temp;
    }
    return *this;
  }

  int getRows() const { return numRows; }

  int getColumns() const { return numColumns; }

  const std::vector<T> & operator[](int index) const {
    assert(index < numRows && index >= 0);
    return *rows[index];
  }

  std::vector<T> & operator[](int index) {
    assert(index < numRows && index >= 0);
    return *rows[index];
  }

  bool operator==(const Matrix & rhs) const {
    if (numRows == rhs.getRows() && numColumns == rhs.getColumns()) {
      if (rhs.rows == NULL) {
        if (rows == NULL) {
          return true;
        }
        return false;
      }
      for (int i = 0; i < numRows; i++) {
        if (*rows[i] != rhs[i]) {
          return false;
        }
      }
      return true;
    }
    return false;
  }

  Matrix operator+(const Matrix & rhs) {
    assert(numRows == rhs.getRows());
    assert(numColumns == rhs.getColumns());
    for (int i = 0; i < numRows; i++) {
      //*rows[i] + rhs[i];
      for (int j = 0; j < numColumns; j++) {
        (*this)[i][j] += rhs[i][j];
      }
    }
    return *this;
  }

  friend std::ostream & operator<<(std::ostream & s, const Matrix<T> & rhs) {
    if (rhs.getRows() == 0) {
      s << "[  ]";
      return s;
    }
    else {
      s << "[ ";
      for (int i = 0; i < rhs.getRows(); i++) {
        s << rhs[i];
        if (i != rhs.getRows() - 1) {
          s << "," << std::endl;  //line-endings before the last line
        }
        else {
          s << " ]";  //the last line-ending
        }
      }
    }
    return s;
  }
};

template<typename T>
std::ostream & operator<<(std::ostream & s, const std::vector<T> & rhs) {
  s << "{";
  for (size_t j = 0; j < rhs.size(); j++) {
    if (j == 0) {
      s << rhs[j];  //the first element in each line
    }
    else {
      s << ", " << rhs[j];  //other elements in this line
    }
  }
  s << "}";
  return s;
}
#endif
