#include <math.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "function.h"

int binarySearchForZero(Function<int, int> * f, int low, int high);

class CountedIntFn : public Function<int, int>
{
 protected:
  unsigned remaining;
  Function<int, int> * f;
  const char * mesg;

 public:
  CountedIntFn(unsigned n, Function<int, int> * fn, const char * m) :
      remaining(n),
      f(fn),
      mesg(m) {}
  virtual int invoke(int arg) {
    if (remaining == 0) {
      fprintf(stderr, "Too many function invocations in %s\n", mesg);
      exit(EXIT_FAILURE);
    }
    remaining--;
    return f->invoke(arg);
  }
};

class SinFunction : public Function<int, int>
{
 public:
  virtual int invoke(int arg) { return 10000000 * (sin(arg / 100000.0) - 0.5); }
};

class LinearFunction : public Function<int, int>
{
 public:
  virtual int invoke(int arg) { return 2 * arg - 3; }
};

void check(Function<int, int> * f, int low, int high, int expected_ans, const char * mesg) {
  int max_invct;
  if (high > low) {
    max_invct = (int)log2((double)(high - low)) + 1;
  }
  else {
    max_invct = 1;
  }
  CountedIntFn cf(max_invct, f, mesg);
  if (expected_ans != binarySearchForZero(&cf, low, high)) {
    std::cout << "unexpected search answer";
    exit(EXIT_FAILURE);
  }
}

int main() {
  //SinFunction s;
  LinearFunction f;
  //check(&s, 0, 150000, 52359, "SinFunction");
  check(&f, 0, 4, 1, "LinearFunction");
  std::cout << "Good";
  return EXIT_SUCCESS;
}
