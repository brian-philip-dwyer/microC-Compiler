/* CSE 109
   Brian Dwyer
   bpd219
   Description: 
*/

#include "lexer.h"
#include "token.h"
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>

using namespace std;


//Lexer constructor; takes an input stream as a parameter.
Lexer::Lexer(istream &stream):stream(stream) {  
  line = 1;
  pos = 0;
  ch = nextChar();
}


//Reads the next character and returns it unless it reaches EOF, reads
//a new line character, or reads a '#'.
char Lexer::nextChar() {

  
  //Char variable that gets the following character from the input stream. 
  char ch = stream.get();
  //Comment string; acts like a lexeme.
  string comment = "";
 
  //If nextChar() reaches the end of file, return a '$'.
  if(stream.eof()) {
    return '$';
  }
  //If nextChar() reads a '\n', return a ' ' (space).
  else if(ch == '\n') {
    pos = 0;
    line++;
    return ' ';
  }
  //If nextChar() reads a '#', skip the rest of the characters on the
  //line and return a space.
  else if(ch == '#') {
    comment += ch; 
    ch = nextChar();
    while(ch != '\n') {
      comment += ch;
      ch = nextChar();
    }
    pos = 0;
    line++;
    return ' ';
  }
  
  //Return the read character to the nextToken function.
  pos++;
  return ch;
}

//Returns a Token* with the next token in the input stream. Golden
//Rule is to ALWAYS read one character past the token you're returning.
Token* Lexer::nextToken() {
  
  Token* token;
  //Default type is ERROR so an error is thrown when a type isn't assigned.
  int type = Token::ERROR;
  //String that starts empty, and has each char added onto it to
  //create the lexeme (sequence of chars to match a token). 
  string lexeme = "";
  

  //While the char is a space, read the next character.
  while(ch == ' ' || ch == '\t') {
    ch = nextChar();
  }

    
  //Keeps track of position (of the FIRST character in the token) and line.
  int position = pos;
  int lineNum = line;
  lexeme += ch;

  //If token is a letter, token is an IDENT (string of letters and
  //digits starting with a letter).
  if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
    type = Token::IDENT;
    ch = nextChar();
    //While the character is a letter or number, add it to the lexeme
    //and read the next character.
    while(((ch >= 'A') && (ch <= 'Z')) || ((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9'))) {
      lexeme += ch;
      ch = nextChar();
    }
  }

  
  //If token is a digit, token is an Integer Literal (string of digits).
  else if(ch >= '0' && ch <= '9') {
    type = Token::INTLIT;
    ch = nextChar();
    //While the next character is a number, add it to the lexeme.
    while(ch >= '0' && ch <= '9') {
      lexeme += ch;
      ch = nextChar();
    }
  }

  
  //If token is a string, token is a String Literal (any sequence of
  //chars surrounded by " ").
  else if(ch == '"') {
    type = Token::STRINGLIT;
    ch = nextChar();
    //While there isn't a closing quote, continue.
    while(ch != '"') {
      lexeme += ch;
      ch = nextChar();
    }
    //Add the closing quote to the lexeme and read the next character.
    lexeme += ch;
    ch = nextChar();
  }

  
  //Classification for operators and punctions.
  else if(ch == '+') {
    type = Token::PLUS;
    ch = nextChar();
  }
  else if(ch == '-') {
    type = Token::MINUS;
    ch = nextChar();
  }
  else if(ch == '*') {
    type = Token::TIMES;
    ch = nextChar();
  }
  else if(ch == '/') {
    type = Token::DIVIDE;
    ch = nextChar();
  }

  else if(ch == '=') {
    type = Token::ASSIGN;
    ch = nextChar();
    if(ch == '=') {
      type = Token::EQ;
      lexeme += ch;
      ch = nextChar();
    }
  }
  
  else if(ch == '!') {
    type = Token::ERROR;
    ch = nextChar();
    if(ch == '=') {
      type = Token::NE;
      lexeme += ch;
      ch = nextChar();
    }
  }

  else if(ch == '<') {
    type = Token::LT;
    ch = nextChar();
    if(ch == '=') {
      type = Token::LE;
      lexeme += ch;
      ch = nextChar();
    }
  }

  else if(ch == '>') {
    type = Token::GT;
    ch = nextChar();
    if(ch == '=') {
      type = Token::GE;
      lexeme += ch;
      ch = nextChar();
    }
  }

  else if(ch == '&') {
    type = Token::ERROR;
    ch = nextChar();
    if(ch == '&') {
      type = Token::AND;
      lexeme += ch;
      ch = nextChar();
    }
  }

  else if(ch == '|') {
    type = Token::ERROR;
    ch = nextChar();
    if(ch == '|') {
      type = Token::OR;
      lexeme += ch;
      ch = nextChar();
    }
  }

  else if(ch == '(') {
    type = Token::LPAREN;
    ch = nextChar();
  }

  else if(ch == ')') {
    type = Token::RPAREN;
    ch = nextChar();
  }

  else if(ch == '{') {
    type = Token::LBRACE;
    ch = nextChar();
  }

  else if(ch == '}') {
    type = Token::RBRACE;
    ch = nextChar();
  }

  else if(ch == ',') {
    type = Token::COMMA;
    ch = nextChar();
  }

  else if(ch == ';') {
    type = Token::SEMICOLON;
    ch = nextChar();
  }


  //Classification for special characters.
  else if(ch == '$') {
    type = Token::ENDOFFILE;
    ch = nextChar();
  }

  else {
    type = Token::ERROR;
    ch = nextChar();
  }


  //Classification for keywords. 
  if(type == Token::IDENT) {

    if(lexeme == "if") {
      type = Token::IF;
    }
    
    else if(lexeme == "else") {
      type = Token::ELSE;
    }
    
    else if(lexeme == "while") {
      type = Token::WHILE;
    }
    
    else if(lexeme == "function") {
      type = Token::FUNCTION;
    }
    
    else if(lexeme == "var") {
      type = Token::VAR;
    }

    else if(lexeme == "return") {
      type = Token::RETURN;
    }
    
    else {
      type = Token::IDENT;
    }
  }
  

  //Matches the lexeme with a token; creates an instance of a token
  //with the token type, the lexeme, line number and char position.
  token = new Token(type, lexeme, lineNum, position);
  //cout<<"Type: "<<type<<";  Lexeme: "<<lexeme<<";  Line: "<<lineNum<<";  Position: "<<position<<"\n"<<endl;
  return token;
  
}

