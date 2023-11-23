#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>

#include <fstream>

#include "svm_parser.hh"

const char* Token::token_names[28] = { "ID", "LABEL", "NUM", "EOL", "ERR", "END", "PUSH", "EQ", "GT", "GE", "LT", "LE", "GOTO", "SKIP", "POP", "DUP", "SWAP", "ADD", "SUB", "MUL", "DIV", "STORE", "LOAD", "PRINT", "JMPZ", "JMPN", "AND", "OR" };

Token::Token(Type type):type(type) { lexema = ""; }

Token::Token(Type type, char c):type(type) { lexema = c; }

Token::Token(Type type, const string source):type(type) {
  lexema = source;
}

std::ostream& operator << ( std::ostream& outs, const Token & tok )
{
  if (tok.lexema.empty())
    return outs << Token::token_names[tok.type];
  else
    return outs << Token::token_names[tok.type] << "(" << tok.lexema << ")";
}

std::ostream& operator << ( std::ostream& outs, const Token* tok ) {
  return outs << *tok;
}


Scanner::Scanner(string s):input(s),first(0),current(0) {
  reserved["skip"] = Token::SKIP;
  reserved["pop"] = Token::POP;
  reserved["dup"] = Token::DUP;
  reserved["swap"] = Token::SWAP;
  reserved["add"] = Token::ADD;   
  reserved["sub"] = Token::SUB;
  reserved["mul"] = Token::MUL;
  reserved["div"] = Token::DIV;
  reserved["eq"] = Token::EQ;
  reserved["gt"] = Token::GT;
  reserved["ge"] = Token::GE;
  reserved["lt"] = Token::LT;
  reserved["le"] = Token::LE;
  reserved["and"] = Token::AND;
  reserved["or"] = Token::OR;
  reserved["print"] = Token::PRINT;

  
  reserved["push"] = Token::PUSH;
  reserved["store"] = Token::STORE;
  reserved["load"] = Token::LOAD;
  

  reserved["goto"] = Token::GOTO;
  reserved["jmpz"] = Token::JMPZ;
  reserved["jmpn"] = Token::JMPN;

}

Token* Scanner::nextToken() {
  Token* token;
  char c;
  string lex;
  Token::Type ttype;
  c = nextChar();
  while (c == ' ') c = nextChar();
  if (c == '\0') return new Token(Token::END);
  startLexema();
  state = 0;
  while (1) {
    switch (state) {
    case 0:
       if (isalpha(c)) { state = 1; }
      else if (isdigit(c)) { startLexema(); state = 4; }
      else if (c == '\n') state = 6;
      else return new Token(Token::ERR, c);
      break;
    case 1:
      c = nextChar();
       if (isalpha(c) || isdigit(c) || c=='_') state = 1;
      else if (c == ':') state = 3;
      else state = 2;
      break;
    case 4:
      c = nextChar();
      if (isdigit(c)) state = 4;
      else state = 5;
      break;
    case 6:
      c = nextChar();
      if (c == '\n') state = 6;
      else state = 7;
      break;
    case 2:
      rollBack();
      lex = getLexema();
      ttype = checkReserved(lex);
      if (ttype != Token::ERR)
	return new Token(ttype);
      else
	return new Token(Token::ID, getLexema()); 
    case 3:
      rollBack();
      token = new Token(Token::LABEL,getLexema());
      nextChar();
      return token;
    case 5:
      rollBack();
      return new Token(Token::NUM,getLexema());
    case 7:
      rollBack();
      return new Token(Token::EOL);
    default:
      cout << "Programming Error ... quitting" << endl;
      exit(0);
    }
  }

}

Scanner::~Scanner() { }

char Scanner::nextChar() {
  int c = input[current];
  current++;
  return c;
}

void Scanner::rollBack() { // retract
    current--;
}

void Scanner::startLexema() {
  first = current-1;
  return;
}

void Scanner::incrStartLexema() {
  first++;
}


string Scanner::getLexema() {
  return input.substr(first,current-first);
}

Token::Type Scanner::checkReserved(string lexema) {
  std::unordered_map<std::string,Token::Type>::const_iterator it = reserved.find (lexema);
  if (it == reserved.end())
    return Token::ERR;
 else
   return it->second;
}

Instruction::IType Token::tokenToIType(Token::Type tt) {
  Instruction::IType itype;
  switch (tt) {
  case(Token::SKIP): itype = Instruction::ISKIP; break;
  case(Token::POP): itype = Instruction::IPOP; break;
  case(Token::DUP): itype = Instruction::IDUP; break;
  case(Token::SWAP): itype = Instruction::ISWAP; break;
  case(Token::ADD): itype = Instruction::IADD; break;
  case(Token::SUB): itype = Instruction::ISUB; break;
  case(Token::MUL): itype = Instruction::IMUL; break;
  case(Token::DIV): itype = Instruction::IDIV; break;
  case(Token::EQ): itype = Instruction::IEQ; break;
  case(Token::GT): itype = Instruction::IGT; break;
  case(Token::GE): itype = Instruction::IGE; break;
  case(Token::LT): itype = Instruction::ILT; break;
  case(Token::LE): itype = Instruction::ILE; break;
  case(Token::AND): itype = Instruction::IAND; break;
  case(Token::OR): itype = Instruction::IOR; break;
  case(Token::PRINT): itype = Instruction::IPRINT; break;

  case(Token::PUSH): itype = Instruction::IPUSH; break;
  case(Token::STORE): itype = Instruction::ISTORE; break;
  case(Token::LOAD): itype = Instruction::ILOAD; break;


  case(Token::GOTO): itype = Instruction::IGOTO; break;
  case(Token::JMPZ): itype = Instruction::IJMPZ; break;
  case(Token::JMPN): itype = Instruction::IJMPN; break;

  default: cout << "Error: Unknown Keyword type" << endl; exit(0);
  }
  return itype;
}


/* ******** Parser *********** */


// match and consume next token
bool Parser::match(Token::Type ttype) {
  if (check(ttype)) {
    advance();
    return true;
  }
  return false;
}

bool Parser::check(Token::Type ttype) {
  if (isAtEnd()) return false;
  return current->type == ttype;
}


bool Parser::advance() {
  if (!isAtEnd()) {
    Token* temp =current;
    if (previous) delete previous;
    current = scanner->nextToken();
    previous = temp;
    if (check(Token::ERR)) {
      cout << "Parse error, unrecognised character: " << current->lexema << endl;
      exit(0);
    }
    return true;
  }
  return false;
} 

bool Parser::isAtEnd() {
  return (current->type == Token::END);
} 

Parser::Parser(Scanner* sc):scanner(sc) {
  previous = current = NULL;
  return;
};

SVM* Parser::parse() {
  current = scanner->nextToken();
  if (check(Token::ERR)) {
      cout << "Error en scanner - caracter invalido" << endl;
      exit(0);
  }
  Instruction* instr = NULL;
  list<Instruction*> sl;
  while (current->type != Token::END) {
    instr = parseInstruction();
    sl.push_back(instr);
    // que hacemos con la instruccion?
  }  
  if (current->type != Token::END) {
    cout << "Esperaba fin-de-input, se encontro " << current << endl;
  }
  if (current) delete current;
  
  return new SVM(sl);
}

Instruction* Parser::parseInstruction() {
  Instruction* instr = NULL;
  string label = "";
  string jmplabel;
  Token::Type ttype;
  int tipo = 0; // 0 no args, 1 un arg entero,  1 un arg label
  int n;
  // match label, si existe
  if (match(Token::LABEL)) {
    label = previous->lexema;
  }

  if (match(Token::SKIP) || match(Token::POP) || match(Token::DUP) ||
      match(Token::SWAP) || match(Token::ADD) || match(Token::SUB) ||
      match(Token::MUL) || match(Token::DIV) || match(Token::EQ) ||
      match(Token::GT) || match(Token::GE) || match(Token::LT) ||
      match(Token::LE) || match(Token::AND) || match(Token::OR) ||
      match(Token::PRINT)) { 
    tipo = 0;
    ttype = previous->type;
  } else if (match(Token::PUSH) || match(Token::STORE) || match(Token::LOAD)) {
    tipo = 1;
    ttype = previous->type;
    if (!match(Token::NUM)) {
      cout << "Esperaba NUM despues de " << Token::token_names[ttype] << endl;
      exit(0);
    }
    n = stoi(previous->lexema);
  } else if (match(Token::JMPZ) || match(Token::JMPN) || match(Token::GOTO) ) {
    tipo = 2;
    ttype = previous->type;
    if (!match(Token::ID)) {
      cout << "Esperaba ID despues de " << Token::token_names[ttype] << endl;
      exit(0);
    }   
    jmplabel = previous->lexema;
  } else {
    cout << "Error: no pudo encontrar match para " << current << endl;  
    exit(0);
  }

  
  if (!match(Token::EOL)) {
    cout << "esperaba fin de linea" << endl;
    exit(0);
  }

  if (tipo == 0) {
    instr = new Instruction(label, Token::tokenToIType(ttype));
  } else if (tipo == 1) {
    instr = new Instruction(label, Token::tokenToIType(ttype), n);
  } else { 
    instr = new Instruction(label, Token::tokenToIType(ttype), jmplabel);
  }
			   

  return instr;
}

				    

