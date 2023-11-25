#ifndef IMP_VISITOR
#define IMP_VISITOR

#include "imp.hh"

class ImpVisitor {
public:
    virtual int visit(Program *p) = 0;

    virtual int visit(Body *b) = 0;  // nuevo
    virtual int visit(VarDecList *e) = 0;

    virtual int visit(VarDec *e) = 0;

    virtual int visit(StatementList *e) = 0;

    virtual int visit(AssignStatement *e) = 0;

    virtual int visit(PrintStatement *e) = 0;

    virtual int visit(IfStatement *e) = 0;

    virtual int visit(WhileStatement *e) = 0;

    virtual int visit(DoWhileStatement *e) = 0;

    virtual int visit(ForStatement *e) = 0;

    virtual int visit(BreakStatement *e) = 0;

    virtual int visit(ContinueStatement *e) = 0;

    virtual int visit(BinaryExp *e) = 0;

    virtual int visit(UnaryExp *e) = 0;

    virtual int visit(NumberExp *e) = 0;

    virtual int visit(BoolConstExp *e) = 0;

    virtual int visit(IdExp *e) = 0;

    virtual int visit(ParenthExp *e) = 0;

    virtual int visit(CondExp *e) = 0;
};


#endif
