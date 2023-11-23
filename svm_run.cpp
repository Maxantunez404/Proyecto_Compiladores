#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <fstream>


#include "svm_parser.hh"
#include "svm.hh"


int main(int argc, const char* argv[]) {

  bool useparser = true;
  SVM* svm;

  if (useparser) {
  
    if (argc != 2) {
      cout << "File name missing" << endl;
      exit(1);
    }
    cout << "Reading program from file " << argv[1] << endl;
    std::ifstream t(argv[1]);
    std::stringstream buffer;
    buffer << t.rdbuf();

    // our scanner takes strings
    Scanner scanner(buffer.str());
  
    Parser parser(&scanner);
    svm = parser.parse();

    // test scanner

    /*
      Token* tk = scanner.nextToken();
      while (tk->type != Token::END) {
      cout << "next token " << tk << endl;
      delete tk;
      tk =  scanner.nextToken();
      }
      cout << "last token " << tk << endl;
      delete tk;
    */

  } else {

    list<Instruction*> sl;
    // programa 1

    sl.push_back(new Instruction("",Instruction::IPUSH, 30));
    sl.push_back(new Instruction("",Instruction::IPUSH, 3));
    sl.push_back(new Instruction("",Instruction::IDIV));
    sl.push_back(new Instruction("",Instruction::IDUP));
    sl.push_back(new Instruction("",Instruction::IPUSH, 2));  
    sl.push_back(new Instruction("",Instruction::IADD));
    sl.push_back(new Instruction("",Instruction::ISWAP));
    sl.push_back(new Instruction("",Instruction::IGE));
    sl.push_back(new Instruction("",Instruction::IJMPN,"L20"));
    sl.push_back(new Instruction("",Instruction::IPUSH, 10));		  
    sl.push_back(new Instruction("",Instruction::IGOTO, "LEND")); // 9
    sl.push_back(new Instruction("L20",Instruction::IPUSH, 20));
    sl.push_back(new Instruction("LEND",Instruction::ISKIP));
   
    // programa 2
    /*       
    sl.push_back(new Instruction("",Instruction::IPUSH, 6));
    sl.push_back(new Instruction("",Instruction::ISTORE, 5));
    sl.push_back(new Instruction("",Instruction::IPUSH, 0));  
    sl.push_back(new Instruction("LENTRY",Instruction::ILOAD, 5));
    sl.push_back(new Instruction("",Instruction::IPUSH, 0));
    sl.push_back(new Instruction("",Instruction::IJMPLE, "LEND"));
    sl.push_back(new Instruction("",Instruction::ILOAD, 5));
    sl.push_back(new Instruction("",Instruction::IADD, 0));
    sl.push_back(new Instruction("",Instruction::ILOAD, 5));
    sl.push_back(new Instruction("",Instruction::IPUSH, 1));
    sl.push_back(new Instruction("",Instruction::ISUB));
    sl.push_back(new Instruction("",Instruction::ISTORE, 5));
    sl.push_back(new Instruction("",Instruction::IGOTO, "LENTRY"));
    sl.push_back(new Instruction("LEND",Instruction::ISKIP));
     */
    
    svm = new SVM(sl);

  }
  
  cout << "Program:" << endl;
  svm->print();
  cout << "----------------" << endl;

  
  cout << "Running ...." << endl;
  svm->execute();
  cout << "Finished" << endl;

  svm->print_stack();
  

  
  
  
}
