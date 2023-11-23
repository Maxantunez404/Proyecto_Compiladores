#ifndef IMP_INTERPRETER
#define IMP_INTERPRETER

#include <unordered_map>

#include "imp.hh"
#include "imp_visitor.hh"
#include "environment.hh"

using namespace std;

class ImpInterpreter : public ImpVisitor {
private:
  Environment<int> env;

public:
  int interpret(Program*);
  int visit(Program*);
  int visit(Body*);
  int visit(VarDecList*);
  int visit(VarDec*);
  int visit(StatementList*);
  int visit(AssignStatement*);
  int visit(PrintStatement*);
  int visit(IfStatement*);
  int visit(WhileStatement*);
  int visit(ForStatement*);
  
  int visit(BinaryExp* e);
  int visit(UnaryExp* e);
  int visit(NumberExp* e);
  int visit(BoolConstExp* e);
  int visit(IdExp* e);
  int visit(ParenthExp* e);
  int visit(CondExp* e);
};


#endif

