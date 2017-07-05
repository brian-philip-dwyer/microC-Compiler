/* CSE 109
   Brian Dwyer
   bpd219
   Description: Class used to hold the tokens returned by the scanner. 
*/

#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <string>
#include "token.h"

using namespace std;

//Default Token constructor.
Token::Token() {}

//Token constructor; creates a Token object from nextToken() in the lexer. 
Token::Token(int type, string lexeme, int line, int pos) {
  this->type = type;
  this->lexeme = lexeme;
  this->line = line;
  this->pos = pos;
  //cout<<"Token constructor: "<<type<< " "<<lexeme<<" "<<line<<" "<<pos<<endl;
}

//Token destructor
Token::~Token() {}

//Getter methods
int Token::getTtype() {
  return type;
}

string Token::getLexeme() {
  return lexeme;
}

int Token::getLine() {
  return line;
}

int Token::getPos() {
  return pos;
}


