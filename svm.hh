#ifndef SVMHH
#define SVMHH

#include <string>
#include <list>
#include <stack>

#include <vector>
#include <unordered_map>

using namespace std;

class Instruction {
public:
  enum IType { IPUSH=0, IPOP, IDUP, ISWAP, IADD, ISUB, IMUL, IDIV, IGOTO, IEQ, IGT, IGE, ILT, ILE, ISKIP, ISTORE, ILOAD, IPRINT, IJMPZ, IJMPN, IAND, IOR };
  string label, jmplabel;
  IType type;
  bool hasarg;
  int argint; // could be label or int argument
  Instruction(string l, IType itype);
  Instruction(string l, IType itype, int arg);
  Instruction(string l, IType itype, string arg);  
  //static InstrType convertKeywordType(Token::KeywordType ktype);
};


class SVM {
private:
  stack<int> opstack;
  int max_stack;
  int sp;
  int memory[100];
  vector<Instruction*> instructions;
  unordered_map<string,int> labels;
  int pc; // program counter
  void stack_pop();
  void stack_push(int v);
  void execute(Instruction* s);
  void perror(string msg);
  void register_write(int,int);
  int register_read(int);
public:
  SVM(list<Instruction*>&  sl);
  void execute();
  void print_stack();
  void print();
  int top();
};


#endif
