#include "imp_typechecker.hh"

ImpTypeChecker::ImpTypeChecker():inttype(),booltype() {
  inttype.set_basic_type("int");
  booltype.set_basic_type("bool");
}

void ImpTypeChecker::typecheck(Program* p) {
  env.clear();
  p->accept(this);
  return;
}

void ImpTypeChecker::visit(Program* p) {
  p->body->accept(this);
  return;
}

void ImpTypeChecker::visit(Body* b) {
  env.add_level();
  b->var_decs->accept(this);
  b->slist->accept(this);
  env.remove_level();  
  return;
}

void ImpTypeChecker::visit(VarDecList* decs) {
  list<VarDec*>::iterator it;
  for (it = decs->vdlist.begin(); it != decs->vdlist.end(); ++it) {
    (*it)->accept(this);
  }  
  return;
}

void ImpTypeChecker::visit(VarDec* vd) {
  ImpType type;
  type.set_basic_type(vd->type);
  if (type.ttype==ImpType::NOTYPE || type.ttype==ImpType::VOID) {
    cout << "Tipo invalido: " << vd->type << endl;
    exit(0);
  }
  list<string>::iterator it;
  for (it = vd->vars.begin(); it != vd->vars.end(); ++it) {
    env.add_var(*it, type);
  }   
  return;
}


void ImpTypeChecker::visit(StatementList* s) {
  list<Stm*>::iterator it;
  for (it = s->slist.begin(); it != s->slist.end(); ++it) {
    (*it)->accept(this);
  }
  return;
}

void ImpTypeChecker::visit(AssignStatement* s) {
  ImpType type = s->rhs->accept(this);
  if (!env.check(s->id)) {
    cout << "Variable " << s->id << " undefined" << endl;
    exit(0);
  }
  ImpType var_type = env.lookup(s->id);  
  if (!type.match(var_type)) {
    cout << "Tipo incorrecto en Assign a " << s->id << endl;
    exit(0);
  }
  return;
}

void ImpTypeChecker::visit(PrintStatement* s) {
  s->e->accept(this);
  return;
}

void ImpTypeChecker::visit(IfStatement* s) {
  if (!s->cond->accept(this).match(booltype)) {
    cout << "Condicional en IfStm debe de ser: " << booltype << endl;
    exit(0);
  }    
  s->tbody->accept(this);
  if (s->fbody != NULL)
    s->fbody->accept(this);
  return;
}

void ImpTypeChecker::visit(WhileStatement* s) {
  if (!s->cond->accept(this).match(booltype)) {
    cout << "Condicional en WhileStm debe de ser: " << booltype << endl;
    exit(0);
  }  
  s->body->accept(this);
 return;
}

void ImpTypeChecker::visit(ForStatement* s) {
  ImpType t1 = s->e1->accept(this);
  ImpType t2 = s->e2->accept(this);
  if (!t1.match(inttype) || !t2.match(inttype)) {
    cout << "Tipos de rangos en for deben de ser: " << inttype << endl;
    exit(0);
  }
  env.add_level();
  env.add_var(s->id,inttype);
  s->body->accept(this);
  env.remove_level();
 return;
}

ImpType ImpTypeChecker::visit(BinaryExp* e) {
  ImpType t1 = e->left->accept(this);
  ImpType t2 = e->right->accept(this);
  ImpType result, argtype;
  argtype = inttype;
  switch(e->op) {
  case PLUS: 
  case MINUS:
  case MULT:
  case DIV:
  case EXP:
    result = inttype;
    break;
  case LT: 
  case LTEQ:
  case EQ:
    result = booltype;
    break;
  case AND: case OR:
    result = booltype; argtype = booltype;
    break;
  }
  if (!t1.match(argtype) || !t2.match(argtype)) {
    cout << "Tipos en BinExp deben de ser: " << argtype << endl;
    exit(0);
  }

  return result;
}

ImpType ImpTypeChecker::visit(UnaryExp* e) {
  ImpType etype = e->e->accept(this);
  ImpType rtype;
  if (e->op == NEG) rtype.set_basic_type("int");
  else rtype.set_basic_type("bool");
  if (!etype.match(rtype)) {
    cout << "Tipo incorrecto para operacion binaria" << endl;
    exit(0);
  }
  return rtype;
}

ImpType ImpTypeChecker::visit(NumberExp* e) {
  return inttype;
}

ImpType ImpTypeChecker::visit(BoolConstExp* e) {
  return booltype;
}

ImpType ImpTypeChecker::visit(IdExp* e) {
  if (env.check(e->id))
    return env.lookup(e->id);
  else {
    cout << "Variable indefinida: " << e->id << endl;
    exit(0);
  }
}

ImpType ImpTypeChecker::visit(ParenthExp* ep) {
  return ep->e->accept(this);
}

ImpType ImpTypeChecker::visit(CondExp* e) {
  if (!e->cond->accept(this).match(booltype)) {
    cout << "Tipo en ifexp debe de ser bool" << endl;
    exit(0);
  }
  ImpType ttype =  e->etrue->accept(this);  
  if (!ttype.match(e->efalse->accept(this))) {
    cout << "Tipos en ifexp deben de ser iguales" << endl;
    exit(0);
  }
  return ttype;
}
