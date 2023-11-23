#include <iostream>

#include "svm.hh"

string snames[22] = { "push", "pop", "dup", "swap", "add", "sub", "mult", "div", "goto", "eq", "gt", "ge", "lt", "le", "skip", "store", "load", "print", "jmpz", "jmpn", "and", "or" };

Instruction::Instruction(string l, IType itype):label(l),type(itype),hasarg(false) {
}

Instruction::Instruction(string l, IType itype, int arg):label(l),type(itype),hasarg(true), argint(arg),jmplabel("") {
}

Instruction::Instruction(string l, IType itype, string arg):label(l),type(itype),hasarg(true),jmplabel(arg) {

}

SVM::SVM(list<Instruction*>& sl) {
  instructions.reserve(sl.size());
  copy(begin(sl), end(sl), back_inserter(instructions));
  pc = 0;
  sp = max_stack = 0;
  // resolver labels
  for(int i=0; i < instructions.size(); i++) {
    string l = instructions[i]->label;
    if (l != "")
      labels[l] = i; 
  }
  for(int i=0; i < instructions.size(); i++) {
    string jl = instructions[i]->jmplabel;
    if (jl != "") {
      std::unordered_map<std::string,int>::const_iterator it = labels.find(jl);
      if (it == labels.end()) {
	cout << "No se encontro label " << jl << endl;
	exit(0);
      } else
	instructions[i]->argint = it->second;
    }
  }
  

					   
}

void SVM::execute() {
  while (true) {   
    if (pc >= instructions.size()) break;
    // cout << "pc " << pc << " ";
    // print_stack();
    execute(instructions[pc]);
  }
  cout << "Max Stack = " << max_stack << endl;
}

void SVM::stack_pop() {
  opstack.pop();
  sp--;
}

void SVM::stack_push(int v) {
  opstack.push(v);
  sp++;
  if (sp > max_stack) max_stack = sp;
}

void SVM::execute(Instruction* instr) {
  Instruction::IType itype = instr->type;
  int next, top;
  //cout << "type: " << itype << endl;
  if (itype==Instruction::IPOP || itype==Instruction::IDUP ||
      itype==Instruction::IPRINT || itype==Instruction::ISKIP) {
    switch (itype) {
    case(Instruction::IPOP):
      if (opstack.empty()) perror("Can't pop from an empty stack");
      stack_pop(); break;
    case(Instruction::IDUP):
      if (opstack.empty()) perror("Can't dup from an empty stack");
      stack_push(opstack.top()); break;
    case(Instruction::IPRINT): cout << opstack.top() << endl; stack_pop(); break;
    case(Instruction::ISKIP): break;
    default: perror("Programming Error 1");
    }
    pc++;
  } else if (itype==Instruction::IPUSH || itype==Instruction::ISTORE ||
	     itype==Instruction::ILOAD) {
    int arg = instr->argint;
    switch (itype) {
    case(Instruction::IPUSH): stack_push(instr->argint); break;
    case (Instruction::ISTORE):
      if (opstack.empty()) perror("Can't store from an empty stack");
      register_write(instr->argint, opstack.top()); stack_pop(); break;
      break;
    case(Instruction::ILOAD):
      stack_push(register_read(instr->argint)); break;
      break;
    default: perror("Programming Error 2");
    }
    pc++;
  } else if (itype==Instruction::IJMPZ || itype==Instruction::IJMPN ) {
    bool jump = false;
    top = opstack.top(); stack_pop();
    switch(itype) {
    case(Instruction::IJMPZ): jump = (top==0); break;
    case(Instruction::IJMPN): jump = (top!=0); break;
    default: perror("Programming Error 3");
    }
    if (jump) pc=instr->argint; else pc++;
  } else if (itype==Instruction::IADD || itype==Instruction::ISUB || itype==Instruction::IMUL ||
	     itype==Instruction::IDIV || itype==Instruction::IEQ || itype==Instruction::IGT ||
	     itype==Instruction::IGE  || itype==Instruction::ILT || itype==Instruction::ILE ||
	     itype==Instruction::IAND  || itype==Instruction::IOR ||
	     itype==Instruction::ISWAP)  {
    top = opstack.top(); stack_pop();
    next = opstack.top(); stack_pop();    
    switch(itype) {
    case(Instruction::IADD): stack_push(next+top); break;
    case(Instruction::ISUB): stack_push(next-top); break;
    case(Instruction::IMUL): stack_push(next*top); break;
    case(Instruction::IDIV): stack_push(next/top); break;
    case(Instruction::IEQ): stack_push(next==top?1:0); break;
    case(Instruction::IGT): stack_push(next>top?1:0); break;
    case(Instruction::IGE): stack_push(next>=top?1:0); break;
    case(Instruction::ILT): stack_push(next<top?1:0); break;
    case(Instruction::ILE): stack_push(next<=top?1:0); break;
    case(Instruction::IAND): stack_push(next&&top?1:0); break;
    case(Instruction::IOR): stack_push(next||top?1:0); break;
    case(Instruction::ISWAP): stack_push(top); stack_push(next); break;
    default: perror("Programming Error 4");
    }
    pc++;
  } else if (itype == Instruction::IGOTO) {
    pc = instr->argint;
  } else {
    cout << "Programming Error: execute instruction" << endl;
    exit(0);
  }
}

void SVM::print_stack() {
  stack<int> local;
  while(!opstack.empty()) {
    local.push(opstack.top());
    opstack.pop();
  }
  cout << "stack [ ";
  while(!local.empty()) {
    cout << local.top() << " ";    
    opstack.push(local.top());
    local.pop();
  }
  cout << "]" << endl;  
}

void SVM::print() {
  for(int i= 0; i < instructions.size(); i++) {
    Instruction* s = instructions[i];
    if (s->label != "")
      cout << s->label << ": ";
    cout << snames[s->type] << " ";
    if (s->hasarg) {
      if (s->jmplabel == "")
	cout << s->argint;
      else {
	cout << s->jmplabel;
	// cout << "  [" << s->argint << "]";
      }
    }
    cout << endl;
  }					    
}


void SVM::register_write(int r,int v) {
  if (r > 99 || r < 0)
    perror("Insufficient memory");
  memory[r] = v;
}
  
int SVM::register_read(int r) {
  if (r > 99 || r < 0)
    perror("Insufficient memory");
  return memory[r];
}

    
void SVM::perror(string msg) {
  cout << "error: " << msg << endl;
  exit(0);
}



  





