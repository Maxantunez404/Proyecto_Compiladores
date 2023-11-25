#ifndef TYPE_VISITOR
#define TYPE_VISITOR

#include "imp.hh"
#include "imp_type.hh"

class TypeVisitor {
public:
    virtual void visit(Program *p) = 0;

    virtual void visit(Body *b) = 0;  // nuevo
    virtual void visit(VarDecList *e) = 0;

    virtual void visit(VarDec *e) = 0;

    virtual void visit(StatementList *e) = 0;

    virtual void visit(AssignStatement *e) = 0;

    virtual void visit(PrintStatement *e) = 0;

    virtual void visit(IfStatement *e) = 0;

    virtual void visit(WhileStatement *e) = 0;

    virtual void visit(DoWhileStatement *e) = 0;

    virtual void visit(ForStatement *e) = 0;

    virtual void visit(BreakStatement *e) = 0;

    virtual void visit(ContinueStatement *e) = 0;

    virtual ImpType visit(BinaryExp *e) = 0;

    virtual ImpType visit(UnaryExp *e) = 0;

    virtual ImpType visit(NumberExp *e) = 0;

    virtual ImpType visit(BoolConstExp *e) = 0;

    virtual ImpType visit(IdExp *e) = 0;

    virtual ImpType visit(ParenthExp *e) = 0;

    virtual ImpType visit(CondExp *e) = 0;
};


#endif
