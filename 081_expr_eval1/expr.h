#ifndef __EXPR_H__
#define __EXPR_H__

#include <iostream>
#include <sstream>
#include <string>

class Expression
{
 public:
  virtual std::string toString() const = 0;
  virtual ~Expression() {}
};

class NumExpression : public Expression
{
  long num;

 public:
  NumExpression(long n) : num(n) {}
  virtual std::string toString() const {
    std::ostringstream temp;
    temp << num;
    std::string str = temp.str();  //convert number to string by using stringstream
    return str;
  }
  virtual ~NumExpression() {}
};

class PlusExpression : public Expression
{
  Expression * E1;
  Expression * E2;

 public:
  PlusExpression(Expression * lhs, Expression * rhs) : E1(lhs), E2(rhs) {}
  virtual std::string toString() const {
    std::string str1 = E1->toString();
    std::string str2 = E2->toString();
    std::string str = "(" + str1 + " + " + str2 + ")";
    return str;
  }
  virtual ~PlusExpression() {
    delete E1;
    delete E2;
  }
};

#endif
