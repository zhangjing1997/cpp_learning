#include "IntArray.h"

#include <assert.h>

#include <iostream>
#include <ostream>

//default constructor
IntArray::IntArray() : data(NULL), numElements(0) {}
IntArray::IntArray(int n) : data(new int[n]), numElements(n) {}

//copy constructor
IntArray::IntArray(const IntArray & rhs) : numElements(rhs.size()) {
  if (rhs.data == NULL) {
    data = rhs.data;
  }
  else {
    data = new int[numElements];
    for (int i = 0; i < numElements; i++) {
      data[i] = rhs[i];
    }
  }
}

//assignment constructor
IntArray & IntArray::operator=(const IntArray & rhs) {
  numElements = rhs.size();
  if (rhs.data == NULL) {
    data = NULL;
    return *this;
  }
  if (this != &rhs) {
    int * temp = new int[numElements];
    for (int i = 0; i < numElements; i++) {
      temp[i] = rhs[i];
    }
    delete[] data;
    data = temp;
  }
  return *this;
}

//[] operator
const int & IntArray::operator[](int index) const {
  assert(index < numElements && index >= 0);
  return data[index];
}

int & IntArray::operator[](int index) {
  assert(index < numElements && index >= 0);
  return data[index];
}

//size function
int IntArray::size() const {
  return numElements;
}

//== operator
bool IntArray::operator==(const IntArray & rhs) const {
  if (size() == rhs.size()) {
    if (rhs.data == NULL) {
      if (data == NULL) {
        return 1;
      }
      return 0;
    }
    // if size > 0, compare iteratively
    for (int i = 0; i < size(); i++) {
      if (data[i] != rhs[i]) {
        return 0;
      }
    }
    return 1;
  }
  return 0;
}

//!= operator
bool IntArray::operator!=(const IntArray & rhs) const {
  return ((*this == rhs) == 0);
}

//<< operator to print the array
std::ostream & operator<<(std::ostream & s, const IntArray & rhs) {
  if (rhs.size() == 0) {
    s << "{}";
  }
  else {
    s << "{" << rhs[0];
    for (int i = 1; i < rhs.size(); i++) {
      s << ", " << rhs[i];
    }
    s << "}";
  }
  return s;
}

//operator+ (wrote for 073)
IntArray IntArray::operator+(const IntArray & rhs) const {
  assert(numElements == rhs.size());
  for (int i = 0; i < numElements; i++) {
    data[i] += rhs[i];
  }
  return *this;
}

IntArray::~IntArray() {
  if (data != NULL) {
    delete[] data;
  }
}
