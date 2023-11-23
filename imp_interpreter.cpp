#include "imp_interpreter.hh"

int ImpInterpreter::interpret(Program* p) {
  env.clear();
  p->accept(this);
  return 0;
}

int ImpInterpreter::visit(Program* p) {
  p->body->accept(this);
  return 0;
}

int ImpInterpreter::visit(Body* b) {
  env.add_level(); 
  b->var_decs->accept(this);
  b->slist->accept(this);
  env.remove_level();  
  return 0;
}


int ImpInterpreter::visit(VarDecList* decs) {
  list<VarDec*>::iterator it;
  for (it = decs->vdlist.begin(); it != decs->vdlist.end(); ++it) {
    (*it)->accept(this);
  }  
  return 0;
}

int ImpInterpreter::visit(VarDec* vd) {
  list<string>::iterator it;
  for (it = vd->vars.begin(); it != vd->vars.end(); ++it) {
    env.add_var(*it);
  }   
  return 0;
}

int ImpInterpreter::visit(StatementList* s) {
  list<Stm*>::iterator it;
  for (it = s->slist.begin(); it != s->slist.end(); ++it) {
    (*it)->accept(this);
  }
  return 0;
}

int ImpInterpreter::visit(AssignStatement* s) {
  int v = s->rhs->accept(this);
  if (!env.check(s->id)) {
    cout << "Variable " << s->id << " undefined" << endl;
    exit(0);
  }
  env.update(s->id, v);  
  //memoria_update(s->id, v);
  
  return 0;
}

int ImpInterpreter::visit(PrintStatement* s) {
  int v = s->e->accept(this);
  cout << v << endl;
  return 0;
}

int ImpInterpreter::visit(IfStatement* s) {
  if (s->cond->accept(this)) {
    s->tbody->accept(this);
  } else {
    if (s->fbody != NULL)
      s->fbody->accept(this);
  }
  return 0;
}

int ImpInterpreter::visit(WhileStatement* s) {
 while (s->cond->accept(this)) {
    s->body->accept(this);
  }
 return 0;
}

int ImpInterpreter::visit(ForStatement* s) {
  int n1 = s->e1->accept(this);
  int n2 = s->e2->accept(this);
  env.add_level();
  env.add_var(s->id);
  for (int i = n1; i <= n2; i++) {
    env.update(s->id,i);
    s->body->accept(this);
  }
  env.remove_level();
 return 0;
}

int ImpInterpreter::visit(BinaryExp* e) {
  int v1 = e->left->accept(this);
  int v2 = e->right->accept(this);
  int result = 0;
  switch(e->op) {
  case PLUS: result = v1+v2; break;
  case MINUS: result = v1-v2; break;
  case MULT: result = v1 * v2; break;
  case DIV: result = v1 / v2; break;
  case EXP:
    result = 1;
    while (v2 > 0) { result *= v1; v2--; }
    break;
  case LT: result = (v1 < v2) ? 1 : 0; break;
  case LTEQ: result = (v1 <= v2) ? 1: 0; break;
  case EQ: result = (v1 == v2) ? 1 : 0; break;
  case AND: result = (v1 && v2) ? 1 : 0; break;
  case OR: result = (v1 || v2) ? 1 : 0; break;
  }
  return result;
}

int ImpInterpreter::visit(UnaryExp* e) {
  int v = e->e->accept(this);
  if (e->op == NEG) return -v;
  else return (v?0:1);
}

int ImpInterpreter::visit(NumberExp* e) {
  return e->value;
}

int ImpInterpreter::visit(BoolConstExp* e) {
  return e->b?1:0;
}

int ImpInterpreter::visit(IdExp* e) {
  if (env.check(e->id))
    return env.lookup(e->id);
  else {
    cout << "Variable indefinida: " << e->id << endl;
    exit(0);
  }
  return 0;
  /*
  if (memoria_check(e->id))
    return memoria_lookup(e->id);
  else {
    cout << "Variable indefinida: " << e->id << endl;
    exit(0);
  }
  */
}

int ImpInterpreter::visit(ParenthExp* ep) {
  return ep->e->accept(this);
}

int ImpInterpreter::visit(CondExp* e) {
  if (e->cond->accept(this) == 0)
    return e->efalse->accept(this);
  else
    return e->etrue->accept(this);
}
