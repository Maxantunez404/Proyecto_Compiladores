#ifndef SVM_PARSER
#define SVM_PARSER

#include <string>

#include "svm.hh"

using namespace std;

class Token {
public:
  enum Type { ID=0, LABEL, NUM, EOL, ERR, END, PUSH, EQ, GT, GE, LT, LE, GOTO, SKIP, POP, DUP, SWAP, ADD, SUB, MUL, DIV, STORE, LOAD, PRINT, JMPZ, JMPN, AND, OR };
  static const char* token_names[28]; 
  Type type;
  string lexema;
  Token(Type);
  Token(Type, char c);
  Token(Type, const string source);
  static Instruction::IType tokenToIType(Token::Type tt);
};

std::ostream& operator << ( std::ostream& outs, const Token* tok );

class Scanner {
public:
  Scanner(string in_s);
  Token* nextToken();
  ~Scanner();
private:
  string input;
  int first, current;
  int state;
  unordered_map<string, Token::Type> reserved;
  char nextChar();
  void rollBack();
  void startLexema();
  void incrStartLexema();
  string getLexema();
  Token::Type checkReserved(string lexema);
};


class Parser {
private:
  Scanner* scanner;
  Token *current, *previous;
  bool match(Token::Type ttype);
  bool check(Token::Type ttype);
  bool advance();
  bool isAtEnd();
  Instruction* parseInstruction();
public:
  Parser(Scanner* scanner);
  SVM* parse();
};


#endif
