
#include <iostream>
#include <sstream>  //stringstream.str()
#include <string>

class Expression
{
 public:
  virtual std::string toString() const = 0;
  virtual long evaluate() const = 0;
  virtual Expression* clone = 0;
  virtual ~Expression() {}
};

class NumExpression : public Expression
{
  long num;

 public:
  explicit NumExpression(long n) : num(n) {}
  virtual std::string toString() const {
    std::ostringstream temp;
    temp << num;
    std::string str = temp.str();  //convert number to string by using stringstream
    return str;
  }
  virtual long evaluate() const { return num; }
  virtual Expression * clone() const{
    return new NumExpression(num);
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
  virtual long evaluate() const = 0;
  virtual Expression * clone() const{
    return new OperatorExpression(*this);
  }
  OperatorExpression(const OperatorExpression & rhs):E1(rhs.E1->clone()), E2(rhs.E2->clone()){}
  operatoeExpression& operator= (const OperatorExpression & rhs){
    if (this != &rhs){
      OperatorExpression temp(rhs);
      std::swap(E1, temp.E1);
      std::swap(E2, temp.E1);
    }
    return *this;
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
  virtual long evaluate() const { return E1->evaluate() + E2->evaluate(); }
};

class MinusExpression : public OperatorExpression
{
 public:
  MinusExpression(Expression * lhs, Expression * rhs) : OperatorExpression(lhs, rhs, '+') {}
  virtual long evaluate() const { return E1->evaluate() - E2->evaluate(); }
};

class TimesExpression : public OperatorExpression
{
 public:
  TimesExpression(Expression * lhs, Expression * rhs) : OperatorExpression(lhs, rhs, '+') {}
  virtual long evaluate() const { return E1->evaluate() * E2->evaluate(); }
};

class DivExpression : public OperatorExpression
{
 public:
  DivExpression(Expression * lhs, Expression * rhs) : OperatorExpression(lhs, rhs, '+') {}
  virtual long evaluate() const { return E1->evaluate() / E2->evaluate(); }
};
