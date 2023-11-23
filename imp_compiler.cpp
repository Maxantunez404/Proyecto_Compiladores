#include <sstream>
#include <iostream>
#include <iostream>

#include "imp.hh"
#include "imp_parser.hh"
#include "imp_printer.hh"
#include "imp_interpreter.hh"
#include "imp_typechecker.hh"
#include "imp_codegen.hh"

int main(int argc, const char* argv[]) {

  Program *program; 
   
  if (argc != 2) {
    cout << "Incorrect number of arguments" << endl;
    exit(1);
  }

  std::ifstream t(argv[1]);
  std::stringstream buffer;
  buffer << t.rdbuf();
  Scanner scanner(buffer.str());
  
  Parser parser(&scanner);
  program = parser.parse();  // el parser construye la aexp
  
  ImpPrinter printer;
  ImpInterpreter interpreter;
  ImpTypeChecker checker;

  ImpCodeGen cg;
  
  printer.print(program);
  
  cout << endl << "Type checking:" << endl;
  checker.typecheck(program);
  
  cout << endl << "Run program:" << endl;
  interpreter.interpret(program);

  string outfname = argv[1];
  outfname += ".sm";
  cout << endl << "Compiling to: " << outfname << endl;
  cg.codegen(program, outfname);

  delete program;

}
