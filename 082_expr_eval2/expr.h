
#include <iostream>
#include <sstream>  //stringstream.str()
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

class OperatorExpression : public Expression
{
 protected:
  Expression * E1;
  Expression * E2;
  char op;

 public:
  OperatorExpression(Expression * lhs, Expression * rhs, char c) : E1(lhs), E2(rhs), op(c) {}
  virtual std::string toString() const {
    std::ostringstream temp;
    temp << "(" << E1->toString() << " " << op << " " << E2->toString() << ")";
    return temp.str();
  }
  virtual ~OperatorExpression() {
    delete E1;
    delete E2;
  }
};

class PlusExpression : public OperatorExpression
{
 public:
  PlusExpression(Expression * lhs, Expression * rhs) : OperatorExpression(lhs, rhs, '+') {}
};

class MinusExpression : public OperatorExpression
{
 public:
  MinusExpression(Expression * lhs, Expression * rhs) : OperatorExpression(lhs, rhs, '-') {}
};

class TimesExpression : public OperatorExpression
{
 public:
  TimesExpression(Expression * lhs, Expression * rhs) : OperatorExpression(lhs, rhs, '*') {}
};

class DivExpression : public OperatorExpression
{
 public:
  DivExpression(Expression * lhs, Expression * rhs) : OperatorExpression(lhs, rhs, '/') {}
};
