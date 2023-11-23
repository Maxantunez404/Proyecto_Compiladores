#include "imp.hh"
#include "imp_visitor.hh"
#include "type_visitor.hh"


string Exp::binopToString(BinaryOp op) {
  switch(op) {
  case PLUS: return "+";
  case MINUS: return "-";
  case MULT: return "*";
  case DIV: return "/";
  case EXP: return "**";
  case LT: return "<";
  case LTEQ: return "<=";
  case EQ: return "==";
  case AND: return "and";
  case OR: return "or";
  }
  return "";
}


// Constructors
BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp op):left(l),right(r),op(op) {}
UnaryExp::UnaryExp(Exp* e, UnaryOp op): e(e), op(op) {}
NumberExp::NumberExp(int v):value(v) {}
BoolConstExp::BoolConstExp(bool v):b(v) {}
IdExp::IdExp(string id):id(id) {}
ParenthExp::ParenthExp(Exp *e):e(e){}
CondExp::CondExp(Exp *c, Exp* et, Exp* ef):cond(c), etrue(et), efalse(ef){}

Exp::~Exp() {}
BinaryExp::~BinaryExp() { delete left; delete right; }
UnaryExp::~UnaryExp() { }
NumberExp::~NumberExp() { }
BoolConstExp::~BoolConstExp() { }
IdExp::~IdExp() { }
ParenthExp::~ParenthExp(){ delete e; }
CondExp::~CondExp(){ delete cond; delete etrue; delete efalse; }

int BinaryExp::accept(ImpVisitor* v) {
  return v->visit(this);
}

int UnaryExp::accept(ImpVisitor* v) {
  return v->visit(this);
}

int NumberExp::accept(ImpVisitor* v) {
  return v->visit(this);
}

int BoolConstExp::accept(ImpVisitor* v) {
  return v->visit(this);
}

int IdExp::accept(ImpVisitor* v) {
  return v->visit(this);
}

int ParenthExp::accept(ImpVisitor* v) {
  return v->visit(this);
}

int CondExp::accept(ImpVisitor* v) {
  return v->visit(this);
}



// type visitor
ImpType BinaryExp::accept(TypeVisitor* v) {
  return v->visit(this);
}

ImpType UnaryExp::accept(TypeVisitor* v) {
  return v->visit(this);
}

ImpType NumberExp::accept(TypeVisitor* v) {
  return v->visit(this);
}

ImpType BoolConstExp::accept(TypeVisitor* v) {
  return v->visit(this);
}

ImpType IdExp::accept(TypeVisitor* v) {
  return v->visit(this);
}

ImpType ParenthExp::accept(TypeVisitor* v) {
  return v->visit(this);
}

ImpType CondExp::accept(TypeVisitor* v) {
  return v->visit(this);
}

AssignStatement::AssignStatement(string id, Exp* e):id(id), rhs(e) { }
PrintStatement::PrintStatement(Exp* e):e(e) { }
IfStatement::IfStatement(Exp* c,Body *tb, Body* fb):cond(c),tbody(tb), fbody(fb) { }
WhileStatement::WhileStatement(Exp* c,Body *b):cond(c),body(b) { }
ForStatement::ForStatement(string id, Exp* e1,Exp* e2, Body *b):id(id),e1(e1),e2(e2), body(b) { }

StatementList::StatementList():slist() {}
VarDec::VarDec(string type, list<string> vars):type(type), vars(vars) {}
VarDecList::VarDecList():vdlist() {}
Body::Body(VarDecList* vdl, StatementList* sl):var_decs(vdl), slist(sl) {}
Program::Program(Body* b):body(b) {}

Stm::~Stm() {}
AssignStatement::~AssignStatement() { delete rhs; }
PrintStatement::~PrintStatement() { delete e; }
IfStatement::~IfStatement() { delete fbody; delete tbody; delete cond; }
WhileStatement::~WhileStatement() { delete body; delete cond; }
ForStatement::~ForStatement() { delete body; delete e2; delete e1; }

StatementList::~StatementList() { }
VarDec::~VarDec() { }
VarDecList::~VarDecList() { }
Body::~Body() { delete slist; delete var_decs; }
Program::~Program() { delete body; }

int AssignStatement::accept(ImpVisitor* v) {
  return v->visit(this);
}

int PrintStatement::accept(ImpVisitor* v) {
  return v->visit(this);
}

int IfStatement::accept(ImpVisitor* v) {
  return v->visit(this);
}

int WhileStatement::accept(ImpVisitor* v) {
  return v->visit(this);
}

int ForStatement::accept(ImpVisitor* v) {
  return v->visit(this);
}

void StatementList::add(Stm* s) { slist.push_back(s);  }

int StatementList::accept(ImpVisitor* v) {
  return v->visit(this);
}

int VarDec::accept(ImpVisitor* v) {
  return v->visit(this);
}

void VarDecList::add(VarDec* vd) { vdlist.push_back(vd);  }

int VarDecList::accept(ImpVisitor* v) {
  return v->visit(this);
}

int Body::accept(ImpVisitor* v) {
  return v->visit(this);
}

int Program::accept(ImpVisitor* v) {
  return v->visit(this);
}

// Type visitor
void AssignStatement::accept(TypeVisitor* v) {
  return v->visit(this);
}

void PrintStatement::accept(TypeVisitor* v) {
  return v->visit(this);
}

void IfStatement::accept(TypeVisitor* v) {
  return v->visit(this);
}

void WhileStatement::accept(TypeVisitor* v) {
  return v->visit(this);
}

void ForStatement::accept(TypeVisitor* v) {
  return v->visit(this);
}


void StatementList::accept(TypeVisitor* v) {
  return v->visit(this);
}

void VarDec::accept(TypeVisitor* v) {
  return v->visit(this);
}

void VarDecList::accept(TypeVisitor* v) {
  return v->visit(this);
}

void Body::accept(TypeVisitor* v) {
  return v->visit(this);
}

void Program::accept(TypeVisitor* v) {
  return v->visit(this);
}









