/* CSE 109
   Brian Dwyer
   bpd219
   Compiler
*/

#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <iostream>
#include <string>

using namespace std;

class Lexer {

public:

  //Constructor
  Lexer(istream& stream);

  //Member function
  Token* nextToken();
  
  //Member variables
  istream &stream;  //Stores a reference to the istream.

  //Member variables to keep track of the line and position in the
  //lexer class.
  int line;
  int pos;
  char ch;
  
  
  
private:

  //Member functions
  char nextChar();
  
  
};

#endif
