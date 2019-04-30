#include "IntMatrix.h"

#include "IntArray.h"

IntMatrix::IntMatrix() : numRows(0), numColumns(0), rows(NULL) {}

IntMatrix::IntMatrix(int r, int c) : numRows(r), numColumns(c) {
  rows = new IntArray *[numRows];
  for (int i = 0; i < numRows; i++) {
    IntArray temp(numColumns);
    rows[i] = new IntArray();
    //should use "classType * = new classType();" rather than "classType * = new classType[1];"
    *rows[i] = temp;
  }
}

//copy constructor
IntMatrix::IntMatrix(const IntMatrix & rhs) : numRows(rhs.numRows), numColumns(rhs.numColumns) {
  if (rhs.rows == NULL) {
    rows = NULL;
  }
  else {
    rows = new IntArray *[numRows];  //allocate memory for array of IntArray *
    for (int i = 0; i < numRows; i++) {
      rows[i] = new IntArray();
      *rows[i] = rhs[i];
    }
  }
}

//assignment copy operator
IntMatrix & IntMatrix::operator=(const IntMatrix & rhs) {
  numRows = rhs.numRows;
  numColumns = rhs.numColumns;
  if ((rhs.rows) == NULL) {
    rows = NULL;
    return *this;
  }
  if (this != &rhs) {
    IntArray ** temp = new IntArray *[numRows];
    for (int i = 0; i < numRows; i++) {
      temp[i] = new IntArray();
      *temp[i] = rhs[i];
    }
    delete[] rows;
    rows = temp;
  }
  return *this;
}

//get the number of rows
int IntMatrix::getRows() const {
  return numRows;
}

//get the number of columns
int IntMatrix::getColumns() const {
  return numColumns;
}

//return the reference of some row according to index
const IntArray & IntMatrix::operator[](int index) const {
  assert(index < numRows && index >= 0);
  return *(rows[index]);
}

IntArray & IntMatrix::operator[](int index) {
  assert(index < numRows && index >= 0);
  return *(rows[index]);
}

//compare two matrices for equality
bool IntMatrix::operator==(const IntMatrix & rhs) const {
  if (numRows == rhs.getRows() && numColumns == rhs.getColumns()) {
    if (rhs.rows == NULL) {
      if (rows == NULL) {
        return 1;
      }
      return 0;
    }
    for (int i = 0; i < numRows; i++) {
      if (*(rows[i]) != rhs[i]) {
        return 0;
      }
    }
    return 1;
  }
  return 0;
}

//Matrix +operator
IntMatrix IntMatrix::operator+(const IntMatrix & rhs) const {
  assert(numRows == rhs.getRows());
  for (int i = 0; i < numRows; i++) {
    //IntArray temp = *(rows[i]);
    //temp + rhs[i];
    *(rows[i]) + rhs[i];
  }
  return *this;
}

//<<operator printing the Matrix
std::ostream & operator<<(std::ostream & s, const IntMatrix & rhs) {
  if (rhs.getRows() == 0) {
    s << "[  ]";
  }
  else {
    s << "[ ";
    for (int i = 0; i < rhs.getRows() - 1; i++) {
      s << rhs[i] << ",\n";
    }
    s << rhs[rhs.getRows() - 1] << " ]";
  }
  return s;
}

//destructor
IntMatrix::~IntMatrix() {
  if (rows != NULL) {
    for (int i = 0; i < numRows; i++) {
      delete rows[i];
    }
    delete[] rows;
  }
}
