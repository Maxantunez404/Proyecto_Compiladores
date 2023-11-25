#include "imp_printer.hh"

void ImpPrinter::print(Program* p) {
  p->accept(this);
  return;
}

int ImpPrinter::visit(Program* p) {
  p->body->accept(this);
  return 0;
}

int ImpPrinter::visit(Body * b) {
  b->var_decs->accept(this);
  b->slist->accept(this);
  return 0;
}

int ImpPrinter::visit(VarDecList* s) {
  list<VarDec*>::iterator it;
  for (it = s->vdlist.begin(); it != s->vdlist.end(); ++it) {
    (*it)->accept(this);
    cout << ";" << endl;
  }  
  return 0;
}
			  
int ImpPrinter::visit(VarDec* vd) {
  bool first = true;
  cout << "var " << vd->type << " ";
  list<string>::iterator it;
  for (it = vd->vars.begin(); it != vd->vars.end(); ++it){
    if (!first) cout << ", ";
    first = false;
    cout << *it;
  }
  return 0;
}

int ImpPrinter::visit(StatementList* s) {
  list<Stm*>::iterator it;
  for (it = s->slist.begin(); it != s->slist.end(); ++it) {
    (*it)->accept(this);
    cout << ";" << endl;
  }
  return 0;
}

int ImpPrinter::visit(AssignStatement* s) {
  cout << s->id << " = ";
  s->rhs->accept(this);
  return 0;
}

int ImpPrinter::visit(PrintStatement* s) {
  cout << "print(";
  s->e->accept(this);
  cout << ")";
  return 0;
}

int ImpPrinter::visit(IfStatement* s) {
  cout << "if (";
  s->cond->accept(this);
  cout << ") then" << endl;;
  s->tbody->accept(this);
  if (s->fbody!=NULL) {
    cout << "else" << endl;
    s->fbody->accept(this);
  }
  cout << "endif";
  return 0;
}

int ImpPrinter::visit(WhileStatement* s) {
  cout << "while (";
  s->cond->accept(this);
  cout << ") do" << endl;;
  s->body->accept(this);
  cout << "endwhile";
  return 0;
}

int ImpPrinter::visit(DoWhileStatement* s) {
    cout << "do" << endl;
    s->body->accept(this);
    cout << "while (";
    s->cond->accept(this);
    cout << ");" << endl;
    return 0;
}

int ImpPrinter::visit(ForStatement* s) {
  cout << "for " << s->id << " : ";
  s->e1->accept(this);
  cout << " , " << endl;
  s->e2->accept(this);
  cout << "do" << endl;
  s->body->accept(this);
  cout << "endfor"; //correcion de endwhile
  return 0;
}

int ImpPrinter::visit(BreakStatement *s) {
    cout << "break" << endl;
    return 0;
}

int ImpPrinter::visit(ContinueStatement *s) {
    cout << "continue" << endl;
    return 0;
}

int ImpPrinter::visit(BinaryExp* e) {
  e->left->accept(this);
  cout << ' ' << Exp::binopToString(e->op) << ' ';
  e->right->accept(this);
  return 0;
}

int ImpPrinter::visit(UnaryExp* e) {
  UnaryOp op = e->op;
  if (op==NEG) cout << "-";
  else if (op==NOT) cout << "!";
  else {
    cout << "Printer: operator not implemented" << endl;
    exit(0);
  }
  e->e->accept(this);  
  return 0;
}

int ImpPrinter::visit(NumberExp* e) {
  cout << e->value;
  return 0;
}

int ImpPrinter::visit(BoolConstExp* e) {
  if (e->b)
    cout << "true";
  else
    cout << "false";
  return 0;
}

int ImpPrinter::visit(IdExp* e) {
  cout << e->id;
  return 0;
}

int ImpPrinter::visit(ParenthExp* ep) {
  cout << '(';
  ep->e->accept(this);
  cout << ')';
  return 0;
}

int ImpPrinter::visit(CondExp* e) {
  cout << "ifexp(";
  e->cond->accept(this);
  cout << ",";
  e->etrue->accept(this);
  cout << ",";
  e->efalse->accept(this);
  cout << ')';
  return 0;
}
