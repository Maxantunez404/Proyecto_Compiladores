#ifndef IMP
#define IMP

#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>

#include <list>
#include <vector>

#include "imp_type.hh"

using namespace std;

class ImpVisitor;
class TypeVisitor;

enum BinaryOp { PLUS, MINUS, MULT, DIV, EXP, LT, LTEQ, EQ, AND, OR};
enum UnaryOp { NEG, NOT };
  
class Exp {
public:
  virtual int accept(ImpVisitor* v) = 0;
  virtual ImpType accept(TypeVisitor* v) = 0;
  static string binopToString(BinaryOp op);
  virtual ~Exp() = 0;
};

class BinaryExp : public Exp {
public:
  Exp *left, *right;
  BinaryOp op;
  BinaryExp(Exp* l, Exp* r, BinaryOp op);
  int accept(ImpVisitor* v);
  ImpType accept(TypeVisitor* v);
  ~BinaryExp();
};

class UnaryExp : public Exp {
public:
  Exp* e;
  UnaryOp op;
  UnaryExp(Exp* e, UnaryOp op);
  int accept(ImpVisitor* v);
  ImpType accept(TypeVisitor* v);
  ~UnaryExp();
};

class NumberExp : public Exp {
public:
  int value;
  NumberExp(int v);
  int accept(ImpVisitor* v);
  ImpType accept(TypeVisitor* v);
  ~NumberExp();
};

class BoolConstExp : public Exp {
public:
  bool b;
  BoolConstExp(bool v);
  int accept(ImpVisitor* v);
  ImpType accept(TypeVisitor* v);
  ~BoolConstExp();
};

class IdExp : public Exp {
public:
  string id;
  IdExp(string id);
  int accept(ImpVisitor* v);
  ImpType accept(TypeVisitor* v);
  ~IdExp();
};

class ParenthExp : public Exp {
public:
  Exp *e;
  ParenthExp(Exp *e);
  int accept(ImpVisitor* v);
  ImpType accept(TypeVisitor* v);
  ~ParenthExp();
};

class CondExp : public Exp {
public:
  Exp *cond, *etrue, *efalse;
  CondExp(Exp* c, Exp* et, Exp* ef);
  int accept(ImpVisitor* v);
  ImpType accept(TypeVisitor* v);
  ~CondExp();
};



class Stm {
public:
  virtual int accept(ImpVisitor* v) = 0;
  virtual void accept(TypeVisitor* v) = 0;
  virtual ~Stm() = 0;
};

class StatementList;
class Body;

class AssignStatement : public Stm {
public:
  string id;
  Exp* rhs;  
  AssignStatement(string id, Exp* e);
  int accept(ImpVisitor* v);
  void accept(TypeVisitor* v);
  ~AssignStatement();
};

class PrintStatement : public Stm {
public:
  Exp* e;  
  PrintStatement(Exp* e);
  int accept(ImpVisitor* v);
  void accept(TypeVisitor* v);
  ~PrintStatement();
};

class IfStatement : public Stm {
public:
  Exp* cond;
  Body *tbody, *fbody;
  IfStatement(Exp* c, Body* tbody, Body *fbody);
  int accept(ImpVisitor* v);
  void accept(TypeVisitor* v);
  ~IfStatement();
};

class WhileStatement : public Stm {
public:
  Exp* cond;
  Body *body;
  WhileStatement(Exp* c, Body* b);
  int accept(ImpVisitor* v);
  void accept(TypeVisitor* v);
  ~WhileStatement();
};

class DoWhileStatement : public Stm {
public:
  Exp* cond;
  Body *body;
  DoWhileStatement(Exp* c, Body* b);
  int accept(ImpVisitor* v);
  void accept(TypeVisitor* v);
  ~DoWhileStatement();
};

class ForStatement : public Stm {
public:
  string id;
  Exp* e1,*e2;
  Body *body;
  ForStatement(string id, Exp* e1, Exp* e2, Body* b);
  int accept(ImpVisitor* v);
  void accept(TypeVisitor* v);
  ~ForStatement();
};


class StatementList {
public:
  list<Stm*> slist;
  StatementList();
  void add(Stm* s);
  int accept(ImpVisitor* v);
  void accept(TypeVisitor* v);
  ~StatementList();
};


class VarDec {
public:
  string type;
  list<string> vars;
  VarDec(string type, list<string> vars);
  int accept(ImpVisitor* v);
  void accept(TypeVisitor* v);  
  ~VarDec();
};


class VarDecList {
public:
  list<VarDec*> vdlist;
  VarDecList();
  void add(VarDec* s);
  int accept(ImpVisitor* v);
  void accept(TypeVisitor* v);  
  ~VarDecList();
};


class Body {
public:
  VarDecList* var_decs;
  StatementList* slist;
  Body(VarDecList* vdl, StatementList* sl);
  int accept(ImpVisitor* v);
  void accept(TypeVisitor* v);  
  ~Body();
};

class Program {
public:
  Body* body;
  Program(Body* body);
  int accept(ImpVisitor* v);
  void accept(TypeVisitor* v);  
  ~Program();
};



#endif

